"""tools/recomp_trace.py on made-up generated C and a made-up trace (no recomp build, no dump)."""

import unittest

from tools.tests import _path  # noqa: F401
import recomp_trace as rt

# The shape of the recomp translator's output, with invented bodies.
GENERATED = (
    "#include \"cpu.h\"\r\n"
    "void F_8005100(CPU* RESTRICT c, u64 e){\r\n"
    " u8* const M=c->mem;\r\n"
    " if(UNLIKELY(e!=0x8005100)){ CALLDYN(e); return; }\r\n"
    " c->x[0]=1;\r\n"
    "}\r\n"
    "SWR_FN(F_8005200)\r\n"
    " c->x[0]=2;\r\n"
    "}\r\n"
)


class Instrument(unittest.TestCase):
    def test_hooks_each_prologue_once_and_keeps_line_endings(self):
        text, n = rt.instrument_text(GENERATED)
        self.assertEqual(n, 2)
        self.assertIn("dread_trace_hit(0x8005100ull)", text)
        self.assertIn("dread_trace_hit(0x8005200ull)", text)
        self.assertTrue(text.startswith("void dread_trace_hit(unsigned long long); " + rt.TAG + "\r\n"))
        self.assertNotIn("\r\r", text)
        self.assertEqual(text.count("\n"), text.count("\r\n"))
        self.assertEqual(rt.instrument_text(text), (text, 0))  # idempotent

    def test_undo_restores_the_file(self):
        text, _ = rt.instrument_text(GENERATED)
        self.assertEqual(rt.undo_text(text), GENERATED)

    def test_snapshot_guard_only_on_the_chosen_function(self):
        text, n = rt.instrument_text(GENERATED, frozenset({0x8005100}))
        self.assertEqual(n, 2)
        self.assertEqual(text.count("dread_snap_begin(c, 0x8005100ull)"), 1)
        self.assertNotIn("dread_snap_begin(c, 0x8005200ull)", text)
        self.assertEqual(rt.undo_text(text), GENERATED)

    def test_a_file_without_functions_is_left_alone(self):
        self.assertEqual(rt.instrument_text("#include \"cpu.h\"\n"), ("#include \"cpu.h\"\n", 0))


ROWS = [
    {"address": "0x7100000100", "size": "16", "name": "fn_7100000100", "name_source": "unknown",
     "subsystem": "unknown", "status": "O"},
    {"address": "0x7100000200", "size": "32", "name": "CFoo::refl_GetClass", "name_source": "reflection",
     "subsystem": "engine", "status": "U"},
    {"address": "0x7100000300", "size": "8", "name": "fn_7100000300", "name_source": "unknown",
     "subsystem": "unknown", "status": "M"},
    {"address": "0x7100000400", "size": "16", "name": "end", "name_source": "symbol",
     "subsystem": "unknown", "status": "U"},
    {"address": "0x7100000410", "size": "16", "name": "<EXTERNAL>::strlen", "name_source": "symbol",
     "subsystem": "unknown", "status": "U"},
]


class Map(unittest.TestCase):
    def test_order_addresses_and_modules(self):
        trace = ["2 30 7 8005200", "0 10 7 8005100", "1 20 8 7b0c6010", "3 40 7 8005100", "junk line", "4 50 7 8005300"]
        hits = rt.read_trace(trace)
        self.assertEqual([h[3] for h in hits], [0x8005100, 0x7B0C6010, 0x8005200, 0x8005300])
        index = rt.FunctionIndex(ROWS)
        order = rt.map_trace(hits, index, None)
        self.assertEqual([(o["module"], o["address"]) for o in order],
                         [("main", "0x7100000100"), ("sdk", "0x10"), ("main", "0x7100000200"), ("main", "0x7100000300")])
        self.assertEqual(order[2]["name"], "CFoo::refl_GetClass")
        self.assertEqual(order[2]["status"], "U")
        until = index.lookup("CFoo::refl_GetClass")
        self.assertEqual(len(rt.map_trace(hits, index, until)), 3)
        self.assertEqual(index.lookup("fn_7100000300"), 0x7100000300)

    def test_next_starts_after_our_furthest_trap(self):
        index = rt.FunctionIndex(ROWS)
        order = [{k: str(v) for k, v in o.items()} for o in
                 rt.map_trace(rt.read_trace(["0 0 1 8005100", "1 0 1 8005200", "2 0 1 8005300"]), index, None)]
        lines = rt.next_up(order, {"traps": {"order": [{"name": "fn_7100000100"}]}, "aborting_trap": None}, 10)
        self.assertIn("fn_7100000100 at 1 of 3", lines[0])
        self.assertEqual(sum("0x71000002" in l or "0x71000003" in l for l in lines), 2)
        lines = rt.next_up(order, {"traps": {"order": []}, "aborting_trap": "CFoo::refl_GetClass"}, 10)
        self.assertEqual(sum("0x71000003" in l for l in lines), 1)
        self.assertFalse(any("0x71000002" in l for l in lines[1:]))


    def test_import_stubs_are_marked_and_left_out_of_next(self):
        index = rt.FunctionIndex(ROWS)
        order = rt.map_trace(rt.read_trace(["0 0 1 8005400", "1 0 1 8005410", "2 0 1 8005200"]), index, None)
        self.assertEqual([o["import"] for o in order], [True, True, False])
        self.assertIn("2 import stubs", rt.summary(order)[0])
        lines = rt.next_up([{k: str(v) for k, v in o.items()} for o in order], None, 10)
        self.assertEqual(sum("0x71000004" in l for l in lines), 0)
        self.assertEqual(sum("0x7100000200" in l for l in lines), 1)


if __name__ == "__main__":
    unittest.main()
