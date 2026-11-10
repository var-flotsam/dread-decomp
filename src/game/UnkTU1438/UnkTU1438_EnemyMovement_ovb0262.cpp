// Unit 1438 (CCharClassEnemyMovement): the movement's constructor and destructor. The walking animation
// ids (a CEnemyMovementWalkingAnims at +0x80, as src/game/CEnemyMovementWalkingAnims/
// CEnemyMovementWalkingAnims_Ctor_A5_b68.cpp builds it) and the id record at +0xc0 sit between the base
// part and the movement's own settings and ids. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a84fc0[];
extern void* g_unk_7101a38c40[];

struct UnkMovementWalkingAnims_ovb0262l {
    void** vtable;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
};

struct UnkMovementIdRecord_ovb0262l;

struct UnkEnemyMovement_ovb0262l {
    void** vtable;  // 0x0
    u8 pad_0x8[0x61];
    bool field_0x69;
    u8 pad_0x6a[2];
    f32 field_0x6c;
    f32 field_0x70;
    u8 pad_0x74[4];
    CStrId relax;  // 0x78
    UnkMovementWalkingAnims_ovb0262l anims;  // 0x80
    u8 record[0x68];  // 0xc0
    f32 field_0x128;
    f32 field_0x12c;
    f32 field_0x130;
    u8 pad_0x134[8];
    f32 field_0x13c;
    f32 field_0x140;
    u8 pad_0x144[4];
    CStrId relax_0x148;
    CStrId walk_0x150;
    CStrId walkinit_0x158;
    u32 field_0x160;
    u32 field_0x164;
    bool field_0x168;
    u8 pad_0x169[3];
    f32 field_0x16c;
    f32 field_0x170;
    f32 field_0x174;
    f32 field_0x178;
    f32 field_0x17c;
    f32 field_0x180;
    u8 pad_0x184[4];
    CStrId walkup;  // 0x188
    CStrId walkdown;  // 0x190
    f32 field_0x198;
    f32 field_0x19c;
    f32 field_0x1a0;
    f32 field_0x1a4;
    bool field_0x1a8;
    bool field_0x1a9;
    bool field_0x1aa;
    u8 pad_0x1ab[5];
    u16 field_0x1b0;
    u8 pad_0x1b2[10];
    u32 field_0x1bc;
    u16 field_0x1c0;
    u8 pad_0x1c2[6];
    u16 field_0x1c8;
};
static_assert(__builtin_offsetof(UnkEnemyMovement_ovb0262l, field_0x1c8) == 0x1c8, "layout");

void fn_710078e668(void* self);
void fn_710078e6ac(void* self);
void fn_710096485c(void* record);
void fn_7100964b64(void* record);
void fn_710046e2e0(void* anims);

struct UnkObj_710052d200;

void fn_7100964670(UnkObj_710052d200* self_) {
    UnkEnemyMovement_ovb0262l* self = reinterpret_cast<UnkEnemyMovement_ovb0262l*>(self_);
    fn_710078e668(self);
    self->field_0x69 = true;
    self->vtable = &g_unk_7101a84fc0[2];
    self->field_0x6c = 10.0f;
    self->field_0x70 = 0.0f;
    new (&self->relax) CStrId("relax", true);
    self->anims.vtable = &g_unk_7101a38c40[2];
    new (&self->anims.field_0x8) CStrId("relax", true);
    new (&self->anims.field_0x10) CStrId("walk", true);
    new (&self->anims.field_0x18) CStrId("back", true);
    new (&self->anims.field_0x20) CStrId("walkinit", true);
    new (&self->anims.field_0x28) CStrId("backinit", true);
    new (&self->anims.field_0x30) CStrId("walkend", true);
    new (&self->anims.field_0x38) CStrId("backend", true);
    fn_710096485c(self->record);
    self->field_0x128 = -1.0f;
    self->field_0x12c = 0.5f;
    self->field_0x130 = 0.5f;
    self->field_0x13c = 0.0f;
    self->field_0x140 = 0.0f;
    new (&self->relax_0x148) CStrId("relax", true);
    new (&self->walk_0x150) CStrId("walk", true);
    new (&self->walkinit_0x158) CStrId("walkinit", true);
    self->field_0x160 = 2;
    self->field_0x164 = 3;
    self->field_0x168 = false;
    self->field_0x16c = 0.0f;
    self->field_0x170 = 100.0f;
    self->field_0x174 = 0.52359879f;
    self->field_0x178 = 0.0f;
    self->field_0x17c = 0.52359879f;
    self->field_0x180 = 0.0f;
    new (&self->walkup) CStrId("walkup", true);
    new (&self->walkdown) CStrId("walkdown", true);
    self->field_0x198 = -1.0f;
    self->field_0x19c = -1.0f;
    self->field_0x1a0 = 50.0f;
    self->field_0x1a4 = 75.0f;
    self->field_0x1a8 = true;
    self->field_0x1a9 = true;
    self->field_0x1aa = true;
    self->field_0x1b0 = 0;
    self->field_0x1bc = 1;
    self->field_0x1c0 = 0x100;
    self->field_0x1c8 = 0;
}

void fn_7100964974(UnkEnemyMovement_ovb0262l* self) {
    self->vtable = &g_unk_7101a84fc0[2];
    self->walkdown.Reset();
    self->walkup.Reset();
    self->walkinit_0x158.Reset();
    self->walk_0x150.Reset();
    self->relax_0x148.Reset();
    fn_7100964b64(self->record);
    fn_710046e2e0(&self->anims);
    self->relax.Reset();
    fn_710078e6ac(self);
}
