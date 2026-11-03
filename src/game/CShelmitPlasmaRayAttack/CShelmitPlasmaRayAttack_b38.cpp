// CShelmitPlasmaRayAttack: moves the attack's state on (3-6 to 6; 7-9 to 10, which also notifies the
// owner's CShelmitAIComponent through fn_71006baf90). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkShelmitAI {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkShelmitAI*)>(vtable[0])(this);
    }
};

struct UnkPlasmaRayAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x20];
    u32 state;  // 0x30
};

void fn_71006baf90(UnkShelmitAI* ai);

void fn_71006b638c(UnkPlasmaRayAttack* self) {
    switch (self->state) {
    case 3:
    case 4:
    case 5:
    case 6:
        self->state = 6;
        break;
    case 7:
    case 8:
    case 9: {
        self->state = 10;
        UnkActor* owner = self->owner;
        if (!owner)
            return;
        UnkShelmitAI* ai = static_cast<UnkShelmitAI*>(owner->field_0x240);
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_71006b98c0()))
            fn_71006baf90(ai);
        break;
    }
    }
}
