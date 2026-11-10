// Unit 90 of tools/tu_map.py (no class hint): the version writer's (UnkVersionWriter_A1_b71.cpp)
// stack of 32-byte entries, one per nested value being written; these pop the last one. Names and
// layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkWriterEntry_b0202 {
    u64 key;     // 0x0: the type
    u64 owner;   // 0x8
    bool flag;   // 0x10: a value was written with a length prefix at `extra`
    u64 extra;   // 0x18: the file position of the prefix
};
static_assert(sizeof(UnkWriterEntry_b0202) == 0x20, "entry");

struct UnkWriter_b0202 {
    u32 count;
    u8 entries_0x4[0xc80];
    u8 pad_0xc84[4];
    base::global::CRntFile* file;  // 0xc88
};
static_assert(__builtin_offsetof(UnkWriter_b0202, file) == 0xc88, "writer");

static inline UnkWriterEntry_b0202* Entries(UnkWriter_b0202* self) {
    return reinterpret_cast<UnkWriterEntry_b0202*>(self->entries_0x4);
}

struct UnkWriteValue_b0202 {
    u8 pad_0x0[0xe8];
    base::reflection::UnkReflObject* object;  // 0xe8
};

const base::reflection::CType* fn_710009c640(base::reflection::UnkReflObject* obj);

// Sets the entry count, clearing the entries that come into use.
static inline void Resize(UnkWriter_b0202* self, u32 size) {
    for (u32 i = self->count; i < size; ++i) {
        UnkWriterEntry_b0202& e = Entries(self)[i];
        e.key = 0;
        e.owner = 0;
        e.flag = false;
        e.extra = 0;
    }
    self->count = size;
}

s32 fn_71000ed680(UnkWriter_b0202* self, UnkWriteValue_b0202* value) {
    fn_710009c640(value->object);
    Resize(self, self->count - 1);
    return 1;
}

s32 fn_71000eda3c(UnkWriter_b0202* self) {
    UnkWriterEntry_b0202& last = Entries(self)[self->count - 1];
    if (last.flag) {
        base::global::CRntFile* file = self->file;
        u64 position = file->position;
        fn_710007c00c(file, last.extra, 0);
        fn_710007c00c(self->file, position, 0);
    }
    Resize(self, self->count - 1);
    return 1;
}
