// CCharClassComponent's unit (tools/tu_map.py): a record's constructor (two empty string ids, an
// array, an in-memory file) and a table's reader (a count, then that many 0x98-byte entries, the old
// ones destroyed first). Names and layouts are placeholders.
#include <new>

#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkCharClassRecord_ovb0127 {
    CStrId field_0x0;
    CStrId field_0x8;
    base::UnkVector<void*> field_0x10;
    UnkHashStream_0x38 field_0x38;
    u64 field_0x70;
};
static_assert(__builtin_offsetof(UnkCharClassRecord_ovb0127, field_0x70) == 0x70, "layout");

// The empty id, the pool created first when there is none yet.
static inline void InitEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_71011ad358(UnkCharClassRecord_ovb0127* self) {
    InitEmpty(&self->field_0x0);
    InitEmpty(&self->field_0x8);
    new (&self->field_0x10) base::UnkVector<void*>(5);
    fn_710007be8c(&self->field_0x38, 0x16, nullptr);
    self->field_0x70 = 0;
}


// A 0x98-byte entry, read from a file (fn_71011ad60c), made by fn_71011ad29c and destroyed by
// fn_71011ad580.
struct UnkCharClassEntry_ovb0127 {
    u8 data[0x98];
};
void fn_71011ad29c(UnkCharClassEntry_ovb0127* self);
void fn_71011ad580(UnkCharClassEntry_ovb0127* self);
void fn_71011ad60c(UnkCharClassEntry_ovb0127* self, void* context, base::global::CRntFile* file);

struct UnkCharClassEntries_ovb0127 {
    UnkCharClassEntry_ovb0127* data;
    u32 count;
};
void fn_71011ad054(UnkCharClassEntries_ovb0127* self, u32 capacity);  // grows the array to at least `capacity`

struct UnkCharClassTable_ovb0127 {
    u8 pad_0x0[0x8];
    u8 context[0x28];                     // 0x8
    UnkCharClassEntries_ovb0127 entries;  // 0x30
};

// Adds a default entry and returns it.
static inline UnkCharClassEntry_ovb0127& AddEntry(UnkCharClassEntries_ovb0127* entries) {
    u32 n = entries->count + 1;
    fn_71011ad054(entries, n);
    for (u32 i = entries->count; i < n; ++i)
        fn_71011ad29c(&entries->data[i]);
    entries->count = n;
    UnkCharClassEntry_ovb0127* end = &entries->data[n];
    return n ? end[-1] : *end;
}

bool fn_71011acf1c(UnkCharClassTable_ovb0127* self, base::global::CRntFile* file) {
    s32 count = 0;
    fn_710007c714(file, &count, sizeof(count), 1);
    UnkCharClassEntries_ovb0127* entries = &self->entries;
    for (u64 i = 0; i < entries->count; ++i)
        fn_71011ad580(&entries->data[i]);
    entries->count = 0;
    fn_71011ad054(entries, count);
    for (s32 i = 0; i < count; ++i)
        fn_71011ad60c(&AddEntry(entries), self->context, file);
    return true;
}
