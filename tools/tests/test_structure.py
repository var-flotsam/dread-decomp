"""asmdiff.structure: opcode order, frame size and stack slots, on hand-encoded AArch64 words."""
import struct
import unittest

from tools import asmdiff


def code(*words: int) -> bytes:
    return struct.pack(f"<{len(words)}I", *words)


SUB_SP_0x20 = 0xD10083FF   # sub sp, sp, #0x20
ADD_SP_0x20 = 0x910083FF   # add sp, sp, #0x20
STR_W0_SP4 = 0xB90007E0    # str w0, [sp, #4]
STR_W0_SP8 = 0xB9000BE0    # str w0, [sp, #8]
NOP = 0xD503201F
RET = 0xD65F03C0


class StructureTest(unittest.TestCase):
    def test_same_code(self):
        c = code(SUB_SP_0x20, STR_W0_SP4, ADD_SP_0x20, RET)
        line = asmdiff.structure(c, c)
        self.assertIn("opcodes 100.0% in the same order", line)
        self.assertIn("frame 0x20 vs 0x20", line)
        self.assertIn("1 vs 1 stack slots", line)

    def test_extra_instruction_and_slot(self):
        orig = code(SUB_SP_0x20, STR_W0_SP4, ADD_SP_0x20, RET)
        ours = code(SUB_SP_0x20, NOP, STR_W0_SP4, STR_W0_SP8, ADD_SP_0x20, RET)
        line = asmdiff.structure(orig, ours)
        self.assertIn("opcodes 80.0% in the same order", line)
        self.assertIn("1 vs 2 stack slots", line)


if __name__ == "__main__":
    unittest.main()
