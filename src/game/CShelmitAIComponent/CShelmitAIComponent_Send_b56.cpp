// The CShelmitAIComponent unit: sends 0x9f65ed8ea84722af with mode 1 to the owner's sub-object at
// +0x220 when there is one (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h), then gives the
// owner's object at +0x1b8 its value at +0x54 capped by the global tunable's +0x1ec (vf_0x170)
// and its value at +0x50 (vf_0x168). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CCharClassLifeComponent.h"
#include "types.h"

// Declared with this type in src/game/CCharClassCentralUnitAIComponent/fn_7100408770.cpp.
struct UnkGroupIds;

struct UnkShelmitObject_b56a3 {
    virtual void vf_0x0();
    u8 pad_0x8[0x48];
    f32 field_0x50;
    f32 field_0x54;

    void vf_0x168(f32 value, bool flag) {
        reinterpret_cast<void (*)(UnkShelmitObject_b56a3*, f32, bool)>((*reinterpret_cast<void***>(this))[0x168 / 8])(this, value, flag);
    }
    void vf_0x170(f32 value) {
        reinterpret_cast<void (*)(UnkShelmitObject_b56a3*, f32)>((*reinterpret_cast<void***>(this))[0x170 / 8])(this, value);
    }
};

struct UnkShelmitOwner_b56a3 {
    u8 pad_0x0[0x1b8];
    UnkShelmitObject_b56a3* field_0x1b8;
    u8 pad_0x1c0[0x60];
    void* field_0x220;
};

struct UnkShelmitAI_b56a3 {
    u8 pad_0x0[0x30];
    UnkShelmitOwner_b56a3* owner;
};

void fn_71006bc004(UnkShelmitAI_b56a3* self) {
    if (void* target = self->owner->field_0x220) {
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 1;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x9f65ed8ea84722af, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    }
    if (UnkShelmitObject_b56a3* obj = self->owner->field_0x1b8) {
        f32 v = obj->field_0x54;
        f32 cap = *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_717272fd38) + 0x1ec);
        obj->vf_0x170(v < cap ? v : cap);
        obj->vf_0x168(obj->field_0x50, true);
    }
}
