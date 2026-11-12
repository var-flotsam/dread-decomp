// CEmmyFloorSlideCameraCtrl's unit (tools/tu_map.py): the slide camera's set-up behind the player and its
// update (not matched yet). The set-up places the start (+0x38) 450 behind, 30 below and 200 in front of the player, looking
// along the unit's direction (g_unk_7101a722c8, normalised in place) or its mirror; the update eases the
// start position and direction (+0x38, +0x44) from the slide ones (+0x1dc, +0x1e8) over the slide time
// (+0x1f4 of +0x1f8). The layout is CEmmyFloorSlideCameraCtrl_b44.cpp's. Names are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU53.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSlideCamera_ovb0135 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkVec3f start_pos;  // 0x38
    UnkVec3f start_dir;  // 0x44
    u8 pad_0x50[0x18c];
    UnkVec3f slide_pos;  // 0x1dc
    UnkVec3f slide_dir;  // 0x1e8
    f32 time;            // 0x1f4
    f32 duration;        // 0x1f8

    void vf_0x48(bool flag) { reinterpret_cast<void (*)(UnkSlideCamera_ovb0135*, bool)>(vtable[0x48 / 8])(this, flag); }
};
static_assert(__builtin_offsetof(UnkSlideCamera_ovb0135, duration) == 0x1f8, "layout");

struct UnkPlayer;

struct UnkSlidePlayer_ovb0135 {
    u8 pad_0x0[0x38];
    u8 transform[0xc];  // 0x38
    f32 x;              // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

extern __attribute__((visibility("hidden"))) UnkVec3f g_unk_7101a722c8;

static inline void Normalize(f32& x, f32& y, f32& z) {
    f32 inv = 1.0f / (__builtin_sqrtf(x * x + y * y + z * z) + 0.0001f);
    x = x * inv;
    y = inv * y;
    z = inv * z;
}

static const UnkVec3f& Mirrored() {
    static UnkVec3f s_mirrored(-g_unk_7101a722c8.x, g_unk_7101a722c8.y, g_unk_7101a722c8.z);
    return s_mirrored;
}

void fn_71007d337c(UnkSlideCamera_ovb0135* self) {
    auto* player = reinterpret_cast<UnkSlidePlayer_ovb0135*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    f32 x = player->x, y = player->y, z = player->z;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(player->transform));
    f32 facing = m.m[2];
    Normalize(g_unk_7101a722c8.x, g_unk_7101a722c8.y, g_unk_7101a722c8.z);
    const UnkVec3f& mirrored = Mirrored();
    bool right = facing > 0.0f;
    self->start_pos = UnkVec3f(x + (right ? -450.0f : 450.0f), y + -30.0f, z + 200.0f);
    if (right)
        self->start_dir = g_unk_7101a722c8;
    else
        self->start_dir = mirrored;
}
