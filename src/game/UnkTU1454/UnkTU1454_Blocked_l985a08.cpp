// Unit 1454: whether nothing blocks the target: queries the collision tree with a circle (the grapple
// tunable's value at +0x2c plus 50) placed at the target's position, and returns true when it hits
// nothing. Names and layouts are placeholders.
#include "game/CGrappleBeamComponent.h"
#include "game/CSamusGrappleMovementState.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU54.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a862b8[];

struct UnkNpc_b0248 {
    u8 pad_0x0[0x3d8];
    s32 state;  // 0x3d8
};

// The object tracked: a position source with a virtual 0x188 that gives a position.
class UnkTarget_b0248 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual UnkVec3f vf_0x188();
};

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// The grapple movement tunable (g_unk_717273ba88).
struct UnkGrappleTunable_l985a08 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
};

static inline const UnkGrappleTunable_l985a08& GrappleTunable() {
    return reinterpret_cast<const UnkGrappleTunable_l985a08&>(g_unk_717273ba88);
}

// A circle: its centre and radius.
struct UnkCircle_l985a08 {
    f32 x;
    f32 y;
    f32 radius;
};

struct UnkCircleShape_l985a08;
void fn_7100d616fc(UnkCircleShape_l985a08* self, const UnkCircle_l985a08* circle);

struct UnkCircleShape_l985a08 {
    u8 body[0xb0];

    explicit UnkCircleShape_l985a08(const UnkCircle_l985a08& circle) { fn_7100d616fc(this, &circle); }
    ~UnkCircleShape_l985a08() { fn_7100d61750(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(this)); }
};

static inline void ReleaseRef_l985a08(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

#pragma pack(push, 4)
struct UnkHitRecord_l985a08 {
    UnkRefObject* first;
    UnkRefObject* second;
    u8 pad_0x10[0x38];
};

// The hits of one query (vtable g_unk_7101a75548), up to 16.
struct UnkHitList_l985a08 {
    void** vtable;
    u32 count;
    u8 storage[16 * sizeof(UnkHitRecord_l985a08)];  // 0xc

    UnkHitList_l985a08() : count(0) {}

    UnkHitRecord_l985a08* items() { return reinterpret_cast<UnkHitRecord_l985a08*>(storage); }

    ~UnkHitList_l985a08() {
        vtable = g_unk_7101a75548 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_l985a08(items()[i].second);
            ReleaseRef_l985a08(items()[i].first);
        }
        count = 0;
    }
};

// Two directions the visitor keeps.
struct UnkHitDirs_l985a08 {
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;

    UnkHitDirs_l985a08() = default;
    UnkHitDirs_l985a08(f32 ax, f32 ay, f32 bx, f32 by) : x0(ax), y0(ay), x1(bx), y1(by) {}
};

// The query's visitor (vtable g_unk_7101a862b8).
struct UnkHitVisitor_l985a08 : UnkHitList_l985a08 {
    UnkHitDirs_l985a08 dirs;  // 0x48c

    UnkHitVisitor_l985a08() {
        vtable = g_unk_7101a862b8 + 2;
        dirs = UnkHitDirs_l985a08(0.0f, -1.0f, 0.0f, 0.6f);
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkHitRecord_l985a08) == 0x48, "record");
static_assert(__builtin_offsetof(UnkHitVisitor_l985a08, dirs) == 0x48c, "visitor");

bool fn_7100985a08(UnkNpc_b0248* self, UnkTarget_b0248* target) {
    UnkVec3f pos = target->vf_0x188();
    f32 radius = GrappleTunable().field_0x2c + 50.0f;
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
    UnkCircleShape_l985a08 shape(UnkCircle_l985a08{0.0f, 0.0f, radius});
    UnkHitVisitor_l985a08 hits;
    UnkMatrix44_7100078c9c xf = fn_7100078cd8(pos);
    bool hit = fn_7100d5b484(tree, 0x209, reinterpret_cast<UnkShape*>(&shape),
                             reinterpret_cast<const UnkMtx44*>(&xf), &hits, 1);
    return !hit;
}
