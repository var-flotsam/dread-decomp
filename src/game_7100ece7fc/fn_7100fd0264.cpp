// Destructors of the eight-entry object (constructor fn_7100fcfcdc), complete and deleting,
// each also entered through the secondary base: set both vtables, destroy the entries last to
// first (fn_7100fd11e8), reset the secondary base and run the primary base's destructor
// (fn_7100fe1050).
#include <new>

#include "types.h"

struct UnkEntry_0x150 {
    u64 data[0x150 / 8];
    ~UnkEntry_0x150();  // fn_7100fd11e8
};

struct UnkBase_0x30 {
    void** vtable;
    u8 unk_0x8[0x28];
    ~UnkBase_0x30();  // fn_7100fe1050
};

extern void* g_unk_7101cac080[];
extern void* g_unk_7101cabfa8[];

struct UnkSecondaryBase_0x30 {
    void** vtable;
    void* field_0x8;
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;

    ~UnkSecondaryBase_0x30() {
        vtable = &g_unk_7101cac080[2];
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        field_0x20 = 0;
    }
};

struct UnkObject_7100fcfcdc : UnkBase_0x30, UnkSecondaryBase_0x30 {
    u8 unk_0x54[0x80];
    s32 field_0xd4;
    u16 field_0xd8;
    UnkEntry_0x150 entry0, entry1, entry2, entry3, entry4, entry5, entry6, entry7;
    u8 unk_0xb60[0x18];

    ~UnkObject_7100fcfcdc() {
        UnkBase_0x30::vtable = &g_unk_7101cabfa8[2];
        UnkSecondaryBase_0x30::vtable = &g_unk_7101cabfa8[21];
    }
};

void fn_7100fd0264(UnkObject_7100fcfcdc& self) {
    delete &self;
}

void fn_7100fd0140(UnkObject_7100fcfcdc& self) {
    self.~UnkObject_7100fcfcdc();
}

// The same two, entered through the secondary base.
void fn_7100fd01d0(UnkSecondaryBase_0x30& base) {
    static_cast<UnkObject_7100fcfcdc&>(base).~UnkObject_7100fcfcdc();
}

void fn_7100fd02fc(UnkSecondaryBase_0x30& base) {
    delete &static_cast<UnkObject_7100fcfcdc&>(base);
}
