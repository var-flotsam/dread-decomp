// CNailongThornsAttack's unit. fn_710060c1bc: switches the attack to a new state (+0x50) and runs what
// the state starts with: sending one of the unit's string ids to the target, and in state 4 picking the
// phase from the owner's state (+0x110). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkShootAttack_b56a3;
struct UnkNailongAI_b59a4;

void fn_710060cd30(UnkShootAttack_b56a3* self, const CStrId* id);
void fn_7100621d38(UnkNailongAI_b59a4* self);

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f4d28;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f4d30;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f4d38;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f4d40;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f4d48;

struct UnkNailongAI_l60c1bc {
    u8 pad_0x0[0x14848];
    CStrId* ids;  // 0x14848
    u32 count;    // 0x14850
    u8 pad_0x14854[0x148c8 - 0x14854];
    u32 field_0x148c8;
};

struct UnkThornsOwner_l60c1bc {
    u8 pad_0x0[0x110];
    u32 state;
};

struct UnkThornsAttack_l60c1bc {
    u8 pad_0x0[0x8];
    UnkActor* actor;  // 0x8
    u8 pad_0x10[0x18];
    UnkThornsOwner_l60c1bc* owner;  // 0x28
    u8 pad_0x30[0x20];
    u32 state;  // 0x50
    u8 pad_0x54[0x18];
    u32 field_0x6c;
    u8 pad_0x70[0x4];
    u32 field_0x74;
    CStrId id;  // 0x78
    u8 pad_0x80[0x4];
    u32 field_0x84;
    u32 field_0x88;
    u8 pad_0x8c[0x4];
    bool field_0x90;
};

void fn_710060ce44(UnkThornsAttack_l60c1bc* self, u32 mode);

static inline UnkNailongAI_l60c1bc* CastToAI(UnkReflObject* component) {
    if (fn_710009d50c(component->vf_0x0(), fn_710061f464()))
        return reinterpret_cast<UnkNailongAI_l60c1bc*>(component);
    return nullptr;
}

static inline UnkNailongAI_l60c1bc* GetAI(UnkThornsAttack_l60c1bc* self) {
    UnkReflObject* component = static_cast<UnkReflObject*>(self->actor->field_0x240);
    return component ? CastToAI(component) : nullptr;
}

// Starts the attack's phase `mode` and resets the AI's flags.
static inline void StartPhase(UnkThornsAttack_l60c1bc* self, u32 mode) {
    {
        CStrId id = g_unk_71726f4d30;
        fn_710060cd30(reinterpret_cast<UnkShootAttack_b56a3*>(self), &id);
    }
    fn_710060ce44(self, mode);
    fn_7100621d38(reinterpret_cast<UnkNailongAI_b59a4*>(GetAI(self)));
}

void fn_710060c1bc(UnkThornsAttack_l60c1bc* self, u32 state) {
    if (self->state == state)
        return;
    self->state = state;
    UnkNailongAI_l60c1bc* ai = GetAI(self);
    switch (self->state) {
    case 1:
        self->field_0x6c = 0;
        {
            CStrId id = g_unk_71726f4d28;
            fn_710060cd30(reinterpret_cast<UnkShootAttack_b56a3*>(self), &id);
        }
        break;
    case 2:
        {
            CStrId id = g_unk_71726f4d40;
            fn_710060cd30(reinterpret_cast<UnkShootAttack_b56a3*>(self), &id);
        }
        self->field_0x74 = 0;
        break;
    case 3:
        {
            CStrId id = g_unk_71726f4d48;
            fn_710060cd30(reinterpret_cast<UnkShootAttack_b56a3*>(self), &id);
        }
        break;
    case 4:
        if (ai->field_0x148c8 == 0) {
            StartPhase(self, 0);
            break;
        }
        switch (self->owner->state) {
        case 0:
        case 0x7fffffff:
            StartPhase(self, 0);
            break;
        case 1: {
            UnkActor* target = fn_7100d7c990(self->actor);
            self->field_0x90 = target->field_0x44 <= self->actor->field_0x44;
            UnkNailongAI_l60c1bc* current = CastToAI(static_cast<UnkReflObject*>(self->actor->field_0x240));
            u32 index = 0;
            for (u32 i = 0; i < current->count; ++i) {
                if (current->ids[i] == self->id) {
                    index = i;
                    break;
                }
            }
            self->field_0x84 = index;
            self->field_0x88 = index;
            break;
        }
        case 2:
            StartPhase(self, 1);
            break;
        }
        break;
    case 5:
        {
            CStrId id = g_unk_71726f4d38;
            fn_710060cd30(reinterpret_cast<UnkShootAttack_b56a3*>(self), &id);
        }
        break;
    }
}
