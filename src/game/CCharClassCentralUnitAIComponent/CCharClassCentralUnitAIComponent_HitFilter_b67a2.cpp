// CCharClassCentralUnitAIComponent unit (tools/tu_map.py, the name from reflection): a hit result. 3 unless the
// hit info's bit 15 of the word at +0xb4 is set and the actor's AI component (+0x240) is a CCentralUnitAIComponent
// (fn_71004030f4); then 2 while that component's byte at +0x146f8 is clear, else 1. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkCentralUnitAI_b67a2 {
    void** vtable;
    u8 pad_0x8[0x146f8 - 0x8];
    bool field_0x146f8;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkCentralUnitAI_b67a2*)>(vtable[0])(this);
    }
};

struct UnkCentralUnitActor_b67a2 {
    u8 pad_0x0[0x240];
    UnkCentralUnitAI_b67a2* ai;
};

struct UnkCentralUnitHitInfo_b67a2 {
    u8 pad_0x0[0xb4];
    u32 low : 15;
    u32 flag : 1;  // bit 7 of the byte at +0xb5
    u32 high : 16;
};

u32 fn_7100408824(void* self, UnkCentralUnitActor_b67a2* actor, void* arg2, UnkCentralUnitHitInfo_b67a2* info) {
    if (info->flag) {
        if (UnkCentralUnitAI_b67a2* ai = actor->ai) {
            if (fn_710009d50c(ai->vf_0x0(), fn_71004030f4()))
                return ai->field_0x146f8 ? 1 : 2;
        }
    }
    return 3;
}
