// nvn_startup_check
//
// Replays the game's graphics start-up and frame loop against the shim, in the game's order and
// with its argument shapes, as far as they are known from our own sources and the call sites
// (port/nvn/README.md): the graphics start-up fn_71001587f0 (src/game_s7/fn_71001587f0.cpp), the
// memory pool set-up fn_710015385c, the buffer creation of the shader loader fn_710015c9b0, the
// frame's begin fn_7100159920, end fn_7100159ac0 and present fn_7100159bf0, the texture and sampler
// pools fn_710015f100, the sampler fn_710015e9fc, the render targets fn_7100160f60 and the cube
// target fn_710015f880. The game's objects live in storage this program owns, as the game's
// do in its own structs; every function is fetched by name through nvnBootstrapLoader and
// nvnDeviceGetProcAddress, as fn_71001587f0 and fn_7100154770 do.
//
// It checks what the implemented parts must do (the device, queue, command buffers, pools and
// buffers, syncs, state objects, window) and prints, in the order reached, the NVN functions the
// shim still traps: the next work list. Exit status 0 when every check passes; traps are not
// failures. Needs a Vulkan device (the port's Vulkan image: lavapipe); $DREAD_NVN_VALIDATION=1 adds
// the validation layers, whose errors fail the check.
//
//   nvn_startup_check [<shader dir> <dump's romfs/system/shd>]
//
// With the installed shaders (the `shaders` target's DREAD_SHADER_DIR) and the dump's packages, it
// also draws (DrawFrames): clears read back, nvnTextureWriteTexels read back, and every program of
// the manifest drawn with the engine's render state, vertex and uniform setup, in full and as the
// depth-only pass. Prints counts only.

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "nvn_shim/command.h"
#include "nvn_shim/device.h"
#include "nvn_shim/draw.h"
#include "nvn_shim/memory.h"
#include "nvn_shim/nvn_types.h"
#include "nvn_shim/procs.h"
#include "nvn_shim/queue.h"
#include "nvn_shim/shader_manifest.h"
#include "nvn_shim/texture.h"
#include "nvn_shim/window.h"

using namespace dread::nvn;

namespace {

int g_failures = 0;
void Check(bool ok, const char* what) {
    if (!ok) {
        ++g_failures;
        std::fprintf(stderr, "FAIL %s\n", what);
    }
}

using GetProcFn = void* (*)(const void* device, const char* name);
GetProcFn g_get_proc;

template <class F>
F Fn(const char* name) {
    return reinterpret_cast<F>(g_get_proc(nullptr, name));
}

// Storage for one game object: generous, 16-aligned, zeroed; the shim never writes into it.
struct alignas(16) Storage {
    unsigned char bytes[0x400] = {};
};

void* AlignedAlloc(std::size_t size, std::size_t align) {
    void* p = nullptr;
    if (posix_memalign(&p, align, size) != 0)
        return nullptr;
    std::memset(p, 0, size);
    return p;
}

// Storage for a large pool, reserved but never touched (the kernel commits pages on first write),
// as the engine's own pools are before it fills them.
void* LargeStorage(std::size_t size) {
    void* p = nullptr;
    return posix_memalign(&p, 0x1000, size) == 0 ? p : nullptr;
}


// The game's objects the drawing frames use, made by the start-up replay in main.
struct DrawContext {
    void* device;
    void* queue;
    void* sync;
    void* window;
    void* window_sync;
    void* cmd;
    void* pool;  // an engine pool (12 MB) for vertex, index and uniform data
    const void* target;
    const void* other_target;
    const void* depth;
    void* texture_pool;
    void* texture_builder;
    void* blend;
    void* channel_mask;
    void* color;
    void* depth_stencil;
    void* multisample;
    void* polygon;
    std::int32_t texture_id, cube_id, sampler_id, next_texture_id;
};

bool ReadFile(const std::string& path, std::vector<unsigned char>* out) {
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f)
        return false;
    std::fseek(f, 0, SEEK_END);
    const long n = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    out->resize(n > 0 ? static_cast<std::size_t>(n) : 0);
    const bool ok = n > 0 && std::fread(out->data(), 1, out->size(), f) == out->size();
    std::fclose(f);
    return ok;
}

