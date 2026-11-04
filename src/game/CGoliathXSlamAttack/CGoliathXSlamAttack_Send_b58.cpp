// The CGoliathXSlamAttack unit's update: after the base update (fn_710058e67c) with the flag at +0x78
// raised, when the owner (+0x8) has its animation (+0x220), reads a value of the owner's AI component
// (+0x240, cast to CGoliathAIComponent) with fn_71010fc68c. Above the AI's +0x14a28 plus 10 it sends,
// with mode 1, the id the object at +0x70 gives for the owner (its slot 0x18); otherwise it sends the id
// fn_71003627f8 builds and drops the flag. Descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h).
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::global::CStrId;

struct UnkGoliathAI_b58a3 {
    void** vtable;
    u8 pad_0x8[0x14a28 - 0x8];
    f32 field_0x14a28;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGoliathAI_b58a3*)>(vtable[0])(this);
    }
};

void fn_71010fc68c(UnkGoliathAI_b58a3* ai, f32* out);

struct UnkGoliathIdSource_b58a3 {
    void** vtable;
    const CStrId* vf_0x18(UnkActor* owner) {
        return reinterpret_cast<const CStrId* (*)(UnkGoliathIdSource_b58a3*, UnkActor*)>(vtable[3])(this, owner);
    }
};

struct UnkGoliathXSlamAttack_b58a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x60];
    UnkGoliathIdSource_b58a3* field_0x70;
    bool field_0x78;
};

void fn_710058e67c(UnkGoliathXSlamAttack_b58a3* self);
CStrId fn_71003627f8(UnkGoliathXSlamAttack_b58a3* self, UnkActor* owner);

static inline u64 Hash_b58a3(const CStrId& id) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id.data) + 0x30);
}

void fn_7100586a30(UnkGoliathXSlamAttack_b58a3* self) {
    fn_710058e67c(self);
    self->field_0x78 = true;
    auto* comp = static_cast<UnkGoliathAI_b58a3*>(self->owner->field_0x240);
    UnkGoliathAI_b58a3* ai = comp && fn_710009d50c(comp->vf_0x0(), fn_71005908ac()) ? comp : nullptr;
    void* target = self->owner->field_0x220;
    if (!target)
        return;
    f32 value = 0.0f;
    fn_71010fc68c(ai, &value);
    // The goto skips the declarations of `mode`, `mode2` and `mask2`, so they get no lifetime
    // markers: the original gives each send its own slots (none shared), the 4-byte mode on top.
    if (!(value <= ai->field_0x14a28 + 10.0f))
        goto send_given;
    u32 mode;
    {
        CStrId id = fn_71003627f8(self, self->owner);
        base::reflection::UnkReflTypeIdArg mask;
        u64 hash = Hash_b58a3(id);
        mode = 1;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, reinterpret_cast<base::reflection::UnkReflTypeIdArg*>(&mode),
                      &g_unk_710164b958, &g_unk_7172769740, &mask);
    }
    self->field_0x78 = false;
    return;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
send_given:
    {
        u64 hash2 = Hash_b58a3(*self->field_0x70->vf_0x18(self->owner));
        mode2.id = 1;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash2, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740, &mask2);
    }
}
