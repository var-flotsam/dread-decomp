// CAbilityGhostAura's unit (tools/tu_map.py): a stop. Runs fn_7100309fd8, gives the linked actor (+0xc0,
// +0x30) with a timeline component (+0x208) the key g_unk_7101648d80 when `keep` is set, else resets the
// value at +0x47c to -1 and gives it g_unk_7101648d78 (fn_7100d7c828); then clears the counters and flags
// and calls fn_71002f9460 with 0. Names and layouts are placeholders.
#include "game/CAbility.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern u64 g_unk_7101648d80;
extern u64 g_unk_7101648d78;

struct UnkGhostAuraActor_A2_b70 {
    u8 pad_0x0[0x208];
    void* timeline;  // 0x208
};

struct UnkGhostAuraLink_A2_b70 {
    u8 pad_0x0[0x30];
    UnkGhostAuraActor_A2_b70* actor;
};

struct UnkGhostAura_A2_b70 {
    u8 pad_0x0[0xc0];
    UnkGhostAuraLink_A2_b70* link;  // 0xc0
    u8 pad_0xc8[0x3d0 - 0xc8];
    u32 field_0x3d0;
    u8 pad_0x3d4[0x460 - 0x3d4];
    u64 field_0x460;
    u8 pad_0x468[4];
    u32 field_0x46c;
    u8 pad_0x470[0xc];
    f32 field_0x47c;
    u8 pad_0x480[0x1b];
    u8 field_0x49b;
    u8 pad_0x49c[5];
    u8 field_0x4a1;
};
static_assert(__builtin_offsetof(UnkGhostAura_A2_b70, field_0x4a1) == 0x4a1, "UnkGhostAura_A2_b70 layout");

struct UnkSelf;

void fn_710030d358(UnkGhostAura_A2_b70* self, bool keep) {
    fn_7100309fd8(self, 0, 1);
    if (keep) {
        if (self->link && self->link->actor && self->link->actor->timeline)
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->link->actor), &g_unk_7101648d80, false);
    } else {
        self->field_0x47c = -1.0f;
        if (self->link && self->link->actor && self->link->actor->timeline)
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->link->actor), &g_unk_7101648d78, false);
    }
    self->field_0x3d0 = 0;
    self->field_0x460 = 0;
    self->field_0x46c = 0;
    self->field_0x4a1 = 0;
    self->field_0x49b = 0;
    fn_71002f9460(reinterpret_cast<UnkSelf*>(self), 0);
}
