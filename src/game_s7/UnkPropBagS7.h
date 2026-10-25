#pragma once

// A bag of named values that objects fill to describe their state, then hand on
// (fn_71010e560c). The keys are string ids from the large id table built by fn_7100031894.
// Names and layouts are placeholders.

#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"

namespace unk_prop_bag {

using base::global::CStrId;

// The bag's hash stream (base/UnkHashStream.h), built with the bag's allocation flags.
struct UnkLock : UnkHashStream_0x38 {
    UnkLock() { fn_710007be8c(this, 0x3d, nullptr); }
};

// A string id whose destructor decrements and tests the count in two steps (fn_710050e8e4).
struct UnkStrId {
    base::global::UnkStringData* data;
};

struct UnkPropBag;
void fn_710050e8e4(UnkPropBag* bag);  // the destructor

struct UnkPropBag {
    base::UnkVector<u64> values{0x3d};
    UnkLock lock;
    base::UnkVector<UnkStrId> names{0x3d};

    ~UnkPropBag() { fn_710050e8e4(this); }
};

void fn_710033187c(UnkPropBag* bag, const CStrId& key, f32 value);
void fn_710050f4ac(UnkPropBag* bag, const CStrId& key, bool value);
void fn_710050e4dc(UnkPropBag* bag, const CStrId& key, const char* value);
void fn_7100aa3b9c(UnkPropBag* bag, const CStrId& key, u32 value);

// The key at `offset` in the id table.
inline const CStrId& Key(u32 offset) {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) +
                                            offset);
}

// A shared name: taking its text counts a use (a union member, so the owner's pointer is
// reloaded after the increment).
struct UnkName {
    u8 unk_0x0[0x10];
    const char* str;
    u8 unk_0x18[0x2c];
    union {
        u32 use_count;
    };
};

inline const char* UseName(UnkName* const& name) {
    ++name->use_count;
    return name->str ? name->str : "";
}

// Two zeroed words passed by reference to fn_71010e560c.
struct UnkPair {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
};

}  // namespace unk_prop_bag
