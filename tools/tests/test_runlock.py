"""tools/dread/runlock.py, the lock check.py and verify.py hold on build/check/<version>/, and check.py's retried
replace of a cache file. Two real processes contend for the lock; no game data or compiler is involved."""

import json
import os
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
from dread import runlock

TOOLS = Path(__file__).resolve().parent.parent

# A second process: takes the lock on argv[1], says so, holds it for argv[2] seconds, releases it.
HOLDER = """
import sys, time
sys.path.insert(0, {tools!r})
from dread import runlock
with runlock.RunLock(sys.argv[1], "holder", wait=0):
    print("held", flush=True)
    time.sleep(float(sys.argv[2]))
"""


class RunLockTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.dir = Path(self.tmp.name) / "build" / "check" / "9.9.9"
        self.lines: list[str] = []
        env = mock.patch.dict(os.environ)
        env.start()
        os.environ.pop(runlock.ENV, None)
        self.addCleanup(env.stop)

    def tearDown(self):
        self.tmp.cleanup()

    def lock(self, wait, **kw) -> runlock.RunLock:
        kw.setdefault("poll", 0.05)
        kw.setdefault("report_every", 0.3)
        return runlock.RunLock(self.dir, "check.py", wait=wait, worktree=self.tmp.name, say=self.lines.append, **kw)

    def hold(self, seconds: float) -> subprocess.Popen:
        """Start a process that holds the lock for `seconds`; returns once it holds it."""
        env = {k: v for k, v in os.environ.items() if k != runlock.ENV}
        p = subprocess.Popen([sys.executable, "-c", HOLDER.format(tools=str(TOOLS)), str(self.dir), str(seconds)],
                             stdout=subprocess.PIPE, text=True, env=env)
        self.addCleanup(lambda: (p.kill(), p.wait(), p.stdout.close()))
        self.assertEqual(p.stdout.readline().strip(), "held")
        return p

    def write_holder(self, **over) -> runlock.Holder:
        h = runlock.Holder(pid=os.getpid(), host=runlock.socket.gethostname(), worktree="/elsewhere",
                           command="check.py", started=time.time(), token="t-other")
        h = runlock.dataclasses.replace(h, **over)
        self.dir.mkdir(parents=True, exist_ok=True)
        (self.dir / runlock.LOCK_NAME).write_text(json.dumps(runlock.dataclasses.asdict(h)), encoding="utf-8")
        return h

    def test_take_and_release(self):
        with self.lock(0) as lk:
            h = runlock.read_holder(self.dir / runlock.LOCK_NAME)
            self.assertEqual((h.pid, h.token, h.command), (os.getpid(), lk.token, "check.py"))
            self.assertEqual(os.environ[runlock.ENV], lk.token)
        self.assertFalse((self.dir / runlock.LOCK_NAME).exists())
        self.assertNotIn(runlock.ENV, os.environ)

    def test_second_process_waits_then_runs(self):
        p = self.hold(1.0)
        start = time.monotonic()
        with self.lock(30):
            waited = time.monotonic() - start
            h = runlock.read_holder(self.dir / runlock.LOCK_NAME)
            self.assertEqual(h.pid, os.getpid())
        self.assertGreater(waited, 0.3)
        self.assertEqual(p.wait(10), 0)
        self.assertTrue(self.lines, "no waiting line")
        self.assertIn(f"pid {p.pid}", self.lines[0])
        self.assertIn("waiting", self.lines[0])
        # One line per report period, not one per poll.
        self.assertLess(len(self.lines), waited / 0.3 + 2)

    def test_timeout_message(self):
        p = self.hold(30)
        with self.assertRaises(runlock.Busy) as cm:
            self.lock(0.5).acquire()
        msg = str(cm.exception)
        self.assertIn("gave up after", msg)
        self.assertIn(f"pid {p.pid}", msg)
        self.assertIn("--wait", msg)
        self.assertTrue((self.dir / runlock.LOCK_NAME).exists(), "the holder's lock must stay")

    def test_no_wait(self):
        self.hold(30)
        with self.assertRaises(runlock.Busy):
            self.lock(0).acquire()
        self.assertEqual(self.lines, [], "--wait 0 prints no waiting line")

    def test_stale_lock_of_a_dead_process_is_taken_over(self):
        dead = subprocess.Popen([sys.executable, "-c", "pass"])
        dead.wait()
        self.write_holder(pid=dead.pid)
        with self.lock(0) as lk:
            self.assertEqual(runlock.read_holder(self.dir / runlock.LOCK_NAME).token, lk.token)
        self.assertEqual(len(self.lines), 1)
        self.assertIn("taking over", self.lines[0])
        self.assertIn(f"pid {dead.pid}", self.lines[0])
        self.assertIn("no longer running", self.lines[0])

    @unittest.skipIf(runlock.process_started(os.getpid()) is None, "no process start time on this platform")
    def test_reused_pid_is_stale(self):
        # This process is running, but it started long after the lock was taken: the holder is gone.
        self.write_holder(started=1_000_000.0)
        with self.lock(0):
            pass
        self.assertIn("taking over", self.lines[0])

    def test_live_holder_on_another_host_is_not_taken_over(self):
        self.write_holder(pid=99999999, host="some-other-host")
        with self.assertRaises(runlock.Busy) as cm:
            self.lock(0).acquire()
        self.assertIn("on some-other-host", str(cm.exception))

    def test_unreadable_lock_is_taken_over_after_grace(self):
        self.dir.mkdir(parents=True)
        path = self.dir / runlock.LOCK_NAME
        path.write_text("{", encoding="utf-8")
        with self.assertRaises(runlock.Busy):
            self.lock(0).acquire()  # fresh: may be a run that is still writing it
        old = time.time() - runlock.UNREADABLE_GRACE - 5
        os.utime(path, (old, old))
        with self.lock(0):
            pass
        self.assertIn("unreadable", self.lines[-1])

    def test_child_of_the_holder_runs_under_its_lock(self):
        with self.lock(0) as outer:
            inner = self.lock(0)
            inner.acquire()  # same token in the environment: no wait, no second lock
            self.assertTrue(inner.inherited)
            inner.release()
            self.assertEqual(runlock.read_holder(self.dir / runlock.LOCK_NAME).token, outer.token)
        self.assertFalse((self.dir / runlock.LOCK_NAME).exists())

    def test_release_leaves_another_runs_lock(self):
        lk = self.lock(0)
        lk.acquire()
        self.write_holder(token="t-new")  # taken over meanwhile
        lk.release()
        self.assertEqual(runlock.read_holder(self.dir / runlock.LOCK_NAME).token, "t-new")

    def test_process_alive(self):
        self.assertTrue(runlock.process_alive(os.getpid()))
        dead = subprocess.Popen([sys.executable, "-c", "pass"])
        dead.wait()
        self.assertFalse(runlock.process_alive(dead.pid))


