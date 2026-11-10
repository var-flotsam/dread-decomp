// CIceMissileMovement's unit (tools/tu_map.py unit 1615): the missile's check against the lava pools and
// the ice shards it scatters. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "base/UnkVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

// As src/engine/HostArguments.cpp, src/game/CGrappleBeamComponent/CGrappleBeamComponent_b50.cpp and
// src/game/CFreezeRoomComponent/CFreezeRoomComponent_Pools_ovb0106.cpp declare them.
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkGrappleAnswer_b50;
void fn_710117364c(void* target, const u64* id, UnkGrappleAnswer_b50* out);
struct UnkMagmaArea_b50;
bool fn_7100a47e00(UnkMagmaArea_b50*, const CVector2D*);

extern const u64 g_unk_710164b860;

struct UnkIcePlayer_ovb0122 {
    u8 pad_0x0[0x228];
    void* field_0x228;
};

struct UnkIceLavaEntry_ovb0122 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkIceLavaList_ovb0122 {
    UnkIceLavaEntry_ovb0122** data;
    u32 count;
};

struct UnkIceLavaPool_ovb0122 {
    u8 pad_0x0[0xcb1];
    bool frozen;  // 0xcb1
};

struct UnkIceMissileMovement_ovb0122 {
    u8 pad_0x0[0x620];
    bool field_0x620;
};

// As src/game/CVulkranMagmaBallMovementComponent/CVulkranMagmaBallMovementComponent_b50.cpp declares it.
struct UnkMagmaList_b50;
UnkMagmaList_b50* fn_71008fbdb4();
void fn_7100aba584(UnkIceMissileMovement_ovb0122* self);

// With the player (fn_7100dec788), asks its component at +0x228 for the position under the id
// g_unk_710164b860 (fn_710117364c) and, when that lies in a CLavaPoolComponent that is not frozen
// (+0xcb1; fn_7100a47e00 on the 2D point), clears +0x620; then goes on with fn_7100aba584.
void fn_7100ab46ac(UnkIceMissileMovement_ovb0122* self) {
    if (UnkIcePlayer_ovb0122* player = reinterpret_cast<UnkIcePlayer_ovb0122*>(fn_7100dec788(g_unk_7101d47310))) {
        CVector3D pos = g_unk_710164779c;
        fn_710117364c(player->field_0x228, &g_unk_710164b860, reinterpret_cast<UnkGrappleAnswer_b50*>(&pos));
        UnkIceLavaList_ovb0122* list = reinterpret_cast<UnkIceLavaList_ovb0122*>(fn_71008fbdb4());
        for (UnkIceLavaEntry_ovb0122 **it = list->data, **end = list->data + list->count; it != end; ++it) {
            UnkIceLavaEntry_ovb0122* entry = *it;
            UnkReflObject* component = entry ? entry->component : nullptr;
            UnkIceLavaPool_ovb0122* pool = nullptr;
            if (component)
                pool = fn_710009d50c(component->vf_0x0(), fn_71008fb79c()) ? reinterpret_cast<UnkIceLavaPool_ovb0122*>(component) : nullptr;
            if (pool->frozen)
                continue;
            CVector2D point;
            fn_710007b430(&point, &pos);
            if (fn_7100a47e00(reinterpret_cast<UnkMagmaArea_b50*>(pool), &point)) {
                self->field_0x620 = false;
                break;
            }
        }
    }
    fn_7100aba584(self);
}

// As src/engine/HostArguments.cpp declares them.
struct UnkGameObject_0x150;
extern UnkGameObject_0x150* g_unk_7101d47370;
void fn_7100dde0c8(UnkGameObject_0x150* self);

struct UnkIceVec3_ovb0122 {
    u32 x, y, z;
};

struct UnkIceShard_ovb0122 {
    u32 kind;      // 0x0
    f32 pos_x;     // 0x4
    f32 pos_y;     // 0x8
    f32 pos_z;     // 0xc
    UnkIceVec3_ovb0122 dir;  // 0x10
    f32 size_a;    // 0x1c
    f32 size_b;    // 0x20
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 angle;     // 0x48
    f32 field_0x4c;
};

struct UnkIcePos_ovb0122 {
    f32 x, y, z;
};

void fn_7100dde3e4(UnkGameObject_0x150* self, const UnkIceShard_ovb0122* shard);

static inline f32 Random01_ovb0122() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange_ovb0122(f32 lo, f32 hi) {
    return Random01_ovb0122() * (hi - lo) + lo;
}

static inline UnkGameObject_0x150* Shards_ovb0122() {
    if (!g_unk_7101d47370) {
        UnkGameObject_0x150* p = static_cast<UnkGameObject_0x150*>(fn_71000839a0(0x150, 0x3f));
        fn_7100dde0c8(p);
        g_unk_7101d47370 = p;
    }
    return g_unk_7101d47370;
}

// Hands the shard manager (g_unk_7101d47370, created on first use) one shard of kind 1 at `pos`, raised by
// a random 0..15, moving along `dir`, of a random size 100..150 and a random angle (0..6.28), with fixed
// parameters otherwise.
void fn_7100ab481c(void* self, const UnkIcePos_ovb0122* pos, const UnkIceVec3_ovb0122* dir) {
    UnkIceShard_ovb0122 shard;
    f32 size = RandomRange_ovb0122(100.0f, 150.0f);
    shard.kind = 1;
    shard.angle = Random01_ovb0122() * 6.28f;
    shard.dir.x = dir->x;
    shard.dir.y = dir->y;
    shard.dir.z = dir->z;
    shard.pos_x = pos->x;
    shard.pos_y = pos->y;
    shard.pos_z = pos->z;
    shard.pos_z += RandomRange_ovb0122(0.0f, 15.0f);
    shard.size_a = size;
    shard.size_b = size;
    shard.field_0x24 = 30.0f;
    shard.field_0x4c = 4.0f;
    shard.field_0x28 = 2.0f;
    shard.field_0x2c = 2.0f;
    shard.field_0x30 = 2.0f;
    shard.field_0x34 = 1.0f;
    shard.field_0x38 = 0.4f;
    shard.field_0x3c = 0.4f;
    shard.field_0x40 = 0.4f;
    shard.field_0x44 = 1.0f;
    fn_7100dde3e4(Shards_ovb0122(), &shard);
}
