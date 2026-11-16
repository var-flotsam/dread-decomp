// Unit 110 (2D geometry): triangulates every polygon of a list (fn_7100131370) and appends the
// vertices and the triangle indices, rebased onto the vertices already there, to two arrays.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector2D;
using base::math::CVector3D;

// The caller's view of the vertex array (src/game/UnkTU2563/UnkTU2563_Records_ovb0223.cpp).
struct UnkBoundsRec_b0223 {
    CVector3D* points;  // 0x0
    u32 count;          // 0x8
    u8 unk_0xc[0x1c];
    u8 field_0x28[0x28];
    CVector2D min;  // 0x50
    CVector2D max;  // 0x58
};

// A mesh vertex: three words, copied one by one (their type is a placeholder).
struct UnkMeshVertex {
    u32 field_0x0[3];

    UnkMeshVertex(const UnkMeshVertex& o) {
        for (u32 i = 0; i < 3; ++i)
            field_0x0[i] = o.field_0x0[i];
    }
};

// A polygon of the list (0x48 bytes).
struct UnkPolygonShape {
    u8 pad_0x0[0x48];
};

// Triangulates one polygon into a vertex array and a u16 index array.
void fn_7100131370(const UnkPolygonShape* polygon, CRntVector<UnkMeshVertex>* vertices, CRntVector<u16>* indices,
                   bool flag);

void fn_7100131020(void* shape, UnkBoundsRec_b0223* rec, void* v, bool flag) {
    CRntVector<UnkPolygonShape>* polygons = static_cast<CRntVector<UnkPolygonShape>*>(shape);
    CRntVector<UnkMeshVertex>* vertices = reinterpret_cast<CRntVector<UnkMeshVertex>*>(rec);
    CRntVector<u16>* indices = static_cast<CRntVector<u16>*>(v);

    CRntVector<UnkMeshVertex> polygonVertices(0xf);
    CRntVector<u16> polygonIndices(0xf);
    if (polygons->count != 0) {
        u16 base = vertices->count;
        const UnkPolygonShape* end = polygons->data + polygons->count;
        for (const UnkPolygonShape* polygon = polygons->data; polygon != end; ++polygon) {
            polygonVertices.Clear();
            polygonIndices.Clear();
            fn_7100131370(polygon, &polygonVertices, &polygonIndices, flag);
            for (const UnkMeshVertex* p = polygonVertices.data, *e = p + polygonVertices.count; p != e; ++p)
                vertices->PushBackCopy(*p);
            for (const u16* p = polygonIndices.data, *e = p + polygonIndices.count; p != e; ++p)
                indices->PushBack((u16)(*p + base));
            base += polygonVertices.count;
        }
    }
    polygonIndices.Free();
    polygonVertices.Free();
}
