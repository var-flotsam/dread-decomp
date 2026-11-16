// Unit 800 (CEmmyGrabSamusAttack hint): sets a bool under a string id in a property bag: writes it into
// the bag's file at the position recorded for the id (recording the file's end for a new id), then
// remembers the id. Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "types.h"

#include <algorithm>


using base::global::CStrId;

// The pooled string's 64-bit hash at 0x30.
struct UnkHashedStringKey_50f4ac {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline u64 HashOf(const CStrId& id) {
    return reinterpret_cast<const UnkHashedStringKey_50f4ac*>(id.data)->hash;
}

// Where a value was written: its position in the file, the id's hash and the value's kind.
struct UnkPropRecord_50f4ac {
    u64 position;
    u64 hash;
    u32 kind;

    UnkPropRecord_50f4ac(u64 position, u64 hash, u32 kind) : position(position), hash(hash), kind(kind) {}
    UnkPropRecord_50f4ac(const UnkPropRecord_50f4ac& other)
        : position(other.position), hash(other.hash), kind(other.kind) {}
};

// The property bag as this unit sees it.
struct UnkPropBagLayout_50f4ac {
    base::global::CRntVector<UnkPropRecord_50f4ac> records;
    base::global::CRntFile file;
    base::UnkStrIdVec names;

    UnkPropRecord_50f4ac* RecordsEnd() { return records.data + records.count; }
    UnkPropRecord_50f4ac* FindRecord(const CStrId& id) {
        UnkIter<UnkPropRecord_50f4ac> it =
            std::find_if(UnkIter<UnkPropRecord_50f4ac>{records.data}, UnkIter<UnkPropRecord_50f4ac>{RecordsEnd()},
                         [&id](const UnkPropRecord_50f4ac& r) { return r.hash == HashOf(id); });
        return it.p;
    }
    CStrId* NamesEnd() { return names.data + names.count; }
    CStrId* FindName(const CStrId& id) {
        u32 i = 0;
        for (; i < names.count; ++i)
            if (names.data[i].data == id.data)
                break;
        return &names.data[i];
    }
};
static_assert(__builtin_offsetof(UnkPropBagLayout_50f4ac, file) == 0x28, "file");
static_assert(__builtin_offsetof(UnkPropBagLayout_50f4ac, names) == 0x60, "names");

namespace unk_prop_bag {

void fn_710050f4ac(UnkPropBag* bag, const CStrId& key, bool value) {
    UnkPropBagLayout_50f4ac* self = reinterpret_cast<UnkPropBagLayout_50f4ac*>(bag);
    UnkPropRecord_50f4ac* record = self->FindRecord(key);
    u64 position = fn_710007c1a4(&self->file);
    if (record == &self->records.data[self->records.count])
        self->records.PushBackCopy(UnkPropRecord_50f4ac(position, HashOf(key), 0x62));
    else
        position = record->position;
    fn_710007c00c(&self->file, position, 0);
    fn_710007c450(&self->file, &value, 1, 1);
    if (self->FindName(key) == self->NamesEnd())
        self->names.PushBack(key);
}

}  // namespace unk_prop_bag
