// CChozoWarriorXAIComponent unit: after the base reset, clears the component's flags and drops its two
// handles, clears the owner's model flag g_unk_7101649990 and switches off two of the owner's parts.
// Names and layouts are placeholders.
#include "game/CFXComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkSwitchObject.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkChozoXOwner_l48e254 {
    u8 pad_0x0[0x228];
    UnkFlagTarget* model;  // 0x228
    u8 pad_0x230[0x50];
    UnkWorld* parts;  // 0x280
};

struct UnkChozoXPart_l48e254 {
    u8 pad_0x0[0xb];
    u8 id;  // 0xb
};

struct UnkChozoWarriorXAI_l48e254 {
    u8 pad_0x0[0x30];
    UnkChozoXOwner_l48e254* owner;  // 0x30
    u8 pad_0x38[0x14999 - 0x38];
    bool field_0x14999;
    u8 pad_0x1499a[0x154e8 - 0x1499a];
    UnkWeakHandle* field_0x154e8;
    UnkWeakHandle* field_0x154f0;
    u8 pad_0x154f8[0x8];
    bool field_0x15500;
    bool field_0x15501;
    bool field_0x15502;
};
static_assert(__builtin_offsetof(UnkChozoWarriorXAI_l48e254, field_0x15500) == 0x15500, "layout");

struct UnkChozoWarriorAI_l498b30;

void fn_7100498b30(UnkChozoWarriorAI_l498b30*);

// The owner's model flag cleared on reset: read-only data.
extern char g_unk_7101649990[];

// Switches off the part with this name hash when the owner shows it.
inline void HideChozoXPart_l48e254(UnkWorld* parts, u64 hash) {
    auto* part = reinterpret_cast<UnkChozoXPart_l48e254*>(fn_7101152b90(parts, hash));
    if (part && fn_710114e668(reinterpret_cast<UnkObject*>(parts), part->id))
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), false);
}

void fn_710048e254(UnkChozoWarriorXAI_l48e254* self) {
    fn_7100498b30(reinterpret_cast<UnkChozoWarriorAI_l498b30*>(self));
    self->field_0x14999 = false;
    ResetRef(&self->field_0x154e8);
    ResetRef(&self->field_0x154f0);
    self->field_0x15500 = false;
    self->field_0x15501 = false;
    self->field_0x15502 = true;
    if (UnkFlagTarget* model = self->owner->model)
        fn_7101172780(model, g_unk_7101649990);
    if (UnkWorld* parts = self->owner->parts) {
        HideChozoXPart_l48e254(parts, 0xa0cda40259a7a253ull);
        HideChozoXPart_l48e254(parts, 0xa5a377e0a0458614ull);
    }
}
