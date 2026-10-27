# port/nvn/: the NVN shim over Vulkan

Milestone M2 of the PC port (`docs/PORT_PLAN.md`, decision 3: Vulkan first, shaders compiled to
SPIR-V at build time). The game drives the GPU through NVN, which it loads as a table of function
pointers (`fn_7100154770` asks `nvnDeviceGetProcAddress` for 523 names and stores each in a global).
The shim provides those functions on Vulkan. This directory holds the parts that exist so far: the
shader manifest and the binding layer. The device, memory, command buffers, textures, samplers,
pipelines and presentation come next.

Rules: nothing here comes from NVN's SDK headers or from an emulator. Every NVN type, constant and
argument order is a placeholder inferred from the game's own calls, and the file that uses it names
the call site. The Vulkan side compiles against the Khronos headers, pinned in
`tools/fetch_toolchains.py` (`vulkan-headers-1.4.328`, `data/toolchains.json`). No Vulkan loader
or SDK is needed to compile.

| File | Contents |
|---|---|
| `include/nvn_shim/nvn_types.h` | the NVN values the binding layer handles: stages, stage bits, buffer addresses, texture handles, `nvnProgramSetShaders`' stage array |
| `include/nvn_shim/shader_manifest.h`, `src/shader_manifest.cpp` | reads `shaders.manifest` (written by `tools/bshdat.py --install`) and finds a stage by its control section |
| `include/nvn_shim/bindings.h`, `src/bindings.cpp` | descriptor set layouts per stage; NVN's per-stage slots turned into descriptor writes |
| `check/manifest_check.cpp` | `nvn_manifest_check`: runs the manifest reader and the binding layer over the real build output (below) |
| `CMakeLists.txt` | `dread_nvn` (static library), built when `DREAD_VULKAN_INCLUDE` is set; `nvn_manifest_check` when `DREAD_LINK` is on |

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
- **Limits.** At most five blocks and fourteen texture slots per stage (slots 0 to 13). The
  uniform-buffer offsets the game binds must meet the device's `minUniformBufferOffsetAlignment`.
  The engine allocates its uniform buffers in 0x1000-aligned pages (`fn_710015c9b0`), but where it
  suballocates them is not checked yet.

Not covered yet: the resolver's implementations (memory pools, buffers, texture and sampler pools),
descriptor pools, pipeline creation, `nvnCommandBufferUpdateUniformBuffer`, and any test that runs.
**What runs.** `nvn_manifest_check` (Linux image, Clang 18, system libc++) runs the manifest reader
and the binding layer with no GPU and no Vulkan loader; the binding layer only fills Vulkan structs.
For every stage it checks four things. The SPIR-V file is there. The control section, read from
the package at the manifest's offset, hashes to the stage's key, and `FindStage` returns that
stage. The descriptor set layout has no binding twice and no count of 0. With every NVN slot bound
through a fake resolver, `BuildWrites` covers the layout and resolves each resource NVN uses to its
slot (blocks at binding = slot, samplers at 16 + slot). Only the unused resources get placeholders,
and every info pointer stays in its arrays. On 2.1.0 (2026-10-26) it passes for all 1,342 stages;
`FindStage` returns the stage itself for 1,010 and an identical stage for the rest. Damaged
manifests are caught: a sampler or a block moved by one binding, or a missing end record. The code
compiles without warnings under `-Wall -Wextra -Wconversion -Wshadow` (checked by hand; the target
does not set them). Not tested yet: anything against a Vulkan driver.

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
