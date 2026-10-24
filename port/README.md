# port/

The PC port layer: the `nn::` runtime, the NVN graphics shim, audio, and the x86-64 build that
links the matched and generated sources into one program. It is empty until the owner decides
between the two routes in [`docs/PORT_PLAN.md`](../docs/PORT_PLAN.md); that page lists what the
layer must provide (from the game's SDK imports and RomFS), the milestones and their gates.

Rules that apply here as everywhere: no game data in git (the port reads the user's own dump at
build time), our own code only, and nothing from `lib/nnsdk` copied (it is referenced as a
submodule and the port must not depend on it without asking its maintainers).
