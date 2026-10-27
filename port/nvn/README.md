# port/nvn/: the NVN shim over Vulkan

Milestone M2 of the PC port (`docs/PORT_PLAN.md`, decision 3: Vulkan first, shaders compiled to
SPIR-V at build time). The game drives the GPU through NVN, which it loads as a table of function
pointers (`fn_7100154770` asks `nvnDeviceGetProcAddress` for 523 names and stores each in a global).
The shim provides those functions on Vulkan. This directory holds the parts that exist so far:
the entry layer (the bootstrap loader and `nvnDeviceGetProcAddress`, a named trap for every
function not yet written), the device, memory pools and buffers, queues and syncs, command buffers
(their memory, recording and bindings), the fixed-function state objects, the window, textures
and the texture and sampler pools and samplers; the shader manifest and the binding layer.
Programs, vertex state, pipelines and draws, clears and render targets, and presentation to a real
window come next (the 38 functions the game calls that the shim lacks, all on the drawing side). `data/nvn_procs.csv`
(`tools/nvn_procs.py`) lists the 523 functions with the global each fills (the `g_unk_716e29xxxx`
of `src/game_s7/fn_71001587f0.cpp`), the number of game call sites (146 have one) and whether the
shim implements it (143, 108 of the 146 the game calls).

Rules: nothing here comes from NVN's SDK headers or from an emulator. Every NVN type, constant and
argument order is a placeholder inferred from the game's own calls, and the file that uses it names
the call site. The Vulkan side compiles against the Khronos headers, pinned in
`tools/fetch_toolchains.py` (`vulkan-headers-1.4.328`, `data/toolchains.json`). No Vulkan loader
or SDK is needed to compile.

| File | Contents |
|---|---|
| `include/nvn_shim/procs.h`, `src/procs.cpp` | the entry layer: `nvnBootstrapLoader`, the name table, the trap pool |
| `include/nvn_shim/device.h`, `src/device.cpp` | the device over Vulkan (the shim's one instance, device and queue), `nvnDeviceGetInteger`'s names and answers |
| `include/nvn_shim/memory.h`, `src/memory.cpp` | memory pools (a GPU address range and a host-visible Vulkan buffer each) and buffers; the binding layer's address resolution |
| `include/nvn_shim/queue.h`, `src/queue.cpp` | queues, syncs, submit, fence, present (counted) |
| `include/nvn_shim/command.h`, `src/command.cpp` | command buffers: memory, recording, viewport and scissor, program, uniform-buffer and texture bindings |
| `src/state.cpp` | the blend, channel-mask, colour, depth-stencil, multisample and polygon state objects (values kept as given) |
| `include/nvn_shim/window.h`, `src/window.cpp` | the window: its textures in turn, the present interval |
| `include/nvn_shim/texture.h`, `src/texture.cpp` | textures as Vulkan images (the format and target table with its evidence), the texture and sampler pools, samplers, texture handles, the binding layer's resolver over all of it |
| `src/objects.h` | the side tables that hold the shim's state for the game's objects |
| `include/nvn_shim/nvn_types.h` | the NVN values the binding layer handles: stages, stage bits, buffer addresses, texture handles, `nvnProgramSetShaders`' stage array |
| `include/nvn_shim/shader_manifest.h`, `src/shader_manifest.cpp` | reads `shaders.manifest` (written by `tools/bshdat.py --install`) and finds a stage by its control section |
| `include/nvn_shim/bindings.h`, `src/bindings.cpp` | descriptor set layouts per stage; NVN's per-stage slots turned into descriptor writes |
| `check/manifest_check.cpp` | `nvn_manifest_check`: runs the manifest reader and the binding layer over the real build output (below) |
| `check/vulkan_check.cpp` | `nvn_vulkan_check`: the same against a real Vulkan implementation (lavapipe), with the validation layers: modules, layouts, descriptor sets, pipelines (below) |
| `check/startup_check.cpp` | `nvn_startup_check`: the game's graphics start-up and three frames replayed against the shim (below) |
| `check/spirv_scan.h` | the checks' own SPIR-V reader (bindings, interface variables): the reference the manifest is compared with |
| `CMakeLists.txt` | `dread_nvn` (static library), built when `DREAD_VULKAN_INCLUDE` is set; `nvn_manifest_check` when `DREAD_LINK` is on; `nvn_vulkan_check` and `nvn_startup_check` when a Vulkan loader is found too |

```sh
python tools/fetch_toolchains.py vulkan-headers-1.4.328    # once; the configure finds them in the tool cache
cmake --build build/port/cmake --target shaders dread_nvn  # Windows: compile only (no Windows SDK)
```

In the port's Linux image (`port/README.md`, "Linux in Docker"), which links, after the `shaders`
target has run on the host (the image has no glslang; `build/` is shared):

```sh
port/docker/run.sh python3 tools/fetch_toolchains.py vulkan-headers-1.4.328   # once, into build/cache
port/docker/run.sh cmake -S port -B build/port/linux/cmake-nvn -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DDREAD_SHADERS=OFF
port/docker/run.sh cmake --build build/port/linux/cmake-nvn --target nvn_manifest_check
port/docker/run.sh build/port/linux/cmake-nvn/nvn/nvn_manifest_check build/port/cmake/shaders orig/2.1.0/romfs/system/shd
```

The first argument is the host build's `DREAD_SHADER_DIR` (`<host build dir>/shaders`).

Against Vulkan, in the port's Vulkan image (`port/docker/vulkan/Dockerfile`: the base image plus
Ubuntu's Vulkan loader, Mesa's lavapipe and the Khronos validation layers; no GPU needed):

