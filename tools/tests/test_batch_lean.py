"""tools/batch/lean/coord.py and try.py: which job a function gets next, what its worker is told, and the
attempt cap (no agents, no game data)."""
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401

KIT = Path(__file__).resolve().parents[1] / "batch" / "lean"


def load(name):
    spec = importlib.util.spec_from_file_location(f"lean_{name}", KIT / f"{name}.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


coord = load("coord")
grader = load("try")


def entry(tier="small", prior=None, attempts=()):
    return {"addr": "7100000100", "size": 200, "tier": tier, "prior": prior, "attempts": list(attempts),
            "file": "src/game/CFoo/CFoo_<What>_l000100.cpp", "status": "queued"}


def attempt(outcome, best=0, model="sonnet"):
    return {"outcome": outcome, "best": best, "model": model, "blocker": "x9 and x10 swapped"}


class Plan(unittest.TestCase):
    def test_first_pass_by_size(self):
        self.assertEqual(coord.plan(entry(), {}), ("sonnet", 2))
        self.assertEqual(coord.plan(entry("large"), {}), ("opus", 3))

    def test_a_match_is_finished(self):
        self.assertIsNone(coord.plan(entry(attempts=[attempt("match", 100)]), {}))

    def test_a_close_non_match_gets_one_retry(self):
        self.assertEqual(coord.plan(entry(attempts=[attempt("differs", 80)]), {}), ("opus", 2))
        self.assertIsNone(coord.plan(entry(attempts=[attempt("differs", 60)]), {}))
        self.assertIsNone(coord.plan(entry(attempts=[attempt("differs", 80), attempt("differs", 90, "opus")]), {}))

    def test_a_registers_only_result_gets_no_model_retry(self):
        # CLAUDE.md, tier 2: a permuter and the machine test take it from there, however close it is
        self.assertIsNone(coord.plan(entry(attempts=[attempt("registers", 98)]), {}))
        self.assertIsNone(coord.plan(entry(attempts=[attempt("minor", 98)]), {}))

    def test_one_model_means_one_job(self):
        cfg = {"model": "sonnet", "budget": 6}
        self.assertEqual(coord.plan(entry("large"), cfg), ("sonnet", 6))
        self.assertIsNone(coord.plan(entry(attempts=[attempt("differs", 90)]), cfg))


class Brief(unittest.TestCase):
    def test_the_worker_is_sent_to_the_tracked_brief(self):
        text = coord.prompt(entry(), "sonnet", 6, {})
        self.assertIn("Follow tools/batch/lean/worker.md exactly", text)
        self.assertIn("Address: 7100000100 (200 bytes); context build/lean/ctx/7100000100.txt", text)
        self.assertIn("Attempt budget: 6. It is a cap", text)
        self.assertIn("after the 6th", text)
        self.assertNotIn("reference matches", text)  # prepare.py --refs 0 wrote none
        self.assertNotIn("NON_MATCHING", text)       # a worker never leaves a non-match in src/
        self.assertNotIn("Ghidra", text)

    def test_ghidra_s_view_is_for_reading(self):
        text = coord.prompt(entry(), "sonnet", 6, {"ghidra": True})
        self.assertIn("build/lean/ghidra/7100000100.txt", text)
        self.assertIn("Never copy its text", text)

    def test_the_reference_matches_are_announced(self):
        text = coord.prompt(entry(), "sonnet", 3, {"refs": 3})
        self.assertIn("The context ends with the reference matches", text)
        self.assertIn("after the 3rd", text)

    def test_an_earlier_run_s_result_is_passed_on(self):
        text = coord.prompt(entry(prior={"best": 71, "blocker": "loop form"}), "sonnet", 2, {})
        self.assertIn("An earlier run reached 71% (blocker: loop form).", text)

    def test_the_brief_keeps_non_matches_out_of_src_and_names_the_forms(self):
        brief = (KIT / "worker.md").read_text(encoding="utf-8")
        for needle in ('"match"|"registers"|"differs"|"skipped"', "build/lean/drafts/<addr>.cpp", "flatten",
                       "Readable first", "tools/batch/lean/try.py", "## Reference matches", "BUDGET SPENT",
                       "`ATTEMPT n of N`"):
            self.assertIn(needle, brief)


def running(budget=3, addr="7100000100", attempts=()):
    return dict(entry(attempts=attempts), addr=addr, status="running", budget=budget)


DIFFERS = ["  87.5%  0x7100000100  fn_7100000100  orig 200 B, ours 204 B, 6 differing insns",
           "  KIND   registers", "0/1 functions match"]
MATCH = [" MATCH  0x7100000100  fn_7100000100  orig 200 B, ours 200 B, 0 differing insns", "1/1 functions match"]


class Cap(unittest.TestCase):
    def state(self, budget=3):
        return {"addr": "7100000100", "job": 0, "budget": budget, "graded": [], "build_failures": 0, "confirm": 0}

    def test_each_graded_run_is_counted_and_the_worker_sees_what_is_left(self):
        s = self.state()
        self.assertEqual(grader.count(s, DIFFERS, 1), "ATTEMPT 1 of 3 graded (best 87.5%). 2 left.")
        self.assertIsNone(grader.spent(s))
        grader.count(s, DIFFERS, 1)
        last = grader.count(s, DIFFERS, 1)
        self.assertIn("ATTEMPT 3 of 3 graded (best 87.5%): the budget is spent.", last)
        self.assertEqual(grader.spent(s), "3 of 3 attempts are graded")
        self.assertIn("BUDGET SPENT: 3 of 3 attempts are graded (best 87.5%, last KIND registers)",
                      grader.spent_line(s, grader.spent(s)))

    def test_a_build_failure_is_free_until_as_many_as_the_budget(self):
        s = self.state(budget=2)
        self.assertIn("BUILD FAILURE 1 (not an attempt; 1 more are free", grader.count(s, ["x.cpp:3: error: no"], 2))
        grader.count(s, ["error"], 2)
        self.assertEqual(s["graded"], [])
        self.assertIn("ATTEMPT 1 of 2 graded (best 0.0%). 1 left.", grader.count(s, ["error"], 2))
        self.assertEqual(s["graded"][0]["kind"], "build failure")

    def test_a_match_is_graded_again_after_the_declaration_fix_and_then_no_more(self):
        s = self.state()
        self.assertIn("ATTEMPT 1 of 3: MATCH.", grader.count(s, MATCH, 0))
        self.assertIsNone(grader.spent(s))
        self.assertIn("CONFIRM run 1 of 2", grader.count(s, MATCH, 0))
        grader.count(s, MATCH, 0)
        self.assertEqual(len(s["graded"]), 1)
        self.assertEqual(grader.spent(s), "the function matched and the confirmation runs are used")

    def test_a_wrong_callee_is_not_a_match(self):
        reloc = [MATCH[0], "  RELOC  call26 at +0x10: fn_7100000200, the original calls fn_7100000300",
                 "0/1 functions match; 1 with a wrong relocation target"]
        self.assertEqual(grader.grade_of(reloc, "7100000100", 1), (100.0, "", False))
        self.assertEqual(grader.grade_of(MATCH, "7100000100", 0), (100.0, "", True))

    def test_another_function_of_the_file_is_not_the_grade(self):
        lines = [" MATCH  0x7100000200  fn_7100000200  orig 8 B, ours 8 B, 0 differing insns", *DIFFERS]
        self.assertEqual(grader.grade_of(lines, "7100000100", 1), (87.5, "registers", False))

    def test_the_job_is_found_by_the_file_name_and_only_while_it_runs(self):
        q = [running(), running(addr="7100000200"), dict(running(addr="7100000300"), status="matched")]
        self.assertEqual(grader.job_of("src/game/CFoo/CFoo_Reset_l000100.cpp", q)["addr"], "7100000100")
        self.assertEqual(grader.job_of("build/lean/drafts/7100000200.cpp", q)["addr"], "7100000200")
        self.assertIsNone(grader.job_of("src/game/CFoo/CFoo_Init_l000300.cpp", q))   # coord.py land grades freely
        self.assertIsNone(grader.job_of("src/game/CFoo/CFoo.cpp", q))

    def test_a_renamed_copy_is_still_the_job_s(self):
        with tempfile.TemporaryDirectory() as tmp:
            f = Path(tmp) / "again.cpp"
            f.write_text("#include \"types.h\"\nvoid fn_7100000300(int);\n\nvoid fn_7100000100(int a) {\n"
                         "    fn_7100000300(a);\n}\n", encoding="utf-8")
            q = [running(), running(addr="7100000300")]
            self.assertEqual(grader.job_of(f, q)["addr"], "7100000100")

    def test_the_count_belongs_to_one_job(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp)
            state, path = grader.load_state(running(), out)
            grader.count(state, DIFFERS, 1)
            path.parent.mkdir(parents=True)
            path.write_text(json.dumps(state), encoding="utf-8")
            again, _ = grader.load_state(running(), out)
            self.assertEqual(len(again["graded"]), 1)
            # coord.record reads the same file for the job it closes ...
            e = running()
            self.assertEqual(coord.graded(e, out)["graded"][0]["score"], 87.5)
            # ... and the retry job of the same function starts at nothing
            retry = running(budget=2, attempts=[attempt("differs", 88)])
            fresh, _ = grader.load_state(retry, out)
            self.assertEqual((fresh["graded"], fresh["budget"], fresh["job"]), ([], 2, 1))
            self.assertIsNone(coord.graded(retry, out))

    def test_try_py_counts_and_then_refuses_to_build(self):
        from unittest import mock
        import contextlib
        import io
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp)
            (out / "queue.json").write_text(json.dumps([running(budget=2)]), encoding="utf-8")
            runs = []

            def fake(cmd, **kw):
                runs.append(cmd)
                return mock.Mock(returncode=1, stdout="\n".join(DIFFERS) + "\n", stderr="")

            printed = []
            with mock.patch.object(grader, "OUT", out), mock.patch.object(grader.subprocess, "run", fake):
                for _ in range(3):
                    buf = io.StringIO()
                    with mock.patch.object(grader.sys, "argv", ["try.py", "src/game/CFoo/CFoo_Get_l000100.cpp"]), \
                            contextlib.redirect_stdout(buf):
                        printed.append((grader.main(), buf.getvalue().strip().splitlines()[-1]))
            self.assertEqual(len(runs), 2)   # the third run did not build
            self.assertEqual(printed[0], (1, "ATTEMPT 1 of 2 graded (best 87.5%). 1 left."))
            self.assertTrue(printed[1][1].startswith("ATTEMPT 2 of 2 graded (best 87.5%): the budget is spent."))
            self.assertEqual(printed[2][0], 3)
            self.assertTrue(printed[2][1].startswith("BUDGET SPENT: 2 of 2 attempts are graded"))
            state = json.loads((out / "attempts" / "7100000100.json").read_text(encoding="utf-8"))
            self.assertEqual([g["score"] for g in state["graded"]], [87.5, 87.5])

    def test_wave_writes_the_budget_try_py_enforces(self):
        q = [entry(), entry("large")]
        q[1]["addr"] = "7100000200"
        jobs = coord.wave(q, 8)
        self.assertEqual([e["budget"] for e in q], [2, 3])
        self.assertIn("Attempt budget: 2. It is a cap", jobs[0]["prompt"])

    def test_stats_say_which_attempt_matched(self):
        q = [dict(entry(attempts=[dict(attempt("match", 100), graded=[60.0, 100.0], matched_on=2)]), status="matched",
                  refs=[{"addr": "7100000900", "score": 0.6}]),
             dict(entry(attempts=[dict(attempt("match", 100), graded=[100.0], matched_on=1)]), status="matched", refs=[]),
             dict(entry(attempts=[dict(attempt("differs", 70), graded=[50.0, 70.0, 70.0], matched_on=None)]),
                  status="failed", refs=[]),
             dict(entry(attempts=[attempt("match", 100)]), status="matched")]   # an older run: not counted
        self.assertEqual(coord.attempt_stats(q), {
            "matched_on_attempt": {"1": 1, "2": 1}, "graded_attempts": 6,
            "references": {"with a reference/matched": 1, "without a reference/failed": 1,
                           "without a reference/matched": 1}})


if __name__ == "__main__":
    unittest.main()
