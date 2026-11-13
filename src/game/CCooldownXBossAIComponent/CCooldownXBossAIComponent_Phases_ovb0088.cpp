// CCooldownXBossAIComponent's unit (tools/tu_map.py): pieces of the boss's AI. Two record destructors
// (string ids released last first, fn_71004cf1d4 and fn_71004d0b00, the latter after dropping a counted
// reference at +0x30); whether the target faces away from the boss (fn_71004d4054: the sign of the target's
// facing against the boss's side, fn_7101178e6c's flag 5 picking g_unk_7101649b70 or g_unk_7101649b7c; yes
// while the target plays a CCharClassMelee360ActionTrack); the boss put back at (2400, -7800, 0) unless it
// stands within 5 of it (fn_71004d4208, the owner's slot 0xd0 moving the point while fn_7101196fd8 holds);
// what a grab does in each phase (fn_71004d3944); a message handler (fn_71004d4814) leaving a pending state
// and reacting to three action ids; the damage filter (fn_71004d2e14) that lets the hits named by
// g_unk_71726d86f0, g_unk_71726d8700 and four hashes through with their flags; and the collider switch
// (fn_71004d2cc4) turning the colliders on while the locator g_unk_7101649bb8 is between -400 and 50 high.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAIAttackComponent.h"
#include "game/CAnimationComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/CGrabComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "game/UnkShakernautId.h"

using base::global::CStrId;

struct UnkCooldownXIds_ovb0088 {
    CStrId ids[4];
};

void fn_71004cf1d4(UnkCooldownXIds_ovb0088* self) {
    self->ids[3].Reset();
    self->ids[2].Reset();
    self->ids[1].Reset();
    self->ids[0].Reset();
}

using base::global::CStrId;

struct UnkCooldownXRecord_ovb0088 {
    CStrId ids[6];
    UnkRefObject* ref;  // 0x30
};

void fn_71004d0b00(UnkCooldownXRecord_ovb0088* self) {
    if (UnkRefObject* ref = self->ref) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
    for (int i = 5; i >= 0; --i)
        self->ids[i].Reset();
}

struct UnkTakumakuComponent;

extern const base::math::CVector3D g_unk_7101649b70 __attribute__((visibility("hidden")));  // facing right
extern const base::math::CVector3D g_unk_7101649b7c __attribute__((visibility("hidden")));  // facing left

struct UnkCooldownXBossAI_ovb0088 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

static inline s32 Sign(f32 v) { return (v > 0.0f) - (v < 0.0f); }

bool fn_71004d4054(UnkCooldownXBossAI_ovb0088* self) {
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;
    if (UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(target->field_0x220)) {
        if (fn_710117b348(cls, fn_7100c3d888()))
            return true;
    }
    auto* cls = static_cast<UnkTakumakuComponent*>(self->owner->field_0x220);
    if (!cls)
        return false;
    const base::math::CVector3D& dir = fn_7101178e6c(cls, 5) ? g_unk_7101649b70 : g_unk_7101649b7c;
    f32 facing = (f32)Sign(dir.x);
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(target->field_0x38));
    return facing != (f32)Sign(m.m[2]);
}

using base::math::CVector3D;

struct UnkCooldownXOwner_ovb0088 {
    virtual void vf_0x0();
    // slot 0xd0: moves the position to where the owner may stand.
    void Place(CVector3D* pos, bool flag) {
        reinterpret_cast<void (*)(UnkCooldownXOwner_ovb0088*, CVector3D*, bool)>(
            (*reinterpret_cast<void***>(this))[0xd0 / 8])(this, pos, flag);
    }
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x348];
    bool placed;  // 0x3b0
};
static_assert(__builtin_offsetof(UnkCooldownXOwner_ovb0088, placed) == 0x3b0, "layout");

struct UnkCooldownXPlaceAI_ovb0088 {
    u8 pad_0x0[0x30];
    UnkCooldownXOwner_ovb0088* owner;  // 0x30
};

struct UnkCooldownXXform_ovb0088 {
    f32 m[12];
};

void fn_71010fa940(UnkCooldownXPlaceAI_ovb0088* self, const UnkCooldownXXform_ovb0088* xform);

static inline bool Near(f32 v, f32 c) { return v - 5.0f <= c && v + 5.0f >= c; }

