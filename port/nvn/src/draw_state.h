#pragma once

// The fixed-function and vertex state objects as state.cpp keeps them, for the command buffer to
// copy when the game binds one: NVN's bind takes the state's values at the time of the call
// (fn_7100159ce0 binds one blend state eight times, changing its target in between). Values are
// NVN's numbers as the game passes them (nvn_shim/draw.h). Callers hold detail::StateLock().

#include <cstdint>

#include "nvn_shim/draw.h"

namespace dread::nvn::detail {

constexpr int kColorTargets = 8;    // fn_7100159ce0 binds blend targets 0-7; fn_710015a3e0 enables 0-7
constexpr int kVertexAttribs = 16;  // fn_7100161fd0 resets 16 attribute and 16 stream states
constexpr int kVertexStreams = 16;

struct BlendTargetState {
    std::int32_t src = kBlendOne, dst = kBlendZero, src_alpha = kBlendOne, dst_alpha = kBlendZero;
    std::int32_t color_equation = kBlendAdd, alpha_equation = kBlendAdd;
};
struct BlendState {
    std::int32_t target = 0;  // nvnBlendStateSetBlendTarget: which colour target a bind sets
    BlendTargetState blend;
};
struct ColorState {
    bool blend[kColorTargets] = {};
    std::int32_t alpha_func = kCompareAlways;  // nvnColorStateSetAlphaTest; 8 is off (fn_710015a760)
};
struct ChannelMaskState {
    // Bit 0 red .. bit 3 alpha (nvnChannelMaskStateSetChannelMask(state, target, r, g, b, a)).
    std::uint8_t mask[kColorTargets] = {0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};
};
struct StencilFaceState {
    std::int32_t func = kCompareAlways;
    std::int32_t fail = kStencilKeep, depth_fail = kStencilKeep, depth_pass = kStencilKeep;
};
struct DepthStencilState {
    bool depth_test = false, depth_write = false, stencil_test = false;
    std::int32_t depth_func = kCompareLess;
    StencilFaceState front, back;
};
struct MultisampleState {
    bool enable = false;
};
struct PolygonState {
    std::int32_t cull_face = kFaceNone;
    std::int32_t front_face = kFrontFaceCounterClockwise;
    std::int32_t polygon_mode = kPolygonFill;
};
// A vertex attribute's format 0 is "not fed" (its SetDefaults): fn_7100161fd0 resets all 16 and
// sets the format only of those its vertex buffer has.
struct VertexAttribState {
    std::int32_t format = 0;
    std::int32_t relative_offset = 0;
    std::int32_t stream = 0;
};
struct VertexStreamState {
    std::int32_t stride = 0;
    std::int32_t divisor = 0;  // 0 per vertex, 1 per instance (fn_7100162530's second buffer)
};

// The state at an object's storage, null when the game never set one there.
const BlendState* FindBlendState(const void* storage);
const ColorState* FindColorState(const void* storage);
const ChannelMaskState* FindChannelMaskState(const void* storage);
const DepthStencilState* FindDepthStencilState(const void* storage);
const MultisampleState* FindMultisampleState(const void* storage);
const PolygonState* FindPolygonState(const void* storage);
const VertexAttribState* FindVertexAttribState(const void* storage);
const VertexStreamState* FindVertexStreamState(const void* storage);

// The Vulkan faces an NVN face mask names (Face: 1 back, 2 front).
inline bool FaceHasFront(std::int32_t face) { return (face & kFaceFront) != 0; }
inline bool FaceHasBack(std::int32_t face) { return (face & kFaceBack) != 0; }

}  // namespace dread::nvn::detail