```sh
docker build -t dread-port-linux-vulkan port/docker/vulkan
DREAD_PORT_IMAGE=dread-port-linux-vulkan port/docker/run.sh cmake -S port -B build/port/linux/cmake-vk -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DDREAD_SHADERS=OFF
DREAD_PORT_IMAGE=dread-port-linux-vulkan port/docker/run.sh cmake --build build/port/linux/cmake-vk --target nvn_vulkan_check
DREAD_PORT_IMAGE=dread-port-linux-vulkan port/docker/run.sh build/port/linux/cmake-vk/nvn/nvn_vulkan_check build/port/cmake/shaders
```

## The start-up and the frame (what the shim's parts are written against)

The graphics start-up `fn_71001587f0` is matched (`m`) in `src/game_s7/`, so its NVN calls are
known with their arguments; `data/nvn_procs.csv` names the global each goes through. In order:

1. `nvnBootstrapLoader("nvnDeviceGetProcAddress")`, the table loader `fn_7100154770` with no device,
   `nvnDeviceGetInteger(null, 0 / 1)` (the API version, unused), the device builder (defaults,
   flags 0), `nvnDeviceInitialize`, the table loader again with the device.
2. The memory pools of `fn_7100153350` and `fn_710015385c` (each: builder defaults, device, flags
   0x22, storage from the game's allocator, 4 KB aligned; `MemoryPoolInitialize` on a 0x100-byte
   NVN object the engine keeps at +8 of its own 0x120-byte record), the depth mode 0, the window
   origin mode 1, three fast-clear colours and a fast-clear depth.
3. The queue: builder, compute memory 0, command memory and flush threshold
   `nvnDeviceGetInteger(0x51)`, queue memory of `nvnQueueBuilderGetQueueMemorySize` from the game's
   allocator, `nvnQueueInitialize`.
4. Per command buffer (`fn_7100153d90` of them): a 15 MB command memory pool aligned as
   `nvnDeviceGetInteger(0x13)`, 2 MB of control memory aligned as `nvnDeviceGetInteger(0x14)`,
   `CommandBufferInitialize`, `AddCommandMemory(cmd, pool, 0, 0xf00000)`,
   `AddControlMemory(cmd, memory, 0x200000)`, `SetMemoryCallback`.
5. Two syncs, the six state objects with the game's settings, the buffer and texture builders, the
   window builder, a fence and flush and wait, the engine's own set-up (`fn_7100161520` ...
   `fn_710015bde0`: the texture and sampler pools, `fn_710015f100`), the depth target and two colour
   targets (`fn_710015c1e0`, `fn_7100160f60`: texture builder, storage from a pool,
   `TextureInitialize`, `TexturePoolRegisterTexture`), `WindowBuilderSetTextures(builder, 2, ...)`,
   `WindowInitialize`, the present interval 1, a viewport and scissor, the first `WindowAcquireTexture`.

