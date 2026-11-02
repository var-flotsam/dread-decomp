// Two identical constructors in the CChozoRobotSoldierCannonShotAttack unit (fn_7100464980,
// fn_7100467890): CAttack's constructor fn_710036069c, two angles at +0x30 (-pi), a scale at +0x38
// (1.0), the vtable, then cleared fields up to +0x70. Names and layouts are placeholders.
#include "types.h"

void fn_710036069c(void* attack);  // CAttack's constructor
extern void* g_unk_7101a383a0[];

struct UnkCannonShotAttack {
    void** vtable;
    u8 base_0x8[0x28];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    u8 field_0x3c[0x11];
    u8 pad_0x4d[3];
    u8 field_0x50[0xd];
    u8 pad_0x5d[2];
    u8 field_0x5f;
    void* field_0x60;
    u8 field_0x68;
    u8 pad_0x69[3];
    u32 field_0x6c;
};

__attribute__((always_inline)) static inline void ConstructCannonShotAttack(UnkCannonShotAttack* self) {
    fn_710036069c(self);
    self->vtable = g_unk_7101a383a0 + 2;
    self->field_0x30 = -3.14159274f;
    self->field_0x34 = -3.14159274f;
    self->field_0x38 = 1.0f;
    self->field_0x5f = 0;
    self->field_0x60 = nullptr;
    self->field_0x68 = 0;
    self->field_0x6c = 0;
    __builtin_memset(self->field_0x3c, 0, sizeof(self->field_0x3c));
    __builtin_memset(self->field_0x50, 0, sizeof(self->field_0x50));
}

void fn_7100464980(UnkCannonShotAttack* self) {
    ConstructCannonShotAttack(self);
}

void fn_7100467890(UnkCannonShotAttack* self) {
    ConstructCannonShotAttack(self);
}