// The drawing side, with the game's argument shapes (nvn_shim/draw.h has the call sites): a frame
// that clears the window's texture and reads it back; nvnTextureWriteTexels read back; then every
// program of the manifest drawn twice into the window's texture and the depth target, with the
// render state, vertex and uniform setup of the engine's draw path, once with all its stages and
// once as the depth-only pass binds it (fragment stage unbound), with an instanced draw.
void DrawFrames(const char* shader_dir, const char* shd_dir, const DrawContext& x) {
    ShaderManifest manifest;
    std::string error;
    if (!manifest.Load(shader_dir, &error)) {
        Check(false, ("the shader manifest: " + error).c_str());
        return;
    }
    SetShaderDirectory(shader_dir);
    auto BeginRecording = Fn<void (*)(void*)>("nvnCommandBufferBeginRecording");
    auto EndRecording = Fn<std::uint64_t (*)(void*)>("nvnCommandBufferEndRecording");
    auto SubmitCommands = Fn<void (*)(void*, std::int32_t, const std::uint64_t*)>("nvnQueueSubmitCommands");
    auto QueueFenceSync = Fn<void (*)(void*, void*, std::int32_t, std::int32_t)>("nvnQueueFenceSync");
    auto SyncWait = Fn<std::int32_t (*)(const void*, std::uint64_t)>("nvnSyncWait");
    auto SetRenderTargets =
        Fn<void (*)(void*, std::int32_t, const void* const*, const void* const*, const void*, const void*)>(
            "nvnCommandBufferSetRenderTargets");
    auto SetViewport = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnCommandBufferSetViewport");
    auto SetScissor = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnCommandBufferSetScissor");
    auto ClearColor = Fn<void (*)(void*, std::int32_t, const float*, std::int32_t)>("nvnCommandBufferClearColor");
    auto ClearDepthStencil =
        Fn<void (*)(void*, float, std::uint8_t, std::int32_t, std::int32_t)>("nvnCommandBufferClearDepthStencil");
    auto BindChannelMaskState = Fn<void (*)(void*, const void*)>("nvnCommandBufferBindChannelMaskState");
    auto SetChannelMask =
        Fn<void (*)(void*, std::int32_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t)>(
            "nvnChannelMaskStateSetChannelMask");
    auto MemoryPoolMap = Fn<void* (*)(const void*)>("nvnMemoryPoolMap");
    auto MemoryPoolGetBufferAddress = Fn<BufferAddress (*)(const void*)>("nvnMemoryPoolGetBufferAddress");
    auto submit_and_wait = [&]() {
        const std::uint64_t handle = EndRecording(x.cmd);
        SubmitCommands(x.queue, 1, &handle);
        QueueFenceSync(x.queue, x.sync, 0, 0);
        Check(SyncWait(x.sync, ~0ull) == 0, "nvnSyncWait reaches the fence");
    };

    // fn_7100159920 and fn_7100159660: render targets, then clears of colour target 0 and depth.
    const void* targets[1] = {x.target};
    BeginRecording(x.cmd);
    SetRenderTargets(x.cmd, 1, targets, nullptr, x.depth, nullptr);
    Fn<void (*)(void*, const void*, std::int64_t, std::int64_t)>("nvnCommandBufferSetShaderScratchMemory")(x.cmd, x.pool, 0,
                                                                                                          0x100000);
    SetViewport(x.cmd, 0, 0, 1280, 720);
    SetScissor(x.cmd, 0, 0, 1280, 720);
    for (int t = 0; t < 8; ++t)
        SetChannelMask(x.channel_mask, t, 1, 1, 1, 1);
    BindChannelMaskState(x.cmd, x.channel_mask);
    const float clear[4] = {0.25f, 0.5f, 0.75f, 1.0f};
    ClearColor(x.cmd, 0, clear, 0xf);
    ClearDepthStencil(x.cmd, 1.0f, 1, 0, -1);
    submit_and_wait();
    std::vector<std::uint8_t> texels;
    Check(ReadTexels(x.target, &texels) && texels.size() == 1280u * 720u * 4u && texels[0] == 64 && texels[1] == 128 &&
              texels[2] == 191 && texels[3] == 255 && texels[texels.size() - 1] == 255,
          "nvnCommandBufferClearColor fills the render target with the colour");

    // fn_7100160ef0 and fn_7100160e80: a level's texels, then one texel.
    {
        Storage texture;
        auto TB = [&](const char* name) { return Fn<void (*)(void*, std::int32_t)>(name); };
        Fn<void (*)(void*)>("nvnTextureBuilderSetDefaults")(x.texture_builder);
        TB("nvnTextureBuilderSetTarget")(x.texture_builder, 1);
        TB("nvnTextureBuilderSetFormat")(x.texture_builder, 0x25);
        Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnTextureBuilderSetSize2D")(x.texture_builder, 4, 4);
        TB("nvnTextureBuilderSetLevels")(x.texture_builder, 1);
        Fn<void (*)(void*, const void*, std::int64_t)>("nvnTextureBuilderSetStorage")(x.texture_builder, x.pool, 0x100000);
        Check(Fn<std::int32_t (*)(void*, const void*)>("nvnTextureInitialize")(&texture, x.texture_builder) == 1,
              "a 4x4 texture for nvnTextureWriteTexels");
        auto WriteTexels = Fn<void (*)(const void*, const void*, const std::int32_t*, const void*)>("nvnTextureWriteTexels");
        std::uint32_t level[16];
        for (std::uint32_t i = 0; i < 16; ++i)
            level[i] = 0x01010101u * i;
        const std::int32_t whole[6] = {0, 0, 0, 4, 4, 1}, one[6] = {2, 1, 0, 1, 1, 1};
        const std::uint32_t texel = 0xdeadbeefu;
        WriteTexels(&texture, nullptr, whole, level);
        WriteTexels(&texture, nullptr, one, &texel);
        std::vector<std::uint8_t> got;
        level[1 * 4 + 2] = texel;
        Check(ReadTexels(&texture, &got) && got.size() == sizeof(level) && std::memcmp(got.data(), level, sizeof(level)) == 0,
              "nvnTextureWriteTexels writes the level, then the texel at (2, 1)");
        Fn<void (*)(void*)>("nvnTextureFinalize")(&texture);
    }

    // The vertex, index and uniform data, in the engine pool (its storage is the game's memory).
    auto* storage = static_cast<unsigned char*>(MemoryPoolMap(x.pool));
    const BufferAddress base = MemoryPoolGetBufferAddress(x.pool);
    const float positions[3][4] = {{-1, -1, 0, 1}, {3, -1, 0, 1}, {-1, 3, 0, 1}};
    std::memset(storage, 0, 0x3000);
    std::memcpy(storage, positions, sizeof(positions));
    const std::uint16_t indices[4] = {0, 1, 2, 0};
    std::memcpy(storage + 0x1000, indices, sizeof(indices));
    const BufferAddress vertices = base, index_address = base + 0x1000, uniforms = base + 0x2000;

    // fn_7100159ce0's render-state flush: the blend state bound per target, colour, depth-stencil,
    // multisample and polygon states; the alpha reference; fn_710015acf0's stencil mask and reference;
    // the draws' depth bounds.
    auto SetBlendTarget = Fn<void (*)(void*, std::int32_t)>("nvnBlendStateSetBlendTarget");
    auto BindBlendState = Fn<void (*)(void*, const void*)>("nvnCommandBufferBindBlendState");
    Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnBlendStateSetBlendFunc")(x.blend, 5, 6, 5, 6);
    Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnBlendStateSetBlendEquation")(x.blend, 1, 1);
    for (int t = 0; t < 8; ++t) {
        SetBlendTarget(x.blend, t);
        BindBlendState(x.cmd, x.blend);
        Fn<void (*)(void*, std::int32_t, std::uint8_t)>("nvnColorStateSetBlendEnable")(x.color, t, 1);
    }
    Fn<void (*)(void*, std::int32_t)>("nvnColorStateSetAlphaTest")(x.color, 8);
    Fn<void (*)(void*, float)>("nvnCommandBufferSetAlphaRef")(x.cmd, 0.5f);
    Fn<void (*)(void*, const void*)>("nvnCommandBufferBindColorState")(x.cmd, x.color);
    // Depth and stencil tests on, so their dynamic state is exercised: depth less-or-equal (4),
    // stencil always (8) replacing (3) where the depth test passes.
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetDepthTestEnable")(x.depth_stencil, 1);
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetDepthWriteEnable")(x.depth_stencil, 1);
    Fn<void (*)(void*, std::int32_t)>("nvnDepthStencilStateSetDepthFunc")(x.depth_stencil, 4);
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetStencilTestEnable")(x.depth_stencil, 1);
    Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnDepthStencilStateSetStencilFunc")(x.depth_stencil, 3, 8);
    Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnDepthStencilStateSetStencilOp")(
        x.depth_stencil, 3, 1, 1, 3);
    Fn<void (*)(void*, const void*)>("nvnCommandBufferBindDepthStencilState")(x.cmd, x.depth_stencil);
    Fn<void (*)(void*, const void*)>("nvnCommandBufferBindMultisampleState")(x.cmd, x.multisample);
    Fn<void (*)(void*, const void*)>("nvnCommandBufferBindPolygonState")(x.cmd, x.polygon);
    for (std::int32_t face : {2, 1}) {
        Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnCommandBufferSetStencilRef")(x.cmd, face, 0);
        Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnCommandBufferSetStencilMask")(x.cmd, face, 0xff);
    }
    Fn<void (*)(void*, std::uint8_t, float, float)>("nvnCommandBufferSetDepthBounds")(x.cmd, 0, 0.0f, 1.0f);

    auto ProgramInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnProgramInitialize");
    auto ProgramSetShaders = Fn<std::int32_t (*)(void*, std::int32_t, const ShaderData*)>("nvnProgramSetShaders");
    auto ProgramFinalize = Fn<void (*)(void*)>("nvnProgramFinalize");
    auto BindProgram = Fn<void (*)(void*, const void*, std::int32_t)>("nvnCommandBufferBindProgram");
    auto BindUniformBuffer =
        Fn<void (*)(void*, std::int32_t, std::int32_t, BufferAddress, std::uint64_t)>("nvnCommandBufferBindUniformBuffer");
    auto UpdateUniformBuffer =
        Fn<void (*)(void*, BufferAddress, std::uint64_t, std::int64_t, std::uint64_t, const void*)>(
            "nvnCommandBufferUpdateUniformBuffer");
    auto BindTexture = Fn<void (*)(void*, std::int32_t, std::int32_t, TextureHandle)>("nvnCommandBufferBindTexture");
    auto GetTextureHandle = Fn<TextureHandle (*)(const void*, std::int32_t, std::int32_t)>("nvnDeviceGetTextureHandle");
    auto AttribSetDefaults = Fn<void (*)(void*)>("nvnVertexAttribStateSetDefaults");
    auto AttribSetFormat = Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnVertexAttribStateSetFormat");
    auto AttribSetStream = Fn<void (*)(void*, std::int32_t)>("nvnVertexAttribStateSetStreamIndex");
    auto StreamSetDefaults = Fn<void (*)(void*)>("nvnVertexStreamStateSetDefaults");
    auto StreamSetStride = Fn<void (*)(void*, std::int32_t)>("nvnVertexStreamStateSetStride");
    auto StreamSetDivisor = Fn<void (*)(void*, std::int32_t)>("nvnVertexStreamStateSetDivisor");
    auto BindAttribs = Fn<void (*)(void*, std::int32_t, const void*)>("nvnCommandBufferBindVertexAttribState");
    auto BindStreams = Fn<void (*)(void*, std::int32_t, const void*)>("nvnCommandBufferBindVertexStreamState");
    auto BindVertexBuffer =
        Fn<void (*)(void*, std::int32_t, BufferAddress, std::uint32_t)>("nvnCommandBufferBindVertexBuffer");
    auto DrawElements =
        Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, BufferAddress)>("nvnCommandBufferDrawElements");
    auto DrawElementsInstanced =
        Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, BufferAddress, std::int32_t, std::int32_t, std::int32_t)>(
            "nvnCommandBufferDrawElementsInstanced");
    auto Barrier = Fn<void (*)(void*, std::int32_t)>("nvnCommandBufferBarrier");
    const TextureHandle texture = GetTextureHandle(x.device, x.texture_id, x.sampler_id);
    const TextureHandle cube = GetTextureHandle(x.device, x.cube_id, x.sampler_id);
    std::uint32_t attribs[16];  // the engine's 4-byte attribute states and 8-byte stream states
    std::uint64_t streams[16];
    std::vector<unsigned char> uniform_data(0x1000, 0);

    const DrawStats before = GetDrawStats();
    std::size_t programs = 0, failed = 0;
    BeginRecording(x.cmd);
    SetRenderTargets(x.cmd, 1, targets, nullptr, x.depth, nullptr);
    SetViewport(x.cmd, 0, 0, 1280, 720);
    SetScissor(x.cmd, 0, 0, 1280, 720);
    for (const PackageInfo& pkg : manifest.packages()) {
        std::vector<unsigned char> bytes;
        if (!ReadFile(std::string(shd_dir) + "/" + pkg.name + ".bshdat", &bytes)) {
            Check(false, ("the package " + pkg.name + ".bshdat").c_str());
            continue;
        }
        for (const ProgramInfo& prog : pkg.programs) {
            Storage program;
            ShaderData data[2];
            std::int32_t count = 0, bits = 0;
            for (const StageInfo& st : prog.stages) {
                if (st.control_offset + static_cast<std::uint64_t>(st.control_size) > bytes.size())
                    continue;
                data[count++] = {uniforms, bytes.data() + st.control_offset};
                bits |= st.stage == Stage::Vertex ? kStageBitVertex : kStageBitFragment;
            }
            ProgramInitialize(&program, x.device);
            if (ProgramSetShaders(&program, count, data) != 1) {
                if (++failed <= 5)
                    std::fprintf(stderr, "FAIL nvnProgramSetShaders: %s program %u\n", pkg.name.c_str(), prog.index);
                ProgramFinalize(&program);
                continue;
            }
            programs++;
            // fn_710015d900: the program, its blocks at their slots; fn_710015e700's updates;
            // fn_7100161250: texture slots (the cube in every other, mismatches included).
            BindProgram(x.cmd, &program, bits);
            for (const StageInfo& st : prog.stages) {
                const std::int32_t stage = static_cast<std::int32_t>(st.stage);
                for (const StageResource& b : st.blocks)
                    if (b.nvn_slot >= 0)
                        BindUniformBuffer(x.cmd, stage, b.nvn_slot, uniforms, b.size_or_count);
                for (const StageResource& s : st.samplers)
                    for (std::uint32_t k = 0; s.nvn_slot >= 0 && k < s.size_or_count; ++k)
                        BindTexture(x.cmd, stage, s.nvn_slot + static_cast<std::int32_t>(k), k % 2 ? cube : texture);
            }
            // Every other program without an update, its depth-only draw first: after the previous
            // program's full and depth-only draws, that is the sequence "A full, B vertex-only, B
            // full" with nothing changed in between, which must rebind B's fragment set itself.
            const bool updates = programs % 2 == 1;  // the 1st, 3rd ... program
            if (updates) {
                uniform_data[0] = static_cast<unsigned char>(programs);
                UpdateUniformBuffer(x.cmd, uniforms, 0x1000, 0, 0x1000, uniform_data.data());
            }
            // fn_7100161fd0 and fn_7100161800: every attribute and stream reset, then one stream per
            // input the vertex stage reads, four floats 16 bytes apart; the second stream steps per
            // instance (fn_7100162530's instance buffer).
            std::int32_t streams_used = 0, attrib_count = 0;
            for (int i = 0; i < 16; ++i) {
                AttribSetDefaults(&attribs[i]);
                StreamSetDefaults(&streams[i]);
            }
            for (const StageInfo& st : prog.stages) {
                if (st.stage != Stage::Vertex)
                    continue;
                for (const StageInterface& in : st.inputs) {
                    if (in.location < 0 || in.location >= 16 || streams_used >= 16)
                        continue;
                    AttribSetDefaults(&attribs[in.location]);
                    AttribSetStream(&attribs[in.location], streams_used);
                    AttribSetFormat(&attribs[in.location], 0x2e, 0);
                    StreamSetDefaults(&streams[streams_used]);
                    StreamSetStride(&streams[streams_used], 16);
                    StreamSetDivisor(&streams[streams_used], streams_used == 1 ? 1 : 0);
                    BindVertexBuffer(x.cmd, streams_used, vertices, 0x1000);
                    attrib_count = std::max(attrib_count, in.location + 1);
                    streams_used++;
                }
            }
            BindAttribs(x.cmd, attrib_count, attribs);
            BindStreams(x.cmd, streams_used, streams);
            // In full, and as the depth-only pass binds it (the fragment stage unbound) with an
            // instanced triangle strip.
            auto depth_only = [&] {
                BindProgram(x.cmd, nullptr, kStageBitFragment);
                DrawElementsInstanced(x.cmd, 5, 1, 4, index_address, 0, 0, 2);
            };
            if (!updates) {
                depth_only();
                BindProgram(x.cmd, &program, bits);
            }
            DrawElements(x.cmd, 4, 1, 3, index_address);
            if (updates)
                depth_only();
            BindProgram(x.cmd, nullptr, kStageBitsAll);
            ProgramFinalize(&program);
            if (programs % 64 == 0) {  // a frame's worth: submit, as fn_7100159ac0 does
                Barrier(x.cmd, 2);
                submit_and_wait();
                BeginRecording(x.cmd);
                SetRenderTargets(x.cmd, 1, targets, nullptr, x.depth, nullptr);
            }
        }
    }
    // The frame's end, the window's crop and the present (fn_7100159bf0).
    submit_and_wait();
    Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnWindowSetCrop")(x.window, 0, 0, 1280, 720);
    Fn<void (*)(void*, const void*, std::int32_t)>("nvnQueuePresentTexture")(x.queue, x.window, 0);
    std::int32_t index = -1;
    Fn<std::int32_t (*)(void*, void*, std::int32_t*)>("nvnWindowAcquireTexture")(x.window, x.window_sync, &index);
    Check(SyncWait(x.window_sync, ~0ull) == 0, "the acquired texture's sync is reached");
    const WindowStats ws = GetWindowStats(x.window);
    Check(ws.presented != nullptr && ws.cropped && ws.crop[2] == 1280, "the present and the window's crop");

    const DrawStats after = GetDrawStats();
    std::printf("nvn shim: drew %zu programs (%zu failed): %llu draws, %llu skipped, %llu pipelines, %llu render passes, "
                "%llu uniform updates, %llu submitted\n",
                programs, failed, static_cast<unsigned long long>(after.draws - before.draws),
                static_cast<unsigned long long>(after.skipped_draws - before.skipped_draws),
                static_cast<unsigned long long>(after.pipelines), static_cast<unsigned long long>(after.render_passes),
                static_cast<unsigned long long>(after.uniform_updates), static_cast<unsigned long long>(after.submitted));
    Check(failed == 0 && programs > 0, "every program of the manifest initialises");
    Check(after.draws - before.draws == 2 * programs && after.skipped_draws == before.skipped_draws,
          "every draw is recorded");
    Check(after.pipelines > 0 && after.uniform_updates >= programs / 2, "pipelines and uniform updates");
    std::vector<std::uint8_t> drawn;
    Check(ReadTexels(x.target, &drawn) && drawn.size() == texels.size(), "the render target reads back after the draws");
}

}  // namespace

