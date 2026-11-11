// CCharClassBigkranXAIComponent's unit (tools/tu_map.py): the Bigkran X AI component's constructor and
// destructor. Over the AI base (fn_71003e8db0, destroyed by fn_710041a230) it holds three distance ranges
// (+0x530, +0x578, +0x5c0: a list, a name and two radii each; fn_71003f62a8 picks one), the near and far
// radii (+0x608), a name (+0x610), two more pairs and its two attack definitions (+0x628, +0x730). Names
// and layouts are placeholders.
#include <new>
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassBaseBigFistAIComponent.h"
#include "game/CCharClassBossAIComponent.h"
#include "types.h"

using base::global::CStrId;

// The AI base's constructor and destructor (as CCharClassGoliathXAIComponent_Ctor_b55.cpp and
// CCharClassCooldownXBossAIComponent_Dtor_ovb0102.cpp declare them).
struct UnkGoliathXAI_b55;
struct UnkGoliathXBase;
extern void* g_unk_7101a31ca8[];

struct UnkBigkranXSpitAttackDef;
void fn_71003f0aac(UnkBigkranXSpitAttackDef* self);
void fn_71003f0bac(UnkBigkranXSpitAttackDef* self);
struct UnkBigkranXAttackDef_ovb0135;
void fn_71003ef808(UnkBigkranXAttackDef_ovb0135* self);
void fn_71003ef84c(UnkBigkranXAttackDef_ovb0135* self);

struct UnkBigkranRange_ovb0135 {
    base::UnkVector<void*> list;  // 0x0
    CStrId name;                  // 0x28
    f32 min;                      // 0x30
    f32 max;                      // 0x34
    u64 field_0x38;
    u64 field_0x40;

    UnkBigkranRange_ovb0135() : list(0xf), name(g_unk_7101d470e0), min(-1.0f), max(-1.0f), field_0x38(0), field_0x40(0) {}
    // What the destructor does (the members are destroyed in place).
    void Destroy() {
        name.Reset();
        list.Free();
    }
};

struct UnkBigkranXAI_ovb0135 {
    void** vtable;
    u8 pad_0x8[0x528];
    UnkBigkranRange_ovb0135 near_range;  // 0x530
    UnkBigkranRange_ovb0135 mid_range;   // 0x578
    UnkBigkranRange_ovb0135 far_range;   // 0x5c0
    f32 near;                            // 0x608
    f32 far;                             // 0x60c
    CStrId name;                         // 0x610
    f32 field_0x618;
    f32 field_0x61c;
    f32 field_0x620;
    f32 field_0x624;
    u8 attack[0x108];  // 0x628
    u8 spit[0x8];      // 0x730
};
static_assert(__builtin_offsetof(UnkBigkranXAI_ovb0135, spit) == 0x730, "layout");

void fn_71003f5f3c(UnkBigkranXAI_ovb0135* self) {
    fn_71003e8db0(reinterpret_cast<UnkGoliathXAI_b55*>(self));
    self->vtable = g_unk_7101a31ca8 + 2;
    new (&self->near_range) UnkBigkranRange_ovb0135();
    new (&self->mid_range) UnkBigkranRange_ovb0135();
    new (&self->far_range) UnkBigkranRange_ovb0135();
    self->near = -1.0f;
    self->far = -1.0f;
    new (&self->name) CStrId(g_unk_7101d470e0);
    self->field_0x618 = -1.0f;
    self->field_0x61c = -1.0f;
    self->field_0x620 = -1.0f;
    self->field_0x624 = -1.0f;
    fn_71003ef808(reinterpret_cast<UnkBigkranXAttackDef_ovb0135*>(self->attack));
    fn_71003f0aac(reinterpret_cast<UnkBigkranXSpitAttackDef*>(self->spit));
}

void fn_71003f60c8(UnkBigkranXAI_ovb0135* self) {
    self->vtable = g_unk_7101a31ca8 + 2;
    fn_71003f0bac(reinterpret_cast<UnkBigkranXSpitAttackDef*>(self->spit));
    fn_71003ef84c(reinterpret_cast<UnkBigkranXAttackDef_ovb0135*>(self->attack));
    self->name.Reset();
    self->far_range.Destroy();
    self->mid_range.Destroy();
    self->near_range.Destroy();
    fn_710041a230(reinterpret_cast<UnkGoliathXBase*>(self));
}
