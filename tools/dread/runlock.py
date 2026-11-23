"""One run at a time per cache directory: a lock file that tools/check.py and tools/verify.py hold for a whole run.

Two git worktrees whose build/ is the same folder (a junction or a symbolic link) share build/check/<version>/.
Two check.py runs there at once overwrite each other's cache files; on Windows the second os.replace() onto a
file the other run has open fails with PermissionError and the run dies (2026-11-23). So a run first creates
`check.lock` in the cache directory (O_EXCL: exactly one creator wins), with the holder's pid, host, checkout,
command and start time in it, and deletes it when it ends.

A second run waits, printing one line every 30 seconds that says who holds the lock, and gives up after `wait`
seconds (check.py --wait, in minutes) with Busy, whose message says the same. A lock whose holder process no
longer exists (killed, crashed, a cancelled CI job) is taken over with a note: the holder's pid is not running,
or the process running under that pid started after the lock was taken (Windows reuses pids quickly). A lock
from another host is never judged stale. A process that holds the lock exports its token in DREAD_RUN_LOCK, so
the check.py that verify.py starts runs under verify.py's lock instead of waiting for it.

Standard library only; Windows and Linux (macOS: the pid test without the start-time test).
"""

from __future__ import annotations

import dataclasses
import json
import os
import socket
import sys
import time
import uuid
from pathlib import Path

LOCK_NAME = "check.lock"
ENV = "DREAD_RUN_LOCK"  # token of the lock an ancestor process holds
EXIT_BUSY = 3  # exit status of a run that gave up waiting
REPORT_EVERY = 30.0  # seconds between "waiting" lines
UNREADABLE_GRACE = 10.0  # a lock file still unreadable after this long is a leftover of a crash
TAKEOVER_GRACE = 60.0  # a takeover marker older than this belongs to a run that died during its takeover


class Busy(Exception):
    """The lock was still held when the wait ran out; str() is the message for the user."""


