// CQuetzoaXMultiTargetAttack's unit (tools/tu_map.py): whether the attack may start. The base check
// (fn_7100360b84) must pass with the owner's AI component a CQuetzoaXAIComponent that holds no target
// (its weak reference at +0x14a20), the owner's +0x298 part must not be in state 1 with its flag set,
// and no attack in the list before the first one with the default name (g_unk_71726fa880) may share
// this attack's name (fn_7100360890). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

// The attack's name id (its data pointer first).
const u64* fn_7100360890(CAttack* attack);
extern u64 g_unk_71726fa880 __attribute__((visibility("hidden")));  // the default name's data

struct UnkQuetzoaXAI_ovb0127 {
    u8 pad_0x0[0x14a20];
    UnkWeakHandle* target;  // 0x14a20
};

// What a weak reference points at, read through a counted copy of the reference.
static inline UnkWeakTarget* Lock(UnkWeakHandle* handle) {
    if (!handle)
        return nullptr;
    ++handle->ref_count;
    UnkWeakTarget* target = handle->target;
    if (--handle->ref_count == 0)
        handle->vf_0x10();
    return target;
}

struct UnkQuetzoaXStateRef_ovb0127 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkQuetzoaXStateHandle_ovb0127 {
    u8 pad_0x0[0x10];
    UnkQuetzoaXStateRef_ovb0127* target;  // 0x10
};

struct UnkQuetzoaXStatus_ovb0127 {
    UnkQuetzoaXStateHandle_ovb0127* handle;
    void* flag;
};

struct UnkQuetzoaXPart_ovb0127 {
    u8 pad_0x0[0xb0];
    UnkQuetzoaXStatus_ovb0127* status;  // 0xb0
};

struct UnkQuetzoaXAttackNode_ovb0127 {
    u8 pad_0x0[0x8];
    CAttack* attack;  // 0x8
    u8 pad_0x10[0x8];
    UnkQuetzoaXAttackNode_ovb0127* next;  // 0x18
};

struct UnkQuetzoaXAttackList_ovb0127 {
    u8 pad_0x0[0x10];
    UnkQuetzoaXAttackNode_ovb0127* head;  // 0x10
};

struct UnkQuetzoaXContext_ovb0127 {
    u8 pad_0x0[0x20];
    UnkQuetzoaXAttackList_ovb0127* attacks;  // 0x20
};

static inline bool SameName(const u64& a, const u64& b) {
    return a == b;
}

bool fn_710063ddcc(CAttack* self, UnkQuetzoaXContext_ovb0127* ctx, u32 mode) {
    bool ok = fn_7100360b84(self, ctx, mode);
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    if (!ai)
        return false;
    if (!(ok & fn_710009d50c(ai->vf_0x0(), fn_710063efe4())))
        return false;
    if (Lock(reinterpret_cast<UnkQuetzoaXAI_ovb0127*>(ai)->target))
        return false;
    if (auto* part = static_cast<UnkQuetzoaXPart_ovb0127*>(self->field_0x8->field_0x298)) {
        UnkQuetzoaXStatus_ovb0127* status = part->status;
        UnkQuetzoaXStateRef_ovb0127* ref = status->handle ? status->handle->target : nullptr;
        if (ref && ref->state == 1 && status->flag)
            return false;
    }
    for (UnkQuetzoaXAttackNode_ovb0127* node = ctx->attacks->head; node; node = node->next) {
        CAttack* attack = node->attack;
        if (SameName(*fn_7100360890(attack), *fn_7100360890(self)))
            return false;
        if (SameName(*fn_7100360890(attack), g_unk_71726fa880))
            break;
    }
    return true;
}