try:
    import check
except ImportError:  # pyelftools missing
    check = None


@unittest.skipUnless(check, "needs pyelftools")
class ReplaceRetry(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.path = Path(self.tmp.name) / "x.o.diff.json"
        self.path.write_text("old", encoding="utf-8")

    def test_retries_permission_error(self):
        real, calls = os.replace, []

        def flaky(src, dst):
            calls.append(dst)
            if len(calls) < 3:
                raise PermissionError(5, "Access is denied")
            real(src, dst)
        with mock.patch.object(check.os, "replace", flaky), mock.patch.object(check.time, "sleep"):
            check.write_json(self.path, {"a": 1})
        self.assertEqual(len(calls), 3)
        self.assertEqual(json.loads(self.path.read_text(encoding="utf-8")), {"a": 1})
        self.assertEqual(list(self.path.parent.glob("*.tmp")), [])

    def test_gives_up_without_killing_the_run(self):
        def denied(src, dst):
            raise PermissionError(5, "Access is denied")
        with mock.patch.object(check.os, "replace", denied), mock.patch.object(check.time, "sleep"), \
                mock.patch("builtins.print") as out:
            check.write_json(self.path, {"a": 1})
        self.assertEqual(self.path.read_text(encoding="utf-8"), "old")
        self.assertEqual(list(self.path.parent.glob("*.tmp")), [])
        self.assertIn("could not write", out.call_args[0][0])


if __name__ == "__main__":
    unittest.main()
