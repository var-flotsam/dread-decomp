// CPoisonFlyDiveAttack: two identical constructors (fn_71006311e8, fn_71006335bc). The attack base constructor fn_710036069c,
// the vtable, a cleared word at +0x28, three vectors at +0x30, +0x3c and +0x48 set to the zero vector
// g_unk_7101647790, cleared fields up to +0x6c and a member at +0x70 (vtable g_unk_7101a2aef8) with
// its fields cleared. Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "game/CAutclastChargeAttack.h"
#include "types.h"

void fn_710036069c(void* self);
extern void* g_unk_7101a57f68[];

struct UnkPoisonFlyDiveVec {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkPoisonFlyDiveAttack {
    void** vtable;
    u8 pad_0x8[0x20];
    u64 field_0x28;
    UnkPoisonFlyDiveVec field_0x30;
    UnkPoisonFlyDiveVec field_0x3c;
    UnkPoisonFlyDiveVec field_0x48;
    u32 field_0x54;
    u8 pad_0x58[0x4];
    u32 field_0x5c[4];
    u8 pad_0x6c[0x4];
    void** field_0x70;
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
};

__attribute__((always_inline)) static inline void Construct(UnkPoisonFlyDiveAttack* self) {
    fn_710036069c(self);
    self->vtable = g_unk_7101a57f68 + 2;
    self->field_0x28 = 0;
    // Copied word by word; the third word is read after the first two are stored.
    const UnkPoisonFlyDiveVec& zero = *reinterpret_cast<const UnkPoisonFlyDiveVec*>(&g_unk_7101647790);
    u32 x = reinterpret_cast<const u32*>(&zero)[0];
    u32 y = reinterpret_cast<const u32*>(&zero)[1];
    reinterpret_cast<u32*>(&self->field_0x30)[0] = x;
    reinterpret_cast<u32*>(&self->field_0x30)[1] = y;
    u32 z = reinterpret_cast<const u32*>(&zero)[2];
    reinterpret_cast<u32*>(&self->field_0x30)[2] = z;
    reinterpret_cast<u32*>(&self->field_0x3c)[0] = x;
    reinterpret_cast<u32*>(&self->field_0x3c)[1] = y;
    reinterpret_cast<u32*>(&self->field_0x3c)[2] = z;
    reinterpret_cast<u32*>(&self->field_0x48)[0] = x;
    reinterpret_cast<u32*>(&self->field_0x48)[1] = y;
    reinterpret_cast<u32*>(&self->field_0x48)[2] = z;
    self->field_0x54 = 0;
    __builtin_memset(self->field_0x5c, 0, sizeof(self->field_0x5c));
    self->field_0x70 = g_unk_7101a2aef8 + 2;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->field_0x88 = 0;
}

void fn_71006311e8(UnkPoisonFlyDiveAttack* self) {
    Construct(self);
}

void fn_71006335bc(UnkPoisonFlyDiveAttack* self) {
    Construct(self);
}