void fn_71004d4208(UnkCooldownXPlaceAI_ovb0088* self, const UnkCooldownXXform_ovb0088* xform) {
    fn_71010fa940(self, xform);
    CVector3D pos;
    pos.x = 2400.0f;
    pos.y = -7800.0f;
    pos.z = 0.0f;
    if (!Near(xform->m[3], pos.x) || !Near(xform->m[7], pos.y) || !Near(xform->m[11], pos.z)) {
        if (fn_7101196fd8(g_unk_7101d47330))
            self->owner->Place(&pos, true);
    }
    UnkCooldownXOwner_ovb0088* owner = self->owner;
    owner->x = pos.x;
    owner->y = pos.y;
    owner->z = pos.z;
    owner->placed = true;
}

struct UnkCooldownXGrab_ovb0088 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkCooldownXPart_ovb0088 {
    u8 pad_0x0[0xe8];
    s32 mode;  // 0xe8
    u8 pad_0xec[4];
    UnkCooldownXGrab_ovb0088* grab;  // 0xf0
};

void fn_710113da34(UnkCooldownXPart_ovb0088* part, void* arg);

struct UnkCooldownXReaction_ovb0088 {
    virtual void vf_0x0();
    void Set(bool a, bool b) {
        reinterpret_cast<void (*)(UnkCooldownXReaction_ovb0088*, bool, bool)>(
            (*reinterpret_cast<void***>(this))[0x180 / 8])(this, a, b);
    }
};

struct UnkCooldownXState_ovb0088 {
    u32 phase;  // 0x15680
    u8 pad_0x4[0xd0];
    bool field_0xd4;
    u8 pad_0xd5[0xe0];
    bool field_0x1b5;
    bool field_0x1b6;
};

struct UnkCooldownXHitAI_ovb0088 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1108];
    s32 field_0x1140;
    u8 pad_0x1144[0x15680 - 0x1144];
    UnkCooldownXState_ovb0088 state;  // 0x15680
    u8 pad_0x15837[0x15838 - 0x15680 - sizeof(UnkCooldownXState_ovb0088)];
    u8 field_0x15838[8];

    void vf_0x208(bool a, bool b) {
        reinterpret_cast<void (*)(UnkCooldownXHitAI_ovb0088*, bool, bool)>(vtable[0x208 / 8])(this, a, b);
    }
};
static_assert(__builtin_offsetof(UnkCooldownXHitAI_ovb0088, field_0x15838) == 0x15838, "layout");

// The actor's part at +0x260 (UnkActor leaves it in padding).
struct UnkCooldownXActor_ovb0088 {
    u8 pad_0x0[0x260];
    UnkCooldownXPart_ovb0088* part;  // 0x260
};

static inline UnkCooldownXPart_ovb0088* Part(UnkActor* actor) {
    return reinterpret_cast<UnkCooldownXActor_ovb0088*>(actor)->part;
}

void fn_71004d3944(UnkCooldownXHitAI_ovb0088* self) {
    UnkCooldownXState_ovb0088* state = &self->state;
    bool grabbed;
    if (Part(self->owner) && fn_710113d83c(Part(self->owner))) {
        grabbed = true;
    } else if (!fn_7100d7c990(self->owner) || !Part(fn_7100d7c990(self->owner))) {
        grabbed = false;
    } else {
        UnkCooldownXPart_ovb0088* part = Part(fn_7100d7c990(self->owner));
        grabbed = part->mode == 1 && part->grab && part->grab->field_0x10;
    }
    if (state->field_0xd4 || state->phase >= 4) {
        if (grabbed) {
            state->field_0x1b5 = true;
            return;
        }
        auto* reaction = static_cast<UnkCooldownXReaction_ovb0088*>(self->owner->field_0x1b8);
        if (!reaction)
            return;
        reaction->Set(true, false);
        if (UnkCooldownXPart_ovb0088* part = Part(self->owner))
            fn_710113da34(part, self->field_0x15838);
        return;
    }
    if (grabbed) {
        state->field_0x1b6 = true;
        return;
    }
    if (self->field_0x1140 == 1)
        return;
    self->vf_0x208(true, true);
}

struct UnkShakernautCtrl_b53;
struct UnkCommanderPlayer_b67a2;
struct UnkEntityState;

struct UnkCooldownXMsgTarget_ovb0088 {
    u8 pad_0x0[0x10];
    UnkEntityState* state;  // 0x10
};

