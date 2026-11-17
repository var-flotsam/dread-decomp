// CNailongThornsAttack's unit. fn_710060d518: advances the attack by its state (+0x50), handing the
// next state to fn_710060c1bc; in state 4 resets the Nailong AI's flags, and in state 5 sends the
// global id g_unk_71726f4d50 to the target. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkShootAttack_b56a3;
struct UnkNailongAI_b59a4;

void fn_710060cd30(UnkShootAttack_b56a3* self, const CStrId* id);
void fn_7100621020(UnkNailongAI_b59a4* self);

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f4d50;

struct UnkNailongAI_l60d518 {
    u8 pad_0x0[0x148c8];
    u32 field_0x148c8;
};

struct UnkThornsActor_l60d518 {
    u8 pad_0x0[0x240];
    UnkReflObject* field_0x240;
};

struct UnkThornsOwner_l60d518 {
    u8 pad_0x0[0x110];
    u32 field_0x110;
};

struct UnkThornsAttack_l60d518 {
    u8 pad_0x0[0x8];
    UnkThornsActor_l60d518* actor;  // 0x8
    u8 pad_0x10[0x18];
    UnkThornsOwner_l60d518* owner;  // 0x28
    u8 pad_0x30[0x20];
    u32 state;  // 0x50
    u8 pad_0x54[0x1c];
    bool field_0x70;
};

void fn_710060c1bc(UnkThornsAttack_l60d518* self, u32 state);

static inline UnkNailongAI_l60d518* GetAI(UnkThornsAttack_l60d518* self) {
    UnkReflObject* component = self->actor->field_0x240;
    if (component && fn_710009d50c(component->vf_0x0(), fn_710061f464()))
        return reinterpret_cast<UnkNailongAI_l60d518*>(component);
    return nullptr;
}

// Moves on to state 5 (resetting the AI's flags) when the flag at +0x70 is set, else to state 2.
static inline void Finish(UnkThornsAttack_l60d518* self) {
    if (!self->field_0x70) {
        fn_710060c1bc(self, 2);
        return;
    }
    fn_7100621020(reinterpret_cast<UnkNailongAI_b59a4*>(GetAI(self)));
    fn_710060c1bc(self, 5);
}

void fn_710060d518(UnkThornsAttack_l60d518* self) {
    UnkNailongAI_l60d518* ai = GetAI(self);
    switch (self->state) {
    case 0:
    case 3:
        fn_710060c1bc(self, 1);
        break;
    case 2:
        fn_710060c1bc(self, 3);
        break;
    case 4: {
        if (ai->field_0x148c8 == 0) {
            Finish(self);
            break;
        }
        u32 ownerState = self->owner->field_0x110;
        if (ownerState == 0 || ownerState == 0x7fffffff || ownerState == 2)
            Finish(self);
        break;
    }
    case 5: {
        CStrId id = g_unk_71726f4d50;
        fn_710060cd30(reinterpret_cast<UnkShootAttack_b56a3*>(self), &id);
        break;
    }
    }
}
