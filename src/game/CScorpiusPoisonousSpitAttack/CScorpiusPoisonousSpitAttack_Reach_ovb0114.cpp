// CScorpiusPoisonousSpitAttack's unit (tools/tu_map.py): whether the target is out of reach. After the
// base check (fn_71006865a8), the Scorpius AI component gives a reference x and a facing x (vf_0x5e0);
// the target's x distance from the reference, signed towards the facing, is compared with the attack
// definition's reach (vf_0x28) plus the target's radius (fn_7100977b20 of its +0x200). Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

f32 fn_7100977b20(void*);

struct UnkScorpiusSpitAI_ovb0114 {
    virtual void vf_0x0();

    void Reference(f32* x, f32* facing) {
        using Fn = void (*)(UnkScorpiusSpitAI_ovb0114*, f32*, f32*);
        (*reinterpret_cast<Fn**>(this))[0x5e0 / 8](this, x, facing);
    }
};

struct UnkScorpiusSpitDef_ovb0114 {
    virtual void vf_0x0();

    f32 Reach(UnkActor* actor) {
        using Fn = f32 (*)(UnkScorpiusSpitDef_ovb0114*, UnkActor*);
        return (*reinterpret_cast<Fn**>(this))[0x28 / 8](this, actor);
    }
};

// The target's body (+0x200), not spelled out in UnkActor.
struct UnkScorpiusSpitTarget_ovb0114 {
    u8 pad_0x0[0x200];
    void* body;  // 0x200
};

struct UnkScorpiusSpit_ovb0114 {
    void* vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    UnkScorpiusSpitDef_ovb0114* def;  // 0x30
};

bool fn_71006865a8(UnkScorpiusSpit_ovb0114* self);  // the base check

bool fn_710068ae0c(UnkScorpiusSpit_ovb0114* self) {
    if (!fn_71006865a8(self))
        return false;
    UnkReflObject* obj = static_cast<UnkReflObject*>(self->owner->field_0x240);
    UnkScorpiusSpitAI_ovb0114* ai = reinterpret_cast<UnkScorpiusSpitAI_ovb0114*>(
        obj && fn_710009d50c(obj->vf_0x0(), fn_7100690c74()) ? obj : nullptr);
    f32 x = 0.0f;
    f32 facing = 0.0f;
    ai->Reference(&x, &facing);
    f32 d = fn_7100d7c990(self->owner)->field_0x44 - x;
    f32 distance = facing < x ? -d : d;
    auto* target = reinterpret_cast<UnkScorpiusSpitTarget_ovb0114*>(fn_7100d7c990(self->owner));
    if (!target || !target->body)
        return true;
    void* body = target->body;
    return distance > self->def->Reach(self->owner) + fn_7100977b20(body);
}
