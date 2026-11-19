// CTakumakuAIComponent's unit (tools/tu_map.py unit 1079): the damage handler. After the base handler, unless
// the owner is on the allow-spikes track, it asks the owner's attack component what the hit does and takes
// its attack id, damage and kind; then a hit from in front is cancelled (kind 0) unless the owner is frozen.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2702.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkTakumakuRequest;
struct UnkTakumakuSettings;
void fn_71006d4d4c(UnkTakumakuRequest* self, UnkWeakTarget* owner, const UnkTakumakuSettings* src,
                   UnkWeakTarget* other);

// The query handed to the attack component (built by fn_71006d4d4c): the two weak handles and the settings.
struct UnkTakumakuQuery_l6d49e4 {
    UnkRefPtr owner;
    UnkRefPtr other;
    u32 settings[12];
    u32 field_0x40;
};

// What the attack component answers: the attack's id, its kind (0x7fffffff: none) and its damage
// (negative: the id's default).
struct UnkTakumakuHit_l6d49e4 {
    CStrId id;
    s32 kind;
    f32 damage;

    UnkTakumakuHit_l6d49e4() : id(CStrId::Empty()), kind(0x7fffffff), damage(-1.0f) {}
};

// The damage being handled.
struct UnkTakumakuDamage_l6d49e4 {
    u8 pad_0x0[0x60];
    UnkWeakTarget* source;  // 0x60
    u8 settings[0x30];      // 0x68
    UnkWeakTarget* other;   // 0x98
    u8 pad_0xa0[0x18];
    CStrId attackId;  // 0xb8
    u8 pad_0xc0[8];
    s32 kind;  // 0xc8
};
static_assert(__builtin_offsetof(UnkTakumakuDamage_l6d49e4, kind) == 0xc8, "layout");

struct UnkTakumakuAttack_l6d49e4 : UnkReflObject {
    u8 pad_0x8[0x50];
    CStrId id;  // 0x58
};

struct UnkTakumakuAttackSlot_l6d49e4 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkTakumakuOwner_l6d49e4 {
    void** vtable;
    u8 pad_0x8[0x30];
    u8 transform[0xc];  // 0x38
    u8 pad_0x44[0x184];
    void* field_0x1c8;
    u8 pad_0x1d0[0x30];
    void* field_0x200;
    u8 pad_0x208[0x18];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x30];
    UnkTakumakuAttackSlot_l6d49e4* attack;  // 0x258
    u8 pad_0x260[0x68];
    void* frozen;  // 0x2c8
};
static_assert(__builtin_offsetof(UnkTakumakuOwner_l6d49e4, attack) == 0x258, "layout");
static_assert(__builtin_offsetof(UnkTakumakuOwner_l6d49e4, frozen) == 0x2c8, "layout");

struct UnkTakumakuAI_l6d49e4 {
    u8 pad_0x0[0x30];
    UnkTakumakuOwner_l6d49e4* owner;  // 0x30
    u8 pad_0x38[0x14698];
    u8 field_0x146d0[8];
};

bool fn_71006d01fc(void* self);
bool fn_71011368a0(UnkTakumakuAttackSlot_l6d49e4* slot, UnkTakumakuQuery_l6d49e4* query, UnkTakumakuHit_l6d49e4* hit);
UnkVec3f fn_7101158fa0(void* component, UnkTakumakuDamage_l6d49e4* damage);
UnkVec3f fn_71010d8590(void* component, const UnkVec3f& point, UnkTakumakuDamage_l6d49e4* damage);

// Whether an id names a string (not the pool's empty one).
static inline bool IsSet(const CStrId& id) {
    return id.data != &g_unk_7101d47108->empty;
}

static inline UnkActor* AsActor(UnkTakumakuOwner_l6d49e4* owner) {
    return reinterpret_cast<UnkActor*>(owner);
}

void fn_71006d49e4(UnkTakumakuAI_l6d49e4* self, void* arg, UnkTakumakuDamage_l6d49e4* damage) {
    fn_7101105c90(reinterpret_cast<UnkKraidAI2*>(self), arg, reinterpret_cast<UnkKraidRequest*>(damage));
    UnkTakumakuOwner_l6d49e4* owner = self->owner;
    bool onSpikes = owner && owner->animation && fn_710117b348(owner->animation, fn_7100c51c88());
    if (!onSpikes && fn_71006d01fc(self->field_0x146d0)) {
        UnkTakumakuAttackSlot_l6d49e4* slot = self->owner->attack;
        UnkReflObject* component = slot ? slot->component : nullptr;
        if (component && fn_710009d50c(component->vf_0x0(), fn_710078a054())) {
            UnkTakumakuQuery_l6d49e4 query;
            fn_71006d4d4c(reinterpret_cast<UnkTakumakuRequest*>(&query), damage->source,
                          reinterpret_cast<const UnkTakumakuSettings*>(damage->settings), damage->other);
            UnkTakumakuHit_l6d49e4 hit;
            bool found = fn_71011368a0(slot, &query, &hit);
            UnkTakumakuAttack_l6d49e4* attack = static_cast<UnkTakumakuAttack_l6d49e4*>(component);
            const CStrId& id = found && IsSet(hit.id) ? hit.id : attack->id;
            damage->attackId = CStrId(id);
            if (found) {
                fn_71008c1538(reinterpret_cast<UnkRequest*>(damage),
                              hit.damage < 0.0f ? fn_7100789f5c(&damage->attackId) : hit.damage);
                damage->kind = hit.kind == 0x7fffffff ? 0 : hit.kind;
            } else {
                fn_71008c1538(reinterpret_cast<UnkRequest*>(damage), fn_7100789f5c(&damage->attackId));
                damage->kind = 0;
            }
        }
    }

    if (damage->kind == 0)
        return;
    void* frozen = self->owner->frozen;
    bool canTurn = frozen ? !fn_71007f9f20(frozen) : true;
    UnkActor* target = fn_7100d7c990(AsActor(self->owner));
    if (!target)
        return;
    UnkTakumakuOwner_l6d49e4* other = reinterpret_cast<UnkTakumakuOwner_l6d49e4*>(target);
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->transform));
    UnkVec3f forward(m.m[2], m.m[6], m.m[10]);
    UnkVec3f direction = forward;
    if (other->field_0x1c8 && other->field_0x200) {
        direction = fn_71010d8590(other->field_0x200, fn_7101158fa0(other->field_0x1c8, damage), damage);
    }
    f32 facing = forward.x * direction.x + forward.y * direction.y + forward.z * direction.z;
    if (!(facing >= 0.0f) && canTurn)
        damage->kind = 0;
}
