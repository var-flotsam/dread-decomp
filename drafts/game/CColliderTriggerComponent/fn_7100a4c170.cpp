// CColliderTriggerComponent's unit (tools/tu_map.py unit 1564): whether the trigger's collider (+0x58) touches
// the player's shape named `name` (the player's own shape for g_unk_7101d470e0, else the collision object of
// that name). When the two boxes overlap, the shapes are tested with the shape tester (created on first use)
// and, on a hit, the contact point goes to `point` when given. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharacterMovement.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

using base::global::CStrId;

// The contact a shape test reports (vtable g_unk_7101a22c60): the point at +0xc.
extern void* g_unk_7101a22c60[];

struct UnkTriggerContact_la4c170 {
    void** vtable;
    s32 field_0x8;
    f32 point[3];  // 0xc
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;

    UnkTriggerContact_la4c170()
        : vtable(g_unk_7101a22c60 + 2), field_0x8(-1), point{}, field_0x18(0), field_0x1c(0), field_0x20(0),
          field_0x24(0), field_0x28(0), field_0x2c(0), field_0x30(0), field_0x34(0) {}
};

struct UnkTriggerBox_la4c170 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

static inline bool Overlaps(const UnkTriggerBox_la4c170& a, const UnkTriggerBox_la4c170& b) {
    return b.max_x >= a.min_x && ((b.min_y <= a.max_y) & (b.max_y >= a.min_y) & (b.min_x <= a.max_x));
}

// A shape with its bounding box, brought up to date (vf_0x98) when it is stale.
struct UnkTriggerShape_la4c170 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkTriggerBox_la4c170 box;  // 0x40
    u8 pad_0x50[0x10];
    bool stale;  // 0x60
    u8 pad_0x61[0xf7];
    void* owner;  // 0x158

    const UnkTriggerBox_la4c170& Bounds() {
        if (stale)
            reinterpret_cast<void (*)(UnkTriggerShape_la4c170*)>(vtable[0x98 / 8])(this);
        return box;
    }
};
static_assert(__builtin_offsetof(UnkTriggerShape_la4c170, owner) == 0x158, "layout");

struct UnkTriggerPlayerBody_la4c170 {
    void** vtable;

    UnkTriggerShape_la4c170* vf_0x3a8() {
        return reinterpret_cast<UnkTriggerShape_la4c170* (*)(UnkTriggerPlayerBody_la4c170*)>(vtable[0x3a8 / 8])(this);
    }
};

struct UnkTriggerPlayer_la4c170 {
    u8 pad_0x0[0x200];
    UnkTriggerPlayerBody_la4c170* body;  // 0x200
    u8 pad_0x208[0x98];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkColliderTrigger_la4c170 {
    u8 pad_0x0[0x58];
    UnkTriggerShape_la4c170* collider;  // 0x58
};

bool fn_7100a5f3a8(UnkColliderTrigger_la4c170* self);

static inline UnkShapeTester_A5_b68* Tester() {
    if (!g_unk_71726a47a0) {
        UnkShapeTester_A5_b68* t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return g_unk_71726a47a0;
}

// The shape test with both transforms given as matrices.
static inline UnkColliderResult TestShapes(UnkShapeTester_A5_b68* tester, void* owner, const UnkMatrix44& transform,
                                           void* other, const UnkMatrix44& other_transform,
                                           UnkTriggerContact_la4c170* contact) {
    return fn_7100d51368(tester, owner, const_cast<UnkMatrix44*>(&transform), other, other_transform,
                         reinterpret_cast<UnkShapeFilter_ovb0107*>(contact), 0);
}

bool fn_7100a4c170(UnkColliderTrigger_la4c170* self, const CStrId* name, f32* point) {
    auto* player = reinterpret_cast<UnkTriggerPlayer_la4c170*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    UnkTriggerShape_la4c170* other;
    if (*name == g_unk_7101d470e0) {
        UnkTriggerPlayerBody_la4c170* body = player->body;
        if (fn_7100a5f3a8(self))
            other = body->vf_0x3a8();
        else
            other = reinterpret_cast<UnkTriggerShape_la4c170*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(body)));
    } else {
        other = reinterpret_cast<UnkTriggerShape_la4c170*>(fn_71008640a4(player->collision, name));
    }
    if (!other)
        return false;

    UnkTriggerBox_la4c170 bounds = self->collider->Bounds();
    if (!Overlaps(bounds, other->Bounds()))
        return false;

    UnkTriggerContact_la4c170 contact;
    UnkShapeTester_A5_b68* tester = Tester();
    if (!(TestShapes(tester, self->collider->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->collider)),
                     other->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(other)), &contact).bits & 1))
        return false;
    if (point) {
        point[0] = contact.point[0];
        point[1] = contact.point[1];
        point[2] = contact.point[2];
    }
    return true;
}
