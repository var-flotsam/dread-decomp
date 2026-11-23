#pragma once

#include "base/UnkTunableField.h"
#include "types.h"

// A type descriptor as the lookups see it: its class (vtable slot 0) and its fields by name hash.
struct UnkTunableType {
    virtual const base::reflection::CType* GetClass();
    u8 pad_0x8[0xe0];
    UnkTunableField* fields;  // +0xe8
    u64* keys;                // +0xf0
    u32 count;                // +0xf8

    UnkTunableField* End() { return fields + count; }
    UnkTunableField* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
};
