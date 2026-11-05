// CChozoCommanderEnergyShardsSphereMovementComponent's unit (tools/tu_map.py unit 671): with the movement
// component as second argument, sets its state (+0xc8) to 1, gives its damage request the tunable's value
// (+0xa4 of g_unk_7172716160, fn_71008c1538), copies the unit's static string id into +0xb8 and sets bit
// 26 of the flags at +0xb4 (as CCooldownXBossFireBallMovement_Launch_A2_b70.cpp). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726c9e00 __attribute__((visibility("hidden")));

struct UnkShardsSphereMover_A2_b71 {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 state;  // 0xc8
};

void fn_710045a8b4(void*, UnkShardsSphereMover_A2_b71* self) {
    self->state = 1;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0xa4));
    self->id = g_unk_71726c9e00;
    self->flags |= 0x4000000;
}
