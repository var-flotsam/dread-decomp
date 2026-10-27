#pragma once

// The numbers the game passes to NVN's drawing functions, and what each means. Nothing here comes
// from NVN's SDK. The engine keeps its render state in its own reflected enumerations
// (msapi::api::types::* and msapi::api::idx::*, whose value names the engine's reflection registers;
// mercury-engine-data-structures lists them in dread_types.json) and converts each to the number it
// passes to NVN in one place, cited per enumeration below. So the meaning of each NVN number is the
// engine value that becomes it. The names are ours (placeholders).
//
// Two device modes are read from the game's shaders. The start-up sets nvnDeviceSetDepthMode(0) and
// nvnDeviceSetWindowOriginMode(1) (fn_71001587f0), and the deferred lighting passes rebuild a
// position from the depth buffer as clip-space z = 2 * depth - 1 (a -1..1 depth range) and
// clip-space y = (1 - v) * 2 - 1 with v = gl_FragCoord.y / height (row 0 of a render target is
// clip-space y = +1: an upper-left origin, y up in clip space). The shim draws with
// VK_EXT_depth_clip_control's -1..1 range and flips each viewport vertically.

#include <cstdint>

namespace dread::nvn {

// Comparison functions (depth, stencil, alpha test). The depth setter fn_710015a8c0, the stencil
// setter fn_710015acf0 and the alpha test fn_710015a7d0 (through its table at 0x7101647da8) convert
// cmpMode::ECmpMode alike: ALWAYS 0, NEVER 1, EQUAL 2, NOTEQUAL 3, LESS 4, LESSEQUAL 5, GREATER 6,
// GREATEREQUAL 7 become 8, 1, 3, 6, 2, 4, 5, 7. fn_710015a760 sets 8 to switch the alpha test off.
enum CompareFunc : std::int32_t {
    kCompareNever = 1,
    kCompareLess = 2,
    kCompareEqual = 3,
    kCompareLessEqual = 4,
    kCompareGreater = 5,
    kCompareNotEqual = 6,
    kCompareGreaterEqual = 7,
    kCompareAlways = 8,
};

// Blend factors. fn_710015a500 converts blendFactor::EBlendFactor (ZERO, ONE, SRC_COLOR,
// INV_SRC_COLOR, DST_COLOR, INV_DST_COLOR, SRC_ALPHA, INV_SRC_ALPHA, DST_ALPHA, INV_DST_ALPHA) to 1,
// 2, 3, 4, 9, 10, 5, 6, 7, 8 and calls nvnBlendStateSetBlendFunc(state, src, dst, src, dst): the
// colour factors, then the alpha factors.
enum BlendFactor : std::int32_t {
    kBlendZero = 1,
    kBlendOne = 2,
    kBlendSrcColor = 3,
    kBlendOneMinusSrcColor = 4,
    kBlendSrcAlpha = 5,
    kBlendOneMinusSrcAlpha = 6,
    kBlendDstAlpha = 7,
    kBlendOneMinusDstAlpha = 8,
    kBlendDstColor = 9,
    kBlendOneMinusDstColor = 10,
};

// Blend equations. fn_710015a680 converts blendOp::EBlendOp (ADD, SUBDST, SUBSRC, MIN, MAX) to 1-5 and
// calls nvnBlendStateSetBlendEquation(state, colour, alpha). SUBDST is read as source minus
// destination and SUBSRC as destination minus source (provisional: the engine's names only).
enum BlendEquation : std::int32_t {
    kBlendAdd = 1,
    kBlendSubtract = 2,
    kBlendReverseSubtract = 3,
    kBlendMin = 4,
    kBlendMax = 5,
};

// Stencil operations. fn_710015afd0 (its table at 0x7101647dc8 and its switch agree) converts
// stencilOp::EStencilOp (KEEP, ZERO, REPLACE, INCRSAT, DECRSAT, INVERT, INCR, DECR) to 1, 2, 3, 7, 8,
// 6, 4, 5 and calls nvnDepthStencilStateSetStencilOp(state, face, fail, depth fail, depth pass).
// INCRSAT and DECRSAT are read as clamping and INCR and DECR as wrapping, as in Direct3D, whose
// names the engine's follow (provisional).
enum StencilOp : std::int32_t {
    kStencilKeep = 1,
    kStencilZero = 2,
    kStencilReplace = 3,
    kStencilIncrementWrap = 4,
    kStencilDecrementWrap = 5,
    kStencilInvert = 6,
    kStencilIncrementClamp = 7,
    kStencilDecrementClamp = 8,
};

// Faces. fn_710015ab00 converts cullMode::ECullMode (CW 2, CCW 3, NONE 4) to 1, 2, 0 for
// nvnPolygonStateSetCullFace; the stencil setters (fn_710015acf0, fn_710015afd0) and the stencil
// mask and reference (fn_710015b160) set faces 2 and 1 alike. The start-up sets the front face to 1
// (fn_71001587f0), the only front face the game sets. Which of faces 1 and 2 NVN calls front does
// not matter: with the front face 1, CULL_CW culls clockwise triangles either way. The shim takes
// front face 1 as counter-clockwise, face 1 as the back and face 2 as the front.
enum Face : std::int32_t {
    kFaceNone = 0,
    kFaceBack = 1,
    kFaceFront = 2,
    kFaceBoth = 3,
};
constexpr std::int32_t kFrontFaceCounterClockwise = 1;

// Polygon modes. fn_710015aa80 converts fillMode::EFillMode (SOLID 0, WIRE 1) to 2, 1.
enum PolygonMode : std::int32_t {
    kPolygonLine = 1,
    kPolygonFill = 2,
};

// Primitives. The index buffer setters fn_7100154020 and fn_7100154380 convert
// idx::EPrimitiveType: TRIANGLES (and anything unknown) to 4, and TRIANGLE_STRIP, TRIANGLE_FAN,
// LINES, LINE_STRIP to 5, 6, 1, 3 (their table at 0x7101647bac); the draws pass the result.
enum Primitive : std::int32_t {
    kPrimitiveLines = 1,
    kPrimitiveLineStrip = 3,
    kPrimitiveTriangles = 4,
    kPrimitiveTriangleStrip = 5,
    kPrimitiveTriangleFan = 6,
};

// Index types. The same setters convert idx::EDataType (BYTE 1, SHORT 2, INT 3) to 0, 1, 2; the
// draws (fn_7100159f90, fn_710015ac00) step through indices 1, 2, 4 bytes apart (the table at
// 0x7101647eb0).
enum IndexType : std::int32_t {
    kIndexUint8 = 0,
    kIndexUint16 = 1,
    kIndexUint32 = 2,
};

// Vertex attribute formats. fn_7100161e60 gives an engine vertex element (components << 16 | type)
// the NVN format: type 4 is 0x25 (the texture format of that number, R8G8B8A8: texture.cpp), any
// other type 1-4 components 0xa (R32 float, likewise), 0x16, 0x22, 0x2e: one to four 32-bit floats
// (fn_7100161fd0 strides types 2 and 3 four bytes a component). A new element has 0x16.
enum VertexFormat : std::int32_t {
    kVertexR32Float = 0x0a,
    kVertexRG32Float = 0x16,
    kVertexRGB32Float = 0x22,
    kVertexRGBA32Float = 0x2e,
    kVertexRGBA8Unorm = 0x25,
};

// What the shim's draw path has done (for checks).
struct DrawStats {
    std::uint64_t draws = 0;            // draw calls recorded into Vulkan command buffers
    std::uint64_t skipped_draws = 0;    // draws without a program, render target or index buffer
    std::uint64_t pipelines = 0;        // Vulkan pipelines made
    std::uint64_t clears = 0;
    std::uint64_t render_passes = 0;    // vkCmdBeginRendering
    std::uint64_t barriers = 0;
    std::uint64_t uniform_updates = 0;  // nvnCommandBufferUpdateUniformBuffer
    std::uint64_t recordings = 0;       // command buffers ended
    std::uint64_t submitted = 0;        // command buffers submitted
    std::uint64_t descriptor_warnings = 0;
};
DrawStats GetDrawStats();

// Where nvnProgramSetShaders finds the installed SPIR-V and shaders.manifest (tools/bshdat.py
// --install; the `shaders` target's DREAD_SHADER_DIR). Without a call, $DREAD_NVN_SHADERS. The
// manifest is read on the first nvnProgramSetShaders; false with a message if it cannot be.
void SetShaderDirectory(const char* dir);
bool ShadersLoaded();

}  // namespace dread::nvn
