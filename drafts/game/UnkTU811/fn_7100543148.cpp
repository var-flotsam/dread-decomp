// CEmmyAIComponent's unit (tools/tu_map.py unit 811): draws the Emmy's fading diamond marker. Advances
// its timer by dt up to two seconds, rebuilds the diamond's vertices, colours and indices into the
// other of two draw slots, places it 20 units below its position, scales it in and switches it on.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU140.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;
using base::math::CVector4D;

typedef u32 __attribute__((may_alias)) a_u32_9230;

struct UnkRecord16_9230 {
    u32 words[4];

    UnkRecord16_9230() : words{0, 0, 0, 0} {}
    UnkRecord16_9230(UnkRecord16_9230&& o) {
        ((a_u32_9230*)this)[0] = ((const a_u32_9230*)&o)[0];
        ((a_u32_9230*)this)[1] = ((const a_u32_9230*)&o)[1];
        ((a_u32_9230*)this)[2] = ((const a_u32_9230*)&o)[2];
        ((a_u32_9230*)this)[3] = ((const a_u32_9230*)&o)[3];
    }
};

UnkRecord16_9230* fn_7100149230(base::global::CRntVector<UnkRecord16_9230>* self, u32 index);
void fn_7100148148(UnkSceneObject* self, u32 layer, const UnkSceneSpan& a, const UnkSceneSpan& b);
void fn_7100149488(UnkSceneObject* object, const UnkVec3f* scale);
f32 fn_7100076840(f32 t);

struct UnkEmmyMarker_l543148 {
    UnkSceneObject* object;  // 0x0
    f32 time;                // 0x8
    UnkVec3f position;       // 0xc
};

struct UnkEmmyAI_l543148 {
    u8 pad_0x0[0x14bf0];
    UnkEmmyMarker_l543148 marker;  // 0x14bf0
    u8 pad_0x14c08[0x2a6b4 - 0x14c08];
    u32 markerLayer;               // 0x2a6b4
};

struct UnkMarkerVertices_l543148 {
    u32 count;
    CVector3D items[4];
};

static inline f32 Clamp01(f32 value) {
    return value > 0.0f ? (!(value >= 1.0f) ? value : 1.0f) : 0.0f;
}

void fn_7100543148(UnkEmmyAI_l543148* self, f32 dt) {
    UnkEmmyMarker_l543148& marker = self->marker;
    fn_71001be430(marker.object->field_0x130, 0, false);
    fn_71001be430(marker.object->field_0x130, 1, false);
    fn_7100149490(marker.object, false);
    if (!(marker.time < 2.0f))
        return;

    f32 time = marker.time + dt;
    marker.time = !(time >= 2.0f) ? time : 2.0f;
    CVector4D color = {1.0f, 1.0f, 0.0f, fn_7100076840(1.0f - Clamp01((marker.time - 0.1f) / 1.9f)) * 0.75f};
    UnkMarkerVertices_l543148 vertices = {
        4, {{0.0f, 50.0f, 0.0f}, {50.0f, 0.0f, 0.0f}, {0.0f, -50.0f, 0.0f}, {-50.0f, 0.0f, 0.0f}}};

    base::UnkAllocator* allocator = GetDefaultAllocator();
    base::global::CRntVector<UnkRecord16_9230> colors(0xf);
    colors.Reserve(4);
    for (u32 i = 0; i < 4; i++)
        fn_710007b518(reinterpret_cast<CVector4D*>(fn_7100149230(&colors, i)), &color);
    // The diamond as a fan of two triangles around its first vertex.
    u16* indices = static_cast<u16*>(allocator->vf_0x40(6 * sizeof(u16), 8, 0xf));
    for (u32 i = 0; i < 2; i++) {
        indices[i * 3 + 0] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }

    fn_71001be430(marker.object->field_0x130, 0, false);
    fn_71001be430(marker.object->field_0x130, 1, false);
    self->markerLayer = self->markerLayer == 0;
    fn_7100148148(marker.object, self->markerLayer, UnkSceneSpan{vertices.items, vertices.count},
                  UnkSceneSpan{colors.data, colors.count});
    fn_7100148178(marker.object, self->markerLayer, UnkSceneSpan{indices, 6}, 1);

    UnkVec3f position(marker.position.x + 0.0f, marker.position.y - 20.0f, marker.position.z + 0.0f);
    fn_7100149460(marker.object, &position);
    f32 scale = fn_7100076840(Clamp01(marker.time / 0.1f)) + 0.5f;
    UnkVec3f scales(scale, scale, scale);
    fn_7100149488(marker.object, &scales);
    fn_71001be430(marker.object->field_0x130, self->markerLayer, true);
    fn_7100149490(marker.object, true);

    colors.Free();
    allocator->vf_0x48(indices);
}
