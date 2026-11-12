// Unit 1444 of tools/tu_map.py: the wall the stored object names, as the position it pushes against:
// the stored point moved back along the side's normal by the answer of the owner's shape, and, when the
// stored object asks for it, snapped to the owner's own x or y. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CCharacterMovement.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU376.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkOwner_o0309 {
    u8 pad_0x0[0x44];
    u32 x;  // 0x44
    u8 pad_0x48[0xc];
    u32 y;  // 0x54
};

struct UnkUnit1444_o0309 {
    void* vtable;
    u8 pad_0x8[0x28];
    UnkOwner_o0309* owner;  // 0x30
    void* field_0x38;
    u8 pad_0x40[0x90];
    u8 object[0x100];  // 0xd0
    bool field_0x1d0;
};

// A 0x100-byte object built by fn_71002b57bc and destroyed by fn_71002b6094.
struct UnkTempObject_o0309 {
    alignas(8) u8 raw[0x100];

    UnkTempObject_o0309() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(raw)); }
    ~UnkTempObject_o0309() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(raw)); }
};

struct UnkShapeInner_o0309 {
    void** vtable;
    u8 pad_0x8[0x20];
};
struct UnkShape {
    u8 pad_0x0[0x158];
    UnkShapeInner_o0309* field_0x158;
};
struct UnkFlagged;

UnkVec3f fn_7100973658(UnkUnit1444_o0309* self) {
    UnkTempObject_o0309 stored;
    if (self->field_0x1d0)
        fn_71002b5c90(reinterpret_cast<UnkObject_0x100*>(stored.raw),
                      reinterpret_cast<const UnkObject_0x100*>(self->object));
    const f32* pointp = reinterpret_cast<const f32*>(stored.raw + 0x20);
    f32 px = pointp[0];
    f32 py = pointp[1];
    UnkVec3f result(px, py, 0.0f);
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    UnkNavMesh* mesh = manager->navmesh;
    if (mesh && stored.raw[0x1c] != 0xff) {
        u32 ownerX = self->owner->x;
        u32 ownerY = self->owner->y;
        UnkNavMeshSideSegment segment;
        segment.start.x = 0.0f;
        segment.start.y = 0.0f;
        segment.end.x = 0.0f;
        segment.end.y = 0.0f;
        segment.field_0x10 = -1.0f;
        UnkNavMeshCell* cell = fn_710027a33c(mesh, *reinterpret_cast<u16*>(stored.raw + 0x1e));
        {
            // 16-byte aligned: the original's shared slot for the side, normal and depth sits at sp+0.
            alignas(16) u8 side = stored.raw[0x1c];
            fn_71002afea8(cell, &side, &segment);
        }
        f32 nx, ny;
        {
            UnkVec2Result normal = fn_7100125e78(reinterpret_cast<const UnkSegment2D*>(&segment));
            nx = normal.x;
            ny = normal.y;
        }
        UnkShapeInner_o0309* shape = fn_7100976588(static_cast<UnkFlagged*>(self->field_0x38))->field_0x158;
        UnkVec3f depth = reinterpret_cast<UnkVec3f (*)(UnkShapeInner_o0309*, void*)>(shape->vtable[0x28 / 8])(
            shape, reinterpret_cast<u8*>(shape) + 8);
        result.x = px - nx * depth.y;
        result.y = py - ny * depth.y;
        result.z = 0.0f - 0.0f * depth.y;
        if (stored.raw[0x40]) {
            f32 dot = nx + ny * 0.0f + 0.0f;
            if (dot > 0.8f || dot < -0.8f)
                *reinterpret_cast<a_u32*>(&result.y) = ownerY;
            else
                *reinterpret_cast<a_u32*>(&result.x) = ownerX;
        }
    }
    return result;
}
