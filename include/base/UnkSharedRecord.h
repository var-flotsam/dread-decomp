#pragma once

// A polymorphic record of trivially copyable blocks and a std::shared_ptr, copy-assigned member
// by member past its first 0x10 bytes (fn_7100b8f094 and its identical copies). Layout and
// names are placeholders inferred from the code.

#include <memory>

#include "types.h"

template <class Tag>
struct UnkSharedRecord {
    struct Block32 { u64 words[4]; };
    struct Block16 { u64 words[2]; };
    struct Block24 { u64 words[3]; };
    struct Bytes3 { u8 bytes[3]; };
    virtual ~UnkSharedRecord();
    u64 field_0x8;
    Block32 field_0x10;
    Block32 field_0x30;
    u32 field_0x50;
    u32 field_0x54;
    bool field_0x58;
    Block16 field_0x60;
    u32 field_0x70;
    Bytes3 field_0x74;
    Block32 field_0x78;
    u32 field_0x98;
    u32 field_0x9c;
    bool field_0xa0;
    Block24 field_0xa8;
    std::shared_ptr<void> field_0xc0;

    UnkSharedRecord& operator=(const UnkSharedRecord& other) {
        if (this != &other) {
            field_0x10 = other.field_0x10;
            field_0x30 = other.field_0x30;
            field_0x50 = other.field_0x50;
            field_0x54 = other.field_0x54;
            field_0x58 = other.field_0x58;
            field_0x60 = other.field_0x60;
            field_0x70 = other.field_0x70;
            field_0x74 = other.field_0x74;
            field_0x78 = other.field_0x78;
            field_0x98 = other.field_0x98;
            field_0x9c = other.field_0x9c;
            field_0xa0 = other.field_0xa0;
            field_0xa8 = other.field_0xa8;
            field_0xc0 = other.field_0xc0;
        }
        return *this;
    }
};
static_assert(sizeof(UnkSharedRecord<int>) == 0xd0, "UnkSharedRecord layout");
