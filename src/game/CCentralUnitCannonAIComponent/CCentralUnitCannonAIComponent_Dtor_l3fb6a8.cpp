// CCentralUnitCannonAIComponent unit (tools/tu_map.py): the destructor. The tracking listener at
// +0x14a28, the state machine at +0x14978, its six states (value holders, +0x146d8 on, 0x70 apart),
// then the base AI component's destructor (fn_71010f8b98). Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "base/UnkCloneHelpers.h"
#include "types.h"

extern void* g_unk_7101a326c0[];  // CCentralUnitCannonAIComponent's vtable
extern char g_unk_7101a32f38[];   // the state machine's vtable
extern char g_unk_7101a32ee0[];   // the states' base vtable

void fn_71010f8b98(void* self);

struct UnkCannonState_l3fb6a8;
template <>
struct base::UnkValueHolderTraits<UnkCannonState_l3fb6a8> {
    static char* vtable() { return g_unk_7101a32ee0; }
};
using UnkCannonStateHolder_l3fb6a8 = base::UnkValueHolder<UnkCannonState_l3fb6a8>;

struct UnkCannonMachine_l3fb6a8 {
    void* vtable;
    u64 field_0x8;
    void* current;
    void* previous;
    u32 count;

    ~UnkCannonMachine_l3fb6a8() {
        vtable = g_unk_7101a32f38 + 0x10;
        previous = nullptr;
        current = nullptr;
        count = 0;
    }
};

struct UnkCannonAI_l3fb6a8 {
    void** vtable;
    u8 pad_0x8[0x146d0];
    UnkCannonStateHolder_l3fb6a8 states[6];  // 0x146d8
    UnkCannonMachine_l3fb6a8 machine;        // 0x14978
    u8 pad_0x149a0[0x88];
    UnkAttackListenerRaw listener;  // 0x14a28
};
static_assert(__builtin_offsetof(UnkCannonAI_l3fb6a8, machine) == 0x14978, "layout");
static_assert(__builtin_offsetof(UnkCannonAI_l3fb6a8, listener) == 0x14a28, "layout");

void fn_71003fb6a8(UnkCannonAI_l3fb6a8* self) {
    self->vtable = &g_unk_7101a326c0[2];
    self->listener.Destroy();
    self->machine.~UnkCannonMachine_l3fb6a8();
    for (int i = 5; i >= 0; --i)
        base::refl_ValueHolderDtor(&self->states[i]);
    fn_71010f8b98(self);
}
