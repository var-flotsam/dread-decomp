// CChozoWarriorXSpitAttack's unit (tools/tu_map.py): an event handler. On a kind-1 event whose source
// carries a fixed hash it flags the attack, and while the count at +0x54 is positive sends the owner's
// (+0x8) sub-object at +0x220 the value 2 under the hash of the object the member at +0x40 returns
// (refl_EmitU32, base/reflection/UnkReflStatics.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkCwxHashed_b57 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkCwxSourceInner_b57 {
    u8 pad_0x0[0x38];
    UnkCwxHashed_b57* field_0x38;
};

struct UnkCwxSource_b57 {
    u8 pad_0x0[0x10];
    UnkCwxSourceInner_b57* field_0x10;
};

struct UnkCwxEvent_b57 {
    s32 kind;
    UnkCwxSource_b57* source;  // 0x8
};

struct UnkCwxOwner_b57;

struct UnkCwxProvider_b57 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual UnkCwxHashed_b57** vf_0x18(UnkCwxOwner_b57* owner);
};

struct UnkCwxOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkCwxAttack_b57 {
    u8 pad_0x0[0x8];
    UnkCwxOwner_b57* owner;  // 0x8
    u8 pad_0x10[0x30];
    UnkCwxProvider_b57* field_0x40;
    bool field_0x48;
    u32 field_0x4c;
    u8 pad_0x50[0x4];
    s32 field_0x54;
    u8 pad_0x58[0x1c];
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
};

bool fn_71004850c8(UnkCwxAttack_b57* self, const UnkCwxEvent_b57* event) {
    if (event->kind == 1 && event->source->field_0x10->field_0x38->hash == 0xe0d27fab456b7e6a) {
        self->field_0x48 = true;
        self->field_0x4c = 0;
        if (self->field_0x54 >= 1) {
            UnkCwxOwner_b57* owner = self->owner;
            void* target = owner->field_0x220;
            if (target) {
                base::reflection::refl_EmitU32(target, (*self->field_0x40->vf_0x18(owner))->hash, 0xf4061ff25a2163d6, 2,
                                               &g_unk_710164b958, &g_unk_7172769740);
                self->field_0x74 = 0;
                self->field_0x78 = 0;
                self->field_0x7c = 0;
            }
        }
    }
    return true;
}
