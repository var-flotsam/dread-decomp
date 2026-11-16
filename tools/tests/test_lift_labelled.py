"""tools/gen_lift.py's labelled_inline: a member function named fn_X by an asm label counts as defined
outside src/lifted/ when the same file defines it, so --prune drops its lifted copy. No game data.
Skipped when pyelftools or capstone is not installed."""

import unittest

from tools.tests import _path  # noqa: F401

try:
    import gen_lift
except ImportError:  # pyelftools or capstone missing
    gen_lift = None


@unittest.skipIf(gen_lift is None, "gen_lift needs pyelftools and capstone")
class LabelledInline(unittest.TestCase):
    def test_destructor_defined_in_the_same_header(self):
        text = (
            "struct T {\n"
            "    ~T() asm(\"fn_7100de9f54\");\n"
            "};\n"
            "inline T::~T() {\n"
            "    Free();\n"
            "}\n"
        )
        self.assertEqual(gen_lift.labelled_inline(text), {0x7100de9f54})

    def test_label_without_a_definition_is_not_counted(self):
        # Declared here, defined in another file (or only in the original): the lifted copy stays.
        text = "struct T {\n    T(const T& other) asm(\"fn_7100fddf18\");\n};\n"
        self.assertEqual(gen_lift.labelled_inline(text), set())


if __name__ == "__main__":
    unittest.main()
