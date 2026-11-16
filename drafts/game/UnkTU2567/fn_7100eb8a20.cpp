// Unit 2567 (the minimap's items): appends a marker's shape to the minimap's draw lists: its indices
// rebased past the vertices already there, its vertices moved by `offset`, and one colour for each
// vertex. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;

struct UnkMinimapDrawList;
struct UnkMinimapMarkerColor;
struct UnkMinimapMarkerArg;

// A vertex colour, copied word by word as UnkVec3f is.
struct UnkMarkerColor_eb8a20 {
    f32 r, g, b, a;
    UnkMarkerColor_eb8a20(const UnkMarkerColor_eb8a20& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
    }
};

// A marker's shape: a triangle list.
struct UnkMarkerShape_eb8a20 {
    CRntVector<UnkVec3f> vertices;  // 0x0
    CRntVector<u16> indices;        // 0x28
};

void fn_7100149810(CRntVector<UnkVec3f>* self, const UnkVec3f* v);

void fn_7100eb8a20(UnkMinimapDrawList* lists0, UnkMinimapDrawList* lists3, UnkMinimapDrawList* lists1,
                   const void* shape, const UnkMinimapMarkerColor& color, const UnkMinimapMarkerArg& arg) {
    if (!shape)
        return;
    const UnkMarkerShape_eb8a20* marker = static_cast<const UnkMarkerShape_eb8a20*>(shape);
    CRntVector<UnkVec3f>* vertices = reinterpret_cast<CRntVector<UnkVec3f>*>(lists0);
    CRntVector<u16>* indices = reinterpret_cast<CRntVector<u16>*>(lists3);
    CRntVector<UnkMarkerColor_eb8a20>* colors = reinterpret_cast<CRntVector<UnkMarkerColor_eb8a20>*>(lists1);
    const UnkMarkerColor_eb8a20& rgba = reinterpret_cast<const UnkMarkerColor_eb8a20&>(color);
    const UnkVec3f& offset = reinterpret_cast<const UnkVec3f&>(arg);

    const u16* index_end = marker->indices.data + marker->indices.count;
    u32 first = vertices->count;
    for (const u16* index = marker->indices.data; index != index_end; ++index)
        indices->PushBack(*index + first);

    const UnkVec3f* vertex_end = marker->vertices.data + marker->vertices.count;
    for (const UnkVec3f* vertex = marker->vertices.data; vertex != vertex_end; ++vertex) {
        UnkVec3f moved(vertex->x + offset.x, vertex->y + offset.y, vertex->z + offset.z);
        fn_7100149810(vertices, &moved);
        colors->PushBackCopy(rgba);
    }
}
