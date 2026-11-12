// CSamusMagnetGloveMovementState::CParams: the reflected copy- and move-assignments (fn_0x48, fn_0x50; identical),
// shaped as CSamusHangMovementState's (src/game/CSamusHangMovementState/CParams/fn_71009a3048.cpp): the vtable is
// kept, the base's fields copied as one block, the segment at +0x64 (fn_7100125b44), the handle at +0x80
// (fn_710031d208) and the slot at +0x88 (fn_71002a5d1c) assigned out of line. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU365.h"
#include "game/malloc.h"
#include "types.h"

struct UnkAssignedSlot;

// Five words assigned out of line (the first 0x14 bytes of a segment).
struct UnkSub_0x14_A4_b72 {
    u32 field_0x0[5];

    UnkSub_0x14_A4_b72& operator=(const UnkSub_0x14_A4_b72& other) {
        fn_7100125b44(reinterpret_cast<UnkSegment2D*>(this), reinterpret_cast<const UnkSegment2D*>(&other));
        return *this;
    }
};

struct UnkSlot_A4_b72 {
    void* field_0x0;

    UnkSlot_A4_b72& operator=(const UnkSlot_A4_b72& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

struct UnkVec3_A4_b72 {
    f32 x, y, z;
};

// Polymorphic, so a derived class reuses its tail padding (+0x2c).
struct UnkMagnetRecordBase_A4_b72 {
    virtual void vf0();
    void* field_0x8;
    void* field_0x10;
    void* field_0x18;
    void* field_0x20;
    u32 field_0x28;
};

struct UnkMagnetParams_A4_b72 : UnkMagnetRecordBase_A4_b72 {
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;  // not copied
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    UnkSub_0x14_A4_b72 field_0x64;
    u32 field_0x78;
    base::UnkRefHandle field_0x80;
    UnkSlot_A4_b72 field_0x88;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;
    UnkVec3_A4_b72 field_0x9c;

    __attribute__((always_inline)) UnkMagnetParams_A4_b72& operator=(const UnkMagnetParams_A4_b72& other) {
        if (this != &other) {
            UnkMagnetRecordBase_A4_b72::operator=(other);
            field_0x2c = other.field_0x2c;
            field_0x30 = other.field_0x30;
            field_0x38 = other.field_0x38;
            field_0x3c = other.field_0x3c;
            field_0x40 = other.field_0x40;
            field_0x44 = other.field_0x44;
            field_0x48 = other.field_0x48;
            field_0x4c = other.field_0x4c;
            field_0x50 = other.field_0x50;
            field_0x54 = other.field_0x54;
            field_0x58 = other.field_0x58;
            field_0x5c = other.field_0x5c;
            field_0x60 = other.field_0x60;
            field_0x64.operator=(other.field_0x64);
            field_0x78 = other.field_0x78;
            fn_710031d208(&field_0x80, &other.field_0x80);
            field_0x88.operator=(other.field_0x88);
            field_0x90 = other.field_0x90;
            field_0x94 = other.field_0x94;
            field_0x98 = other.field_0x98;
            field_0x9c = other.field_0x9c;
        }
        return *this;
    }
};
static_assert(__builtin_offsetof(UnkMagnetParams_A4_b72, field_0x9c) == 0x9c, "layout");

void* fn_71009af428(void* dst, const void* src) {
    return &(*static_cast<UnkMagnetParams_A4_b72*>(dst) = *static_cast<const UnkMagnetParams_A4_b72*>(src));
}

void* fn_71009af530(void* dst, const void* src) {
    return &(*static_cast<UnkMagnetParams_A4_b72*>(dst) = *static_cast<const UnkMagnetParams_A4_b72*>(src));
}
