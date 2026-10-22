#pragma once

// A named setting passed to an object's property handler: the handler compares the property's
// name with keys from the id table built by fn_7100031894, then reads the value from the
// property's stream or writes it there. Handlers return a u32 flag (whether the property was
// handled): with bool, returning the base handler's result would mask it first. Names and
// layouts are placeholders.

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"

namespace unk_prop {

using base::global::CStrId;

// The string data behind a key, seen as a shared name. The use count is a union member, so the
// key's pointer is reloaded after the increment.
struct UnkKeyName {
    u8 unk_0x0[0x30];
    u64 hash;
    u8 unk_0x38[0xc];
    union {
        u32 use_count;
    };
};

// The same data behind the property's name. Here the count is a plain member and the property's
// pointer a union member: the key's pointer is read before the name's count is stored, and the
// property's pointer after.
struct UnkPropName {
    u8 unk_0x0[0x30];
    u64 hash;
    u8 unk_0x38[0xc];
    u32 use_count;
};

struct UnkProp {
    u8 unk_0x0[0x8];
    union {
        UnkPropName* name;
    };
    u8 unk_0x10[0x28];
    u8 stream[0x40];
    bool set;  // true: store the stream's value in the object; false: write the value out
};

// The id table (the function-local static that fn_7100031894 constructs). It needs a size:
// with an empty type, Clang treats a store through any pointer as unable to reach a key, and
// the key's pointer is not reloaded.
struct UnkIdTable {
    u8 unk_0x0[0x2800];
    UnkIdTable();  // fn_7100031894
    ~UnkIdTable();
};

inline UnkIdTable& IdTable() {
    static UnkIdTable s_table;
    return s_table;
}

// The key at `offset` in the id table.
inline const CStrId& Key(u32 offset) {
    return *reinterpret_cast<const CStrId*>(IdTable().unk_0x0 + offset);
}

inline UnkKeyName* KeyName(const CStrId& key) {
    return reinterpret_cast<UnkKeyName*>(key.data);
}

// Whether the property is named `key`; counts a use of both names.
inline bool Is(UnkProp* p, const CStrId& key) {
    ++KeyName(key)->use_count;
    UnkKeyName* k = KeyName(key);
    ++p->name->use_count;
    return p->name->hash == k->hash;
}

// Whether the property is named `name` (a string built for the comparison); counts a use of
// the property's name only.
inline bool IsName(UnkProp* p, const base::global::CRntString& name) {
    ++p->name->use_count;
    return p->name->hash == (u64)name.key;
}

}  // namespace unk_prop

// Formatted stream access ("i" integer, "b" bool, "f" float, "s" string; reads take pointers).
s64 fn_710007cf30(void* stream, const char* format, ...);  // read
s64 fn_710007cb4c(void* stream, const char* format, ...);  // write
s64 fn_710007cee4(void* stream, const char** out);          // the next string, in place
void fn_710007e77c(base::global::CRntString* str, const char* value);  // assigns
