# Maintaining the public repository

For the maintainer (`@var-flotsam`) and anyone who becomes one. Contributors read
`CONTRIBUTING.md`; this page covers publication, the GitHub settings that protect `main`, and
what to do when something goes wrong.

## History and the archive

On 2026-10-18 the repository was recreated with a cleaned history, ready to publish: every commit's author email
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

**Current state: private until the owner decides to publish.** It was briefly public on
2026-10-18 (no forks were made). To publish: make it public, then check that the `main` ruleset
is still there (`gh api repos/var-flotsam/dread-decomp/rulesets`) and re-create it if not
(below). The settings in the next section are already applied.

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

Hosted CI checks content policy and declaration consistency (`tools/decl_check.py --base`), and
`.github/workflows/build.yml` compiles every source twice without game data: with the project
compiler (`tools/check.py --compile-only`, upstream Clang 9.0.1 pinned in `data/toolchains.json`)
and for x86-64 Linux (`tools/port_sweep.py`). It can't diff or match: that needs the dump. For any PR that touches `src/`, `include/`, `tools/` or
`data/`, either the self-hosted check job (next section) is green, or run it by hand:

```sh
gh pr checkout <number>
.venv/Scripts/python tools/check.py --version 2.1.0 --update   # no REGRESSION, DUPLICATE or RELOC lines on hand-written files
.venv/Scripts/python tools/decl_check.py --base origin/main     # PASS: no new conflicting or duplicated declarations
git status --short data/functions                               # should match what the PR committed
```

`RELOC` lines on generated files are the generator's bug, not the PR's, unless the PR changed that
generator. For generator changes, also rerun the generator and confirm its output is unchanged
(`git status` clean). Then merge on GitHub with a merge commit.

**Size of a pull request.** Each PR costs a session start and a review. Prefer one hub cluster
(a hub and the callers it unblocks) or at least about 30 functions per PR; a branch under that
keeps accumulating. The exception is a tool or header change that other streams wait on.

## The self-hosted check job

`.github/workflows/check-local.yml` runs `tools/check.py --update` on a runner you host on the PC
that holds the dump, so merges can rely on a green check instead of a local run. It runs only
while the repository variable `SELF_HOSTED_CHECK` is `true` (set on 2026-10-24) and only for
pull requests from this repository, never from forks. The job never uploads artifacts or logs;
only `check.py`'s own status lines reach GitHub.

**The runner as set up on 2026-10-24** (runner `dread-pc`, label `dread`, in
`%USERPROFILE%\actions-runner`, outside the repository):
- The runner package `actions-runner-win-x64-2.338.1.zip` was verified against GitHub's
  published SHA-256 before extraction and registered with a short-lived token from `gh api`.
- It runs as the signed-in user, not as a service: a hidden process started by the logon
  scheduled task `GitHub Actions runner (dread-decomp)` (Task Scheduler), so it has the user's
  `%LOCALAPPDATA%\dread-decomp` toolchain cache and sees `C:\Program Files\LLVM`. If it is not
  running, start the task or run `run.cmd` in that folder; `_diag\` holds its logs.
- Its checkout, `_work\dread-decomp\dread-decomp`, holds junctions `orig` and `build\recon` to
  this clone's copies. They were added *after* the first job: `actions/checkout` deletes the
  contents of a work folder it did not create itself (a pre-clone with a different remote URL
  counts as foreign; it removed the junction links, not their targets), and only fetches into a
  checkout it made (`clean: false` keeps the ignored junctions). If the checkout is ever
  recreated, let one job run, then add the junctions again. Never remove them with a recursive
  delete; `rmdir` removes only the link.
- `.env` in the runner folder sets `DREAD_PYTHON` to this clone's `.venv\Scripts\python.exe`
  and pins `LOCALAPPDATA`, `APPDATA` and `USERPROFILE` (restart the runner after changing it).
- **The toolchain cache must exist in the real profile.** Shells run by the Claude desktop app
  are sandboxed and virtualize `%LOCALAPPDATA%`: a `tools/fetch_toolchains.py` run from such a
  shell writes to a redirected copy that only those shells see (the folder's ACL carries an
  app-container SID), while the runner, started by Task Scheduler, sees the real profile where
  the folder did not exist. The repository folder is not virtualized. The real
  `%LOCALAPPDATA%\dread-decomp` was populated on 2026-10-24 by staging `toolchains\llvm-9.0.1`
  and `sources\libcxx-9.0.1.src` through `build\cache_export\` and copying from a scheduled task.
  Run `fetch_toolchains.py` and `fetch_sources.py` from a normal terminal to refresh it. The workflow's steps are Windows PowerShell: Git for
  Windows puts only `Git\cmd` on PATH, so `git` works in jobs but `bash` does not; the logon task
  also prepends `C:\Program Files\Git\bin` to the runner's PATH for any future `shell: bash` step
  (the runner's `.path` file is not honoured on Windows).
- To retire it: `config.cmd remove --token <token from gh api ... /actions/runners/remove-token>`,
  delete the scheduled task, then the folder.

Required status checks need a ruleset or branch protection, which a private repository without
GitHub Pro cannot have (`maintaining.md`, "Repository settings"). Until the repository is public:
merge a pull request only when its `check` job is green (or run `check.py` locally), and re-run
the job after merging `main` into the branch. `workflow_dispatch` lets you start a run by hand
from the Actions tab.

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
