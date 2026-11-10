// behaviortree::CCanUseHyperDashCondition's unit (tools/tu_map.py unit 663): the condition. False while the owner is on a
// hyper dash track or has no ground-attack Chozo Commander AI; otherwise true when the attack's distance from the
// owner's value exceeds the AI tunable's and no sentence sphere is among the tracked entries. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkHyperDashOwner {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0x220 - 0x48];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkReflObject* ai;  // 0x240
};

struct UnkHyperDashCondition {
    u8 pad_0x0[0x18];
    UnkHyperDashOwner* owner;
};

struct UnkCommanderAI {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x1148 - 0x18];
    UnkReflObject* attack;  // 0x1148
};

struct UnkCommanderAITunable {
    u8 pad_0x0[0x15b8];
    f32 field_0x15b8;
};

struct UnkAttackQuery {
    union {
        u64 zero;
        f32 value;
    };
    u32 count;
};

struct UnkCommanderAttack {
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
    virtual bool vf_0xe8(void* out);
};

struct UnkSphereEntry {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkSphereList {
    UnkSphereEntry** data;
    u32 count;
};
UnkSphereList* fn_7100ac73b0();

template <const base::reflection::CClass* (*Getter)()>
static inline UnkReflObject* Cast(UnkReflObject* p) {
    return fn_710009d50c(p->vf_0x0(), Getter()) ? p : nullptr;
}

bool fn_710044b9d4(UnkHyperDashCondition* self) {
    f32 base = self->owner->field_0x44;
    if (fn_710117b348(self->owner->animation, fn_7100c25518()))
        return false;
    if (!self->owner)
        return false;
    UnkReflObject* raw = self->owner->ai;
    if (!raw)
        return false;
    if (!fn_710009d50c(raw->vf_0x0(), fn_710044e708()))
        return false;
    UnkReflObject* attack = reinterpret_cast<UnkCommanderAI*>(raw)->attack;
    if (!attack)
        return false;
    if (!fn_710009d50c(attack->vf_0x0(), fn_710042b82c()))
        return false;
    UnkCommanderAI* ai = nullptr;
    if (self->owner) {
        UnkReflObject* o = self->owner->ai;
        if (o)
            ai = reinterpret_cast<UnkCommanderAI*>(Cast<fn_710044e708>(o));
    }
    UnkAttackQuery query;
    query.zero = 0;
    query.count = 0;
    bool result = false;
    if (reinterpret_cast<UnkCommanderAttack*>(attack)->vf_0xe8(&query)) {
        UnkReflObject* tunable_object = ai->field_0x10;
        f32 distance = __builtin_fabsf(base - query.value);
        UnkCommanderAITunable* tunable =
            reinterpret_cast<UnkCommanderAITunable*>(Cast<fn_710044d0c0>(tunable_object));
        if (distance > tunable->field_0x15b8) {
            UnkSphereList* list = fn_7100ac73b0();
            result = true;
            if (list->count != 0) {
                UnkSphereEntry** it = list->data;
                UnkSphereEntry** end = it + list->count;
                do {
                    UnkSphereEntry* entry = *it;
                    UnkReflObject* object = entry ? entry->object : nullptr;
                    UnkReflObject* sphere = object ? Cast<fn_710045b404>(object) : nullptr;
                    if (sphere)
                        result = false;
                    ++it;
                } while (it != end);
            }
        }
    }
    return result;
}