@dataclasses.dataclass
class Holder:
    pid: int
    host: str
    worktree: str
    command: str
    started: float  # time.time() when the lock was taken
    token: str

    def describe(self, now: float | None = None) -> str:
        now = time.time() if now is None else now
        since = time.strftime("%H:%M:%S", time.localtime(self.started))
        mins = max(0, int((now - self.started) // 60))
        host = "" if self.host == socket.gethostname() else f" on {self.host}"
        return (f"{self.command}, pid {self.pid}{host}, worktree {self.worktree}, since {since} "
                f"({mins} min ago)")


def read_holder(path: Path) -> Holder | None:
    """The lock file's holder, None when it is missing, being written or not a lock file."""
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
        return Holder(**{f.name: data[f.name] for f in dataclasses.fields(Holder)})
    except (OSError, ValueError, KeyError, TypeError):
        return None


def _windows_process(pid: int):
    """(kernel32, handle) for a query handle on pid, (kernel32, None) when there is no such process,
    None when the answer is unknown (no access, or ctypes failed)."""
    import ctypes
    from ctypes import wintypes
    k32 = ctypes.WinDLL("kernel32", use_last_error=True)
    k32.OpenProcess.restype = wintypes.HANDLE
    k32.OpenProcess.argtypes = (wintypes.DWORD, wintypes.BOOL, wintypes.DWORD)
    k32.GetExitCodeProcess.argtypes = (wintypes.HANDLE, ctypes.POINTER(wintypes.DWORD))
    k32.GetProcessTimes.argtypes = (wintypes.HANDLE, *[ctypes.POINTER(wintypes.FILETIME)] * 4)
    k32.CloseHandle.argtypes = (wintypes.HANDLE,)
    h = k32.OpenProcess(0x1000, False, pid)  # PROCESS_QUERY_LIMITED_INFORMATION
    if h:
        return k32, h
    return None if ctypes.get_last_error() == 5 else (k32, None)  # ERROR_ACCESS_DENIED: it exists


def _linux_stat(pid: int) -> list[str] | None:
    """The fields of /proc/<pid>/stat after the command name (field 3 onwards), None without one."""
    try:
        text = Path(f"/proc/{pid}/stat").read_text()
    except OSError:
        return None
    return text[text.rfind(")") + 2:].split()


def process_alive(pid: int) -> bool | None:
    """Whether a process with this pid is running on this host; None when that cannot be told."""
    if pid <= 0:
        return False
    if sys.platform == "win32":
        try:
            import ctypes
            from ctypes import wintypes
            p = _windows_process(pid)
            if p is None:
                return True
            k32, h = p
            if h is None:
                return False
            try:
                code = wintypes.DWORD()
                if not k32.GetExitCodeProcess(h, ctypes.byref(code)):
                    return None
                return code.value == 259  # STILL_ACTIVE
            finally:
                k32.CloseHandle(h)
        except (OSError, AttributeError):
            return None
    try:
        os.kill(pid, 0)  # signal 0 only tests (never on Windows, where os.kill terminates)
    except ProcessLookupError:
        return False
    except PermissionError:
        return True
    except OSError:
        return None
    stat = _linux_stat(pid)
    return not (stat and stat[0] in ("Z", "X"))  # a zombie has exited


def process_started(pid: int) -> float | None:
    """When the process running under pid started (time.time() scale), None when that cannot be told."""
    try:
        if sys.platform == "win32":
            import ctypes
            from ctypes import wintypes
            p = _windows_process(pid)
            if p is None or p[1] is None:
                return None
            k32, h = p
            try:
                times = [wintypes.FILETIME() for _ in range(4)]
                if not k32.GetProcessTimes(h, *map(ctypes.byref, times)):
                    return None
                t = times[0]
                return ((t.dwHighDateTime << 32) | t.dwLowDateTime) / 1e7 - 11644473600.0  # 1601 -> 1970
            finally:
                k32.CloseHandle(h)
        stat = _linux_stat(pid)
        if stat is None:
            return None
        btime = next(int(ln.split()[1]) for ln in Path("/proc/stat").read_text().splitlines()
                     if ln.startswith("btime "))
        return btime + int(stat[19]) / os.sysconf("SC_CLK_TCK")
    except (OSError, ValueError, IndexError, AttributeError, StopIteration):
        return None


def is_stale(h: Holder) -> bool:
    """True when the holder's process is gone: not running, or its pid now names a newer process."""
    if h.host != socket.gethostname():
        return False
    if process_alive(h.pid) is False:
        return True
    started = process_started(h.pid)
    return started is not None and started > h.started + 2.0  # 2 s: clock tick and rounding slack


def _unlink(path: Path, tries: int = 20) -> None:
    """Delete a file; on Windows a reader holding it open for an instant makes this fail, so retry."""
    for i in range(tries):
        try:
            path.unlink(missing_ok=True)
            return
        except PermissionError:
            if i == tries - 1:
                raise
            time.sleep(0.05)


class RunLock:
    """`with RunLock(directory, "check.py"):` holds directory/check.lock for the block.

    `wait` is in seconds (None: forever, 0: give up at once); `say` prints the waiting lines and the takeover
    note. `poll`, `report_every`, `clock` and `sleep` are for the tests.
    """

    def __init__(self, directory: Path, command: str, wait: float | None = 1800.0, worktree: Path | str = "",
                 say=None, poll: float = 1.0, report_every: float = REPORT_EVERY, clock=time.monotonic,
                 sleep=time.sleep):
        self.dir = Path(directory)
        self.path = self.dir / LOCK_NAME
        self.command = command
        self.wait = wait
        self.worktree = str(worktree or Path.cwd())
        self.say = say or (lambda msg: print(msg, flush=True))
        self.poll, self.report_every, self.clock, self.sleep = poll, report_every, clock, sleep
        self.token: str | None = None  # set while this object holds the lock
        self.inherited = False
        self._env_before: str | None = None

    def where(self) -> str:
        try:
            return self.dir.relative_to(Path(self.worktree)).as_posix()
        except ValueError:
            return str(self.dir)

    def _create(self) -> bool:
        token = uuid.uuid4().hex
        h = Holder(os.getpid(), socket.gethostname(), self.worktree, self.command, time.time(), token)
        try:
            fd = os.open(self.path, os.O_CREAT | os.O_EXCL | os.O_WRONLY, 0o644)
        except FileExistsError:
            return False
        with os.fdopen(fd, "w", encoding="utf-8") as f:
            json.dump(dataclasses.asdict(h), f)
        self.token = token
        return True

    def _take_over(self, h: Holder | None) -> None:
        """Delete a dead holder's lock (h; None: a lock file that stayed unreadable), unless another
        waiter is doing the same: a takeover marker lets one waiter at a time re-read and delete it."""
        marker = self.dir / (LOCK_NAME + ".takeover")
        try:
            fd = os.open(marker, os.O_CREAT | os.O_EXCL | os.O_WRONLY, 0o644)
        except FileExistsError:
            try:
                if time.time() - marker.stat().st_mtime > TAKEOVER_GRACE:
                    _unlink(marker)
            except OSError:
                pass
            return
        os.close(fd)
        try:
            now = read_holder(self.path)
            if h is None:
                if now is not None or not self._unreadable_long():
                    return
            elif now is None or now.token != h.token:
                return  # released or taken over meanwhile
            _unlink(self.path)
            if h is None:
                self.say(f"lock: {self.where()}/{LOCK_NAME} was left unreadable by a run that died; taking it over")
            else:
                self.say(f"lock: taking over {self.where()} from {h.describe()}, which is no longer running")
        finally:
            _unlink(marker)

    def _unreadable_long(self) -> bool:
        try:
            return time.time() - self.path.stat().st_mtime > UNREADABLE_GRACE
        except OSError:
            return False

    def acquire(self) -> None:
        self.dir.mkdir(parents=True, exist_ok=True)
        inherited = os.environ.get(ENV)
        if inherited:
            h = read_holder(self.path)
            if h is not None and h.token == inherited:
                self.inherited = True  # a parent process (verify.py) holds it for this run
                return
        start = self.clock()
        next_report = start
        while True:
            if self._create():
                self._env_before = os.environ.get(ENV)
                os.environ[ENV] = self.token
                return
            h = read_holder(self.path)
            if h is None:
                if self._unreadable_long():
                    self._take_over(None)
                    continue
            elif is_stale(h):
                self._take_over(h)
                continue
            now = self.clock()
            who = h.describe() if h else "a run that is starting"
            if self.wait is not None and now - start >= self.wait:
                gave_up = f"gave up after {self.wait / 60:g} min: " if self.wait else ""
                raise Busy(f"lock: {gave_up}{self.where()} is in use by {who}. Let that run finish, or stop it, "
                           f"then run again (--wait <minutes> sets how long to wait)")
            if self.wait != 0 and now >= next_report:
                left = "" if self.wait is None else f", up to {self.wait / 60:g} min in all (--wait)"
                self.say(f"lock: {self.where()} is in use by {who}; waiting{left}")
                next_report = now + self.report_every
            self.sleep(self.poll)

    def release(self) -> None:
        if self.inherited:
            self.inherited = False
            return
        if self.token is None:
            return
        h = read_holder(self.path)
        if h is not None and h.token == self.token:
            _unlink(self.path)
        if self._env_before is None:
            os.environ.pop(ENV, None)
        else:
            os.environ[ENV] = self._env_before
        self.token = None

    def __enter__(self) -> "RunLock":
        self.acquire()
        return self

    def __exit__(self, *exc) -> None:
        self.release()
