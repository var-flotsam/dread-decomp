// NVN's fixed-function state objects (blend, channel mask, colour, depth-stencil, multisample,
// polygon). Signatures are those of fn_71001587f0, which sets each to its defaults and then sets the
// polygon, depth-stencil and multisample fields it wants. The values are NVN's enumerations as the
// game passes them; the pipeline layer will translate them into Vulkan state when it is written,
// with each value's meaning taken from the game's uses. Until then they are kept as given.

#include "nvn_shim/procs.h"
#include "objects.h"

namespace dread::nvn {

namespace {

struct BlendState {};
struct ChannelMaskState {};
struct ColorState {};
struct DepthStencilState {
    std::uint8_t depth_test = 0, depth_write = 0, stencil_test = 0;
    std::int32_t depth_func = 0;
    std::int32_t stencil_face = 0, stencil_func = 0;
    std::int32_t stencil_op_face = 0, stencil_fail = 0, depth_fail = 0, depth_pass = 0;
};
struct MultisampleState {
    std::int32_t enable = 0;
};
struct PolygonState {
    std::int32_t cull_face = 0, front_face = 0, polygon_mode = 0;
};

detail::Registry<BlendState> g_blend;
detail::Registry<ChannelMaskState> g_channel_mask;
detail::Registry<ColorState> g_color;
detail::Registry<DepthStencilState> g_depth_stencil;
detail::Registry<MultisampleState> g_multisample;
detail::Registry<PolygonState> g_polygon;

template <class T>
void Defaults(detail::Registry<T>& registry, void* object) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    registry.Reset(object);
}

void BlendStateSetDefaults(void* o) { Defaults(g_blend, o); }
void ChannelMaskStateSetDefaults(void* o) { Defaults(g_channel_mask, o); }
void ColorStateSetDefaults(void* o) { Defaults(g_color, o); }
void DepthStencilStateSetDefaults(void* o) { Defaults(g_depth_stencil, o); }
void MultisampleStateSetDefaults(void* o) { Defaults(g_multisample, o); }
void PolygonStateSetDefaults(void* o) { Defaults(g_polygon, o); }

void PolygonStateSetCullFace(void* o, std::int32_t v) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_polygon.Get(o).cull_face = v;
}
void PolygonStateSetFrontFace(void* o, std::int32_t v) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_polygon.Get(o).front_face = v;
}
void PolygonStateSetPolygonMode(void* o, std::int32_t v) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_polygon.Get(o).polygon_mode = v;
}
void DepthStencilStateSetDepthTestEnable(void* o, std::uint8_t v) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_depth_stencil.Get(o).depth_test = v;
}
void DepthStencilStateSetDepthWriteEnable(void* o, std::uint8_t v) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_depth_stencil.Get(o).depth_write = v;
}
void DepthStencilStateSetStencilTestEnable(void* o, std::uint8_t v) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_depth_stencil.Get(o).stencil_test = v;
}
void DepthStencilStateSetDepthFunc(void* o, std::int32_t v) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_depth_stencil.Get(o).depth_func = v;
}
void DepthStencilStateSetStencilFunc(void* o, std::int32_t face, std::int32_t func) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    DepthStencilState& s = g_depth_stencil.Get(o);
    s.stencil_face = face;
    s.stencil_func = func;
}
void DepthStencilStateSetStencilOp(void* o, std::int32_t face, std::int32_t fail, std::int32_t depth_fail,
                                   std::int32_t depth_pass) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    DepthStencilState& s = g_depth_stencil.Get(o);
    s.stencil_op_face = face;
    s.stencil_fail = fail;
    s.depth_fail = depth_fail;
    s.depth_pass = depth_pass;
}
void MultisampleStateSetMultisampleEnable(void* o, std::int32_t v) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_multisample.Get(o).enable = v;
}

}  // namespace

void detail::RegisterStateProcs() {
    Register("nvnBlendStateSetDefaults", &BlendStateSetDefaults);
    Register("nvnChannelMaskStateSetDefaults", &ChannelMaskStateSetDefaults);
    Register("nvnColorStateSetDefaults", &ColorStateSetDefaults);
    Register("nvnDepthStencilStateSetDefaults", &DepthStencilStateSetDefaults);
    Register("nvnMultisampleStateSetDefaults", &MultisampleStateSetDefaults);
    Register("nvnPolygonStateSetDefaults", &PolygonStateSetDefaults);
    Register("nvnPolygonStateSetCullFace", &PolygonStateSetCullFace);
    Register("nvnPolygonStateSetFrontFace", &PolygonStateSetFrontFace);
    Register("nvnPolygonStateSetPolygonMode", &PolygonStateSetPolygonMode);
    Register("nvnDepthStencilStateSetDepthTestEnable", &DepthStencilStateSetDepthTestEnable);
    Register("nvnDepthStencilStateSetDepthWriteEnable", &DepthStencilStateSetDepthWriteEnable);
    Register("nvnDepthStencilStateSetStencilTestEnable", &DepthStencilStateSetStencilTestEnable);
    Register("nvnDepthStencilStateSetDepthFunc", &DepthStencilStateSetDepthFunc);
    Register("nvnDepthStencilStateSetStencilFunc", &DepthStencilStateSetStencilFunc);
    Register("nvnDepthStencilStateSetStencilOp", &DepthStencilStateSetStencilOp);
    Register("nvnMultisampleStateSetMultisampleEnable", &MultisampleStateSetMultisampleEnable);
}

}  // namespace dread::nvn
