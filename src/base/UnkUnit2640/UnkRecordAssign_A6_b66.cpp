// From unit 2640 of tools/tu_map.py: the self-checked copy assignment of the 0x50-byte record of
// UnkRecordIterAssign.cpp (a word, a part assigned by fn_7100ffbad8, then plain fields copied as one run).
// Names and layouts are placeholders.
#include "game/UnkTU2640.h"
#include "types.h"

struct UnkPartFfca00;

struct UnkPart_A6 {
    u64 unk_0x0[5];
    UnkPart_A6& operator=(const UnkPart_A6& other) {
        fn_7100ffbad8(reinterpret_cast<UnkPartFfca00*>(this), reinterpret_cast<const UnkPartFfca00*>(&other));
        return *this;
    }
};

struct UnkRecordBody_A6 {
    u32 field_0x0;
    UnkPart_A6 part_0x8;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
};

struct UnkRecord_A6 {
    UnkRecordBody_A6 body;
    UnkRecord_A6& operator=(const UnkRecord_A6& other) {
        if (this != &other) body = other.body;
        return *this;
    }
};

// The reflected assignment (slot 0x48 in src/reflection/class_descriptors_02.cpp).
void* fn_7100ffc66c(void* dst, const void* src) {
    return &(*static_cast<UnkRecord_A6*>(dst) = *static_cast<const UnkRecord_A6*>(src));
}
