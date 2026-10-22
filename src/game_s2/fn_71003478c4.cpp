// Move assignment (returns this) of a record: plain fields are copied, the string id at 0x40 is
// released and taken over (the source ends up empty) and the reference-counted pointer at 0x48 is assigned.
#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

struct UnkRefCounted;
void fn_710031d208(UnkRefCounted** self, UnkRefCounted* const* other);

struct UnkMovedRecord {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    UnkStrIdTwoStep field_0x40;
    UnkRefCounted* field_0x48;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;
    u32 field_0x9c;
    u32 field_0xa0;

    __attribute__((always_inline)) UnkMovedRecord& operator=(UnkMovedRecord&& other) {
        u64 first = other.field_0x0;
        u32 second = other.field_0x8;
        field_0x8 = second;
        field_0x0 = first;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        field_0x18 = other.field_0x18;
        field_0x1c = other.field_0x1c;
        field_0x20 = other.field_0x20;
        field_0x24 = other.field_0x24;
        field_0x28 = other.field_0x28;
        field_0x2c = other.field_0x2c;
        field_0x30 = other.field_0x30;
        field_0x34 = other.field_0x34;
        field_0x38 = other.field_0x38;
        field_0x40.~UnkStrIdTwoStep();
        field_0x40.data = other.field_0x40.data;
        other.field_0x40.data = &g_unk_7101d47108->empty;
        fn_710031d208(&field_0x48, &other.field_0x48);
        field_0x50 = other.field_0x50;
        field_0x54 = other.field_0x54;
        field_0x58 = other.field_0x58;
        field_0x5c = other.field_0x5c;
        field_0x60 = other.field_0x60;
        field_0x64 = other.field_0x64;
        field_0x68 = other.field_0x68;
        field_0x6c = other.field_0x6c;
        field_0x70 = other.field_0x70;
        field_0x74 = other.field_0x74;
        field_0x78 = other.field_0x78;
        field_0x7c = other.field_0x7c;
        field_0x80 = other.field_0x80;
        field_0x84 = other.field_0x84;
        field_0x88 = other.field_0x88;
        field_0x8c = other.field_0x8c;
        field_0x90 = other.field_0x90;
        field_0x94 = other.field_0x94;
        field_0x98 = other.field_0x98;
        field_0x9c = other.field_0x9c;
        field_0xa0 = other.field_0xa0;
        return *this;
    }
};

UnkMovedRecord* fn_71003478c4(UnkMovedRecord* self, UnkMovedRecord* other) {
    return &(*self = static_cast<UnkMovedRecord&&>(*other));
}
