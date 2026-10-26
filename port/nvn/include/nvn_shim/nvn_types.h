#pragma once

// The NVN values the binding layer handles, as the game uses them. Nothing here comes from NVN's
// SDK headers: the names are ours (placeholders), and each value or layout is inferred from the
// game's own call sites through its NVN function table (fn_7100154770 fills it from
// nvnDeviceGetProcAddress; the names are the strings it asks for). Where a call site is cited, the
// argument order and constants come from it.

#include <cstdint>

namespace dread::nvn {

// The shader stage argument of nvnCommandBufferBindUniformBuffer and nvnCommandBufferBindTexture.
// The engine's program binding (fn_710015d900) passes 0 for the vertex stage, 1 for the fragment
// stage and 2 for the program's third, always empty, stage (slot 0 of a .bshdat program record;
// tools/bshdat.py's docstring).
enum class Stage : std::uint32_t {
    Vertex = 0,
    Fragment = 1,
    Geometry = 2,  // never present in Metroid Dread's shaders
};

// nvnCommandBufferBindProgram's stage mask: fn_710015d900 binds a program with the OR of its stages'
// bits (the engine's shader loader fn_710015c9b0 gives the vertex stage 1, the fragment stage 2 and
// the third stage 4) and unbinds with (nullptr, 7).
enum StageBits : std::uint32_t {
    kStageBitVertex = 1,
    kStageBitFragment = 2,
    kStageBitGeometry = 4,
    kStageBitsAll = 7,
};

// A GPU virtual address (nvnBufferGetAddress's result, the uniform-buffer argument of
// nvnCommandBufferBindUniformBuffer). The shim allocates the address space, so it can resolve one
// back to a Vulkan buffer and offset (AddressResolver).
using BufferAddress = std::uint64_t;

// What nvnDeviceGetTextureHandle returns and nvnCommandBufferBindTexture takes: a texture pool
// index and a sampler pool index combined (fn_7100161250 passes the texture's pool index, then the
// sampler's). The game never looks inside one, so the encoding is the shim's own:
// texture index in the low 32 bits, sampler index in the high 32. No shader uses 64-bit (bindless)
// handles (tools/bshdat.py: only the Shader capability), so a handle only ever reaches a texture
// binding slot.
using TextureHandle = std::uint64_t;

constexpr TextureHandle MakeTextureHandle(std::uint32_t texture_id, std::uint32_t sampler_id) {
    return (static_cast<TextureHandle>(sampler_id) << 32) | texture_id;
}
constexpr std::uint32_t TextureId(TextureHandle handle) { return static_cast<std::uint32_t>(handle); }
constexpr std::uint32_t SamplerId(TextureHandle handle) { return static_cast<std::uint32_t>(handle >> 32); }

// One element of the array nvnProgramSetShaders takes: the engine's program creation
// (fn_710015d760) fills one per present stage, 16 bytes each: the GPU address of the buffer it
// copied the stage's code into, then a CPU pointer to the stage's control section (section 0 of
// the stage blob; tools/bshdat.py). The shim recognises the stage by the control section
// (ShaderManifest::FindStage) and never reads the code.
struct ShaderData {
    BufferAddress code;
    const void* control;
};
static_assert(sizeof(ShaderData) == 16, "fn_710015d760 steps through the array 16 bytes at a time");

// The per-stage slot counts the engine uses: fn_7100161250 and fn_710015f500 bind texture slots
// 0..15 of each stage; a stage of Metroid Dread has at most five active uniform blocks and declares
// samplers at bindings 0..12 (an array reaches 13).
constexpr std::uint32_t kTextureSlots = 16;
constexpr std::uint32_t kUniformBufferSlots = 16;

}  // namespace dread::nvn
