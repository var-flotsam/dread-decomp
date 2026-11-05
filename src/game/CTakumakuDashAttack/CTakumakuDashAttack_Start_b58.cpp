// A CTakumakuDashAttack start. fn_71006cfcac checks the flag at +0x64, the sub-object at +0x30
// (fn_71006d44a0) and that the owner's character class (+0x220) is in a track of the class of
// fn_7100c51268, sends it the hash at +0x30 of the sub-object's entry (fn_71006d3f50) under the usual track
// kind with mode 1, and on success hands the owner's +0x298 a query (the block of
// src/game/CQuetzoaAIComponent/CQuetzoaAIComponent_b38.cpp) facing one of two constant directions by the sign
// of the owner's facing. Its sibling fn_71006cfab0 (once only, flag +0x75, mode 2) is not matched yet.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkParams.h"
#include "types.h"

extern void* g_unk_7101a24b58[];

struct UnkTakumakuDir_b58 {
    f32 x, y, z;
};
extern const UnkTakumakuDir_b58 g_unk_710164a7a0 __attribute__((visibility("hidden")));
extern const UnkTakumakuDir_b58 g_unk_710164a7ac __attribute__((visibility("hidden")));

struct UnkTakumakuQuery_b58 {
    void** vtable;
    UnkTakumakuDir_b58 field_0x8;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    UnkTakumakuQuery_b58() {
        vtable = g_unk_7101a24b58 + 2;
        __builtin_memset(&field_0x8, 0, 13);
        field_0x1c = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        field_0x34 = 0;
    }
};

struct UnkTakumakuEntry_b58 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkTakumakuStartAttack_b58 {
    void** vtable;
    UnkActor* actor;
    u8 pad_0x10[0x20];
    void* field_0x30;
    u8 pad_0x38[0x2c];
    bool field_0x64;
};

bool fn_71006d44a0(void*);
UnkTakumakuEntry_b58** fn_71006d3f50(void*);

bool fn_71006cfcac(UnkTakumakuStartAttack_b58* self) {
    UnkActor* actor = self->actor;
    UnkAnimationComponent* anim = actor ? static_cast<UnkAnimationComponent*>(actor->field_0x220) : nullptr;
    void* target = actor ? actor->field_0x298 : nullptr;
    if (!self->field_0x64)
        return false;
    bool ready = fn_71006d44a0(self->field_0x30);
    if (!anim || !ready)
        return false;
    if (!self->field_0x30 || !fn_710117b348(anim, fn_7100c51268()))
        return false;
    u64 hash = (*fn_71006d3f50(self->field_0x30))->field_0x30;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    bool sent = fn_7101176690(anim, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    if (!target || !sent)
        return false;
    UnkTakumakuQuery_b58 query;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
    query.field_0x8 = m.m[2] >= 0.0f ? g_unk_710164a7ac : g_unk_710164a7a0;
    query.field_0x14 = 1;
    fn_71006f4b0c(target, &query, 0);
    return true;
}
