// CChozoCommanderAIComponent unit (tools/tu_map.py, the name from reflection): unless the owner's (+0x30)
// +0x278 object already runs the entry at +0x150d0 (its +0x1a0), stops it (its virtual at vtable +0x178) and
// starts the entry at +0x157f0 on it (fn_7101134420, with -1 and no options). Names and layouts are
// placeholders.
#include "game/CAIAttackComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "types.h"

struct UnkCommanderPlayer_b67a2 {
    void** vtable;
    u8 pad_0x8[0x198];
    void* current;  // 0x1a0

    void vf_0x178() { reinterpret_cast<void (*)(UnkCommanderPlayer_b67a2*)>(vtable[0x178 / 8])(this); }
};

struct UnkCommanderOwner_b67a2 {
    u8 pad_0x0[0x278];
    UnkCommanderPlayer_b67a2* player;
};

struct UnkCommanderAI_b67a2 {
    u8 pad_0x0[0x30];
    UnkCommanderOwner_b67a2* owner;
    u8 pad_0x38[0x150d0 - 0x38];
    u8 entry_0x150d0[0x157f0 - 0x150d0];
    u8 entry_0x157f0[1];
};

void fn_71004554d8(UnkCommanderAI_b67a2* self) {
    UnkCommanderPlayer_b67a2* player = self->owner->player;
    if (player != nullptr && player->current == self->entry_0x150d0)
        return;
    asm("" : "+r"(player));  // the original calls through it even when null
    player->vf_0x178();
    fn_7101134420(self->owner->player, self->entry_0x157f0, 0, 0, nullptr, -1.0f);
}
