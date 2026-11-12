// The CKraidAIComponent unit. fn_71005fe760 clears the two words at +0x149a0 and sends the hash of the id
// g_unk_71726f3788 to the owner's sub-object (+0x220 of the owner at +0x30) under kind
// 0x9317bfcf28edee2b with mode 1 (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CKraidAIComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkKraidOwner_b56a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkKraidAI_b56a3 {
    u8 pad_0x0[0x30];
    UnkKraidOwner_b56a3* owner;
    u8 pad_0x38[0x149a0 - 0x38];
    u64 field_0x149a0;
    u64 field_0x149a8;
};

void fn_71005fe760(UnkKraidAI_b56a3* self) {
    self->field_0x149a0 = 0;
    self->field_0x149a8 = 0;
    void* target = self->owner->field_0x220;
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(g_unk_71726f3788.data) + 0x30);
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0x9317bfcf28edee2b, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

// The same send of the id g_unk_71726f3778 while the timer at +0x149a4 has run out; then the scale
// at +0x149a0 is one, the timer restarts from the value at +0x528 of the object at +0x14a30 and
// the word at +0x149a8 is cleared.

struct UnkKraidTimerSource_b56a3 {
    u8 pad_0x0[0x528];
    f32 field_0x528;
};

struct UnkKraidAITimer_b56a3 {
    u8 pad_0x0[0x30];
    UnkKraidOwner_b56a3* owner;
    u8 pad_0x38[0x149a0 - 0x38];
    f32 field_0x149a0;
    f32 field_0x149a4;
    u64 field_0x149a8;
    u8 pad_0x149b0[0x14a30 - 0x149b0];
    UnkKraidTimerSource_b56a3* field_0x14a30;
};

void fn_71005fe88c(UnkKraidAITimer_b56a3* self) {
    if (self->field_0x149a4 <= 0.0f) {
        void* target = self->owner->field_0x220;
        u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(g_unk_71726f3778.data) + 0x30);
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 1;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0x9317bfcf28edee2b, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    }
    f32 t = self->field_0x14a30->field_0x528;
    self->field_0x149a0 = 1.0f;
    self->field_0x149a4 = t;
    self->field_0x149a8 = 0;
}
