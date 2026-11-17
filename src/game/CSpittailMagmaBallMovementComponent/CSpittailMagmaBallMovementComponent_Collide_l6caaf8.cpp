// CSpittailMagmaBallMovementComponent's unit (tools/tu_map.py): the ball touches `other`. Ignores none
// and its own holder's actor; unless already done (+0x5d8), runs fn_71006ca5d4 for anyone but the holder's
// target. When `other` is the player, not already handled (fn_7100ac79d8) and its part (fn_7100623810) is
// armed, fires a shot back along the ball's direction (vf_0x338) and hits the player (vf_0x2a8, unless
// the definition's +0xed says not to); otherwise the base handles it. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" f32 atan2f(f32 y, f32 x);
extern const char g_unk_710164a788[] __attribute__((visibility("hidden")));

struct UnkSpittailMagmaBallMovement;
void fn_71006ca5d4(UnkSpittailMagmaBallMovement* self, void* c);
// Whether the hit was already handled (bombs that touch the player or something they ignore).
bool fn_7100ac79d8(void* self, void* actor, void* info);

struct UnkMagmaBallPlayerPart_l6caaf8 {
    u8 pad_0x0[0x340];
    bool armed;  // 0x340
};
UnkMagmaBallPlayerPart_l6caaf8* fn_7100623810(UnkActor* actor);

struct UnkMagmaBallHolder_l6caaf8 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkMagmaBallDef_l6caaf8 {
    u8 pad_0x0[0xed];
    bool no_hit;  // 0xed
};

// What the shot carries (+0x3b4 of the movement).
struct UnkMagmaBallShot_l6caaf8 {
    UnkVec3f pos;  // 0x0
    UnkVec3f dir;  // 0xc
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    s32 field_0x28;
};

struct UnkMagmaBallMovement_l6caaf8;

struct UnkMagmaBallMovementVtbl_l6caaf8 {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkMagmaBallMovement_l6caaf8* self, UnkActor* other, void* info);
    void* slots_0x2b0[(0x338 - 0x2b0) / 8];
    void (*vf_0x338)(UnkMagmaBallMovement_l6caaf8* self, UnkMagmaBallShot_l6caaf8* shot, const UnkVec3f* angles,
                     const char* kind, base::global::CFilePathStrId* path);
};

struct UnkMagmaBallMovement_l6caaf8 {
    UnkMagmaBallMovementVtbl_l6caaf8* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xd4];
    f32 dir_x;  // 0x10c
    f32 dir_y;  // 0x110
    f32 dir_z;  // 0x114
    u8 pad_0x118[0x118];
    UnkMagmaBallHolder_l6caaf8* holder;  // 0x230
    u8 pad_0x238[0x178];
    s32 shot_id;  // 0x3b0
    UnkMagmaBallShot_l6caaf8 shot;  // 0x3b4
    u8 pad_0x3e0[0x1f8];
    bool done;  // 0x5d8
    u8 pad_0x5d9[7];
    UnkMagmaBallDef_l6caaf8* def;  // 0x5e0
};
static_assert(__builtin_offsetof(UnkMagmaBallMovement_l6caaf8, shot) == 0x3b4, "layout");
static_assert(__builtin_offsetof(UnkMagmaBallMovement_l6caaf8, def) == 0x5e0, "layout");

bool fn_71006dfa88(UnkMagmaBallMovement_l6caaf8* self, UnkActor* other, void* a2, void* a3, void* a4, void* a5);

bool fn_71006caaf8(UnkMagmaBallMovement_l6caaf8* self, UnkActor* other, void* a2, void* a3, void* a4,
                   void* a5) {
    if (!other)
        return false;
    UnkMagmaBallHolder_l6caaf8* holder = self->holder;
    if ((holder ? holder->actor : nullptr) == other)
        return false;
    if (!self->done && !(holder && holder->actor && fn_7100d7c990(holder->actor) == other))
        fn_71006ca5d4(reinterpret_cast<UnkSpittailMagmaBallMovement*>(self), a4);
    if (reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310)) != other || fn_7100ac79d8(self, other, a4))
        return fn_71006dfa88(self, other, a2, a3, a4, a5);
    UnkMagmaBallPlayerPart_l6caaf8* part = fn_7100623810(other);
    if (!part || !part->armed)
        return fn_71006dfa88(self, other, a2, a3, a4, a5);
    UnkActor* owner = self->owner;
    UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    f32 dx = self->dir_x, dy = self->dir_y, dz = self->dir_z;
    f32 len = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    self->shot_id = -1;
    f32 inv = 1.0f / (len + 0.0001f);
    f32 nx = -(dx * inv), ny = -(dy * inv), nz = -(dz * inv);
    UnkMagmaBallShot_l6caaf8* shot = &self->shot;
    shot->pos = pos;
    shot->dir.x = nx;
    shot->dir.y = ny;
    shot->dir.z = nz;
    shot->field_0x18 = 0.0f;
    shot->field_0x1c = 0.0f;
    shot->field_0x20 = 0.0f;
    shot->field_0x24 = 0.0f;
    shot->field_0x28 = 40;
    UnkVec3f angles(0.0f, 0.0f, atan2f(ny, nx));
    self->vtbl->vf_0x338(self, shot, &angles, g_unk_710164a788, &g_unk_7101d470e8);
    if (!self->def->no_hit)
        self->vtbl->vf_0x2a8(self, other, a4);
    return false;
}
