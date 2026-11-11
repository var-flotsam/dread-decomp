// Unit 433 (CAbilityShinespark's neighbourhood): writes a float for a string id into a record
// file, at the id's recorded position or at a new one, and remembers the id.
// Names and layouts are placeholders.

#include "base/UnkCloneRecords.h"
#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "types.h"

#include <algorithm>

#include "../../../src/game_s7/UnkIter.h"


// The part of the string data read here: a 64-bit key at 0x30.
struct UnkStringDataKey {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

inline u64 KeyOf(const base::global::CStrId& id) {
    return reinterpret_cast<const UnkStringDataKey*>(id.data)->field_0x30;
}

struct UnkRecordEntry {
    u64 position;
    u64 key;
    u32 kind;

    UnkRecordEntry(u64 position, u64 key, u32 kind) : position(position), key(key), kind(kind) {}
    UnkRecordEntry(const UnkRecordEntry& other) : position(other.position), key(other.key), kind(other.kind) {}
};

struct UnkRecordWriter {
    base::UnkVector<UnkRecordEntry> entries;
    base::global::CRntFile file;
    base::UnkStrIdVec ids;

    UnkRecordEntry* End() { return entries.data + entries.count; }
    UnkRecordEntry* Find(const base::global::CStrId* id) {
        return std::find_if(UnkIter<UnkRecordEntry>{entries.data}, UnkIter<UnkRecordEntry>{End()},
                            [id](const UnkRecordEntry& e) { return e.key == KeyOf(*id); })
            .p;
    }
    base::global::CStrId* FindId(const base::global::CStrId& id) {
        u32 n = ids.count;
        for (u32 i = 0; i < n; ++i)
            if (ids.data[i].data == id.data)
                return &ids.data[i];
        return &ids.data[n];
    }
};
static_assert(__builtin_offsetof(UnkRecordWriter, file) == 0x28, "file");
static_assert(__builtin_offsetof(UnkRecordWriter, ids) == 0x60, "ids");



void fn_710033187c(UnkRecordWriter* self, const base::global::CStrId* id, f32 value) {
    UnkRecordEntry* it = self->Find(id);
    u64 position = fn_710007c1a4(&self->file);
    if (it == self->End())
        self->entries.PushBackCopy(UnkRecordEntry(position, KeyOf(*id), 0x66));
    else
        position = it->position;
    fn_710007c00c(&self->file, position, 0);
    fn_710007c450(&self->file, &value, 4, 1);
    if (self->FindId(*id) == &self->ids.data[self->ids.count])
        self->ids.PushBack(*id);
}
