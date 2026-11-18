// The CBehaviorTreeAIComponent unit. fn_71003697f0 runs once (flag at +0x146b0): when the owner's (+0x30)
// object at +0x240 names an id, sends that id to the owner's sub-object at +0x220 with mode 1; otherwise
// sends the id table's slot 0xc90 and, if that is not taken, slot 0x1d78. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkBehaviorTreeNamer_l3697f0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual CStrId vf_0x1d0();
};

struct UnkBehaviorTreeOwner_l3697f0 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    UnkBehaviorTreeNamer_l3697f0* field_0x240;
};

struct UnkBehaviorTreeAI_l3697f0 {
    u8 pad_0x0[0x30];
    UnkBehaviorTreeOwner_l3697f0* owner;
    u8 pad_0x38[0x146b0 - 0x38];
    bool sent;
};

// The header's UnkStruct_710036974c is defined in fn_710036974c.cpp without these members yet.
void fn_71003697f0(UnkStruct_710036974c* component) {
    UnkBehaviorTreeAI_l3697f0* self = reinterpret_cast<UnkBehaviorTreeAI_l3697f0*>(component);
    if (self->sent)
        return;
    void* target = self->owner->field_0x220;
    if (self->owner->field_0x240 && !(self->owner->field_0x240->vf_0x1d0() == base::global::CStrId::Empty())) {
        CStrId name = self->owner->field_0x240->vf_0x1d0();
        u64 hash = name.data->Key();
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 1;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        goto done;
    }
    // The goto above skips these declarations, so they get no lifetime markers and keep slots of their own.
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
    u64 hash;
    hash = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0xc90 / 8].data->Key();
    mode1.id = 1;
    mask1.id = UnkAll_SpitSend().value;
    if (fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740, &mask1))
        goto done;
    hash = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x1d78 / 8].data->Key();
    mode2.id = 1;
    mask2.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740, &mask2);
done:
    self->sent = true;
}