struct UnkCooldownXMsgEntry_ovb0088 {
    u8 pad_0x0[0x30];
    UnkCooldownXMsgTarget_ovb0088* target;  // 0x30
    u8 pad_0x38[0x8];
};

struct UnkCooldownXMsgOwner_ovb0088 {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
    u8 pad_0x228[0x50];
    UnkCommanderPlayer_b67a2* player;  // 0x278
};

struct UnkCooldownXMsgAI_ovb0088 {
    u8 pad_0x0[0x30];
    UnkCooldownXMsgOwner_ovb0088* owner;  // 0x30
    u8 pad_0x38[0x15130 - 0x38];
    u8 entry_0x15130[0x15654 - 0x15130];
    bool pending;  // 0x15654
    UnkCooldownXMsgEntry_ovb0088* entries __attribute__((packed, aligned(4)));  // 0x15658
    u8 pad_0x15660[8];
    u32 count;  // 0x15668
};
static_assert(__builtin_offsetof(UnkCooldownXMsgAI_ovb0088, count) == 0x15668, "layout");

struct UnkCooldownXMsgKind_ovb0088 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};
struct UnkCooldownXMsgAction_ovb0088 {
    u8 pad_0x0[0x38];
    UnkCooldownXMsgKind_ovb0088* kind;  // 0x38
};
struct UnkCooldownXMsgSource_ovb0088 {
    u8 pad_0x0[0x10];
    UnkCooldownXMsgAction_ovb0088* action;  // 0x10
};
struct UnkCooldownXMsg_ovb0088 {
    s32 type;
    u8 pad_0x4[4];
    UnkCooldownXMsgSource_ovb0088* source;  // 0x8
};

void fn_71004d1b38(UnkCooldownXMsgAI_ovb0088* self, bool flag);
void fn_71004d0cf4(UnkCooldownXMsgAI_ovb0088* self, bool a, bool b);
void fn_71004d2258(UnkCooldownXMsgAI_ovb0088* self, f32 value);

bool fn_71004d4814(UnkCooldownXMsgAI_ovb0088* self, const UnkCooldownXMsg_ovb0088* msg) {
    if (msg->type != 1)
        return true;
    if (self->pending) {
        fn_71004d1b38(self, true);
        if (UnkShakernautCtrl_b53* ctrl = self->owner->ctrl)
            fn_710117774c(ctrl);
        fn_71004d0cf4(self, true, false);
        self->pending = false;
    }
    switch (msg->source->action->kind->id) {
    case 0xe39d7ff99f9491a8:
    case 0xa437051034736e9c: {
        UnkCooldownXMsgEntry_ovb0088* end = self->entries + self->count;
        for (UnkCooldownXMsgEntry_ovb0088* it = self->entries; it != end; ++it) {
            if (it->target && it->target->state)
                fn_7100232864(it->target->state, true);
        }
        fn_71004d2258(self, 0.0f);
        fn_7101134420(self->owner->player, self->entry_0x15130, 1, 0, nullptr, -1.0f);
        break;
    }
    case 0x1103a527ce41749b: {
        UnkShakernautCtrl_b53* ctrl = self->owner->ctrl;
        fn_71004d1b38(self, false);
        UnkShakernautId_b53 id;
        id.value = 0x80;
        fn_71011785a8(ctrl, &id, true, false);
        break;
    }
    }
    return true;
}

using base::global::CStrId;

extern CStrId g_unk_71726d86f0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726d8700 __attribute__((visibility("hidden")));

struct UnkQuetzoaXAI_b58;
struct UnkQuetzoaXMessage_b58;
struct UnkRobotSoldierTable_A2_b69;
struct UnkRequest;

// A string id's data as the damage check reads it: the hash at +0x30.
struct UnkCooldownXDamageIdData_ovb0088 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkCooldownXDamageSource_ovb0088 {
    u8 pad_0x0[0x20];
    CStrId id;  // 0x20
};

struct UnkCooldownXDamage_ovb0088 {
    u8 pad_0x0[0x60];
    UnkCooldownXDamageSource_ovb0088* source;  // 0x60
    u8 pad_0x68[0x4c];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x40];
    f32 scale;  // 0xf8
};
static_assert(__builtin_offsetof(UnkCooldownXDamage_ovb0088, scale) == 0xf8, "layout");

