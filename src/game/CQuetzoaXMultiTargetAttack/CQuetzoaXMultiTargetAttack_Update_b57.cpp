// The CQuetzoaXMultiTargetAttack unit's update: after the base update, while the flag at +0x30 is set
// the timer at +0x34 runs up to the value at +0xf0 of the object at +0x28; each time it gets there
// fn_710063d920 runs, the timer restarts and the count at +0x38 drops; at zero the flag clears and the
// owner's (+0x8) sub-object at +0x220 is sent hash 0xb60395f638ac5184 under the usual track kind with
// mode 1 (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkSpitAttack;
void fn_71003609a8(UnkSpitAttack*);

struct UnkQuetzoaXMultiOwner_b57a4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkQuetzoaXMultiData_b57a4 {
    u8 pad_0x0[0xf0];
    f32 field_0xf0;
};

struct UnkQuetzoaXMultiAttack_b57a4 {
    u8 pad_0x0[0x8];
    UnkQuetzoaXMultiOwner_b57a4* owner;
    u8 pad_0x10[0x18];
    UnkQuetzoaXMultiData_b57a4* field_0x28;
    bool field_0x30;
    f32 field_0x34;
    s32 field_0x38;
};

void fn_710063d920(UnkQuetzoaXMultiAttack_b57a4* self);

void fn_710063d7c0(UnkQuetzoaXMultiAttack_b57a4* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self));
    if (!self->field_0x30)
        return;
    f32 t = self->field_0x34 + dt;
    self->field_0x34 = t;
    if (t >= self->field_0x28->field_0xf0) {
        fn_710063d920(self);
        self->field_0x34 = 0.0f;
        if (--self->field_0x38 == 0) {
            self->field_0x30 = false;
            if (void* target = self->owner->field_0x220) {
                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 1;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(target, 0xb60395f638ac5184, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                              &g_unk_7172769740, &mask);
            }
        }
    }
}
