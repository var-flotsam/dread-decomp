// CRockDiverSpawnPointComponent's unit: whether the spawn point fires now. The player must be in its
// inner area (fn_7101127964) at least the global distance (g_unk_7101a8ca68) from the spawn position
// (+0x3c4), or, once armed (+0x3b8), still in its outer area (fn_7101127a14); then it fires when allowed
// (fn_710112a558), its delay (+0x3bc) has run out and fn_7100df5740 is false, staying armed. Leaving
// disarms it. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// As src/game/CQuetzoaAIComponent/CQuetzoaAIComponent_CombatArea_A3_b75.cpp declares them.
struct UnkQuetzoaArea_A3_b75 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual bool vf_0x50(const base::math::CVector2D* point);  // whether the area holds the point
};
UnkQuetzoaArea_A3_b75* fn_7101127964(void* spawn_point);
UnkQuetzoaArea_A3_b75* fn_7101127a14(void* spawn_point);

extern f32 g_unk_7101a8ca68;  // the closest the player may be to the spawn position

struct UnkRockDiverPlayer_la1b2e4 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkRockDiverSpawnPoint_la1b2e4 {
    u8 pad_0x0[0x3b8];
    bool armed;  // 0x3b8
    u8 pad_0x3b9[3];
    f32 delay;  // 0x3bc
    u8 pad_0x3c0[4];
    CVector3D position;  // 0x3c4
};
static_assert(__builtin_offsetof(UnkRockDiverSpawnPoint_la1b2e4, position) == 0x3c4, "layout");

bool fn_710112a558(UnkRockDiverSpawnPoint_la1b2e4* self);

static inline bool Holds(UnkQuetzoaArea_A3_b75* area, const UnkRockDiverPlayer_la1b2e4* player) {
    CVector2D point;
    f32 x = player->x, y = player->y, z = player->z;
    CVector3D position = {x, y, z};
    fn_710007b430(&point, &position);
    return area->vf_0x50(&point);
}

static inline bool Disarm(UnkRockDiverSpawnPoint_la1b2e4* self) {
    self->armed = false;
    return false;
}

bool fn_7100a1b2e4(UnkRockDiverSpawnPoint_la1b2e4* self) {
    bool allowed = fn_710112a558(self);
    const UnkRockDiverPlayer_la1b2e4* player =
        reinterpret_cast<const UnkRockDiverPlayer_la1b2e4*>(fn_7100dec788(g_unk_7101d47310));
    UnkQuetzoaArea_A3_b75* inner = fn_7101127964(self);
    UnkQuetzoaArea_A3_b75* outer = fn_7101127a14(self);
    if (!inner)
        return Disarm(self);
    if (!Holds(inner, player)) {
        if (!outer || !self->armed || !Holds(outer, player))
            return Disarm(self);
    } else {
        f32 dx = player->x - self->position.x;
        f32 dy = player->y - self->position.y;
        f32 dz = player->z - self->position.z;
        if (dx * dx + dy * dy + dz * dz < g_unk_7101a8ca68 * g_unk_7101a8ca68)
            return Disarm(self);
    }
    if (!allowed || self->delay > 0.0f || IsOpticCamouflageAbilityInState6(g_unk_7101d47310))
        return false;
    self->armed = true;
    return true;
}
