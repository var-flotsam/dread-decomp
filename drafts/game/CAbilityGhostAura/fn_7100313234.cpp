// CAbilityGhostAura's unit (tools/tu_map.py unit 430): picks the timeline ids the player's timeline
// component (+0x2e8) gets for a ghost aura event: one cleared (fn_7100a2b7b8) and one set to the all-flags
// mask (fn_7100a2cb7c), by the event and the aura's state. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_7101648d88;  // reached through the GOT
extern const u64 g_unk_7101648d90;  // reached through the GOT
extern const u64 g_unk_7101648d98;  // reached through the GOT

static const u64 kNoId = ~0ull;

struct UnkGhostAuraPlayer_l313234 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkGhostAuraSource_l313234 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkGhostAuraComponent_l313234 {
    u8 pad_0x0[0xe8];
    UnkGhostAuraSource_l313234* field_0xe8;
};

struct UnkGhostAura_l313234 {
    u8 pad_0x0[0xc0];
    UnkGhostAuraComponent_l313234* component;  // 0xc0
    u8 pad_0xc8[0x438 - 0xc8];
    u32 field_0x438;
    u8 pad_0x43c[0x448 - 0x43c];
    s32 state;  // 0x448
    u8 pad_0x44c[0x46c - 0x44c];
    f32 field_0x46c;
    u8 pad_0x470[0x494 - 0x470];
    bool field_0x494;
    u8 pad_0x495[0x499 - 0x495];
    bool field_0x499;
    u8 pad_0x49a[0x49c - 0x49a];
    bool field_0x49c;
    bool field_0x49d;
};
static_assert(__builtin_offsetof(UnkGhostAura_l313234, field_0x49d) == 0x49d, "layout");

void fn_7100313234(UnkGhostAura_l313234* self, u32 event) {
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return;
    UnkTimelineComponent* timeline = reinterpret_cast<UnkGhostAuraPlayer_l313234*>(player)->timeline;
    if (!timeline)
        return;

    base::reflection::UnkReflTypeIdArg mask;
    u64 set_id = kNoId;
    u64 clear_id = kNoId;
    s32 state = self->state;
    if (state == 2) {
        if (fn_7100a2be94(timeline, &g_unk_7101648d88))
            fn_7100a2b7b8(timeline, &g_unk_7101648d88);
        fn_7100a2b7b8(timeline, &g_unk_7101648d90);
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &g_unk_7101648d98, &mask.id, false, -1.0f);
    }

    switch (event) {
    case 0:
        switch (state) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (self->field_0x49c || self->field_0x49d) {
                set_id = 0xafb3551591348009;
                clear_id = 0x9c876200f4b05abd;
            } else if (self->field_0x494) {
                set_id = self->field_0x438 == 2 ? 0x701078232111f711 : 0xd8cff15d7d213d80;
                clear_id = self->field_0x438 == 2 ? 0x1662e71c6c2da5e5 : 0x111382869762a34b;
            } else {
                set_id = self->field_0x438 == 2 ? 0x38f7abcb2b41deea : 0xcc54376ffbc95028;
                clear_id = self->field_0x438 == 2 ? 0x8d5762381fcf94a6 : 0xd4fd8bac2f1b3490;
            }
            break;
        case 2:
            set_id = 0x377f6cd70eeb04bd;
            break;
        }
        break;
    case 4:
        if (!self->field_0x499)
            break;
        // fall through
    case 3:
        if (!(self->field_0x46c <= 0.1f)) {
            if (self->field_0x49c || self->field_0x49d)
                set_id = 0x9c876200f4b05abd;
            else if (self->field_0x494)
                set_id = self->field_0x438 == 1 ? 0x111382869762a34b : 0x1662e71c6c2da5e5;
            else
                set_id = self->field_0x438 == 1 ? 0xd4fd8bac2f1b3490 : 0x8d5762381fcf94a6;
        } else {
            set_id = 0x159359190eb2b588;
        }
        break;
    case 2:
        if (self->component->field_0xe8 && self->component->field_0xe8->field_0x10) {
            set_id = 0xa80fd960ed727e93;
            clear_id = 0x7063960bf5e7f4bc;
        }
        break;
    case 1:
        set_id = 0xd1d9e97e21c73cad;
        break;
    case 5:
        if (self->field_0x499)
            set_id = self->field_0x438 == 1 ? 0xd8cff15d7d213d80 : 0x701078232111f711;
        break;
    }

    if (clear_id != kNoId)
        fn_7100a2b7b8(timeline, &clear_id);
    if (set_id != kNoId) {
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &set_id, &mask.id, false, -1.0f);
    }
}
