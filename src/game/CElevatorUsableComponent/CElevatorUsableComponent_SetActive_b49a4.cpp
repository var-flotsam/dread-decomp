// CElevatorUsableComponent unit (tools/tu_map.py), and its copy in CTrainUsableComponent: switching
// on sets the flag at +0x4f8, runs the virtual at +0x3a0, hands the member at +0x540 to the world
// (fn_7100e9b6ac on the game manager's +0x2228) and runs +0x268; switching off runs +0x270. Names
// and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

void fn_7100e9b6ac(void* world, void* item);

struct UnkTransportUsable_b49a4 {
    void** vtable;
    u8 pad_0x8[0x4f0];
    bool active;  // +0x4f8
    u8 pad_0x4f9[0x47];
    u8 field_0x540[8];

    void Call(u32 offset) { reinterpret_cast<void (*)(UnkTransportUsable_b49a4*)>(vtable[offset / 8])(this); }

    __attribute__((always_inline)) void SetActive(bool on) {
        if (on) {
            active = true;
            Call(0x3a0);
            fn_7100e9b6ac(g_unk_7101d47310->field_0x2228, field_0x540);
            Call(0x268);
        } else {
            Call(0x270);
        }
    }
};

void fn_7100a828b4(UnkTransportUsable_b49a4* self, bool on) { self->SetActive(on); }

void fn_7100a9a588(UnkTransportUsable_b49a4* self, bool on) { self->SetActive(on); }
