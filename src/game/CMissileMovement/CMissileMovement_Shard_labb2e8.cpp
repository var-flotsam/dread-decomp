// CMissileMovement unit (tools/tu_map.py unit 1618): hands the shard manager one shard thrown off by the
// missile, as the ice missile does with other fixed parameters. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2550.h"
#include "types.h"

using base::math::CVector3D;

// As src/game/CIceMissileMovement/CIceMissileMovement_Shards_ovb0122.cpp declares them.
struct UnkGameObject_0x150;

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

void fn_7100dde3e4(UnkGameObject_0x150* self, const UnkIceShard_ovb0122* shard);

static inline f32 Random01_labb2e8() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange_labb2e8(f32 lo, f32 hi) {
    return Random01_labb2e8() * (hi - lo) + lo;
}

static inline UnkGameObject_0x150* Shards_labb2e8() {
    if (!g_unk_7101d47370) {
        UnkGameObject_0x150* p = static_cast<UnkGameObject_0x150*>(fn_71000839a0(0x150, 0x3f));
        fn_7100dde0c8(p);
        g_unk_7101d47370 = p;
    }
    return g_unk_7101d47370;
}

// One shard of kind 1 at `pos`, raised by a random 0..15, moving along `dir`, of a random size 100..150
// and a random angle (0..6.28), with fixed parameters otherwise.
void fn_7100abb2e8(void* self, const CVector3D* pos, const UnkIceVec3_ovb0122* dir) {
    UnkIceShard_ovb0122 shard;
    f32 size = RandomRange_labb2e8(100.0f, 150.0f);
    shard.kind = 1;
    shard.angle = Random01_labb2e8() * 6.28f;
    shard.dir.x = dir->x;
    shard.dir.y = dir->y;
    shard.dir.z = dir->z;
    shard.pos_x = pos->x;
    shard.pos_y = pos->y;
    shard.pos_z = pos->z;
    shard.pos_z += RandomRange_labb2e8(0.0f, 15.0f);
    shard.size_a = size;
    shard.size_b = size;
    shard.field_0x24 = 30.0f;
    shard.field_0x4c = 4.0f;
    shard.field_0x28 = 5.0f;
    shard.field_0x2c = 2.0f;
    shard.field_0x30 = 1.0f;
    shard.field_0x34 = 1.0f;
    shard.field_0x38 = 0.0f;
    shard.field_0x3c = 0.0f;
    shard.field_0x40 = 0.0f;
    shard.field_0x44 = 1.0f;
    fn_7100dde3e4(Shards_labb2e8(), &shard);
}