int main(int argc, char** argv) {
    // fn_71001587f0: the bootstrap loader, then the device's entry points.
    auto get_proc = reinterpret_cast<GetProcFn>(nvnBootstrapLoader("nvnDeviceGetProcAddress"));
    Check(get_proc != nullptr, "nvnBootstrapLoader(nvnDeviceGetProcAddress)");
    if (!get_proc)
        return 1;
    g_get_proc = get_proc;
    Check(reinterpret_cast<void*>(get_proc(nullptr, "nvnDeviceGetProcAddress")) == reinterpret_cast<void*>(get_proc),
          "nvnDeviceGetProcAddress returns itself");

    Storage device, device_builder, queue, queue_builder, sync0, window_sync, window_builder_storage, window;
    Storage blend, channel_mask, color, depth_stencil, multisample, polygon, buffer_builder, texture_builder;
    Storage cmd_builders[2];
    Storage pools[2];  // the engine's pool record holds the NVN pool at +8; ours is the object itself

    auto DeviceBuilderSetDefaults = Fn<void (*)(void*)>("nvnDeviceBuilderSetDefaults");
    auto DeviceBuilderSetFlags = Fn<void (*)(void*, std::int32_t)>("nvnDeviceBuilderSetFlags");
    auto DeviceInitialize = Fn<std::int32_t (*)(void*, void*)>("nvnDeviceInitialize");
    auto DeviceGetInteger = Fn<void (*)(const void*, std::int32_t, std::int32_t*)>("nvnDeviceGetInteger");
    auto DeviceSetDepthMode = Fn<void (*)(void*, std::int32_t)>("nvnDeviceSetDepthMode");
    auto DeviceSetWindowOriginMode = Fn<void (*)(void*, std::int32_t)>("nvnDeviceSetWindowOriginMode");
    auto DeviceRegisterFastClearColor = Fn<void (*)(void*, const float*, std::int32_t)>("nvnDeviceRegisterFastClearColor");
    auto DeviceRegisterFastClearDepth = Fn<void (*)(void*, float)>("nvnDeviceRegisterFastClearDepth");

    std::int32_t major = -1, minor = -1;
    DeviceGetInteger(nullptr, 0, &major);
    DeviceGetInteger(nullptr, 1, &minor);
    DeviceBuilderSetDefaults(&device_builder);
    DeviceBuilderSetFlags(&device_builder, 0);
    Check(DeviceInitialize(&device, &device_builder) == 1, "nvnDeviceInitialize");
    Check(CurrentBackend() != nullptr, "a Vulkan backend after nvnDeviceInitialize");
    DeviceSetDepthMode(&device, 0);
    DeviceSetWindowOriginMode(&device, 1);
    const float clear[4] = {};
    DeviceRegisterFastClearColor(&device, clear, 0xa);
    DeviceRegisterFastClearColor(&device, clear, 0x3f);
    DeviceRegisterFastClearColor(&device, clear, 0x25);
    DeviceRegisterFastClearDepth(&device, 1.0f);

    // The queue, its memory from the game's allocator, 4 KB aligned.
    auto QueueBuilderSetDevice = Fn<void (*)(void*, const void*)>("nvnQueueBuilderSetDevice");
    auto QueueBuilderSetDefaults = Fn<void (*)(void*)>("nvnQueueBuilderSetDefaults");
    auto QueueBuilderSetComputeMemorySize = Fn<void (*)(void*, std::int64_t)>("nvnQueueBuilderSetComputeMemorySize");
    auto QueueBuilderSetCommandMemorySize = Fn<void (*)(void*, std::int64_t)>("nvnQueueBuilderSetCommandMemorySize");
    auto QueueBuilderSetCommandFlushThreshold = Fn<void (*)(void*, std::int64_t)>("nvnQueueBuilderSetCommandFlushThreshold");
    auto QueueBuilderGetQueueMemorySize = Fn<std::uint64_t (*)(const void*)>("nvnQueueBuilderGetQueueMemorySize");
    auto QueueBuilderSetQueueMemory = Fn<void (*)(void*, void*, std::uint64_t)>("nvnQueueBuilderSetQueueMemory");
    auto QueueInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnQueueInitialize");
    QueueBuilderSetDevice(&queue_builder, &device);
    QueueBuilderSetDefaults(&queue_builder);
    Check(Fn<const void* (*)(const void*)>("nvnQueueBuilderGetDevice")(&queue_builder) == &device,
          "the queue builder keeps its device across SetDefaults (fn_71001587f0 sets it first)");
    QueueBuilderSetComputeMemorySize(&queue_builder, 0);
    std::int32_t command_size = 0;
    DeviceGetInteger(&device, 0x51, &command_size);
    Check(command_size > 0, "nvnDeviceGetInteger(0x51) > 0");
    QueueBuilderSetCommandMemorySize(&queue_builder, command_size);
    QueueBuilderSetCommandFlushThreshold(&queue_builder, command_size);
    const std::uint64_t queue_memory_size = QueueBuilderGetQueueMemorySize(&queue_builder);
    void* queue_memory = AlignedAlloc(queue_memory_size, 0x1000);
    QueueBuilderSetQueueMemory(&queue_builder, queue_memory, queue_memory_size);
    Check(QueueInitialize(&queue, &queue_builder) == 1, "nvnQueueInitialize");

    // fn_710015385c for each command buffer: a 15 MB command memory pool (flags 0x22), and 2 MB of
    // control memory, both aligned as nvnDeviceGetInteger(0x13 / 0x14) says.
    auto MemoryPoolBuilderSetDefaults = Fn<void (*)(void*)>("nvnMemoryPoolBuilderSetDefaults");
    auto MemoryPoolBuilderSetDevice = Fn<void (*)(void*, const void*)>("nvnMemoryPoolBuilderSetDevice");
    auto MemoryPoolBuilderSetFlags = Fn<void (*)(void*, std::int32_t)>("nvnMemoryPoolBuilderSetFlags");
    auto MemoryPoolBuilderSetStorage = Fn<void (*)(void*, void*, std::uint64_t)>("nvnMemoryPoolBuilderSetStorage");
    auto MemoryPoolInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnMemoryPoolInitialize");
    auto CommandBufferInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnCommandBufferInitialize");
    auto CommandBufferAddCommandMemory = Fn<void (*)(void*, const void*, std::int64_t, std::uint64_t)>("nvnCommandBufferAddCommandMemory");
    auto CommandBufferAddControlMemory = Fn<void (*)(void*, void*, std::uint64_t)>("nvnCommandBufferAddControlMemory");
    auto CommandBufferSetMemoryCallback = Fn<void (*)(void*, void*)>("nvnCommandBufferSetMemoryCallback");
    // fn_7100153350: the engine's own pools, 300 MB, 1008 MB, 208 MB, 12 MB and 1 MB, flags 0x22,
    // 0x21 and 0x24. Their Vulkan buffers are made only when the GPU first needs them.
    const struct {
        std::uint64_t size;
        std::int32_t flags;
    } kEnginePools[] = {{300ull << 20, 0x22}, {1008ull << 20, 0x21}, {208ull << 20, 0x24}, {12ull << 20, 0x22},
                        {1ull << 20, 0x22}};
    Storage engine_pools[5];
    void* engine_storage[5] = {};
    for (int i = 0; i < 5; ++i) {
        Storage builder;
        engine_storage[i] = LargeStorage(kEnginePools[i].size);
        Check(engine_storage[i] != nullptr, "storage for an engine pool");
        MemoryPoolBuilderSetDefaults(&builder);
        MemoryPoolBuilderSetDevice(&builder, &device);
        MemoryPoolBuilderSetFlags(&builder, kEnginePools[i].flags);
        MemoryPoolBuilderSetStorage(&builder, engine_storage[i], kEnginePools[i].size);
        Check(MemoryPoolInitialize(&engine_pools[i], &builder) == 1, "nvnMemoryPoolInitialize (an engine pool)");
    }
    Check(PoolsWithVulkanBuffer() == 0, "no Vulkan buffer before the GPU needs a pool");

    // fn_710015f100's integers and the pools it sizes with them.
    std::int32_t texture_size = 0, sampler_size = 0, reserved_textures = 0, reserved_samplers = 0, max_textures = 0,
                 max_samplers = 0;
    DeviceGetInteger(&device, 0xf, &texture_size);
    DeviceGetInteger(&device, 0x10, &sampler_size);
    DeviceGetInteger(&device, 0x11, &reserved_textures);
    DeviceGetInteger(&device, 0x24, &max_textures);
    DeviceGetInteger(&device, 0x12, &reserved_samplers);
    DeviceGetInteger(&device, 0x25, &max_samplers);
    const std::int32_t textures_n = std::min(reserved_textures + 0x800, max_textures);
    const std::int32_t samplers_n = std::min(reserved_samplers + 0x2000, max_samplers);
    Check(texture_size > 0 && sampler_size > 0 && reserved_textures > 0 && reserved_samplers > 0,
          "descriptor sizes and reserved counts");
    Check(textures_n == reserved_textures + 0x800 && samplers_n == reserved_samplers + 0x2000,
          "the pools hold every texture and sampler the game asks for");

    std::int32_t command_alignment = 0, control_alignment = 0;
    DeviceGetInteger(&device, 0x13, &command_alignment);
    DeviceGetInteger(&device, 0x14, &control_alignment);
    Check(command_alignment > 0 && control_alignment > 0, "command and control alignments");
    const std::uint64_t kCommandPoolSize = 0xf00000, kControlSize = 0x200000;
    void* control_memory[2] = {};
    for (int i = 0; i < 2; ++i) {
        Storage builder;
        void* storage = AlignedAlloc(kCommandPoolSize, 0x1000);
        MemoryPoolBuilderSetDefaults(&builder);
        MemoryPoolBuilderSetDevice(&builder, &device);
        MemoryPoolBuilderSetFlags(&builder, 0x22);
        MemoryPoolBuilderSetStorage(&builder, storage, kCommandPoolSize);
        Check(MemoryPoolInitialize(&pools[i], &builder) == 1, "nvnMemoryPoolInitialize (command memory)");
        Check(CommandBufferInitialize(&cmd_builders[i], &device) == 1, "nvnCommandBufferInitialize");
        control_memory[i] = AlignedAlloc(kControlSize, static_cast<std::size_t>(std::max(control_alignment, 8)));
        CommandBufferAddCommandMemory(&cmd_builders[i], &pools[i], 0, kCommandPoolSize);
        CommandBufferAddControlMemory(&cmd_builders[i], control_memory[i], kControlSize);
        CommandBufferSetMemoryCallback(&cmd_builders[i], reinterpret_cast<void*>(&AlignedAlloc));
    }
    Check(MemoryPoolCount() == 7, "seven memory pools (five of the engine's, two of command memory)");

    // Syncs and the fixed-function state objects, set as fn_71001587f0 sets them.
    auto SyncInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnSyncInitialize");
    Check(SyncInitialize(&sync0, &device) == 1 && SyncInitialize(&window_sync, &device) == 1, "nvnSyncInitialize");
    Fn<void (*)(void*)>("nvnBlendStateSetDefaults")(&blend);
    Fn<void (*)(void*)>("nvnChannelMaskStateSetDefaults")(&channel_mask);
    Fn<void (*)(void*)>("nvnColorStateSetDefaults")(&color);
    Fn<void (*)(void*)>("nvnDepthStencilStateSetDefaults")(&depth_stencil);
    Fn<void (*)(void*)>("nvnMultisampleStateSetDefaults")(&multisample);
    Fn<void (*)(void*)>("nvnPolygonStateSetDefaults")(&polygon);
    Fn<void (*)(void*, std::int32_t)>("nvnPolygonStateSetFrontFace")(&polygon, 1);
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetDepthTestEnable")(&depth_stencil, 1);
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetDepthWriteEnable")(&depth_stencil, 1);
    Fn<void (*)(void*, std::uint8_t)>("nvnDepthStencilStateSetStencilTestEnable")(&depth_stencil, 0);
    Fn<void (*)(void*, std::int32_t)>("nvnDepthStencilStateSetDepthFunc")(&depth_stencil, 3);
    Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnDepthStencilStateSetStencilFunc")(&depth_stencil, 1, 7);
    Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnDepthStencilStateSetStencilOp")(
        &depth_stencil, 1, 1, 1, 1);
    Fn<void (*)(void*, std::int32_t)>("nvnPolygonStateSetCullFace")(&polygon, 0);
    Fn<void (*)(void*, std::int32_t)>("nvnPolygonStateSetPolygonMode")(&polygon, 2);
    Fn<void (*)(void*, std::int32_t)>("nvnMultisampleStateSetMultisampleEnable")(&multisample, 0);
    Fn<void (*)(void*, const void*)>("nvnBufferBuilderSetDevice")(&buffer_builder, &device);
    Fn<void (*)(void*)>("nvnBufferBuilderSetDefaults")(&buffer_builder);
    Fn<void (*)(void*, const void*)>("nvnTextureBuilderSetDevice")(&texture_builder, &device);
    Fn<void (*)(void*)>("nvnTextureBuilderSetDefaults")(&texture_builder);

    // The shader loader's buffers (fn_710015c9b0): a uniform buffer in a pool, its address and map.
    {
        Storage builder, pool, buffer;
        const std::uint64_t size = 0x10000;
        void* storage = AlignedAlloc(size, 0x1000);
        MemoryPoolBuilderSetDefaults(&builder);
        MemoryPoolBuilderSetDevice(&builder, &device);
        MemoryPoolBuilderSetFlags(&builder, 0x22);
        MemoryPoolBuilderSetStorage(&builder, storage, size);
        Check(MemoryPoolInitialize(&pool, &builder) == 1, "nvnMemoryPoolInitialize (buffer memory)");
        Fn<void (*)(void*)>("nvnBufferBuilderSetDefaults")(&buffer_builder);
        Check(Fn<const void* (*)(const void*)>("nvnBufferBuilderGetDevice")(&buffer_builder) == &device,
              "the buffer builder keeps its device across SetDefaults (fn_710015c9b0 sets defaults per buffer)");
        Fn<void (*)(void*, const void*, std::int64_t, std::uint64_t)>("nvnBufferBuilderSetStorage")(&buffer_builder, &pool,
                                                                                                    0x100, 0x200);
        Check(Fn<std::int32_t (*)(void*, const void*)>("nvnBufferInitialize")(&buffer, &buffer_builder) == 1,
              "nvnBufferInitialize");
        const BufferAddress address = Fn<BufferAddress (*)(const void*)>("nvnBufferGetAddress")(&buffer);
        void* mapped = Fn<void* (*)(const void*)>("nvnBufferMap")(&buffer);
        Check(address >= 0x100000000ull, "a buffer address in the GPU address space");
        Check(mapped == static_cast<char*>(storage) + 0x100, "nvnBufferMap is the game's storage + offset");
        Check(GpuAddressToStorage(address) == mapped, "the address maps back to the storage");
        std::memset(mapped, 0x5a, 0x200);
        VkBuffer vk = VK_NULL_HANDLE;
        VkDeviceSize off = 0;
        Check(ResolveGpuAddress(address, 0x200, &vk, &off) && vk != VK_NULL_HANDLE && off == 0x100,
              "the binding layer's address resolution");
        Check(!ResolveGpuAddress(address, 0x10000, &vk, &off), "a range past the pool's end does not resolve");
        Check(SyncPoolRange(address, 0x200), "copying the range into the Vulkan buffer");
        Check(PoolsWithVulkanBuffer() == 1, "a Vulkan buffer only for the pool the GPU used");
        Check(Fn<std::uint64_t (*)(const void*)>("nvnBufferGetSize")(&buffer) == 0x200, "nvnBufferGetSize");
        Fn<void (*)(void*)>("nvnBufferFinalize")(&buffer);
        Fn<void (*)(void*)>("nvnMemoryPoolFinalize")(&pool);
        std::free(storage);
    }

    // fn_710015f100: the texture and sampler pools in an engine pool, sized as the game sizes them.
    Storage texture_pool, sampler_pool, sampler_builder;
    Check(Fn<std::int32_t (*)(void*, const void*, std::int64_t, std::int32_t)>("nvnSamplerPoolInitialize")(
              &sampler_pool, &engine_pools[4], 0, samplers_n) == 1,
          "nvnSamplerPoolInitialize");
    Check(Fn<std::int32_t (*)(void*, const void*, std::int64_t, std::int32_t)>("nvnTexturePoolInitialize")(
              &texture_pool, &engine_pools[4], static_cast<std::int64_t>(sampler_size) * samplers_n, textures_n) == 1,
          "nvnTexturePoolInitialize");
    Fn<void (*)(void*, const void*)>("nvnSamplerBuilderSetDevice")(&sampler_builder, &device);

    // fn_710015e9fc: a sampler made on the stack, registered, finalised at once.
    const std::int32_t sampler_id = reserved_samplers;
    {
        Storage sampler;
        const float border[4] = {0, 0, 0, 1};
        Fn<void (*)(void*)>("nvnSamplerBuilderSetDefaults")(&sampler_builder);
        Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnSamplerBuilderSetMinMagFilter")(&sampler_builder, 3, 1);
        Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t)>("nvnSamplerBuilderSetWrapMode")(&sampler_builder, 0, 0, 0);
        // fn_710015f100's sampler defaults: LOD clamp -16..16, anisotropy 15.
        Fn<void (*)(void*, float, float)>("nvnSamplerBuilderSetLodClamp")(&sampler_builder, -16.0f, 16.0f);
        Fn<void (*)(void*, float)>("nvnSamplerBuilderSetLodBias")(&sampler_builder, 0.0f);
        Fn<void (*)(void*, const float*)>("nvnSamplerBuilderSetBorderColor")(&sampler_builder, border);
        Fn<void (*)(void*, float)>("nvnSamplerBuilderSetMaxAnisotropy")(&sampler_builder, 15.0f);
        Fn<void (*)(void*, std::int32_t)>("nvnSamplerBuilderSetReductionFilter")(&sampler_builder, 0);
        Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnSamplerBuilderSetCompare")(&sampler_builder, 1, 3);
        Check(Fn<std::int32_t (*)(void*, const void*)>("nvnSamplerInitialize")(&sampler, &sampler_builder) == 1,
              "nvnSamplerInitialize");
        Fn<void (*)(void*, std::int32_t, const void*)>("nvnSamplerPoolRegisterSampler")(&sampler_pool, sampler_id, &sampler);
        Fn<void (*)(void*)>("nvnSamplerFinalize")(&sampler);
    }
    Check(RegisteredSampler(static_cast<std::uint32_t>(sampler_id)), "the pool keeps the sampler after it is finalised");

    // fn_7100160f60: the depth target (0x35, flags 0xc) and two colour targets (0x25, flags 0xd),
    // storage suballocated from an engine pool, registered from the first id after the reserved ones.
    auto TB_SetDefaults = Fn<void (*)(void*)>("nvnTextureBuilderSetDefaults");
    auto TB_SetFlags = Fn<void (*)(void*, std::int32_t)>("nvnTextureBuilderSetFlags");
    auto TB_SetTarget = Fn<void (*)(void*, std::int32_t)>("nvnTextureBuilderSetTarget");
    auto TB_SetFormat = Fn<void (*)(void*, std::int32_t)>("nvnTextureBuilderSetFormat");
    auto TB_SetSize2D = Fn<void (*)(void*, std::int32_t, std::int32_t)>("nvnTextureBuilderSetSize2D");
    auto TB_SetSize3D = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t)>("nvnTextureBuilderSetSize3D");
    auto TB_SetLevels = Fn<void (*)(void*, std::int32_t)>("nvnTextureBuilderSetLevels");
    auto TB_SetSwizzle = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnTextureBuilderSetSwizzle");
    auto TB_GetAlignment = Fn<std::uint64_t (*)(const void*)>("nvnTextureBuilderGetStorageAlignment");
    auto TB_GetSize = Fn<std::uint64_t (*)(const void*)>("nvnTextureBuilderGetStorageSize");
    auto TB_SetStorage = Fn<void (*)(void*, const void*, std::int64_t)>("nvnTextureBuilderSetStorage");
    auto TextureInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnTextureInitialize");
    auto RegisterTexture = Fn<void (*)(void*, std::int32_t, const void*, const void*)>("nvnTexturePoolRegisterTexture");
    std::int64_t texture_offset = 0;
    std::int32_t next_texture_id = reserved_textures;
    Storage depth_target, render_targets[2], cube_target, unknown_target;
    auto make_target = [&](void* texture, std::int32_t flags, std::int32_t target, std::int32_t format, std::int32_t w,
                           std::int32_t h, std::int32_t depth3d, const std::int32_t* swizzle) -> std::int32_t {
        TB_SetDefaults(&texture_builder);
        TB_SetFlags(&texture_builder, flags);
        TB_SetTarget(&texture_builder, target);
        TB_SetFormat(&texture_builder, format);
        if (depth3d)
            TB_SetSize3D(&texture_builder, w, h, depth3d);
        else
            TB_SetSize2D(&texture_builder, w, h);
        TB_SetLevels(&texture_builder, 1);
        if (swizzle)
            TB_SetSwizzle(&texture_builder, swizzle[0], swizzle[1], swizzle[2], swizzle[3]);
        const std::uint64_t align = TB_GetAlignment(&texture_builder);
        const std::uint64_t size = TB_GetSize(&texture_builder);
        Check(align > 0 && size > 0 && size % align == 0, "texture storage size and alignment");
        texture_offset = static_cast<std::int64_t>((static_cast<std::uint64_t>(texture_offset) + align - 1) / align * align);
        TB_SetStorage(&texture_builder, &engine_pools[0], texture_offset);
        texture_offset += static_cast<std::int64_t>(size);
        if (!TextureInitialize(texture, &texture_builder))
            return -1;
        RegisterTexture(&texture_pool, next_texture_id, texture, nullptr);
        return next_texture_id++;
    };
    const std::int32_t depth_swizzle[4] = {2, 2, 2, 5}, identity[4] = {2, 3, 4, 5};
    const std::int32_t depth_id = make_target(&depth_target, 0xc, 1, 0x35, 1280, 720, 0, depth_swizzle);
    Check(depth_id >= 0, "the depth target (format 0x35)");
    std::int32_t color_ids[2];
    for (int i = 0; i < 2; ++i) {
        color_ids[i] = make_target(&render_targets[i], 0xd, 1, 0x25, 1280, 720, 0, identity);
        Check(color_ids[i] >= 0, "a colour target (format 0x25)");
    }
    const TextureInfo depth_info = GetTextureInfo(&depth_target);
    Check(depth_info.exists &&
              (depth_info.format == VK_FORMAT_D32_SFLOAT_S8_UINT || depth_info.format == VK_FORMAT_D24_UNORM_S8_UINT) &&
              depth_info.width == 1280,
          "the depth target is a 1280x720 depth-stencil image");
    const TextureInfo color_info = GetTextureInfo(&render_targets[0]);
    Check(color_info.exists && color_info.format == VK_FORMAT_R8G8B8A8_UNORM && color_info.layers == 1,
          "the colour target is a 1280x720 RGBA8 image");
    // fn_710015f880: a cube target (target 8, SetSize3D(w, w, 1)).
    const std::int32_t cube_id = make_target(&cube_target, 0xd, 8, 0x25, 128, 128, 1, identity);
    const TextureInfo cube_info = GetTextureInfo(&cube_target);
    Check(cube_id >= 0 && cube_info.cube && cube_info.layers == 6, "a cube target has six layers");
    // The other colour formats the start-up registers fast clears for (0xa R32F, 0x3f), and the sRGB
    // and BC formats of the game's textures (fn_7100161410's numbers).
    {
        Storage extra[6];
        const std::int32_t formats[6] = {0x0a, 0x3f, 0x38, 0x37, 0x46, 0x48};
        for (int i = 0; i < 6; ++i)
            Check(make_target(&extra[i], 0xd, 1, formats[i], 64, 64, 0, identity) >= 0,
                  "a format of the start-up or of the game's textures");
        for (Storage& e : extra)
            Fn<void (*)(void*)>("nvnTextureFinalize")(&e);
    }
    // A number nothing in the game produces fails (and is reported), rather than being guessed.
    Check(make_target(&unknown_target, 0xd, 1, 0x7f, 64, 64, 0, identity) < 0, "an unknown format fails");
    Check(RegisteredTexture(static_cast<std::uint32_t>(color_ids[1])) == &render_targets[1], "the pool knows the texture");

    // The binding layer's resolver over the shim's objects: a handle from nvnDeviceGetTextureHandle.
    {
        auto GetTextureHandle = Fn<TextureHandle (*)(const void*, std::int32_t, std::int32_t)>("nvnDeviceGetTextureHandle");
        const TextureHandle handle = GetTextureHandle(&device, color_ids[0], sampler_id);
        VkImageView view = VK_NULL_HANDLE;
        VkImageLayout layout{};
        VkSampler vk_sampler = VK_NULL_HANDLE;
        Check(ShimResolver().ResolveTexture(TextureId(handle), &view, &layout) && view == color_info.view,
              "a texture handle resolves to the texture's view");
        Check(ShimResolver().ResolveSampler(SamplerId(handle), &vk_sampler) && vk_sampler != VK_NULL_HANDLE,
              "and its sampler");
        Check(ShimResolver().PlaceholderImage().imageView != VK_NULL_HANDLE &&
                  ShimResolver().PlaceholderBuffer().buffer != VK_NULL_HANDLE,
              "the placeholders exist");
    }

    // The window: two textures (the game's render targets), interval 1, the first acquire.
    auto WindowBuilderSetDefaults = Fn<void (*)(void*)>("nvnWindowBuilderSetDefaults");
    auto WindowBuilderSetDevice = Fn<void (*)(void*, const void*)>("nvnWindowBuilderSetDevice");
    auto WindowBuilderSetNativeWindow = Fn<void (*)(void*, void*)>("nvnWindowBuilderSetNativeWindow");
    auto WindowBuilderSetTextures = Fn<void (*)(void*, std::int32_t, const void* const*)>("nvnWindowBuilderSetTextures");
    auto WindowInitialize = Fn<std::int32_t (*)(void*, const void*)>("nvnWindowInitialize");
    auto WindowSetPresentInterval = Fn<void (*)(void*, std::int32_t)>("nvnWindowSetPresentInterval");
    auto WindowAcquireTexture = Fn<std::int32_t (*)(void*, void*, std::int32_t*)>("nvnWindowAcquireTexture");
    auto QueueFenceSync = Fn<void (*)(void*, void*, std::int32_t, std::int32_t)>("nvnQueueFenceSync");
    auto QueueFlush = Fn<void (*)(void*)>("nvnQueueFlush");
    auto SyncWait = Fn<std::int32_t (*)(const void*, std::uint64_t)>("nvnSyncWait");
    const void* textures[2] = {&render_targets[0], &render_targets[1]};
    int native_window = 0;
    WindowBuilderSetDefaults(&window_builder_storage);
    WindowBuilderSetDevice(&window_builder_storage, &device);
    WindowBuilderSetNativeWindow(&window_builder_storage, &native_window);
    QueueFenceSync(&queue, &sync0, 0, 0);
    QueueFlush(&queue);
    SyncWait(&sync0, ~0ull);
    WindowBuilderSetTextures(&window_builder_storage, 2, textures);
    Check(WindowInitialize(&window, &window_builder_storage) == 1, "nvnWindowInitialize");
    WindowSetPresentInterval(&window, 1);
    std::int32_t index = -1;
    Check(WindowAcquireTexture(&window, &window_sync, &index) == 0 && index == 0, "the first nvnWindowAcquireTexture");
    auto CommandBufferSetViewport = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnCommandBufferSetViewport");
    auto CommandBufferSetScissor = Fn<void (*)(void*, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>("nvnCommandBufferSetScissor");
    CommandBufferSetViewport(&cmd_builders[0], 0, 0, 1280, 720);
    CommandBufferSetScissor(&cmd_builders[0], 0, 0, 1280, 720);

    // Three frames: fn_7100159920 (begin), fn_7100159ac0 (end, submit), fn_7100159bf0 (present).
    auto BeginRecording = Fn<void (*)(void*)>("nvnCommandBufferBeginRecording");
    auto EndRecording = Fn<std::uint64_t (*)(void*)>("nvnCommandBufferEndRecording");
    auto SetSampleMask = Fn<void (*)(void*, std::int32_t)>("nvnCommandBufferSetSampleMask");
    auto SubmitCommands = Fn<void (*)(void*, std::int32_t, const std::uint64_t*)>("nvnQueueSubmitCommands");
    auto PresentTexture = Fn<void (*)(void*, const void*, std::int32_t)>("nvnQueuePresentTexture");
    for (int frame = 0; frame < 3; ++frame) {
        void* cmd = &cmd_builders[frame & 1];
        CommandBufferAddCommandMemory(cmd, &pools[frame & 1], 0, kCommandPoolSize);
        CommandBufferAddControlMemory(cmd, control_memory[frame & 1], kControlSize);
        BeginRecording(cmd);
        SetSampleMask(cmd, -1);
        CommandBufferSetViewport(cmd, 0, 0, 1280, 720);
        CommandBufferSetScissor(cmd, 0, 0, 1280, 720);
        const std::uint64_t handle = EndRecording(cmd);
        Check(handle != 0, "nvnCommandBufferEndRecording gives a handle");
        SyncWait(&sync0, ~0ull);
        SyncWait(&window_sync, ~0ull);
        SubmitCommands(&queue, 1, &handle);
        QueueFenceSync(&queue, &sync0, 0, 0);
        QueueFlush(&queue);
        PresentTexture(&queue, &window, index);
        const std::int32_t previous = index;
        Check(WindowAcquireTexture(&window, &window_sync, &index) == 0 && index == (previous + 1) % 2,
              "nvnWindowAcquireTexture alternates the two textures");
    }
    const QueueStats qs = GetQueueStats(&queue);
    Check(qs.submits == 3 && qs.submitted_handles == 3 && qs.presents == 3 && qs.fences == 4,
          "the queue saw 3 submits, 3 presents and 4 fences");
    const CommandBufferStats cs = GetCommandBufferStats(&cmd_builders[0]);
    Check(cs.recordings == 2 && !cs.recording && cs.has_command_memory && cs.has_control_memory &&
              cs.has_memory_callback && cs.viewport[2] == 1280,
          "command buffer 0 recorded twice with its memory and viewport");
    const WindowStats ws = GetWindowStats(&window);
    Check(ws.textures == 2 && ws.acquires == 4 && ws.native_window == &native_window, "the window's textures and acquires");

    // With the installed shaders and the dump's packages: the drawing side (DrawFrames).
    if (argc >= 3)
        DrawFrames(argv[1], argv[2], {&device, &queue, &sync0, &window, &window_sync, &cmd_builders[0], &engine_pools[3],
                                      &render_targets[0], &render_targets[1], &depth_target, &texture_pool, &texture_builder,
                                      &blend, &channel_mask, &color, &depth_stencil, &multisample, &polygon, color_ids[1],
                                      cube_id, sampler_id, next_texture_id});

    Check(TrapsCalled().empty(), "the start-up replay reaches no unimplemented NVN function");
    // Every name the game fetches resolves (trapped or not); a duplicate registration is a bug.
    Check(DuplicateRegistrations().empty(), "no NVN name registered twice");

    std::printf("nvn shim: %zu NVN functions implemented; the start-up replay reached %zu unimplemented ones%s\n",
                Registered().size(), TrapsCalled().size(), TrapsCalled().empty() ? "" : ", in order:");
    for (const std::string& name : TrapsCalled())
        std::printf("  %s\n", name.c_str());
    // The engine finalises the device with pools alive (the shutdown fn_7100132730); the shim must
    // release their Vulkan objects first (validation reports leaks at vkDestroyDevice). One pool
    // gets a Vulkan buffer first so that there is something to release.
    {
        auto MemoryPoolGetBufferAddress = Fn<BufferAddress (*)(const void*)>("nvnMemoryPoolGetBufferAddress");
        VkBuffer vk = VK_NULL_HANDLE;
        VkDeviceSize off = 0;
        Check(ResolveGpuAddress(MemoryPoolGetBufferAddress(&engine_pools[4]), 0x100, &vk, &off),
              "a 1 MB engine pool resolves");
    }
    Fn<void (*)(void*)>("nvnDeviceFinalize")(&device);
    Check(CurrentBackend() == nullptr, "nvnDeviceFinalize destroys the backend");
    Check(PoolsWithVulkanBuffer() == 0, "nvnDeviceFinalize released the pools' Vulkan buffers");
    auto MemoryPoolFinalize = Fn<void (*)(void*)>("nvnMemoryPoolFinalize");
    for (Storage& pool : pools)
        MemoryPoolFinalize(&pool);
    for (int i = 0; i < 5; ++i) {
        MemoryPoolFinalize(&engine_pools[i]);
        std::free(engine_storage[i]);
    }
    Check(MemoryPoolCount() == 0, "every memory pool finalised");
    const int errors = ValidationErrors();  // after the device is gone: leaks are reported there
    std::printf("%d failures, %d Vulkan validation errors\n", g_failures, errors);
    return g_failures || errors ? 1 : 0;
}
