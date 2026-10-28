// From unit 2567 of tools/tu_map.py (0x7100eb695c-0x7100ebf278, no reflected class): the mesh of a
// dashed line along a path (the minimap's marker paths, src/game/CMinimapData.cpp, draw with it).
// Each dash is a quad across the path's direction, or with `flags` bit 0 an arrow (a quad and a
// tip), its vertices at the given depth; the mesh keeps the path's bounds grown by half the width.
// Names and layouts are placeholders.
// Holds fn_7100ebda30 and fn_7100ebc7f0 (a segment as a quad).
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern "C" f32 sqrtf(f32);

// A path: its length, the point at a distance along it (the segment index and the fraction of the
// segment) and the point of a segment, and its bounds (two points, vf_0x38).
struct UnkDashPath {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual const CVector3D* vf_0x38();
};
f32 fn_7101059514(UnkDashPath* path);
void fn_7101059794(f32 distance, UnkDashPath* path, u32* index, f32* fraction);
// A point of the path, handed back in memory (as src/game/CMinimapData.cpp declares it).
struct UnkMinimapPathPoint {
    CVector3D v;
    ~UnkMinimapPathPoint() {}
};
UnkMinimapPathPoint fn_71010596ac(void* path, u32 index, f32 t);
// Assigns the x and y of a 3D vector to a 2D one.
struct UnkVec3;
void fn_710007b444(void* self, const UnkVec3* v);
f32 fn_7100125fbc(const UnkSegment2D* seg);

static inline void Project(CVector2D* out, const UnkMinimapPathPoint& point) {
    fn_710007b430(out, &point.v);
}
static inline void Assign(CVector2D* out, const UnkMinimapPathPoint& point) {
    fn_710007b444(out, reinterpret_cast<const UnkVec3*>(&point.v));
}

// A vertex: a position and its depth (UnkVec3f of game/UnkXformNode.h, copied word by word).
using UnkMeshVertex = UnkVec3f;

struct UnkDashMesh {
    base::UnkVector<UnkMeshVertex> vertices;
    base::UnkVector<u16> indices;
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};
static_assert(__builtin_offsetof(UnkDashMesh, min_x) == 0x50, "UnkDashMesh placeholder layout");

// Appends a vertex (the vertex array's out-of-line PushBack).
void fn_7100149810(base::UnkVector<UnkMeshVertex>* self, const UnkMeshVertex* v);

static inline void Push(UnkDashMesh* mesh, const UnkMeshVertex& v) {
    fn_7100149810(&mesh->vertices, &v);
}

