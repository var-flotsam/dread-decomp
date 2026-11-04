// CPowerGeneratorComponent unit (tools/tu_map.py): two functions that send the owner's (+0x30)
// sub-object at +0x220 a fixed hash (as UnkSpitSend, src/game_s2/UnkSpitSend.h, with the mode as a
// parameter). fn_7100a7e994 sends mode 2 when the flag at +0x138 is set; fn_7100a7ef9c stores a value at
// +0x7c, calls virtual slot 17 unless the flag at +0x78 is set, then sends mode 0. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkPowerGeneratorOwner_b56 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkPowerGenerator_b56 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    u8 pad_0x8[0x28];
    UnkPowerGeneratorOwner_b56* owner;
    u8 pad_0x38[0x40];
    bool field_0x78;
    f32 field_0x7c;
    u8 pad_0x80[0xb8];
    bool field_0x138;
};

static inline __attribute__((always_inline)) void Send_b56pg(void* target, u64 hash, u32 modeId) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = modeId;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_7100a7e994(UnkPowerGenerator_b56* self) {
    void* target = self->owner->field_0x220;
    if (target && self->field_0x138)
        Send_b56pg(target, 0xe01dce813b23272a, 2);
}

void fn_7100a7ef9c(UnkPowerGenerator_b56* self, f32 value) {
    self->field_0x7c = value;
    if (!self->field_0x78)
        self->vf_17();
    void* target = self->owner->field_0x220;
    if (target)
        Send_b56pg(target, 0x004a25761dbe21b8, 0);
}
