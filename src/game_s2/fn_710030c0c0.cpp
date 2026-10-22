// Once the global manager exists and isn't ready (0x110 clear), a state at 0x448 of 1 or 2
// clears the counter at 0x44c; then the counter at 0x484 is cleared, the flag at 0x49b set and
// fn_71002f9460 called with 0.
#include "types.h"

struct UnkManager {
    u8 unk_0x0[0x110];
    u32 ready;
};
struct UnkSelf {
    u8 unk_0x0[0x448];
    u32 state;
    u32 field_0x44c;
    u8 unk_0x450[0x34];
    u32 field_0x484;
    u8 unk_0x488[0x13];
    u8 field_0x49b;
};

extern UnkManager* g_unk_7101d47360;
void* fn_71000839a0(u32 size, u32 align_mask);
void fn_7100ee1f34(UnkManager* manager);
void fn_71002f9460(UnkSelf* self, u32 a1);

void fn_710030c0c0(UnkSelf* self) {
    UnkManager* manager = g_unk_7101d47360;
    if (!manager) {
        manager = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    if (manager->ready)
        return;
    if (self->state - 1 <= 1)
        self->field_0x44c = 0;
    self->field_0x484 = 0;
    self->field_0x49b = 1;
    fn_71002f9460(self, 0);
}
