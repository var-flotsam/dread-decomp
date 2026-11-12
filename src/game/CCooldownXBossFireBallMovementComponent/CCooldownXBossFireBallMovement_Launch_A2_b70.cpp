// CCooldownXBossFireBallMovementComponent's unit (tools/tu_map.py): with the movement component as second
// argument, sets its state (+0xc8) to 0, gives its damage request the tunable's value (+0x194 of
// g_unk_7172716160, fn_71008c1538), copies the unit's static string id into +0xb8 and sets bit 26 of the
// flags at +0xb4 (as CSentenceSphereMovement_Launch_A2_b70.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;
extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726d8c78 __attribute__((visibility("hidden")));

struct UnkFireBallMover_A2_b70 {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 state;  // 0xc8
};

void fn_71004d78dc(void*, UnkFireBallMover_A2_b70* self) {
    self->state = 0;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x194));
    self->id = g_unk_71726d8c78;
    self->flags |= 0x4000000;
}
