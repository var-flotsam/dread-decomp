#!/usr/bin/env bash
# Run a command in the port's Linux build image (port/docker/Dockerfile) over this checkout.
#
#   port/docker/run.sh python3 tools/port_sweep.py --version 2.1.0 --system-stdlib --objects build/port/linux/sweep
#   port/docker/run.sh bash                          # a shell in /src
#
# The checkout is mounted at /src. orig/ (read-only) and build/ are mounted from the paths they
# resolve to, so a worktree whose orig/ and build/ are Windows junctions to the main checkout works
# (Docker does not follow junctions inside a bind mount), and so does a worktree whose own build/
# holds junctions to another checkout's inputs (mounted read-only, below). Set DREAD_PORT_IMAGE to
# use another tag.
# From Git Bash on Windows the paths are converted with cygpath and MSYS path conversion is off.
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
image="${DREAD_PORT_IMAGE:-dread-port-linux}"

resolve() {
    # The real directory behind a path (follows a junction or symlink).
    if command -v cygpath >/dev/null 2>&1; then
        local win target
        win="$(cygpath -w "$1")"
        # PowerShell resolves junctions; Git Bash's readlink does not.
        target="$(powershell.exe -NoProfile -Command \
            "\$i = Get-Item -LiteralPath '$win'; if (\$i.Target) { @(\$i.Target)[0] } else { \$i.FullName }" \
            2>/dev/null | tr -d '\r' | head -1)"
        echo "${target:-$win}"
    else
        (cd "$1" && pwd -P)
    fi
}

host_path() {
    if command -v cygpath >/dev/null 2>&1; then cygpath -w "$1"; else echo "$1"; fi
}

src="$(host_path "$here")"
mounts=(-v "$src:/src")
[ -d "$here/orig" ] && mounts+=(-v "$(resolve "$here/orig"):/src/orig:ro")
[ -d "$here/build" ] && mounts+=(-v "$(resolve "$here/build"):/src/build")
# Junctions or symlinks inside build/ (one or two levels down: build/recon, build/port/lifted) are
# not followed through that mount either: each gets a read-only mount of its own. They are another
# checkout's inputs, which nothing here should write.
if [ -d "$here/build" ] && [ ! -L "$here/build" ]; then
    for link in "$here"/build/* "$here"/build/*/*; do
        [ -L "$link" ] && [ -d "$link" ] || continue
        mounts+=(-v "$(resolve "$link"):/src/build/${link#"$here/build/"}:ro")
    done
fi

tty=()
[ -t 0 ] && [ -t 1 ] && tty=(-it)

# The tools' download cache (tools/hostenv.py cache_dir: toolchains, libc++ 9.0.1) lives in build/cache, so
# it survives the container.
MSYS_NO_PATHCONV=1 exec docker run --rm "${tty[@]}" "${mounts[@]}" -e XDG_CACHE_HOME=/src/build/cache -w /src "$image" "$@"
