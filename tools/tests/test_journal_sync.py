"""tools/journal_sync.py: journal entries from pull request descriptions. Made-up text; no network."""
import unittest

from tools.tests import _path  # noqa: F401
import journal_sync

SHA_A = "a" * 40
SHA_B = "b" * 40

BODY = """<!-- claude-projects-attribution: {"github_login":"someone"} -->
_Requested by **someone** · [project thread](https://example.invalid)_

## What this changes

Three functions of unit 12 match.

- `fn_7100000100` returns its node.
- `fn_7100000200` is held in drafts.

## Progress

<!-- tools/progress.py before -> after -->
O 10 -> 13.

## Checklist

- [x] No game data.

🤖 Generated with [Claude Code](https://claude.com/claude-code)
"""


class Description(unittest.TestCase):
    def test_only_the_what_this_changes_section(self):
        self.assertEqual(journal_sync.description(BODY),
                         "Three functions of unit 12 match.\n\n- `fn_7100000100` returns its node.\n"
                         "- `fn_7100000200` is held in drafts.")

    def test_a_body_without_the_section_loses_attribution_checklist_and_footer(self):
        body = ("_Requested by **x**_\n\nRenamed a tool.\n\n## Checklist\n\n- [x] done\n\n"
                "🤖 Generated with [Claude Code](https://claude.com/claude-code)\n")
        self.assertEqual(journal_sync.description(body), "Renamed a tool.")

    def test_an_empty_body(self):
        self.assertEqual(journal_sync.description(None), "")


class Entry(unittest.TestCase):
    def test_one_paragraph_follows_the_bold_title(self):
        self.assertEqual(journal_sync.entry(7, "Match two things.", "2026-11-16", "## What this changes\n\nThey match.\n"),
                         "**Match two things (#7, 2026-11-16).** They match.")

    def test_several_paragraphs_go_under_the_title(self):
        e = journal_sync.entry(8, "Batch", "2026-11-16", BODY)
        self.assertTrue(e.startswith("**Batch (#8, 2026-11-16).**\n\nThree functions"))

    def test_no_description_leaves_the_title(self):
        self.assertEqual(journal_sync.entry(9, "Fix", "2026-11-16", ""), "**Fix (#9, 2026-11-16).**")


class Insert(unittest.TestCase):
    JOURNAL = (f"# Journal\n\n## Session log (newest first)\n\n<!-- journal_sync: through {SHA_A} -->\n\n"
               "**Older entry.** Text.\n")

    def test_entries_go_under_the_marker_and_the_marker_moves(self):
        out = journal_sync.insert(self.JOURNAL, ["**New (#2).** B.", "**Newer? no, older (#1).** A."], SHA_B)
        self.assertEqual(out, f"# Journal\n\n## Session log (newest first)\n\n<!-- journal_sync: through {SHA_B} -->\n\n"
                              "**New (#2).** B.\n\n**Newer? no, older (#1).** A.\n\n**Older entry.** Text.\n")

    def test_a_journal_without_the_marker_is_refused(self):
        with self.assertRaises(SystemExit):
            journal_sync.insert("# Journal\n", ["x"], SHA_B)


if __name__ == "__main__":
    unittest.main()
