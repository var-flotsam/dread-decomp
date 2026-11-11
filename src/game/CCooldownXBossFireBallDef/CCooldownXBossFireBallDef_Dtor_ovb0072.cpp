// CCooldownXBossFireBallDef's unit (tools/tu_map.py): the destructor of the fire ball attack
// definition (vtable unk_7101a3e300): the array of owned-object lists at +0x120
// (game_s2/UnkOwnedList.h) is freed and the string ids at +0x110, +0x108 and +0x100 are released, then
// the CCharClassCooldownXBossAttack base's destructor (fn_71004a0a10) runs. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassCooldownXBossAttack.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;

struct UnkCooldownXBossAttackDef;
extern char g_unk_7101a3e300[];

struct UnkFireBallAttackDef_ovb0072 {
    char* vtable;
    u8 pad_0x8[0xf8];
    CStrId field_0x100;
    CStrId field_0x108;
    CStrId field_0x110;
    u8 pad_0x118[0x8];
    base::UnkVector<UnkOwnedList> field_0x120;
};
static_assert(sizeof(UnkFireBallAttackDef_ovb0072) == 0x148, "UnkFireBallAttackDef_ovb0072 layout");

void fn_71004b9ec0(UnkFireBallAttackDef_ovb0072* self) {
    self->vtable = g_unk_7101a3e300 + 0x10;
    self->field_0x120.Free();
    self->field_0x110.Reset();
    self->field_0x108.Reset();
    self->field_0x100.Reset();
    fn_71004a0a10(reinterpret_cast<UnkCooldownXBossAttackDef*>(self));
}
