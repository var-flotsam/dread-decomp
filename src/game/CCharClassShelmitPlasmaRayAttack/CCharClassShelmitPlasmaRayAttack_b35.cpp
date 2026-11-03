// Two identical constructors in the CCharClassShelmitPlasmaRayAttack unit: the attack base constructor
// fn_71003641fc, the vtable, six floats of -1 at +0xec, the flag at +0x54 set. Names and layouts are
// placeholders.
#include "types.h"

struct UnkStruct_71003641fc;
void fn_71003641fc(UnkStruct_71003641fc* self);
extern void* g_unk_7101a63730[];

struct UnkShelmitPlasmaRayAttackDef {
    void** vtable;
    u8 pad_0x8[0x4c];
    bool field_0x54;
    u8 pad_0x55[0x97];
    f32 field_0xec[6];
};

static inline __attribute__((always_inline)) void Construct(UnkShelmitPlasmaRayAttackDef* self) {
    fn_71003641fc(reinterpret_cast<UnkStruct_71003641fc*>(self));
    self->vtable = g_unk_7101a63730 + 2;
    for (int i = 0; i < 6; i++)
        self->field_0xec[i] = -1.0f;
    self->field_0x54 = true;
}

// Declared as src/game/CCharClassShelmitAIComponent/CCharClassShelmitAIComponent.cpp declares it.
void fn_71006b2d74(void* self) { Construct(static_cast<UnkShelmitPlasmaRayAttackDef*>(self)); }
void fn_71006b2e34(UnkShelmitPlasmaRayAttackDef* self) { Construct(self); }
