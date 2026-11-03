// The switch objects of units 116, 133 and 134 of tools/tu_map.py (game/UnkSwitchObject.h): clearing
// the busy flag and applying the recorded state. State 0 switches on and state 1 off, as
// UnkSwitchSet does; state 2 becomes the state matching the object's current flag; any other
// state is left alone. Each unit's copy hands the flag through its own callee. Names are
// placeholders.
#include "base/UnkCloneCalls.h"
#include "game/UnkSwitchObject.h"
#include "types.h"

void fn_71001f7388(void* obj, bool flag);
void fn_7100208004(void* obj, bool flag);
void fn_7100208bb4(void* obj, bool flag);

// Unit 116's layout (as in src/game_lo/fn_7100145e94.cpp): the later members sit 0x58 bytes further.
struct UnkSwitchObject116_b43 {
    u8 pad_0x0[0x28];
    u8 field_0x28[0xf9];
    bool field_0x121;
    u8 pad_0x122[0x9e];
    char field_0x168[0xd8];  // at 0x1c0
    bool field_0x240;        // at 0x298
    u8 pad_0x299[3];
    s32 field_0x244;  // at 0x29c
};
static_assert(__builtin_offsetof(UnkSwitchObject116_b43, field_0x244) == 0x29c, "UnkSwitchObject116_b43 layout");

template <auto Hand, class Self>
static inline void ApplyPending(Self* self) {
    self->field_0x240 = false;
    switch (self->field_0x244) {
    case 0:
        UnkSwitchSet<Hand>(self, true);
        break;
    case 1:
        UnkSwitchSet<Hand>(self, false);
        break;
    case 2:
        self->field_0x244 = !self->field_0x121;
        break;
    }
}

void fn_7100141724(UnkSwitchObject116_b43* self) { ApplyPending<fn_71001f7388>(self); }
void fn_710014677c(UnkSwitchObject* self) { ApplyPending<fn_7100208004>(self); }
void fn_71001473c8(UnkSwitchObject* self) { ApplyPending<fn_7100208bb4>(self); }
