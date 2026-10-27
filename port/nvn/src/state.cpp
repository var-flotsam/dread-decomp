// NVN's fixed-function and vertex state objects (blend, channel mask, colour, depth-stencil,
// multisample, polygon, vertex attribute, vertex stream). Signatures are those of the game's calls:
// fn_71001587f0 sets each fixed-function state to its defaults and then its polygon, depth-stencil and
// multisample fields; the render-state setters fn_710015a500 (blend function), fn_710015a680 (blend
// equation), fn_710015a3e0 (blend enable), fn_710015a760 and fn_710015a7d0 (alpha test),
// fn_710015a0e0 and fn_710015a260 (channel masks), fn_710015a8c0 (depth function), fn_710015acf0 and
// fn_710015afd0 (stencil), fn_710015ab00 (cull face), fn_710015aa80 (polygon mode); the vertex setup
// fn_7100161fd0 and fn_7100162530 for the vertex attribute and stream states. What each number means
// is in nvn_shim/draw.h. The command buffer copies a state when the game binds it (draw_state.h).

#include "draw_state.h"
#include "nvn_shim/procs.h"
#include "objects.h"

namespace dread::nvn {

namespace {

detail::Registry<detail::BlendState> g_blend;
detail::Registry<detail::ChannelMaskState> g_channel_mask;
detail::Registry<detail::ColorState> g_color;
detail::Registry<detail::DepthStencilState> g_depth_stencil;
detail::Registry<detail::MultisampleState> g_multisample;
detail::Registry<detail::PolygonState> g_polygon;
detail::Registry<detail::VertexAttribState> g_vertex_attrib;
detail::Registry<detail::VertexStreamState> g_vertex_stream;

template <class T>
void Defaults(detail::Registry<T>& registry, void* object) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    registry.Reset(object);
}

void BlendStateSetDefaults(void* o) { Defaults(g_blend, o); }
void ChannelMaskStateSetDefaults(void* o) { Defaults(g_channel_mask, o); }
void ColorStateSetDefaults(void* o) { Defaults(g_color, o); }
void DepthStencilStateSetDefaults(void* o) { Defaults(g_depth_stencil, o); }
void MultisampleStateSetDefaults(void* o) { Defaults(g_multisample, o); }
void PolygonStateSetDefaults(void* o) { Defaults(g_polygon, o); }
void VertexAttribStateSetDefaults(void* o) { Defaults(g_vertex_attrib, o); }
void VertexStreamStateSetDefaults(void* o) { Defaults(g_vertex_stream, o); }

bool ValidTarget(std::int32_t target) { return target >= 0 && target < detail::kColorTargets; }

// --- blend, colour, channel mask ---

void BlendStateSetBlendTarget(void* o, std::int32_t target) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_blend.Get(o).target = target;
}
void BlendStateSetBlendFunc(void* o, std::int32_t src, std::int32_t dst, std::int32_t src_alpha, std::int32_t dst_alpha) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    detail::BlendTargetState& b = g_blend.Get(o).blend;
    b.src = src;
    b.dst = dst;
    b.src_alpha = src_alpha;
    b.dst_alpha = dst_alpha;
}
void BlendStateSetBlendEquation(void* o, std::int32_t color, std::int32_t alpha) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    detail::BlendTargetState& b = g_blend.Get(o).blend;
    b.color_equation = color;
    b.alpha_equation = alpha;
}
void ColorStateSetBlendEnable(void* o, std::int32_t target, std::uint8_t enable) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (ValidTarget(target))
        g_color.Get(o).blend[target] = enable & 1;
}
void ColorStateSetAlphaTest(void* o, std::int32_t func) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_color.Get(o).alpha_func = func;
}
void ChannelMaskStateSetChannelMask(void* o, std::int32_t target, std::uint8_t r, std::uint8_t g, std::uint8_t b,
                                    std::uint8_t a) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    if (ValidTarget(target))
        g_channel_mask.Get(o).mask[target] =
            static_cast<std::uint8_t>((r & 1) | (g & 1) << 1 | (b & 1) << 2 | (a & 1) << 3);
}

// --- polygon, depth-stencil, multisample ---

