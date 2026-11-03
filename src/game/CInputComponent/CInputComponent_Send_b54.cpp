// CInputComponent's unit (tools/tu_map.py): calls vf_0x1e8(0) on the owner's (+0x30) object at
// +0x200 when there is one, then sends the owner's sub-object at +0x220 the hash its +0x2b8 holds
// at +0x30, with mode 1, under UnkSpitSend's kind, descriptors and mask
// (src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkInputHashHolder_b54 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkInputTarget_b54 {
    u8 pad_0x0[0x2b8];
    UnkInputHashHolder_b54* field_0x2b8;
};

struct UnkInputObject_b54 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkInputOwner_b54 {
    u8 pad_0x0[0x200];
    void* field_0x200;
    u8 pad_0x208[0x18];
    UnkInputTarget_b54* field_0x220;
};

struct UnkInputComponent_b54 {
    u8 pad_0x0[0x30];
    UnkInputOwner_b54* owner;
};

// UnkSpitSend with a given mode.
static inline void SendMode_b54(void* target, u64 hash, u64 kind, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_7100872268(UnkInputComponent_b54* self) {
    if (void* p = self->owner->field_0x200)
        reinterpret_cast<void (*)(void*, u32)>((*reinterpret_cast<void***>(p))[0x1e8 / 8])(p, 0);
    if (UnkInputTarget_b54* target = self->owner->field_0x220)
        SendMode_b54(target, target->field_0x2b8->hash, 0xf4061ff25a2163d6, 1);
}
