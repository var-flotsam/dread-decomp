// Unit 151 (tools/tu_map.py): resizes an array of 0x20-byte entries (a count, then the entries from +4): new entries
// are built by fn_710014d930; entries cut off release their owned block (fn_7100083c00) and their string id.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU230.h"
#include "types.h"

using base::UnkStrIdMember;

struct __attribute__((packed)) UnkEntry_b0249 {
    base::UnkStrIdMember id;  // 0x0
    union {
        void* owned;  // 0x8
        f32* values;
    };
    u32 count;      // 0x10
    s32 kind;       // 0x14
    u64 location;   // 0x18
};
static_assert(sizeof(UnkEntry_b0249) == 0x20, "entry layout");

struct __attribute__((packed)) UnkEntryArray_b0249 {
    u32 count;
    UnkEntry_b0249 items[1];
};

void fn_710014d930(UnkEntry_b0249* entry);

void fn_710014d838(UnkEntryArray_b0249* self, u32 count) {
    u32 old = self->count;
    if (old < count) {
        for (u32 i = old; i < count; ++i)
            fn_710014d930(&self->items[i]);
    } else if (old > count) {
        for (u64 i = count; i < self->count; ++i) {
            UnkEntry_b0249* e = &self->items[i];
            if (e->owned) {
                fn_7100083c00(e->owned);
                e->owned = nullptr;
            }
            e->id.~UnkStrIdMember();
        }
    }
    self->count = count;
}

// A string id's pooled data as this reads it: the text and a read counter.
struct UnkIdData_b0249 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 reads;  // 0x44
    };
};

struct __attribute__((packed)) UnkEntryHolder_b0249 {
    u8 pad_0x0[0x38];
    void* program;  // 0x38
    u8 pad_0x40[0x7cc - 0x40];
    UnkEntryArray_b0249 entries;  // 0x7cc
};

void fn_710015ddc0(void* state, u32 stage, u64* location, const char* name);

// Adds an entry for `id` with `n` zeroed floats, and looks up its uniform location in the program's state.
void fn_710014ce5c(UnkEntryHolder_b0249* self, u32 kind, const base::global::CStrId* id, u32 n) {
    UnkEntry_b0249* e = &reinterpret_cast<UnkEntry_b0249*>(reinterpret_cast<u8*>(self) + 0x7d0)[self->entries.count];
    fn_710014d930(e);
    self->entries.count = self->entries.count + 1;
    e->id = *reinterpret_cast<const base::UnkStrIdMember*>(id);
    e->kind = kind;
    e->values = static_cast<f32*>(fn_7100083ba0((u64)n * 4, 8));
    __builtin_memset(e->values, 0, (u64)n * 4);
    e->count = n;
    void* state = fn_71001aa028(self->program);
    u64* location = &e->location;
    if (state) {
        UnkIdData_b0249* data = reinterpret_cast<UnkIdData_b0249*>(e->id.data);
        ++data->reads;
        data = reinterpret_cast<UnkIdData_b0249*>(e->id.data);
        fn_710015ddc0(state, kind, location, data->text ? data->text : g_unk_710162e2ed);
    } else {
        *location = 0;
    }
}
