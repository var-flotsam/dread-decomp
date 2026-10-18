#pragma once

// A registry of objects keyed by "<prefix>|<short class name>" (fn_7100072940 and 181 identical
// copies, one per instantiation in the original). The object's class comes from its first
// virtual; the short name is the class name after the last "::". Names are placeholders.

#include <string.h>

#include "base/global/CStrId.h"
#include "types.h"

namespace base {

struct UnkNamedClassInfo {
    u8 unk_0x0[0x10];
    const char* name;
    u8 unk_0x18[0x2c];
    // A union member: the original reloads the class pointer after the increment.
    union {
        s32 field_0x44;
    };
};

struct UnkNamedTypeRef {
    void* unk_0x0;
    UnkNamedClassInfo* cls;
};

struct UnkNamedObject {
    virtual UnkNamedTypeRef* vf_0x0();
    global::CStrId field_0x8;   // the prefix
    global::CStrId field_0x10;  // the short class name
};

struct UnkNamedEntry {
    u8 unk_0x0[0x30];
};

struct UnkNamedRegistry {
    UnkNamedObject** objects;
    UnkNamedEntry* entries;
    // A union member: the original reloads the count after storing the object pointer.
    union {
        u32 count;
    };
    u32 capacity;
    u32 step;
};

}  // namespace base

void fn_710007e704(base::UnkNamedEntry* entry, const char* key);
void fn_710007ea60(base::UnkNamedEntry* dst, const base::UnkNamedEntry* src);
void fn_710007ecd4(base::UnkNamedEntry* entry);
void fn_710007547c(base::UnkNamedRegistry* registry, u32 capacity);
void fn_7100136f38(base::UnkNamedObject* obj);

namespace base {

// Tag is the placeholder for the original's template argument; the code doesn't depend on it.
template <class Tag>
UnkNamedObject* refl_RegisterNamed(UnkNamedRegistry* reg, UnkNamedObject* obj, const char* prefix) {
    char key[0x80] = {};
    if (prefix) {
        u32 len = strlen(prefix);
        if (len) {
            strcpy(key, prefix);
            key[len] = '|';
        }
    }
    UnkNamedTypeRef* type = obj->vf_0x0();
    type->cls->field_0x44++;
    const char* name = type->cls->name ? type->cls->name : "";
    const char* p = strstr(name, "::");
    while (p) {
        name = p + 2;
        p = strstr(name, "::");
    }
    strcat(key, name);
    // Copy-assigned from named temporaries (a move-assignment compiles differently).
    {
        global::CStrId id(prefix, true);
        obj->field_0x8 = id;
    }
    {
        global::CStrId id(name, true);
        obj->field_0x10 = id;
    }
    UnkNamedEntry entry;
    fn_710007e704(&entry, key);
    if (reg->count == reg->capacity)
        fn_710007547c(reg, reg->step + reg->count);
    fn_710007ea60(&reg->entries[reg->count], &entry);
    reg->objects[reg->count] = obj;
    reg->count++;
    fn_710007ecd4(&entry);
    fn_7100136f38(obj);
    return obj;
}

}  // namespace base
