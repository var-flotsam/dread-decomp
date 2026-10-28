// Destructors (complete and deleting, each also entered through the secondary base at 8) of an
// object with a table member at 0x68: restore the vtables, clear the table (fn_710102bcf8) and
// free its two arrays, then the secondary base clears its array (fn_710102be04) and frees it.
#include "base/UnkVector.h"
#include "types.h"

struct UnkBase_0x8 {
    void** vtable;
};

extern void* g_unk_7101cac5d8[];
extern void* g_unk_7101cac6e8[];

struct UnkSecondaryBase_0x30;
void fn_710102be04(base::UnkVector<u64>* array);

struct UnkSecondaryBase_0x30 {
    void** vtable;
    union {
        base::UnkVector<u64> array;
    };

    ~UnkSecondaryBase_0x30() {
        vtable = &g_unk_7101cac6e8[2];
        fn_710102be04(&array);
        array.Free();
    }
};

struct UnkTable_0x28;
void fn_710102bcf8(UnkTable_0x28* self);

struct UnkTable_0x28 {
    void* field_0x0;
    void* field_0x8;
    u8 unk_0x10[0x10];
    base::UnkAllocator* allocator;

    ~UnkTable_0x28() {
        fn_710102bcf8(this);
        if (field_0x8)
            allocator->vf_0x48(field_0x8);
        if (field_0x0)
            allocator->vf_0x48(field_0x0);
    }
};

struct UnkObject_71010233ec : UnkBase_0x8, UnkSecondaryBase_0x30 {
    u8 unk_0x38[0x30];
    UnkTable_0x28 table;

    ~UnkObject_71010233ec() {
        UnkBase_0x8::vtable = &g_unk_7101cac5d8[2];
        UnkSecondaryBase_0x30::vtable = &g_unk_7101cac5d8[12];
    }
};

void fn_71010233ec(UnkObject_71010233ec& self) {
    self.~UnkObject_71010233ec();
}

void fn_7101023544(UnkObject_71010233ec& self) {
    delete &self;
}

// The same two, entered through the secondary base.
void fn_7101023490(UnkSecondaryBase_0x30& base) {
    static_cast<UnkObject_71010233ec&>(base).~UnkObject_71010233ec();
}

void fn_71010235e8(UnkSecondaryBase_0x30& base) {
    fn_7101023544(static_cast<UnkObject_71010233ec&>(base));
}