struct UnkCooldownXDamageTables_ovb0088 {
    u8 pad_0x0[0xc78];
    u8 table[8];  // 0xc78
};

struct UnkCooldownXDamageOwner_ovb0088 {
    u8 pad_0x0[0x260];
    void* part;  // 0x260
};

struct UnkCooldownXDamageAI_ovb0088 {
    u8 pad_0x0[0x30];
    UnkCooldownXDamageOwner_ovb0088* owner;  // 0x30
    u8 pad_0x38[0x149a0 - 0x38];
    UnkCooldownXDamageTables_ovb0088* tables;  // 0x149a0
    u8 pad_0x149a8[0xdb4 - 8];
    bool shielded;  // 0x15754
};
static_assert(__builtin_offsetof(UnkCooldownXDamageAI_ovb0088, shielded) == 0x149a0 + 0xdb4, "layout");

static inline u64 Hash(const CStrId& id) { return reinterpret_cast<UnkCooldownXDamageIdData_ovb0088*>(id.data)->hash; }

static inline bool IsShieldable(const CStrId& id) {
    return id.data == g_unk_71726d86f0.data || Hash(id) == 0xa8714d655b63e895 || Hash(id) == 0xb53af769d6f094b0 ||
           Hash(id) == 0xb597d92716d1c432 || id.data == g_unk_71726d8700.data || Hash(id) == 0xff8cb38672dbf2a3;
}

u32 fn_71004d2e14(UnkCooldownXDamageAI_ovb0088* self, UnkCooldownXDamage_ovb0088* damage) {
    u32 result = fn_71003adaf8(reinterpret_cast<UnkQuetzoaXAI_b58*>(self), reinterpret_cast<UnkQuetzoaXMessage_b58*>(damage));
    if (self->owner->part && fn_710113d83c(self->owner->part)) {
        f32 value = fn_71008c2de8(reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(self->tables->table), damage->flags);
        fn_71008c1538(reinterpret_cast<UnkRequest*>(damage), value * damage->scale);
    }
    CStrId id(damage->source ? damage->source->id : g_unk_7101d470e0);
    if (IsShieldable(id)) {
        if (damage->flags & 0x8000)
            self->shielded = true;
        else if (!(damage->flags & 0x2800841c))
            result = 2;
    } else {
        result = 2;
    }
    return result;
}

struct UnkEntity;
struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::UnkVector<UnkStruct_71008651c8_Entry*>;
void fn_71008651c8(UnkStruct_71008651c8*, void* const*, UnkOut_71008651c8*);

extern char g_unk_7101649bb8[];  // the locator's name
extern CStrId g_unk_71726d86f0 __attribute__((visibility("hidden")));
extern void* const g_unk_71726d86f8 __attribute__((visibility("hidden")));

struct UnkCooldownXBoundsOwner_ovb0088 {
    u8 pad_0x0[0x228];
    UnkEntity* entity;  // 0x228
    u8 pad_0x230[0x70];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkCooldownXBoundsAI_ovb0088 {
    u8 pad_0x0[0x30];
    UnkCooldownXBoundsOwner_ovb0088* owner;  // 0x30
};

void fn_71004d2cc4(UnkCooldownXBoundsAI_ovb0088* self) {
    UnkCooldownXBoundsOwner_ovb0088* owner = self->owner;
    UnkCollisionContainer* container = owner->collision;
    UnkEntity* entity = owner->entity;
    if (!container || !entity)
        return;
    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    bool found = fn_71011735e8(entity, g_unk_7101649bb8, &m, 0);
    UnkCollisionObject* object = fn_71008640a4(container, &g_unk_71726d86f0);
    if (!found || !object)
        return;
    f32 y = m.m[11];
    bool above = y >= -400.0f;
    bool inside = y <= 50.0f && above;
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), inside);
    UnkOut_71008651c8 colliders(0xf);
    fn_71008651c8(reinterpret_cast<UnkStruct_71008651c8*>(container), &g_unk_71726d86f8, &colliders);
    for (UnkStruct_71008651c8_Entry **it = colliders.data, **end = colliders.data + colliders.count; it != end; ++it)
        fn_7100d35a98(*reinterpret_cast<UnkEntry**>(reinterpret_cast<char*>(*it) + 0x28), inside);
    colliders.Free();
}
