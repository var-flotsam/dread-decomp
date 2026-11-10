// Unit 740 (hint CCooldownXBossLavaCarpetDef): the lava carpet definition's destructor. Its vtable is
// restored, the array of owned-object lists at +0x100 is freed (game_s2/UnkOwnedList.h), then the
// CCharClassCooldownXBossAttack base's destructor (fn_71004a0a10) runs. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

struct UnkCooldownXBossAttackDef;
void fn_71004a0a10(UnkCooldownXBossAttackDef* self);
extern void* g_unk_7101a3dfc0[];

struct UnkLavaCarpetDef_b0225 {
    void** vtable;
    u8 pad_0x8[0xf8];
    base::UnkVector<UnkOwnedList> lists;  // 0x100
};

void fn_71004a5060(UnkLavaCarpetDef_b0225* self) {
    self->vtable = g_unk_7101a3dfc0 + 2;
    self->lists.Free();
    fn_71004a0a10(reinterpret_cast<UnkCooldownXBossAttackDef*>(self));
}
