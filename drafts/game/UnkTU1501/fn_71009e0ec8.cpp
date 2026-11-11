// NON_MATCHING(minor): 92%; hit-detail zeroing stores scheduled before the matrix stores, and the
// distance is loaded before the direction (fmul operand order).
// Unit 1501 (near CSamusMovement): casts the movement's collision shape downwards from a point
// 5 units above the given position and returns where it lands, with optional hit details.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkShape;
struct UnkFlagged;
struct UnkTree;
struct Vec2;
struct UnkMtx44;
struct UnkAssignedSlot;

UnkShape* fn_7100976588(UnkFlagged* flagged);
bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg,
                   bool flag);
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);

extern char g_unk_7101a23590[];
extern char g_unk_7101a20ed8[];

// Intrusively counted object, destroyed through its third virtual slot.
struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();
    u32 refs;
};

struct UnkRef {
    UnkRefObj* ptr;

    UnkRef() : ptr(nullptr) {}
    ~UnkRef() {
        if (ptr && --ptr->refs == 0)
            ptr->Destroy();
    }
    UnkRef& operator=(const UnkRef& other) {
        fn_71002a5d1c((UnkAssignedSlot*)this, (const UnkAssignedSlot*)&other);
        return *this;
    }
};

// Details of a shape-cast hit.
struct UnkCastHit {
    UnkRef a;
    UnkRef b;
    s32 field_0x10;
    u32 field_0x14[9];
    f32 distance;  // 0x38
    u32 field_0x3c;
    u32 field_0x40;

    UnkCastHit() : field_0x10(-1) {
        for (int i = 0; i < 9; ++i)
            field_0x14[i] = 0;
        distance = 0;
        field_0x3c = 0;
        field_0x40 = 0;
    }
    UnkCastHit& operator=(const UnkCastHit& o) {
        if (this != &o) {
            a = o.a;
            b = o.b;
            field_0x10 = o.field_0x10;
            for (int i = 0; i < 9; ++i)
                field_0x14[i] = o.field_0x14[i];
            distance = o.distance;
            field_0x3c = o.field_0x3c;
            field_0x40 = o.field_0x40;
        }
        return *this;
    }
};

// The collision query's result object: a vtable over the hit details.
struct UnkCastResult {
    void* vtable;
    UnkCastHit hit;
    u8 field_0x50;

    UnkCastResult() : field_0x50(0) { vtable = g_unk_7101a23590 + 0x10; }
    ~UnkCastResult() { vtable = g_unk_7101a20ed8 + 0x10; }
};

// The movement's collision shape: its world matrix comes from virtual slot 0x70.
struct UnkCastShape;
struct UnkCastShapeVt {
    void* pad_0x0[0x70 / 8];
    const UnkMatrix44* (*vf_0x70)(UnkCastShape* self);
};
struct UnkCastShape {
    UnkCastShapeVt* vt;
    u8 pad_0x8[0x150];
    UnkShape* field_0x158;
};

struct UnkCastMover;
struct UnkCastMoverVt {
    void* pad_0x0[0x288 / 8];
    void (*vf_0x288)(UnkCastMover* self);
};
struct UnkCastMover {
    UnkCastMoverVt* vt;
    u8 pad_0x8[0x3c8];
    u32 field_0x3d0;
};

static inline UnkVec3f Origin(const UnkVec3f& pos) {
    return UnkVec3f(pos.x + 0.0f, pos.y + 5.0f, pos.z + 0.0f);
}

UnkVec3f fn_71009e0ec8(UnkCastMover* self, const UnkVec3f* pos, f32 depth, u8* outHit, UnkCastHit* outDetails) {
    UnkVec3f result = *pos;
    UnkGameManagerSub_0x608* world = g_unk_7101d47310->field_0x608;
    if (world == nullptr)
        return result;

    UnkTree* tree = (UnkTree*)world->field_0x820;
    UnkCastShape* shape = (UnkCastShape*)fn_7100976588((UnkFlagged*)self);
    if (shape == nullptr) {
        self->vt->vf_0x288(self);
        shape = (UnkCastShape*)fn_7100976588((UnkFlagged*)self);
    }

    UnkMatrix44 xf;
    UnkCopyMatrix(&xf, *shape->vt->vf_0x70(shape));
    UnkVec3f origin = Origin(*pos);
    xf.m[3] = origin.x;
    xf.m[7] = origin.y;
    xf.m[11] = origin.z;
    f32 dir[2] = {0.0f, -depth};

    UnkCastResult cast;
    u8 hit = 0;
    if (fn_7100d5babc(tree, self->field_0x3d0, shape->field_0x158, (const UnkMtx44*)&xf, (const Vec2*)dir, &cast,
                      false)) {
        UnkVec3f from = Origin(*pos);
        f32 d = cast.hit.distance;
        result = UnkVec3f(from.x + d * dir[0], from.y + d * dir[1], from.z + d * 0.0f);
        hit = 1;
        if (outDetails != nullptr)
            *outDetails = cast.hit;
    }
    if (outHit != nullptr)
        *outHit = hit;
    return result;
}
