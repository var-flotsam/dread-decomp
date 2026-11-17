#pragma once

// The Sclawk AI component (reflected as CSclawkAIComponent; unit 1005 of tools/tu_map.py), as far as the
// hand-written sources know it: its owner actor at +0x30 and the component's own members after its base
// (CBehaviorTreeAIComponent, which ends at 0x146c8, tools/gen_fields.py). Names and layouts are placeholders.
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkSwitchObject.h"
#include "types.h"

// The tunables the component reads (+0x146c8): the range of its random wait and the two times of state 2.
struct UnkSclawkTunables {
    u8 pad_0x0[0x50c];
    f32 wait_min;  // 0x50c
    f32 wait_max;  // 0x510
    f32 field_0x514;
    f32 field_0x518;
};

struct UnkSclawkAIComponent {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkSclawkTunables* tunables;  // 0x146c8
    f32 wait;                     // 0x146d0: state 0's countdown
    f32 field_0x146d4;
    f32 field_0x146d8;
    u32 field_0x146dc;
    u32 state;  // 0x146e0
    u8 pad_0x146e4[8];
    bool field_0x146ec;
    u8 pad_0x146ed[2];
    bool part_shown;  // 0x146ef
};
static_assert(__builtin_offsetof(UnkSclawkAIComponent, part_shown) == 0x146ef, "UnkSclawkAIComponent placeholder layout");

// A part of the owner, as far as it is read here: its id, which the owner's parts are asked about.
struct UnkSclawkPart {
    u8 pad_0x0[0xb];
    u8 id;  // 0xb
};

// Shows or hides the owner's part whose name hashes to 0xe6de93263e1369e9 (nothing when the owner's parts
// at +0x280 do not have it).
static inline void ShowSclawkPart(void* parts, bool show) {
    auto* part = reinterpret_cast<UnkSclawkPart*>(fn_7101152b90(static_cast<UnkWorld*>(parts), 0xe6de93263e1369e9));
    if (part && !fn_710114e668(static_cast<UnkObject*>(parts), part->id))
        part = nullptr;
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), show);
}
