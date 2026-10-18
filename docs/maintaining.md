# Maintaining the public repository

For the maintainer (`@var-flotsam`) and anyone who becomes one. Contributors read
`CONTRIBUTING.md`; this page covers publication, the GitHub settings that protect `main`, and
what to do when something goes wrong.

## History and the archive

The repository was published on 2026-10-18 with a cleaned history: every commit's author email
was replaced by the GitHub noreply address, and local paths (a user name, where the
maintainer's dump, keys and game files live) were removed from every version of `CLAUDE.md`
and `docs/HANDOFF.md` (`git filter-repo --mailmap --replace-text`). An audit of every blob found
no game files, keys, firmware or assembly listings, and no file over 1 MB.

The original repository, with its unmodified history and pull requests #1–#19, was renamed to
**`var-flotsam/dread-decomp-archive`** and stays private. It is a rollback copy only: never push
to it, and never make it public (its history contains the personal data removed here).

Keep personal data out from now on:
- Commit with the noreply address (`git config user.email <id>+var-flotsam@users.noreply.github.com`)
  and keep "Keep my email addresses private" and "Block command line pushes that expose my
  email" on in GitHub's email settings.
- Machine-specific paths go in the gitignored `CLAUDE.local.md`.
- `tools/policy_check.py` (and CI) rejects home-directory paths with a user name and personal
  email addresses.

## Repository settings

Rulesets need a public repository (or a paid plan):

| Setting | Value | Why |
|---|---|---|
| Default branch | `main` | |
| Merge methods | Merge commits only | The history so far uses merge commits; the ruleset allows only `merge` |
| Automatically delete head branches | On | Keeps upstream to `main` plus open work |
| Actions: fork pull request workflows | Require approval for first-time contributors | Workflows from strangers run only after a look |
| Actions: workflow permissions | Read repository contents | The only workflow needs nothing more |
| Private vulnerability reporting | On | A private channel to report leaked data |
| Wiki / Projects | Off (unless wanted) | Fewer places for data to leak into |

Apply the merge and branch-deletion settings and the ruleset with the GitHub CLI:

```sh
gh repo edit var-flotsam/dread-decomp --enable-merge-commit --enable-squash-merge=false \
    --enable-rebase-merge=false --delete-branch-on-merge
gh api repos/var-flotsam/dread-decomp/rulesets --method POST --input .github/rulesets/main.json
```

## The `main` ruleset (`.github/rulesets/main.json`)

| Rule | Effect |
|---|---|
| Restrict deletions | `main` can't be deleted |
| Block force pushes | History on `main` can't be rewritten |
| Require a pull request | No direct pushes; 1 approving review, from a code owner (`.github/CODEOWNERS`); approvals are dismissed by new pushes; the last push must be approved by someone else; all review threads resolved; merge commits only |
| Require status checks | The `policy` job (`.github/workflows/policy.yml`) must pass |
| Bypass | Repository admins, **through a pull request only**: the maintainer can merge their own PR without a second reviewer, but still can't push to `main` directly |

When there are other maintainers, remove the bypass so every change gets a second pair of eyes.
To change the ruleset, edit the JSON, then
`gh api repos/var-flotsam/dread-decomp/rulesets/<id> --method PUT --input .github/rulesets/main.json`
(`gh api repos/var-flotsam/dread-decomp/rulesets` lists the id).

## Merging a pull request

CI can only check content policy: it has no game data, so it can't build or match. For any PR
that touches `src/`, `include/`, `tools/` or `data/`:

```sh
gh pr checkout <number>
.venv/Scripts/python tools/check.py --version 2.1.0 --update   # no REGRESSION lines
git status --short data/functions                               # should match what the PR committed
```

For generator changes, also rerun the generator and confirm its output is unchanged
(`git status` clean). Then merge on GitHub with a merge commit.

For a stacked PR (based on another PR's branch), change its base to `main` *before* merging and
deleting the lower branch; otherwise GitHub closes it.

## If game data, keys or listings get in

1. Don't quote or copy it anywhere, including the issue that reports it.
2. If it's in an open PR: close the PR and ask the author to delete the branch.
3. If it reached `main`: temporarily disable the ruleset's force-push block, remove the content
   from history with `git filter-repo`, force-push, re-enable the rule, and ask GitHub Support
   to purge cached views of the affected commits and PRs. Tell contributors to re-clone.
4. Rotate nothing: there are no secrets in this repository. If someone posted keys, they are
   not ours to rotate; removal is all we can do.

## Takedown requests

Reply promptly, keep the repository's history intact unless removal is needed, and keep a
private record of the request and what was changed.