Each frame: `fn_7100159920` re-adds the command and control memory and begins recording;
`fn_7100159ac0` ends it (a handle), waits on two syncs, `QueueSubmitCommands(queue, 1, &handle)`,
fences and flushes; `fn_7100159bf0` presents (`QueuePresentTexture(queue, window, index)`) and
acquires the next texture. No caller looks at a sync wait's or an acquire's result.

`nvn_startup_check` replays all of that against the shim (as far as each part exists) and three
frames, through `nvnBootstrapLoader` and `nvnDeviceGetProcAddress` as the game does, with the game's
objects in storage of its own. On lavapipe with the validation layers (2026-10-26): every check
passes (the device and its Vulkan backend, all eleven integers with the pool sizes computed as
`fn_710015f100` does, the engine's five pools of `fn_7100153350` and two 15 MB command pools with
no Vulkan buffer until one is used, builders keeping the device set before their defaults, a buffer whose address, map and Vulkan buffer agree, three frames of
submits, fences and presents, the window's alternating textures, the device finalised with pools
alive and then everything else, validation counted after the device is gone), no validation
message (reintroducing the teardown leak or a builder's lost device fails it), and, since the
texture part, no unimplemented function at all: the texture and sampler pools sized as
`fn_710015f100` sizes them, a sampler made, registered and finalised at once as `fn_710015e9fc`
does (the pool keeps its own Vulkan sampler), the depth target (`0x35`, a D32/S8 image) and two
RGBA8 colour targets as `fn_7100160f60` makes them, a cube target as `fn_710015f880` makes it (six
layers), an unknown format failing by name, and a texture handle resolving through the binding
layer's resolver to the target's view and the sampler.

What the parts do so far, and what they do not yet:

- **Device.** `nvnDeviceInitialize` creates the shim's one Vulkan instance, device and graphics queue
  (Vulkan 1.3, `dynamicRendering`, `maintenance4`, queried first; `$DREAD_NVN_VALIDATION=1` adds the
  validation layers). `nvnDeviceGetInteger` answers the eleven names the game asks (listed with
  their call sites in `device.h`; the values are the shim's own, chosen for how the game uses them)
  and reports any other name once.
- **Memory.** A pool gets a 64 KB-aligned range of a GPU address space from 4 GB up and, when
  the GPU first needs it, a host-visible Vulkan buffer of its size (the engine's own pools of
  `fn_7100153350` are 1.5 GB together; mirroring them up front would double that).
  `nvnDeviceFinalize` releases the pools' Vulkan objects before the device goes. A buffer's address is its pool's plus its offset;
  `nvnBufferMap` is the game's own storage plus the offset. The game writes into its storage;
  `SyncPoolRange` copies a range into the Vulkan buffer, which the submit will call once draws
  exist. `ResolveGpuAddress` is the binding layer's buffer resolution.
- **Queue and sync.** Submits, fences and presents are counted; a fence is signalled when set, since
  nothing runs on the GPU yet. They become a Vulkan submit and a timeline semaphore with the draws.
- **Command buffers** keep their memory, viewport, scissor, sample mask, pools, program and the
  binding layer's `BindingState`; `EndRecording` hands out a handle. No Vulkan command buffer yet.
- **State objects** keep NVN's values as the game passes them, to be translated by the pipeline part.
- **Window** hands out its textures in turn (the game's colour targets, Vulkan images now); there is
  no Vulkan surface yet (the port's window is `nn::vi`'s).
- **Textures.** Each is a Vulkan image of the format and target the table in `texture.h` maps,
  with the evidence for each number: the formats of the game's own `.bctex` files are named by
  mercury-engine-data-structures (`0x1` R8, `0xd` R8G8, `0x25` R8G8B8A8, `0x42` BC1, `0x44` BC3,
  `0x4b` BC5, `0x50` BC6H, `0x6d` B8G8R8A8); the depth target's `0x35` and `0x33` are depth formats
  by use (`fn_71001604e0` swizzles both R,R,R,A), provisionally D32 float with 8-bit stencil; the
  engine's render-target table (`0x7101647fec`, 17 entries) also names `0x19`, `0xa`, `0x29`,
  `0x2a`, `0x2e`, `0x3f`, `0x32`, `0x34`, `0x36`, which stay unknown and fail by name until a use
  shows what they are. Target 1 is 2D and 8 a cube map: over 1,500 sampled `.bctex`, 1,411 are
  target 1 and 89 target 8, every target-8 one square, six faces, BC6H, a cube/IBL/environment
  map. Swizzles are the game's 0-5 (zero, one, R, G, B, A). Asset (packaged) texture data is kept
  by pointer, not uploaded yet: it is in the console's tiled layout. Samplers are linear and
  repeat for now, with the game's LOD clamp, bias and compare: the filter, wrap and compare
  numbers are the engine's packing of NVN's enumerations, which nothing in the game names yet.
- **Not joined to the game's link yet.** `tools/port_link.py --executable` compiles `port/runtime/`
  only; the boot traps long before graphics (`port/README.md`). Once it reaches `fn_71001587f0`,
  `dread_nvn` (and `-lvulkan`) joins the executable, and `nvnBootstrapLoader` resolves to the shim's.

## What the game does with NVN (the parts the bindings depend on)

Each finding comes from the game's own code, read with `tools/ghidra/decompile.py`, and from the
call sites of each NVN table slot: code outside the loader loads 150 of the 523 slots.

- **Programs.** A `.bshdat` package is a `msapi::res::CProgramResource` (the `.bshdat` extension
  is registered in `fn_71001b2e60`). Its loader (`fn_71001b0050`) creates every program of the package
  up front. For each stage it calls the engine's shader wrapper (`fn_710015c9b0`), which reads the
  stage blob's reflection section itself and lays out the uniform blocks. It then copies the code
  part of section 0 into GPU memory and keeps a pointer to the control part. The program creation
  (`fn_710015d760`) calls `nvnProgramInitialize`, then `nvnProgramSetShaders` with one
  `{code address, control pointer}` pair per present stage. The program record's third, always
  empty, slot is the engine's stage type 2, bound as NVN stage 2 with stage bit 4 (a geometry stage).
- **Uniform buffers.** The program binding (`fn_710015d900`) calls `nvnCommandBufferBindProgram(cmd,
  program, stage bits)`. For a depth-only pass it then unbinds the fragment stage
  (`nvnCommandBufferBindProgram(cmd, null, 2)`) and leaves its blocks alone, so the pipeline layer
  needs vertex-only pipelines. When asked to, and only when the program or that flag changed, it
  then calls, for each active block of each bound stage,
  `nvnCommandBufferBindUniformBuffer(cmd, stage, slot, address, size)`. The slot is the block's
  binding in the stage's reflection; the address is one of the block's per-frame buffers. Contents are
  written with `nvnCommandBufferUpdateUniformBuffer` (`fn_710015e700`).
- **Textures.** `fn_7100161250` binds texture slots 0 to 15 of each stage (`fn_71001606f0` and
  `fn_710015f500` bind single slots) with
  `nvnCommandBufferBindTexture(cmd, stage, slot, nvnDeviceGetTextureHandle(device, texture pool
  index, sampler pool index))`. Only those four binding calls are used: no separate textures or
  samplers, no images, no storage buffers, no bindless handles in any shader.
- **Variant selection** is the engine's own, on the packages it loads (below). The shim never
  selects a program; it only recognises the stages it is given.

## The binding design

NVN numbers uniform blocks and textures per stage, in two separate spaces. Vulkan has one
(set, binding) space per pipeline layout. `tools/bshdat.py --install` compiles every stage with one
descriptor set per stage (vertex: set 0, fragment: set 1), the uniform blocks at the bindings NVN's
compiler gave them (the stage's active blocks 0, 1, 2, ... in declaration order), and the combined
image-samplers at their declared binding plus 16. It then checks each module against the NVN
reflection, so these mappings hold for every stage of the dump:

| NVN, per stage `s` | Vulkan |
|---|---|
| uniform buffer slot `b` | set `S(s)`, binding `b`, `UNIFORM_BUFFER`, descriptorCount 1 |
| texture slot `t`, in a sampler (array) declared at `t0` with `n` elements | set `S(s)`, binding `16 + t0`, array element `t - t0`, `COMBINED_IMAGE_SAMPLER`, descriptorCount `n` |
| texture handle `h` | image view of texture pool entry `TextureId(h)` + sampler pool entry `SamplerId(h)` |
| a block or sampler the NVN compiler found unused but the SPIR-V declares (manifest slot `-1`) | its binding, with a placeholder descriptor: a buffer at least as large as the block (the manifest gives its SPIR-V size rounded up to 16, as NVN rounds active blocks), a placeholder image |

- **Finding the stage.** `nvnProgramSetShaders` gives the shim only a code address and a control
  pointer. The manifest keys each stage by FNV-1a 64 over the first 2,048 bytes of its control
  section (`ShaderManifest::FindStage`). On 2.1.0 every control section is at least 2,320 bytes; the
  key separates all 1,010 distinct control sections of the 1,342 stages; and stages that share a
  control section share their code and binding layout. The install checks all three and fails
  otherwise.
- **Pipeline layout.** A program's layout is two set layouts, from `StageLayoutBindings(vertex)`
  and `StageLayoutBindings(fragment)`, with no push constants. Pipelines are created per program
  and per fixed-function state; caching them is the pipeline layer's job.
- **Binding state.** `BindingState` records what the game has bound, per stage and slot, as NVN
  leaves it: binding a program does not reset it. Before a draw, the shim calls `BuildWrites` for
  each stage whose bindings or program changed. It allocates a set from a per-frame pool, writes it
  and binds it. A slot the shader reads but the game left empty, or an address or pool index the
  resolver does not know, gets a placeholder and a warning; the set is always complete. NVN leaves
  that case undefined, and the game is not expected to hit it.
- **Texture handles** are the shim's own encoding: texture index low, sampler index high. The game
  treats them as opaque and only passes them back to `nvnCommandBufferBindTexture`.
- **Device requirements.** Vulkan 1.3 with `dynamicRendering` (what the checks use) and
  `maintenance4`: two programs (`pp_fxaa_nx`, `pp_deferredambient_nx`) write a wider vector from the
  vertex stage than the fragment stage reads, which GL allows and Vulkan only with `maintenance4`.
- **Interfaces.** GL and NVN let a fragment stage declare inputs the vertex stage does not write, as
  long as nothing reads them; Vulkan rejects the pipeline. The build step drops those (inactive in
  NVN's reflection and not covered by a vertex output: 32 inputs in 16 deferred-lighting and ambient
  packages); the interface is otherwise the compiled shaders' own. The install then reads both
  modules' interfaces back (`spirv_interface`) and fails if a fragment input is still unwritten,
  of another component type or wider than the vertex output (`interface_problems`), so the build
  step, not only `nvn_vulkan_check`, enforces the rule.
- **Limits.** At most five blocks and fourteen texture slots per stage (slots 0 to 13). The
  uniform-buffer offsets the game binds must meet the device's `minUniformBufferOffsetAlignment`.
  The engine allocates its uniform buffers in 0x1000-aligned pages (`fn_710015c9b0`), but where it
  suballocates them is not checked yet.

Not covered yet: the resolver's texture side (texture and sampler pools; buffers resolve through
`ResolveGpuAddress`), descriptor pools and pipelines in the shim itself (the checks create their
own), `nvnCommandBufferUpdateUniformBuffer`.

**What runs.** `nvn_manifest_check` (Linux image, Clang 18, system libc++) runs the manifest reader
and the binding layer with no GPU and no Vulkan loader; the binding layer only fills Vulkan structs.
For every stage it checks four things. The bindings the SPIR-V module itself declares (read from
its decorations) are exactly the manifest's blocks and samplers. The control section, read from
the package at the manifest's offset, hashes to the stage's key, and `FindStage` returns that
stage, or one with the same SPIR-V bytes, resources and interface. The descriptor set layout has no binding twice and no count of 0. With every NVN slot bound
through a fake resolver, `BuildWrites` covers the layout and resolves each resource NVN uses to its
slot (blocks at binding = slot, samplers at 16 + slot). Only the unused resources get placeholders,
and every info pointer stays in its arrays. On 2.1.0 (2026-10-26) it passes for all 1,342 stages;
`FindStage` returns the stage itself for 1,010 and an identical stage for the rest. Damaged
manifests are caught: a block or sampler moved (with or without its NVN slot), deleted or added, a
second stage under a shared key changed, a control offset off by one, a missing end record. Block
sizes are checked by `tools/bshdat.py --install`, not here. The code
compiles without warnings under `-Wall -Wextra -Wconversion -Wshadow` (checked by hand; the target
does not set them; `vulkan_check.cpp` also needs `-Wno-missing-field-initializers` for Vulkan's
`{VK_STRUCTURE_TYPE_...}` initialisers).

`nvn_vulkan_check` takes the same output to lavapipe (Mesa 25.2, Vulkan 1.4) with the Khronos
validation layers (1.3.275) on. For every program it creates the two shader modules, each stage's
descriptor set layout from `StageLayoutBindings` and the program's pipeline layout. It writes one
descriptor set per stage through `BuildWrites` and `vkUpdateDescriptorSets`, over real buffers, a
1x1 image and a sampler, every NVN slot bound. Then it creates a graphics pipeline, with vertex
attributes and colour attachments typed from the SPIR-V's own interface, and a vertex-only pipeline
as the depth-only pass binds. On 2.1.0 (2026-10-26) all 671 programs pass: 1,342 modules, layouts
and sets, 671 + 671 pipelines, in 10 to 20 s, with **no validation error or warning**. The layers
were shown to report what they should (a moved fragment input location, a manifest binding the
pipeline layout does not match); validation messages are printed as they arrive, and a truncated
module is a named failure. Vertex attribute and attachment formats come from the shaders' own
interface, so this checks the shaders and the layouts, not the shim's future vertex-format and
render-target mapping. The device features (Vulkan 1.3, `dynamicRendering`, `maintenance4`) are
queried before they are enabled. The first run
found the two interface problems above (32 unwritten inputs, 2 wider outputs); the build step and
the device requirement fix them. Not covered yet: drawing (image layouts and view types are only
validated at draw time; a 2D view stands in for every sampler, so cube and array textures are not
exercised), and everything the shim still lacks: device creation from the game's calls, memory,
textures, command buffers, presentation.

## Variant selection (what the shim needs from the engine)

The engine picks a program from a package by a variant key, entirely in its own code:

- **Loading** (`fn_71001b0050`, `CProgramResource`'s set-up, unit 237). It walks the variant map
  into an array of 12-byte entries `{key, key & key2, program index}` (resource `+0x1290`, count
  `+0x1280`). The program the header's `+0x18` pointer names is the default (`+0x1284`). It also
  builds one table per uniform name list, mapping an engine uniform ID to the list position
  (`+0x18`, `+0x61c`, `+0xc20`; 385 entries each; IDs above 0x11a are reduced by 0x2cd). For every
  program and every name it stores the uniform's location in that program (`fn_710015ddc0`, from
  the reflection the shader wrapper parsed), and it binds the attribute list by engine ID
  (`fn_710015e5a0`). The header's `+0x20` list, empty in every file, is a third name list of the
  same kind.
- **Selection** (`fn_71001b0f24(resource, key)`). It scans the entries from the last down to the
  second for one whose `key & key2` equals the requested key. If none matches it returns entry 0's
  program, and with no entries at all the default program. In 2.1.0 every key equals its key2, keys
  are 0..n-1, and key 1 adds `DEFERRED_AS_FORWARD` in 189 of the 190 two-program packages.
- **Who asks.** A program handle (unit 230: `fn_71001a9f3c` once the resource is loaded,
  `fn_71001aa070` when the key changes) caches the selected index. The renderer's program switch
  (`fn_71001cc608`) applies the renderer's current key (a field at `+0x317af78` of the render
  context) before binding. The key is set by the pass set-up code (`fn_710014d1a0`,
  `fn_71001cafa0`, `fn_71001d4be4`; `fn_71001cde48` resets it to 0).
- **Uniform locations by engine ID** (`fn_71001b0f7c(resource, list, id, &out, program)`, reached
  through `fn_71001aa0b4`, 29 callers). These are how the engine writes uniforms. They come from the
  engine's own reflection parsing, not from NVN (NVN has no uniform-location API).

So the shim needs nothing extra for variants: once these functions run (matched, lifted or
semantically lifted, M1's route), the engine selects the program, builds it from the package's own
blobs and calls `nvnProgramSetShaders` with the stage's control section. The shim then looks the
stage up by that section. The engine needs the original `.bshdat` files at run time, read from
the user's dump like every other asset. The manifest's packages, keys and defines are for tools and
diagnostics. The handle functions (`fn_71001a9f3c`, `fn_71001aa070`, `fn_71001aa0b4`) and the key
reset `fn_71001cde48` are matched. The loader, the selector, the uniform lookup, the program switch
and the other key setters are `U` today, and are the next candidates for M1's lifts on the
graphics path.
