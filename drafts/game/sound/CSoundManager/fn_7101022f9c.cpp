// sound::CSoundManager unit (tools/tu_map.py, unit 2646): rebuilds the index from string id to the owned
// objects whose tables hold that id: clears it, then adds each object to the list of every id in its
// table. Names and layouts are placeholders.
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU32.h"
#include "types.h"

using base::global::CStrId;

// A 0x18-byte record of an object's table (fn_7100026f70 destroys it).
struct UnkSoundRecord_l022f9c {
    u8 unk_0x0[0x18];

    ~UnkSoundRecord_l022f9c() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }
};

// The owned object, as far as its table: the records and their ids, sharing one count.
struct UnkSoundOwned_l022f9c {
    void* owner;
    UnkSoundRecord_l022f9c* records;  // 0x8
    CStrId* ids;                      // 0x10
    u32 count;                        // 0x18
};

// An iterator over such a table: its current value refers to an id and its record.
struct UnkSoundTableIter_l022f9c {
    base::UnkIterValue<UnkSoundRecord_l022f9c, CStrId> current;

    UnkSoundTableIter_l022f9c(CStrId* id, UnkSoundRecord_l022f9c* record) { current.SetRef(id, record); }

    const CStrId& Id() { return current.Index(); }
    bool AtEnd(const UnkSoundOwned_l022f9c* owned) const { return current.ref.index == owned->ids + owned->count; }
    void Next() {
        ++current.ref.index;
        ++current.ref.element;
    }
};

using UnkSoundOwnedList_l022f9c = base::global::CRntVector<UnkSoundOwned_l022f9c*>;

// From string id to the objects holding it: two parallel arrays sharing one count.
struct UnkSoundIndex_l022f9c {
    UnkSoundOwnedList_l022f9c* lists;  // 0x0
    CStrId* ids;                       // 0x8
    u32 count;                         // 0x10
    u32 capacity;                      // 0x14
    u32 step;                          // 0x18

    UnkSoundOwnedList_l022f9c* End() { return &lists[count]; }
    UnkSoundOwnedList_l022f9c* Find(const CStrId& id) {
        for (u64 i = 0; i < count; ++i)
            if (ids[i].data == id.data)
                return &lists[i];
        return End();
    }
};

template <class V>
struct UnkIdMap;
struct UnkOwnedList;
void fn_710102bb48(UnkIdMap<UnkOwnedList>* self, u32 capacity);
struct UnkTable_0x28;
void fn_71010287d4(UnkTable_0x28* self);

// Adds an empty list under `id` and returns it.
static inline UnkSoundOwnedList_l022f9c* Add(UnkSoundIndex_l022f9c* index, const CStrId& id) {
    if (index->count == index->capacity)
        fn_710102bb48(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(index), index->count + index->step);
    new (&index->ids[index->count]) CStrId(id);
    new (&index->lists[index->count]) UnkSoundOwnedList_l022f9c(0xf);
    u32 n = ++index->count;
    UnkSoundOwnedList_l022f9c* end = &index->lists[n];
    return n ? &end[-1] : end;
}

struct UnkSoundEvents_l022f9c {
    u8 pad_0x0[0x58];
    UnkSoundOwned_l022f9c** objects;  // 0x58
    u32 object_count;                 // 0x60
    u8 pad_0x64[0x80 - 0x64];
    UnkSoundIndex_l022f9c index;  // 0x80
};

void fn_7101022f9c(UnkSoundEvents_l022f9c* self) {
    fn_71010287d4(reinterpret_cast<UnkTable_0x28*>(&self->index));
    UnkSoundOwned_l022f9c** end = self->objects + self->object_count;
    for (UnkSoundOwned_l022f9c** it = self->objects; it != end; ++it) {
        for (UnkSoundTableIter_l022f9c i((*it)->ids, (*it)->records); !i.AtEnd(*it); i.Next()) {
            UnkSoundOwnedList_l022f9c* list = self->index.Find(i.Id());
            if (list == self->index.End())
                list = Add(&self->index, i.Id());
            list->PushBack(*it);
        }
    }
}