// Fills `mesh` with the dashes of `path`: `dash` long with `gap` between them, `width` wide, at
// depth `z`. A dash shorter than 0.3 of `dash` (where a segment ends) is skipped; an arrow's tip
// adds a third of the width to the distance covered.
// NON_MATCHING(minor): block placement and register allocation only. The original falls through
// into the arrow branch and the quad one is placed after it (a bool third parameter gives that
// order; the callers declare it u32 and only bit 0 is tested either way), so the inlined index
// pushes are laid out differently; gap is kept in s9/s13 and 0.5 is materialized before the
// reserves.
void fn_7100ebda30(void* path_, void* mesh_, u32 flags, f32 width, f32 dash, f32 gap, f32 z) {
    UnkDashPath* path = static_cast<UnkDashPath*>(path_);
    UnkDashMesh* mesh = static_cast<UnkDashMesh*>(mesh_);
    f32 total = fn_7101059514(path);
    s32 n = (s32)(total / (dash + gap));
    mesh->vertices.Reserve(n * 4);
    mesh->indices.Reserve(n * 6);
    f32 half = width * 0.5f;
    f32 shortest = dash * 0.3f;
    f32 tip = width * 0.33333334f;
    f32 t = 0.0f;
    do {
        u32 index = 0;
        f32 fraction = 0.0f;
        fn_7101059794(t, path, &index, &fraction);
        CVector2D start;
        Project(&start, fn_71010596ac(path, index, fraction));
        u32 end_index = 0;
        f32 end_fraction = 0.0f;
        fn_7101059794(t + dash, path, &end_index, &end_fraction);
        CVector2D end = {0.0f, 0.0f};
        if (end_index == index)
            Assign(&end, fn_71010596ac(path, end_index, end_fraction));
        else
            Assign(&end, fn_71010596ac(path, index, 1.0f));
        UnkSegment2D seg(start, end);
        f32 len = fn_7100125fbc(&seg);
        if (len > shortest) {
            f32 dx = seg.end.x - seg.start.x;
            f32 dy = seg.end.y - seg.start.y;
            f32 l = sqrtf(dx * dx + dy * dy);
            if (l != 0.0f) {
                f32 inv = 1.0f / l;
                dx *= inv;
                dy *= inv;
            }
            UnkVec2Result normal = fn_7100125e78(&seg);
            u16 base = (u16)mesh->vertices.count;
            if (flags & 1) {
                f32 ax = tip * dx;
                f32 ay = tip * dy;
                Push(mesh, UnkMeshVertex(seg.start.x - half * normal.x, seg.start.y - half * normal.y, z));
                Push(mesh, UnkMeshVertex(seg.end.x - half * normal.x, seg.end.y - half * normal.y, z));
                Push(mesh, UnkMeshVertex(ax + seg.start.x, ay + seg.start.y, z));
                Push(mesh, UnkMeshVertex(ax + seg.end.x, ay + seg.end.y, z));
                Push(mesh, UnkMeshVertex(half * normal.x + seg.start.x, half * normal.y + seg.start.y, z));
                Push(mesh, UnkMeshVertex(half * normal.x + seg.end.x, half * normal.y + seg.end.y, z));
                mesh->indices.PushBack(base);
                mesh->indices.PushBack(base + 1);
                mesh->indices.PushBack(base + 3);
                mesh->indices.PushBack(base);
                mesh->indices.PushBack(base + 3);
                mesh->indices.PushBack(base + 2);
                mesh->indices.PushBack(base + 2);
                mesh->indices.PushBack(base + 3);
                mesh->indices.PushBack(base + 5);
                mesh->indices.PushBack(base + 2);
                mesh->indices.PushBack(base + 5);
                len = tip + len;
                mesh->indices.PushBack(base + 4);
            } else {
                Push(mesh, UnkMeshVertex(seg.start.x - half * normal.x, seg.start.y - half * normal.y, z));
                Push(mesh, UnkMeshVertex(half * normal.x + seg.start.x, half * normal.y + seg.start.y, z));
                Push(mesh, UnkMeshVertex(seg.end.x - half * normal.x, seg.end.y - half * normal.y, z));
                Push(mesh, UnkMeshVertex(half * normal.x + seg.end.x, half * normal.y + seg.end.y, z));
                mesh->indices.PushBack(base);
                mesh->indices.PushBack(base + 3);
                mesh->indices.PushBack(base + 1);
                mesh->indices.PushBack(base);
                mesh->indices.PushBack(base + 2);
                mesh->indices.PushBack(base + 3);
            }
        }
        t += len + gap;
    } while (t < total);
    const CVector3D* bounds = path->vf_0x38();
    CVector2D box[2];
    fn_710007b430(&box[0], &bounds[0]);
    fn_710007b430(&box[1], &bounds[1]);
    mesh->min_x = box[0].x - half;
    mesh->min_y = box[0].y - half;
    mesh->max_x = half + box[1].x;
    mesh->max_y = half + box[1].y;
}

// ---- fn_7100ebc7f0: a segment as a quad ----


#include "base/math/UnkSegment2D.h"

// Brings the mesh's bounds up to date with its vertices.
void fn_7100e3e1c0(UnkDashMesh* mesh);

// Adds the segment as a quad `width` wide across it, at depth `z`: four vertices (the start and
// end, each moved half the width either way along the normal) and two triangles.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_7100ebc7f0(const UnkSegment2D* seg, void* mesh_, f32 width, f32 z) {
    auto* mesh = static_cast<UnkDashMesh*>(mesh_);
    UnkVec2Result n = fn_7100125e78(seg);
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    (void)sqrtf(dx * dx + dy * dy);  // the length, unused
    f32 ox = width * 0.5f * n.x;
    f32 oy = width * 0.5f * n.y;
    Push(mesh, UnkMeshVertex(ox + seg->start.x, oy + seg->start.y, z));
    Push(mesh, UnkMeshVertex(seg->start.x - ox, seg->start.y - oy, z));
    Push(mesh, UnkMeshVertex(ox + seg->end.x, oy + seg->end.y, z));
    Push(mesh, UnkMeshVertex(seg->end.x - ox, seg->end.y - oy, z));
    static const u16 kQuad[6] = {0, 1, 3, 0, 3, 2};
    for (u16 i : kQuad)
        mesh->indices.PushBack(i);
    fn_7100e3e1c0(mesh);
}
