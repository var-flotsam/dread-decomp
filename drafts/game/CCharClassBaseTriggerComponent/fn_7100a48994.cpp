// CCharClassBaseTriggerComponent's unit (tools/tu_map.py, unit 1562): whether the trigger's shape (+0x88) touches
// the shape named `name` (the player's own with the empty id): their bounds first, then the shape tester; on a
// hit the contact point goes to `out` when given. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharacterMovement.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

// The vtable of the contact collector below.
extern void* g_unk_7101a22c60[];

struct UnkBounds_la48994 {
    f32 min_x, min_y;
    f32 max_x, max_y;
};

// A trigger or collision shape: its bounds (+0x40) are brought up to date (virtual slot 0x98) when the flag at
// +0x60 says they are stale.
struct UnkTriggerShape_la48994 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkBounds_la48994 bounds;  // 0x40
    u8 pad_0x50[0x10];
    bool bounds_stale;  // 0x60
    u8 pad_0x61[0xf7];
    void* owner;  // 0x158

    const UnkBounds_la48994& Bounds() {
        if (bounds_stale)
            reinterpret_cast<void (*)(UnkTriggerShape_la48994*)>(vtable[0x98 / 8])(this);
        return bounds;
    }
};
static_assert(__builtin_offsetof(UnkTriggerShape_la48994, owner) == 0x158, "UnkTriggerShape_la48994 layout");

// What the shape tester reports back: the contact point at +0xc.
struct UnkContactCollector_la48994 {
    void** vtable;
    s32 field_0x8;
    UnkVec3f point;  // 0xc
    UnkVec3f field_0x18;
    UnkVec3f field_0x24;
    f32 field_0x30;
    u32 field_0x34;

    UnkContactCollector_la48994()
        : vtable(g_unk_7101a22c60 + 2), field_0x8(-1), point(0.0f, 0.0f, 0.0f), field_0x18(0.0f, 0.0f, 0.0f),
          field_0x24(0.0f, 0.0f, 0.0f), field_0x30(0.0f), field_0x34(0) {}
};

struct UnkTriggerPlayer_la48994 {
    u8 pad_0x0[0x200];
    UnkFlagged* movement;  // 0x200
    u8 pad_0x208[0x98];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkTriggerComponent_la48994 {
    u8 pad_0x0[0x88];
    UnkTriggerShape_la48994* shape;  // 0x88
};

static inline bool Overlaps(const UnkBounds_la48994& a, const UnkBounds_la48994& b) {
    return b.max_x >= a.min_x && b.min_y <= a.max_y && b.max_y >= a.min_y && b.min_x <= a.max_x;
}

static inline UnkShapeTester_A5_b68* Tester() {
    if (!g_unk_71726a47a0) {
        UnkShapeTester_A5_b68* t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return g_unk_71726a47a0;
}

static inline UnkColliderResult Test(UnkShapeTester_A5_b68* tester, void* owner_a, const UnkMatrix44& a,
                                     void* owner_b, const UnkMatrix44& b, UnkContactCollector_la48994* collector) {
    return fn_7100d51368(tester, owner_a, const_cast<UnkMatrix44*>(&a), owner_b, b,
                         reinterpret_cast<UnkShapeFilter_ovb0107*>(collector), 0);
}

bool fn_7100a48994(UnkTriggerComponent_la48994* self, const base::global::CStrId* name, UnkVec3f* out) {
    UnkTriggerPlayer_la48994* player = reinterpret_cast<UnkTriggerPlayer_la48994*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    UnkTriggerShape_la48994* other;
    if (*name == g_unk_7101d470e0)
        other = reinterpret_cast<UnkTriggerShape_la48994*>(fn_7100976588(player->movement));
    else
        other = reinterpret_cast<UnkTriggerShape_la48994*>(fn_71008640a4(player->collision, name));
    if (!other)
        return false;

    UnkBounds_la48994 mine = self->shape->Bounds();
    UnkBounds_la48994 theirs = other->Bounds();
    if (!Overlaps(mine, theirs))
        return false;

    UnkContactCollector_la48994 collector;
    UnkShapeTester_A5_b68* tester = Tester();
    UnkColliderResult result =
        Test(tester, self->shape->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->shape)), other->owner,
             fn_7100d36a2c(reinterpret_cast<UnkShape*>(other)), &collector);
    if (result.bits & 1) {
        if (out)
            *out = collector.point;
        return true;
    }
    return false;
}
