// Unit 1607 (CUsableComponent hint): sets a u32 under a string id in a property bag: writes it into the
// bag's file at the position recorded for the id (recording the file's end for a new id), then
// remembers the id. Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "../../../src/game_s7/UnkIter.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "types.h"

#include <algorithm>

using base::global::CStrId;

// The pooled string's 64-bit hash at 0x30.
struct UnkHashedStringKey_aa3b9c {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline u64 HashOf(const CStrId& id) {
    return reinterpret_cast<const UnkHashedStringKey_aa3b9c*>(id.data)->hash;
}

// Where a value was written: its position in the file, the id's hash and the value's kind.
struct UnkPropRecord_aa3b9c {
    u64 position;
    u64 hash;
    u32 kind;

    UnkPropRecord_aa3b9c(u64 position, u64 hash, u32 kind) : position(position), hash(hash), kind(kind) {}
    UnkPropRecord_aa3b9c(const UnkPropRecord_aa3b9c& other)
        : position(other.position), hash(other.hash), kind(other.kind) {}
};

// The property bag as this unit sees it.
struct UnkPropBagLayout_aa3b9c {
    base::global::CRntVector<UnkPropRecord_aa3b9c> records;
    base::global::CRntFile file;
    base::UnkStrIdVec names;

    UnkPropRecord_aa3b9c* RecordsEnd() { return records.data + records.count; }
    UnkPropRecord_aa3b9c* FindRecord(const CStrId& id) {
        return std::find_if(UnkIter<UnkPropRecord_aa3b9c>{records.data}, UnkIter<UnkPropRecord_aa3b9c>{RecordsEnd()},
                            [&id](const UnkPropRecord_aa3b9c& r) { return r.hash == HashOf(id); })
            .p;
    }
    CStrId* NamesEnd() { return names.data + names.count; }
    CStrId* FindName(const CStrId& id) {
        CStrId* it = names.data;
        CStrId* end = NamesEnd();
        for (; it != end; ++it)
            if (it->data == id.data)
                break;
        return it;
    }
};
static_assert(__builtin_offsetof(UnkPropBagLayout_aa3b9c, file) == 0x28, "file");
static_assert(__builtin_offsetof(UnkPropBagLayout_aa3b9c, names) == 0x60, "names");

namespace unk_prop_bag {

void fn_7100aa3b9c(UnkPropBag* bag, const CStrId& key, u32 value) {
    UnkPropBagLayout_aa3b9c* self = reinterpret_cast<UnkPropBagLayout_aa3b9c*>(bag);
    UnkPropRecord_aa3b9c* record = self->FindRecord(key);
    u64 position = fn_710007c1a4(&self->file);
    if (record == self->RecordsEnd())
        self->records.PushBackCopy(UnkPropRecord_aa3b9c(position, HashOf(key), 0x69));
    else
        position = record->position;
    CRntFile_Seek(&self->file, position, 0);
    CRntFile_WriteElements(&self->file, &value, 4, 1);
    if (self->FindName(key) == self->NamesEnd())
        self->names.PushBack(key);
}

}  // namespace unk_prop_bag
