// CCharClassCameraFXPresetEvent's unit (tools/tu_map.py): sending the game manager the event's camera
// preset (fn_7100cae608). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

// A 12-byte argument (as src/game_s2/fn_710035131c.cpp's).
struct UnkVec {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};
void fn_7100e038b0(UnkGameManager* dispatcher, void* key, UnkVec* vec, u64 arg);

struct UnkCameraFXState_ovb0094 {
    u8 unk_0x0[0x70];
    s32 mode;  // 0x70
};

struct UnkCameraFXPlayer_ovb0094 {
    u8 unk_0x0[0x1b8];
    UnkCameraFXState_ovb0094* state;  // 0x1b8
};

// An entity's position, read as raw words (+0x44, +0x54, +0x64).
struct UnkCameraFXEntity_ovb0094 {
    u8 unk_0x0[0x44];
    u32 x;
    u8 unk_0x48[0xc];
    u32 y;
    u8 unk_0x58[0xc];
    u32 z;
};

struct UnkCameraFXPresetEvent_ovb0094 {
    u8 unk_0x0[0x10];
    CStrId preset;  // 0x10
    UnkVec params;  // 0x18
};

// Sends the game manager the camera preset (at the target's position when there is a target), unless
// there is no player or the target is someone else while the player's state is in mode 2.
void fn_7100cae608(UnkCameraFXPresetEvent_ovb0094* self, UnkCameraFXEntity_ovb0094* const& target) {
    if (self->preset.data == &g_unk_7101d47108->empty)
        return;
    UnkVec params;
    u64 head;
    __builtin_memcpy(&head, &self->params, sizeof(head));
    u32 last = self->params.field_0x8;
    __builtin_memcpy(&params, &head, sizeof(head));
    params.field_0x8 = last;
    auto* player = reinterpret_cast<UnkCameraFXPlayer_ovb0094*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->state)
        return;
    UnkCameraFXEntity_ovb0094* entity = target;
    if (reinterpret_cast<void*>(entity) != player && player->state->mode == 2)
        return;
    if (entity) {
        u32 x = entity->x;
        u32 y = entity->y;
        u32 z = entity->z;
        UnkVec position;
        position.field_0x0 = x;
        position.field_0x4 = y;
        position.field_0x8 = z;
        fn_7100e038b0(g_unk_7101d47310, &self->preset, &params, (u64)&position);
    } else {
        fn_7100e038b0(g_unk_7101d47310, &self->preset, &params, 0);
    }
}
