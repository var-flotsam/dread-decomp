// CAutectorAIComponent (tools/tu_map.py): the destructor. Stores the class's vtable, destroys the
// embedded listener at +0x146d8 (game_5821e4/CAttackDtor.h) and runs the base AI component's
// destructor (through the thunk fn_7100661914). Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "game/CRobotAIComponent.h"

extern void* g_unk_7101a2b930[];  // the component's vtable

struct UnkAutsharpAIBase_b47;
// As src/game/CAutsharpAIComponent/CAutsharpAIComponent_Dtor_b47.cpp declares it: a thunk to the base
// AI component's destructor (fn_7100369380).

struct UnkAutectorAI_b49 {
    void** vtable;
    u8 pad_0x8[0x146d0];
    UnkAttackListenerRaw field_0x146d8;
};

void fn_71003bb2d8(UnkAutectorAI_b49* self) {
    self->vtable = g_unk_7101a2b930 + 2;
    self->field_0x146d8.Destroy();
    fn_7100661914(reinterpret_cast<UnkAutsharpAIBase_b47*>(self));
}
