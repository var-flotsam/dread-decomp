// CCooldownXBossFireBallDef's unit (tools/tu_map.py): the fire ball attack definition's constructor:
// the CCharClassCooldownXBossAttack base, the vtable, three empty string ids and an empty array of
// owned-object lists. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassCooldownXBossAttack.h"
#include "game/CCooldownXBossFireBallDef.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkCooldownXBossAttackDef;

struct UnkFireBallAttackDef_l4b9cc4 {
    char* vtable;
    u8 pad_0x8[0xf8];
    UnkStringData* ids[3];  // 0x100
    u8 pad_0x118[0x8];
    base::global::CRntVector<UnkOwnedList> lists;  // 0x120
};
static_assert(sizeof(UnkFireBallAttackDef_l4b9cc4) == 0x148, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71004b9cc4(UnkFireBallAttackDef_l4b9cc4* self) {
    fn_71004a09b4(reinterpret_cast<UnkCooldownXBossAttackDef*>(self));
    self->vtable = g_unk_7101a3e300 + 0x10;
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    new (&self->lists) base::global::CRntVector<UnkOwnedList>(0xf);
}
