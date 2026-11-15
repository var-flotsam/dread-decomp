// CMovementComponent unit: the movement component's constructor. Runs the component base constructor, sets the
// vtable, zeroes the state, builds the record at +0x9c, then sets the unit basis at +0x40 and the defaults
// (+0x108 a distance of 2200.0, +0xf8 flags) and calls its vtable slot 0x44. Names and
// layouts are placeholders.
#include "base/UnkInitRecord_0x5c.h"
#include "game/CComponent.h"
#include "types.h"

extern char g_unk_7101cb09f8[];

struct UnkMovementCtor_l0d0754;


struct UnkMovementCtorVt_l0d0754 {
    u8 pad_0x20[0x200];
    void (*slot_0x44)(UnkMovementCtor_l0d0754*);  // 0x220
};

#pragma pack(push, 1)
struct UnkZeroVec2_l0d0754 {
    f32 x = 0, y = 0;
};

struct UnkMovementCtor_l0d0754 {
    UnkMovementCtorVt_l0d0754* vt;  // 0x0
    u8 pad_0x8[0x38];
    f32 basis[14];  // 0x40
    u32 field_0x78;
    u32 field_0x7c[2];
    u32 field_0x84[6];
    u8 record[0x5c];  // 0x9c
    u8 flag_0xf8;
    u8 flag_0xf9;
    u8 flag_0xfa;
    u8 pad_0xfb;
    u32 field_0xfc[2];
    u8 field_0x104;
    u8 pad_0x105[3];
    f32 distance;  // 0x108
    u32 field_0x10c[3];
    u32 field_0x118[8];
    u32 field_0x138;
    UnkZeroVec2_l0d0754 field_0x13c[6];  // 0x13c
    u8 pad_0x16c[4];
    u16 field_0x170;
    u8 pad_0x172[2];
    u32 field_0x174[4];
    u32 field_0x184;
    u32 field_0x188;
    UnkZeroVec2_l0d0754 field_0x18c[6];  // 0x18c
    u8 pad_0x1bc[4];
    u16 field_0x1c0;
    u8 pad_0x1c2[2];
    u32 field_0x1c8[2];
    u32 field_0x1d0;
    u8 field_0x1d4;
    u8 pad_0x1d5[3];
    u32 field_0x1d8[2];
    u32 field_0x1e0;
    u32 field_0x1e4[2];
    u8 pad_0x1ec[0x14];
    u32 field_0x200[8];
    f32 field_0x220[2];
    f32 field_0x228[2];
};
#pragma pack(pop)

void fn_71010d0754(UnkMovementCtor_l0d0754* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vt = reinterpret_cast<UnkMovementCtorVt_l0d0754*>(g_unk_7101cb09f8 + 0x10);
    for (f32& v : self->basis) v = 0;
    self->field_0x78 = 0;
    for (u32& v : self->field_0x84) v = 0;
    fn_7100025624(self->record);
    self->field_0x10c[0] = self->field_0x10c[1] = self->field_0x10c[2] = 0;
    for (u32& v : self->field_0x118) v = 0;
    self->field_0x138 = 0xffffffff;
    for (UnkZeroVec2_l0d0754& v : self->field_0x13c) v = UnkZeroVec2_l0d0754();
    self->field_0x170 = 0;
    for (u32& v : self->field_0x174) v = 0;
    self->field_0x184 = 0;
    self->field_0x188 = 0xffffffff;
    for (int i = 5; i >= 0; --i) self->field_0x18c[i] = UnkZeroVec2_l0d0754();
    self->field_0x1c0 = 0;
    self->field_0x1d4 = 0;
    self->field_0x1e4[0] = self->field_0x1e4[1] = 0;
    for (u32& v : self->field_0x200) v = 0;
    self->field_0x220[0] = self->field_0x220[1] = 1.0f;
    self->field_0x228[0] = 1.0f;
    self->field_0x228[1] = 0;
    self->field_0x78 = 0;
    self->basis[0] = self->basis[3] = self->basis[6] = self->basis[9] = self->basis[12] = 1.0f;
    self->field_0x7c[0] = self->field_0x7c[1] = 0;
    self->field_0x1d8[0] = self->field_0x1d8[1] = 0;
    self->field_0x1e0 = 0;
    self->flag_0xfa = 0;
    self->field_0xfc[0] = self->field_0xfc[1] = 0;
    self->flag_0xf8 = self->flag_0xf9 = 1;
    self->field_0x104 = 1;
    self->vt->slot_0x44(self);
    self->distance = 2200.0f;
    self->field_0x1c8[0] = self->field_0x1c8[1] = 0;
    self->field_0x1d0 = 0;
}
