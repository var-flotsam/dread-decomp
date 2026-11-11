// CTriggerComponent unit (tools/tu_map.py): two update functions with the same body (fn_7100a5e758,
// fn_7100a606d0). After the checks (fn_710024fbc8) and fn_7100a5e8a4 they record the player's position
// (+0x240 tells whether there is a player), then give every timer of kind 3 whose period has elapsed
// against the game manager's clock (+0x2444) a new start and a random extra delay. Names and layouts
// are placeholders.
#include <math.h>
#include <stdlib.h>

#include "game/CActorComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkTriggerPlayerPos_ovb0067 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkTriggerTimer_ovb0067 {
    u8 pad_0x0[8];
    s32 kind;          // 8
    u8 pad_0xc[0x10];
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
};

struct UnkTriggerUpdate_ovb0067 {
    u8 pad_0x0[0x240];
    bool has_player;                    // 0x240
    UnkVec3f player_pos;                // 0x244
    UnkTriggerTimer_ovb0067** timers;   // 0x250
    u32 timer_count;                    // 0x258
};

void fn_7100a5e8a4(UnkTriggerUpdate_ovb0067* self);

// std::min's order: the second value when it is smaller.
static inline f32 Min(f32 a, f32 b) {
    return b < a ? b : a;
}

static inline f32 Random01() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange(f32 lo, f32 hi) {
    f32 r = Random01();
    return lo + (hi - lo) * r;
}

__attribute__((always_inline)) static inline void UpdateTimers(UnkTriggerUpdate_ovb0067* self) {
    if (!fn_710024fbc8(self))
        return;
    fn_7100a5e8a4(self);
    UnkTriggerPlayerPos_ovb0067* player =
        reinterpret_cast<UnkTriggerPlayerPos_ovb0067*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        self->has_player = true;
        self->player_pos = UnkVec3f(player->x, player->y, player->z);
    } else {
        self->has_player = false;
    }
    for (UnkTriggerTimer_ovb0067** it = self->timers; it != self->timers + self->timer_count; ++it) {
        UnkTriggerTimer_ovb0067* t = *it;
        if (t->kind != 3)
            continue;
        if (!(t->field_0x1c >= 0.0f))
            continue;
        f32 end = t->field_0x1c + t->field_0x24 + t->field_0x28;
        f32 now = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2444);
        if (!(end < now))
            continue;
        t->field_0x24 = now;
        t->field_0x28 = RandomRange(0.0f, Min(t->field_0x20, 0.0f));
    }
}

// Two update functions of the unit with the same body.
void fn_7100a5e758(UnkTriggerUpdate_ovb0067* self) {
    UpdateTimers(self);
}

void fn_7100a606d0(UnkTriggerUpdate_ovb0067* self) {
    UpdateTimers(self);
}
