// base::animation::CBlendSpaceResource: looks a reflected value up by key in the resource's own table, then
// (when asked and nothing was found) in the table of its parent. Names and layouts are placeholders.
#include "base/reflection.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

u64 fn_710009dd04(char* p0);

struct UnkValueEntry {
    void* stored;
    const char* type;
    u32 kind;
    u32 by_pointer;
};

struct UnkValueTable {
    UnkValueEntry* entries;
    u64* keys;
    u32 count;
};

struct UnkBlendSpaceHolder {
    char pad_0x0[0x10];
    char* parent;
    char pad_0x18[0x20];
    UnkValueTable table;
};
static_assert(__builtin_offsetof(UnkBlendSpaceHolder, table) == 0x38);

// Returns the entry for `key`, or the slot past the last entry when there is none.
static inline UnkValueEntry* FindEntry(UnkValueTable* t, const u64* key) {
    u64* keys = t->keys;
    for (u64 i = 0; i != t->count; ++i) {
        if (keys[i] == *key) return &t->entries[i];
    }
    return &t->entries[t->count];
}

static inline void ReadEntry(UnkReflValue* out, UnkValueEntry* e) {
    out->field_0x0 = nullptr;
    out->type = nullptr;
    out->kind = e->kind;
    const char* type = e->type;
    const void* value;
    if (!type) {
        value = nullptr;
    } else if (e->by_pointer == 1 || *(const u64*)(type + 0x18) > 8) {
        value = e->stored;
    } else {
        value = e;
    }
    fn_71000a18d4(out, (const base::reflection::CType*)type, value, 0);
}

// NON_MATCHING(minor): two argument-register moves (x20/x21) at the top are in the other order
UnkReflValue fn_710000cf10(void* holder, const base::reflection::UnkReflAttrKey& attr_key, bool use_parent) {
    const u64* key = (const u64*)&attr_key;
    UnkBlendSpaceHolder* self = (UnkBlendSpaceHolder*)holder;
    UnkReflValue result;
    UnkValueEntry* e = FindEntry(&self->table, key);
    if (e == &self->table.entries[self->table.count]) {
        result.field_0x0 = nullptr;
        result.type = nullptr;
        result.kind = 2;
        result.field_0x14 = 1;
    } else {
        ReadEntry(&result, e);
    }
    if (use_parent && !result.type) {
        UnkValueTable* table = (UnkValueTable*)fn_710009dd04(self->parent);
        UnkReflValue other;
        UnkValueEntry* p = FindEntry(table, key);
        if (p == &table->entries[table->count]) {
            other.field_0x0 = nullptr;
            other.type = nullptr;
            other.kind = 2;
            other.field_0x14 = 1;
        } else {
            ReadEntry(&other, p);
        }
        fn_71000a1ba4((UnkAssignedRecord_a1ba4*)&result, (const UnkAssignedRecord_a1ba4*)&other);
        fn_71000a1c60(&other);
    }
    return result;
}