void PolygonStateSetCullFace(void* o, std::int32_t v) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_polygon.Get(o).cull_face = v;
}
void PolygonStateSetFrontFace(void* o, std::int32_t v) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_polygon.Get(o).front_face = v;
}
void PolygonStateSetPolygonMode(void* o, std::int32_t v) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_polygon.Get(o).polygon_mode = v;
}
void DepthStencilStateSetDepthTestEnable(void* o, std::uint8_t v) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_depth_stencil.Get(o).depth_test = v & 1;
}
void DepthStencilStateSetDepthWriteEnable(void* o, std::uint8_t v) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_depth_stencil.Get(o).depth_write = v & 1;
}
void DepthStencilStateSetStencilTestEnable(void* o, std::uint8_t v) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_depth_stencil.Get(o).stencil_test = v & 1;
}
void DepthStencilStateSetDepthFunc(void* o, std::int32_t v) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_depth_stencil.Get(o).depth_func = v;
}
void DepthStencilStateSetStencilFunc(void* o, std::int32_t face, std::int32_t func) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    detail::DepthStencilState& s = g_depth_stencil.Get(o);
    if (detail::FaceHasFront(face))
        s.front.func = func;
    if (detail::FaceHasBack(face))
        s.back.func = func;
}
void DepthStencilStateSetStencilOp(void* o, std::int32_t face, std::int32_t fail, std::int32_t depth_fail,
                                   std::int32_t depth_pass) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    detail::DepthStencilState& s = g_depth_stencil.Get(o);
    for (detail::StencilFaceState* f : {detail::FaceHasFront(face) ? &s.front : nullptr,
                                        detail::FaceHasBack(face) ? &s.back : nullptr}) {
        if (!f)
            continue;
        f->fail = fail;
        f->depth_fail = depth_fail;
        f->depth_pass = depth_pass;
    }
}
void MultisampleStateSetMultisampleEnable(void* o, std::int32_t v) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_multisample.Get(o).enable = v & 1;
}

// --- vertex attributes and streams ---

void VertexAttribStateSetFormat(void* o, std::int32_t format, std::int32_t relative_offset) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    detail::VertexAttribState& a = g_vertex_attrib.Get(o);
    a.format = format;
    a.relative_offset = relative_offset;
}
void VertexAttribStateSetStreamIndex(void* o, std::int32_t stream) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_vertex_attrib.Get(o).stream = stream;
}
// The game passes a 32-bit stride (fn_7100161fd0: w1).
void VertexStreamStateSetStride(void* o, std::int32_t stride) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_vertex_stream.Get(o).stride = stride;
}
void VertexStreamStateSetDivisor(void* o, std::int32_t divisor) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_vertex_stream.Get(o).divisor = divisor;
}

}  // namespace

const detail::BlendState* detail::FindBlendState(const void* s) { return g_blend.Find(s); }
const detail::ColorState* detail::FindColorState(const void* s) { return g_color.Find(s); }
const detail::ChannelMaskState* detail::FindChannelMaskState(const void* s) { return g_channel_mask.Find(s); }
const detail::DepthStencilState* detail::FindDepthStencilState(const void* s) { return g_depth_stencil.Find(s); }
const detail::MultisampleState* detail::FindMultisampleState(const void* s) { return g_multisample.Find(s); }
const detail::PolygonState* detail::FindPolygonState(const void* s) { return g_polygon.Find(s); }
const detail::VertexAttribState* detail::FindVertexAttribState(const void* s) { return g_vertex_attrib.Find(s); }
const detail::VertexStreamState* detail::FindVertexStreamState(const void* s) { return g_vertex_stream.Find(s); }

void detail::RegisterStateProcs() {
    Register("nvnBlendStateSetDefaults", &BlendStateSetDefaults);
    Register("nvnBlendStateSetBlendTarget", &BlendStateSetBlendTarget);
    Register("nvnBlendStateSetBlendFunc", &BlendStateSetBlendFunc);
    Register("nvnBlendStateSetBlendEquation", &BlendStateSetBlendEquation);
    Register("nvnChannelMaskStateSetDefaults", &ChannelMaskStateSetDefaults);
    Register("nvnChannelMaskStateSetChannelMask", &ChannelMaskStateSetChannelMask);
    Register("nvnColorStateSetDefaults", &ColorStateSetDefaults);
    Register("nvnColorStateSetBlendEnable", &ColorStateSetBlendEnable);
    Register("nvnColorStateSetAlphaTest", &ColorStateSetAlphaTest);
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
    Register("nvnVertexAttribStateSetDefaults", &VertexAttribStateSetDefaults);
    Register("nvnVertexAttribStateSetFormat", &VertexAttribStateSetFormat);
    Register("nvnVertexAttribStateSetStreamIndex", &VertexAttribStateSetStreamIndex);
    Register("nvnVertexStreamStateSetDefaults", &VertexStreamStateSetDefaults);
    Register("nvnVertexStreamStateSetStride", &VertexStreamStateSetStride);
    Register("nvnVertexStreamStateSetDivisor", &VertexStreamStateSetDivisor);
}

}  // namespace dread::nvn
