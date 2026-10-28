// Copy-assignments of polymorphic records of string ids and plain values (the vtable is not
// copied): a value, then ids each followed by plain data, or two ids then a 0x20-byte block.
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

struct UnkIdValueRecord {
    void* vtable;
    u32 field_0x8;
    CStrId id_0x10;
    u64 field_0x18;
    CStrId id_0x20;
    u64 field_0x28;

    __attribute__((always_inline)) UnkIdValueRecord& operator=(const UnkIdValueRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            id_0x10 = other.id_0x10;
            field_0x18 = other.field_0x18;
            id_0x20 = other.id_0x20;
            field_0x28 = other.field_0x28;
        }
        return *this;
    }
};

UnkIdValueRecord* fn_7100bd0260(UnkIdValueRecord* self, const UnkIdValueRecord* other) { return &(*self = *other); }
UnkIdValueRecord* fn_7100bd0364(UnkIdValueRecord* self, const UnkIdValueRecord* other) { return &(*self = *other); }

struct UnkIdPairBlockRecord {
    void* vtable;
    u32 field_0x8;
    CStrId id_0x10;
    CStrId id_0x18;
    u8 data[0x20];

    __attribute__((always_inline)) UnkIdPairBlockRecord& operator=(const UnkIdPairBlockRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            id_0x10 = other.id_0x10;
            id_0x18 = other.id_0x18;
            __builtin_memcpy(data, other.data, sizeof(data));
        }
        return *this;
    }
};

UnkIdPairBlockRecord* fn_7100cc1e10(UnkIdPairBlockRecord* self, const UnkIdPairBlockRecord* other) {
    return &(*self = *other);
}
UnkIdPairBlockRecord* fn_7100cc1f14(UnkIdPairBlockRecord* self, const UnkIdPairBlockRecord* other) {
    return &(*self = *other);
}

// One or two ids then a block of plain bytes.
template <u32 NumIds, u32 Bytes>
struct UnkIdsBlockRecord {
    void* vtable;
    u32 field_0x8;
    CStrId ids[NumIds];
    u8 data[Bytes];

    __attribute__((always_inline)) UnkIdsBlockRecord& operator=(const UnkIdsBlockRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            for (u32 i = 0; i < NumIds; ++i)
                ids[i] = other.ids[i];
            __builtin_memcpy(data, other.data, sizeof(data));
        }
        return *this;
    }
};

using UnkIdsBlockRecord_1_2d = UnkIdsBlockRecord<1, 0x2d>;
using UnkIdsBlockRecord_2_1c = UnkIdsBlockRecord<2, 0x1c>;

UnkIdsBlockRecord_1_2d* fn_7100caca2c(UnkIdsBlockRecord_1_2d* self, const UnkIdsBlockRecord_1_2d* other) {
    return &(*self = *other);
}
UnkIdsBlockRecord_1_2d* fn_7100cacae8(UnkIdsBlockRecord_1_2d* self, const UnkIdsBlockRecord_1_2d* other) {
    return &(*self = *other);
}
UnkIdsBlockRecord_2_1c* fn_7100cc0880(UnkIdsBlockRecord_2_1c* self, const UnkIdsBlockRecord_2_1c* other) {
    return &(*self = *other);
}
UnkIdsBlockRecord_2_1c* fn_7100cc098c(UnkIdsBlockRecord_2_1c* self, const UnkIdsBlockRecord_2_1c* other) {
    return &(*self = *other);
}

// A value, three blocks of plain words, an id, plain data, an id and a final value.
struct UnkWords0x10 {
    u32 words[4];
};
struct UnkWords0x20 {
    u32 words[8];
};
struct UnkWords0x1c {
    u32 words[7];
};

struct UnkWordsIdsRecord {
    void* vtable;
    u32 field_0x8;
    UnkWords0x10 field_0xc;
    UnkWords0x20 field_0x1c;
    UnkWords0x1c field_0x3c;
    CStrId id_0x58;
    u64 field_0x60[3];
    CStrId id_0x78;
    u32 field_0x80;

    __attribute__((always_inline)) UnkWordsIdsRecord& operator=(const UnkWordsIdsRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            field_0xc = other.field_0xc;
            field_0x1c = other.field_0x1c;
            field_0x3c = other.field_0x3c;
            id_0x58 = other.id_0x58;
            __builtin_memcpy(field_0x60, other.field_0x60, sizeof(field_0x60));
            id_0x78 = other.id_0x78;
            field_0x80 = other.field_0x80;
        }
        return *this;
    }
};
static_assert(sizeof(UnkWordsIdsRecord) == 0x88, "");

UnkWordsIdsRecord* fn_7100cb55d8(UnkWordsIdsRecord* self, const UnkWordsIdsRecord* other) { return &(*self = *other); }
UnkWordsIdsRecord* fn_7100cb5734(UnkWordsIdsRecord* self, const UnkWordsIdsRecord* other) { return &(*self = *other); }
