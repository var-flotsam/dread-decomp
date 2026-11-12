"""tools/permute.py: the text side (finding a body, the changes it tries, the search and the two passes that
keep a match readable). No compiler and no game data: `cost` and `matches` are stand-ins."""
import random
import unittest

from tools.tests import _path  # noqa: F401
import permute

SRC = """// A comment.
void fn_7100000100(UnkA* self);
// NON_MATCHING(minor): registers.
// The original loads b first.
void fn_7100000100(UnkA* self) {
    u32 a = self->a;
    u32 b = self->b;
    if (a < b) {
        self->c = a + b;
    }
    self->d = 0;
    return;
}

void fn_7100000200() {
    fn_7100000100(nullptr);
}
""".split("\n")


class Text(unittest.TestCase):
    def test_the_definition_is_found_not_the_declaration_or_the_call(self):
        self.assertEqual(permute.definition_line(SRC, "7100000100"), 4)
        self.assertEqual(permute.body_span(SRC, "7100000100"), (5, 11))
        self.assertEqual(permute.body_span(SRC, "7100000200"), (15, 15))
        self.assertIsNone(permute.body_span(SRC, "7100000300"))

    def test_what_may_be_moved(self):
        self.assertTrue(permute.simple("    u32 a = self->a;"))
        self.assertTrue(permute.simple("    self->d = 0;  // reset"))
        for line in ("    return;", "    if (a < b) {", "    }", "    for (u32 i = 0; i < n; ++i)", "    // a = b;",
                     "    break;", "#define X 1;"):
            self.assertFalse(permute.simple(line), line)

    def change(self, kind, seed=0, lines=SRC):
        lo, hi = permute.body_span(lines, "7100000100")
        return permute.mutate(lines, lo, hi, random.Random(seed), kind)

    def test_swap_exchanges_two_adjacent_statements_of_one_block(self):
        out = self.change("swap")
        self.assertEqual(out[5:7], ["    u32 b = self->b;", "    u32 a = self->a;"])
        self.assertEqual(len(out), len(SRC))

    def test_split_and_merge_are_each_other_s_reverse(self):
        out = self.change("split", seed=1)
        self.assertEqual(len(out), len(SRC) + 1)
        i = next(k for k, line in enumerate(out) if permute.DECL_BARE.match(line))
        name = permute.DECL_BARE.match(out[i]).group(3)
        self.assertEqual(out[i + 1].strip(), f"{name} = self->{name};")
        lo, hi = permute.body_span(out, "7100000100")
        self.assertEqual(permute.mutate(out, lo, hi, random.Random(0), "merge"), SRC)

    def test_commute_and_compare_rewrite_one_expression(self):
        self.assertEqual(self.change("commute")[8], "        self->c = b + a;")
        self.assertEqual(self.change("compare")[7], "    if (b > a) {")

    def test_a_change_with_no_place_is_none(self):
        only = "void fn_7100000100() {\n    return;\n}\n".split("\n")
        for kind in ("swap", "move", "split", "merge", "commute", "compare"):
            self.assertIsNone(permute.mutate(only, 1, 1, random.Random(0), kind), kind)


class Search(unittest.TestCase):
    WANT = "    u32 b = self->b;"

    def cost(self, lines):
        # the stand-in for the compiler: the function "matches" when b is loaded before a
        body = lines[slice(*[x + k for k, x in zip((0, 1), permute.body_span(lines, "7100000100"))])]
        if not any(line.strip().startswith("return") for line in body):
            return None, False
        ok = body.index(self.WANT) < body.index("    u32 a = self->a;") if self.WANT in body and \
            "    u32 a = self->a;" in body else False
        return (0 if ok else 2), ok

    def test_the_search_stops_at_a_match(self):
        row = permute.search(SRC, "7100000100", self.cost, 400, random.Random(3))
        self.assertTrue(row["matched"])
        self.assertEqual((row["start"], row["best"]), (2, 0))
        self.assertLessEqual(row["tries"], 400)
        self.assertTrue(self.cost(row["lines"])[1])

    def test_a_function_that_matches_or_has_no_body_is_not_searched(self):
        done = permute.search(SRC, "7100000100", lambda lines: (0, True), 10, random.Random(0))
        self.assertEqual((done["note"], done["tries"]), ("already matches", 0))
        none = permute.search(SRC, "7100000300", lambda lines: (5, False), 10, random.Random(0))
        self.assertEqual(none["note"], "no body found")
        broken = permute.search(SRC, "7100000100", lambda lines: (None, False), 10, random.Random(0))
        self.assertEqual(broken["note"], "does not build")


class Readable(unittest.TestCase):
    def test_changes_the_match_does_not_need_are_put_back(self):
        match = list(SRC)
        match[5], match[6] = match[6], match[5]            # the change that matters
        match[7] = "    if (b > a) {"                      # two the search picked up on the way
        match[8] = "        self->c = b + a;"

        def matches(lines):
            a, b = "    u32 a = self->a;", "    u32 b = self->b;"
            text = "\n".join(lines)
            if lines.count(a) != 1 or lines.count(b) != 1 or text.count("{") != text.count("}"):
                return False                               # half a move put back, a brace lost: does not build
            return lines.index(b) < lines.index(a)
        small = permute.minimise(SRC, match, matches)
        self.assertEqual(small[7:9], SRC[7:9])
        self.assertTrue(matches(small))
        self.assertNotEqual(small, SRC)

    def test_a_line_left_behind_that_does_nothing_goes(self):
        match = list(SRC)
        match[5], match[6] = match[6], match[5]
        match.insert(7, "    a = self->a;")                # what a split and a put-back leave: assigned twice

        def matches(lines):
            a, b = "    u32 a = self->a;", "    u32 b = self->b;"
            return lines.count(a) == 1 and lines.count(b) == 1 and lines.index(b) < lines.index(a)
        small = permute.minimise(SRC, match, matches)
        self.assertNotIn("    a = self->a;", small)
        self.assertEqual(len(small), len(SRC))

    def test_a_split_declaration_is_merged_again_where_the_match_survives(self):
        lines = list(SRC)
        lines[5:7] = ["    u32 a;", "    u32 b = self->b;", "    a = self->a;"]
        merged = permute.merge_declarations(lines, "7100000100", lambda x: True)
        self.assertEqual(merged[5:7], ["    u32 b = self->b;", "    u32 a = self->a;"])
        kept = permute.merge_declarations(lines, "7100000100", lambda x: False)
        self.assertEqual(kept, lines)
        # a use between the declaration and the assignment keeps them apart
        used = list(SRC)
        used[5:7] = ["    u32 a;", "    u32 b = Use(&a);", "    a = self->a;"]
        self.assertEqual(permute.merge_declarations(used, "7100000100", lambda x: True), used)

    def test_a_match_goes_into_the_file_without_its_note(self):
        match = list(SRC)
        match[5], match[6] = match[6], match[5]
        new, removed = permute.put_back(SRC, match, "7100000100")
        self.assertEqual(removed, 2)
        self.assertEqual(new[:3], ["// A comment.", "void fn_7100000100(UnkA* self);", "void fn_7100000100(UnkA* self) {"])
        self.assertEqual(new[3:5], ["    u32 b = self->b;", "    u32 a = self->a;"])
        self.assertEqual(new[-5:], SRC[-5:])               # the next function is untouched
        with self.assertRaises(ValueError):
            permute.put_back(SRC, match, "7100000300")


if __name__ == "__main__":
    unittest.main()
