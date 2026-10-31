// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0x88 refer to what the other one refers to. Near copy of
// fn_71004caf24 (src/game/CCooldownXBossStrongWhipAttack/fn_71004caf24.cpp).
#include "game_s2/UnkWeakHandle.h"


struct UnkWords_71006338dc {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkBlock32_71006338dc {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct __attribute__((packed)) UnkLongWord_71006338dc {
    u64 field_0x0;
    u32 field_0x8;
};

struct UnkRecord_71006338dc {
    u8 pad_0x0[0x8];
    UnkBlock32_71006338dc field_0x8;
    u64 field_0x28;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    UnkLongWord_71006338dc field_0x54;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u8 pad_0x6c[0xc];
    UnkWords_71006338dc field_0x78;
    UnkWeakHandle* field_0x88;
};


UnkRecord_71006338dc* fn_71006338dc(UnkRecord_71006338dc* self, const UnkRecord_71006338dc* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x34 = other->field_0x34;
    self->field_0x38 = other->field_0x38;
    self->field_0x3c = other->field_0x3c;
    self->field_0x40 = other->field_0x40;
    self->field_0x44 = other->field_0x44;
    self->field_0x48 = other->field_0x48;
    self->field_0x4c = other->field_0x4c;
    self->field_0x50 = other->field_0x50;
    self->field_0x54 = other->field_0x54;
    self->field_0x60 = other->field_0x60;
    self->field_0x64 = other->field_0x64;
    self->field_0x68 = other->field_0x68;
    self->field_0x78 = other->field_0x78;
    if (self != other)
        CopyWeakRef(&self->field_0x88, other->field_0x88);
    return self;
}
