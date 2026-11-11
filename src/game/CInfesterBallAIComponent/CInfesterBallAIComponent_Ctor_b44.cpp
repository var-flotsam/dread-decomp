// CInfesterBallAIComponent (the name from reflection, tools/tu_map.py): the constructor. The base
// AI component's constructor (fn_710036932c), the class's vtable, its fields from +0x146d8 cleared
// (the float at +0x14754 set to -1), then the class-entry pass of base::core::CBaseObject
// (fn_7100023854). The fields are addressed by offset, as in
// src/game/CCentralUnitAIComponent/fn_71004076e4.cpp. Names and layout are placeholders.
#include "base/core/CBaseObject.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "types.h"

extern void* g_unk_7101a524b8[];  // the component's vtable

struct UnkComponent;
struct UnkNode;

typedef u64 UnkU64Align4_b44a2 __attribute__((aligned(4)));

struct UnkInfesterBallAI_b44a2 {
    void** vtable;
    u8 pad_0x8[0x146d8 - 0x8];
    u64 field_0x146d8;
    u64 field_0x146e0;
    u8 pad_0x146e8[0x10];
    u64 field_0x146f8;
    u32 field_0x14700;
    u8 pad_0x14704[0x14];
    u64 field_0x14718;
    u32 pad_0x14720;
    UnkU64Align4_b44a2 field_0x14724;
    u32 pad_0x1472c;
    u64 field_0x14730;
    u32 pad_0x14738;
    UnkU64Align4_b44a2 field_0x1473c;
    UnkU64Align4_b44a2 field_0x14744;
    UnkU64Align4_b44a2 field_0x1474c;
    f32 field_0x14754;
    u8 pad_0x14758[0xc];
    UnkU64Align4_b44a2 field_0x14764;
    u32 field_0x1476c;
    u64 pad_0x14770;
    u64 field_0x14778;
};
static_assert(__builtin_offsetof(UnkInfesterBallAI_b44a2, field_0x14778) == 0x14778, "layout");

void fn_71005d2598(UnkInfesterBallAI_b44a2* self) {
    fn_710036932c(reinterpret_cast<UnkComponent*>(self));
    self->vtable = g_unk_7101a524b8 + 2;
    __builtin_memset(&self->field_0x146d8, 0, 0x10);
    self->field_0x146f8 = 0;
    self->field_0x14700 = 0;
    self->field_0x14718 = 0;
    self->field_0x14724 = 0;
    self->field_0x14730 = 0;
    __builtin_memset(&self->field_0x1473c, 0, 0x18);
    self->field_0x14764 = 0;
    self->field_0x1476c = 0;
    self->field_0x14778 = 0;
    self->field_0x14754 = -1.0f;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
