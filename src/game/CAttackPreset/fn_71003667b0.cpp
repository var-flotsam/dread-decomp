// CAttackPreset's unit (tools/tu_map.py unit 446): whether the preset's attack may start. It needs
// the preset's flag (+0x10) or the context's (+0x1a); with an AI component, a cooldown (+0x24) and
// the AI's id at +0x1780 empty, the attack must also have ended at least the cooldown ago in the
// context's list of attacks (each a CCharClassAttack). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkPresetAI_l3667b0 {
    u8 pad_0x0[0x1780];
    CStrId field_0x1780;
};

struct UnkPresetEntity_l3667b0 {
    u8 pad_0x0[0x240];
    UnkPresetAI_l3667b0* ai;  // 0x240
};

struct UnkPresetOwner_l3667b0 {
    u8 pad_0x0[0x10];
    UnkPresetEntity_l3667b0* entity;  // 0x10
};

struct UnkPresetAttackSlot_l3667b0 {
    u8 pad_0x0[0x20];
    UnkReflObject* attack;  // 0x20
};

struct UnkPresetRecord_l3667b0 {
    u8 pad_0x0[0x8];
    UnkPresetAttackSlot_l3667b0* slot;  // 0x8
    f32 time;                           // 0x10: when the attack was used
    u8 pad_0x14[0x4];
    UnkPresetRecord_l3667b0* next;  // 0x18
};

struct UnkPresetRecords_l3667b0 {
    u8 pad_0x0[0x10];
    UnkPresetRecord_l3667b0* head;  // 0x10
};

struct UnkPresetContext_l3667b0 {
    UnkPresetOwner_l3667b0* owner;
    u8 pad_0x8[0x12];
    bool field_0x1a;
    u8 pad_0x1b[0x5];
    UnkPresetRecords_l3667b0* records;  // 0x20
};

struct UnkAttackPreset_l3667b0 {
    u8 pad_0x0[0x10];
    bool bEnabled;
    u8 pad_0x11[0x13];
    f32 cooldown;  // 0x24
};

static inline UnkPresetAI_l3667b0* Ai(UnkPresetContext_l3667b0* ctx) {
    if (!ctx->owner || !ctx->owner->entity)
        return nullptr;
    return ctx->owner->entity->ai;
}

// The attack as a CCharClassAttack, or null when it is not one.
static inline UnkReflObject* AsCharClassAttack(UnkReflObject* attack) {
    if (!attack)
        return nullptr;
    return fn_710009d50c(attack->vf_0x0(), fn_7100362e04()) ? attack : nullptr;
}

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
bool fn_71003667b0(UnkAttackPreset_l3667b0* self, UnkPresetContext_l3667b0* ctx, UnkReflObject* attack) {
    bool enabled = ctx->field_0x1a || self->bEnabled;
    UnkPresetAI_l3667b0* ai = Ai(ctx);
    if (!enabled)
        return false;
    bool ready = true;
    // Only a cooldown the AI's id at +0x1780 leaves in force is checked.
    if (ai && self->cooldown > 0.0f && ai->field_0x1780.data == &g_unk_7101d47108->empty) {
        for (UnkPresetRecord_l3667b0* record = ctx->records->head; record; record = record->next) {
            if (AsCharClassAttack(record->slot->attack) == attack) {
                ready = !(g_unk_7101d47310->field_0x2444 - record->time < self->cooldown);
                break;
            }
        }
    }
    return ready;
}
