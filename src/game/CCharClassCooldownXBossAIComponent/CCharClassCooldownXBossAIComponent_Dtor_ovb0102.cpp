// CCharClassCooldownXBossAIComponent's unit (tools/tu_map.py unit 752): the destructor of the object
// fn_71004ce858 constructs (src/game/CCharClassCooldownXBossAIComponent/fn_71004ce858.cpp): the
// sub-object at +0xc78, the owned objects at +0xc40, the sub-objects from +0xb38 down to +0x528, then
// the base destructor. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkBossAIComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

extern void* g_unk_7101a3ebe8[];
struct UnkGoliathXBase;
void fn_710041a230(UnkGoliathXBase*);
void fn_71008c2de0(void* member);
void fn_71004a17c0(void* member);
void fn_71004b9ec0(void* member);
void fn_71004b4f18(void* member);
void fn_71004b42e8(void* member);
void fn_71004ae7c0(void* member);
void fn_71004a5060(void* member);

struct UnkCharClassCooldownXAI_ovb0102 : UnkBossAIComponent {
    u8 field_0x528[0x130];
    u8 field_0x658[0x190];
    u8 field_0x7e8[0x100];
    u8 field_0x8e8[0x108];
    u8 field_0x9f0[0x148];
    u8 field_0xb38[0x108];
    UnkOwnedList field_0xc40;
    u8 pad_0xc68[0x10];
    u8 field_0xc78[0x8];
};
static_assert(__builtin_offsetof(UnkCharClassCooldownXAI_ovb0102, field_0xc78) == 0xc78,
              "UnkCharClassCooldownXAI_ovb0102 layout");

void fn_71004ce62c(UnkCharClassCooldownXAI_ovb0102* self) {
    self->vtable = g_unk_7101a3ebe8 + 2;
    fn_71008c2de0(self->field_0xc78);
    self->field_0xc40.~UnkOwnedList();
    fn_71004a17c0(self->field_0xb38);
    fn_71004b9ec0(self->field_0x9f0);
    fn_71004b4f18(self->field_0x8e8);
    fn_71004b42e8(self->field_0x7e8);
    fn_71004ae7c0(self->field_0x658);
    fn_71004a5060(self->field_0x528);
    fn_710041a230(reinterpret_cast<UnkGoliathXBase*>(self));
}
