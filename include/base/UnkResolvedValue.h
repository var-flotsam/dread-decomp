#pragma once

#include "base/UnkResolvedPair.h"
#include "base/reflection.h"
#include "types.h"

struct UnkAssignedRecord_a1ba4;

// A resolved value (0x30 bytes): the reflected value and what came with it.
struct UnkResolvedValue {
    base::reflection::UnkReflValue value;
    u64 field_0x18;
    UnkResolvedPair field_0x20;
    UnkResolvedValue() {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        field_0x18 = 0;
        field_0x20.a = 0;
        field_0x20.b = 0;
    }
    UnkResolvedValue& operator=(const UnkResolvedValue& o) {
        fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(&value),
                      reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&o.value));
        field_0x18 = o.field_0x18;
        field_0x20 = o.field_0x20;
        return *this;
    }
    ~UnkResolvedValue() { fn_71000a1c60(&value); }
    explicit UnkResolvedValue(const base::reflection::UnkReflValue& v) {
        fn_71000a179c(&value, &v);
        field_0x18 = 0;
        field_0x20.a = 0;
        field_0x20.b = 0;
    }
};
