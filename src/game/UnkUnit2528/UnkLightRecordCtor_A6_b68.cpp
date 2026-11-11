// Unit 2528 (no name hint, between the light definitions' units): two identical constructors of a 0x60-byte
// record: a word 1, string ids "Light" and "", cleared values and two floats of 1 (+0x3c and +0x58).
// Names and the layout are placeholders.
#include "game/UnkTU0.h"
#include "types.h"

struct UnkLightRecord_A6_b68 {
    u64 field_0x0;
    u64 id_0x8;
    u64 id_0x10;
    u64 field_0x18[4];
    f32 field_0x38;
    f32 field_0x3c;
    u64 field_0x40;
    u64 field_0x48[2];
    f32 field_0x58;
    u32 field_0x5c;
};

static inline __attribute__((always_inline)) void LightRecordCtor_A6_b68(UnkLightRecord_A6_b68* self) {
    self->field_0x0 = 1;
    fn_71000003d4(&self->id_0x8, "Light", true);
    fn_71000003d4(&self->id_0x10, "", true);
    self->field_0x18[0] = 0;
    self->field_0x18[1] = 0;
    self->field_0x18[2] = 0;
    self->field_0x18[3] = 0;
    self->field_0x38 = 0.0f;
    self->field_0x3c = 1.0f;
    self->field_0x40 = 0;
    self->field_0x48[0] = 0;
    self->field_0x48[1] = 0;
    self->field_0x58 = 1.0f;
    self->field_0x5c = 0;
}

void fn_7100d9b128(UnkLightRecord_A6_b68* self) { LightRecordCtor_A6_b68(self); }

void fn_7100d9bdd4(UnkLightRecord_A6_b68* self) { LightRecordCtor_A6_b68(self); }
