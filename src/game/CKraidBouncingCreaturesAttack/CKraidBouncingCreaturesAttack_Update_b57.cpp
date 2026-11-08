// The CKraidBouncingCreaturesAttack unit's update: after fn_71005f201c, the timer at +0x40 counts down;
// when it runs out it is cleared and, when the slot (+0x3c) of the list (+0x38) in the pattern set at
// +0x30 (lists of 0x28 bytes at its +0xf8; fn_71005e2c08 grows one to the slot) holds an entry, the
// owner's (+0x8) sub-object at +0x220 is sent the hash of the id the pattern set's slot 3 gives for the
// owner, under the usual track kind with mode 2 (UnkSpitSend's descriptors, src/game_s2/UnkSpitSend.h).
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkKraidBouncingList_b57a4 {
    u8 data[0x28];
};

struct UnkKraidBouncingOwner_b57a4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkKraidBouncingSet_b57a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CStrId* vf_0x18(UnkKraidBouncingOwner_b57a4* owner);
    u8 pad_0x8[0xf0];
    UnkKraidBouncingList_b57a4* field_0xf8;
};

struct UnkKraidBouncingAttack_b57a4 {
    u8 pad_0x0[0x8];
    UnkKraidBouncingOwner_b57a4* owner;
    u8 pad_0x10[0x20];
    UnkKraidBouncingSet_b57a4* field_0x30;
    u32 field_0x38;
    u32 field_0x3c;
    f32 field_0x40;
};

static inline __attribute__((always_inline)) void Send_b57a4(void* target, u64 hash, u64 kind, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

// The base attack update, typed as an opaque attack (one declaration for every caller).
struct UnkKraidAttack;
void fn_71005f201c(UnkKraidAttack* self, f32 dt);
void** fn_71005e2c08(UnkKraidBouncingList_b57a4* list, u32 index);

void fn_71005f2f5c(UnkKraidBouncingAttack_b57a4* self, f32 dt) {
    fn_71005f201c(reinterpret_cast<UnkKraidAttack*>(self), dt);
    f32 t = self->field_0x40;
    if (!(t > 0.0f))
        return;
    t -= dt;
    self->field_0x40 = t;
    if (!(t <= 0.0f))
        return;
    self->field_0x40 = 0.0f;
    UnkKraidBouncingList_b57a4* lists = self->field_0x30->field_0xf8;
    u32 slot = self->field_0x3c;
    u32 index = self->field_0x38;
    if (!*fn_71005e2c08(&lists[index], slot))
        return;
    const CStrId* id = self->field_0x30->vf_0x18(self->owner);
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id->data) + 0x30);
    Send_b57a4(self->owner->field_0x220, hash, 0xf4061ff25a2163d6, 2);
}
