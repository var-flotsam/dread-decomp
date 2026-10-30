// The unit of the string-id/file-path pair (base/UnkStrIdPathPair.h; its destructors are in
// src/game_s1/fn_710001cbd8.cpp): parsing a pair from "id:path" text (fn_710001a8b4, what the
// pair's constructor from a string, fn_710001f074, ends with), and two setters of a table keyed by
// string ids whose values are 0x80-byte records with a list of file paths and one file path
// (fn_710001acf8 appends to the list, fn_710001ae44 sets the path; both add the key first when it
// is missing).

#include "../game_aca57c/UnkBufferString.h"
#include "base/UnkDestructors.h"
#include "base/UnkResPathVec.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// Finds `c` from `start` on; -1 when absent (src/base/global/CRntString.cpp).
s32 fn_710007facc(const CRntString* self, char c, u32 start);

// A counted prefix of a C string, on 0x100 bytes of inline storage.
struct UnkPairText : UnkBufferString<0x100, g_unk_7101a194b8> {
    __attribute__((always_inline)) UnkPairText(const char* str, u32 length) {
        fn_710007e7f8(Str(), str, length);
    }
};

// The ids a string's text gives (fn_7100000850 on the default pool, fn_710001a270 on the
// file-path pool), as temporaries that are moved out of.
template <UnkStringPool*& Pool>
struct UnkTextId {
    UnkStringData* data;

    explicit UnkTextId(const UnkPairText& text) {
        if (&Pool == &g_unk_7101d47108)
            fn_7100000850(reinterpret_cast<UnkStrIdBase*>(this), *text.Str(), true);
        else
            fn_710001a270(reinterpret_cast<UnkFilePathStrIdBase*>(this), *text.Str(), true);
    }
    ~UnkTextId() {
        if (UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = &pool->empty;
        }
    }
};

// Releases an id in two steps (the id and its count reloaded after the decrement, the pool after
// a release), leaving the empty id when there is a pool.
template <UnkStringPool*& Pool>
static inline void UnkResetId(base::UnkStrIdMemberOf<Pool>& id) {
    if (UnkStringPool* pool = Pool) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                fn_71000803a4(pool, id.data);
                pool = Pool;
            }
        }
        id.data = &pool->empty;
    }
}

// The id's move-assignment: released as UnkResetId does, then the temporary's taken (CStrId's
// ReleaseAndTake).
static inline void UnkTakeId(base::UnkStrIdMember& id, UnkTextId<g_unk_7101d47108>&& other) {
    UnkResetId(id);
    id.data = other.data;
    other.data = &g_unk_7101d47108->empty;
}

// The path's: the release without the reset (UnkStrIdBase::MoveFrom on the file-path pool).
static inline void UnkTakeId(base::UnkStrIdMemberOf<g_unk_7101d47200>& id,
                             UnkTextId<g_unk_7101d47200>&& other) {
    UnkStringPool* pool = g_unk_7101d47200;
    if (pool) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                fn_71000803a4(pool, id.data);
                pool = g_unk_7101d47200;
            }
        }
    }
    id.data = other.data;
    other.data = &g_unk_7101d47200->empty;
}

// Splits `text` at its first ':' (a colon at the very start does not count) into the string id
// before it and the file path after it. Without one both are emptied.
void fn_710001a8b4(UnkStrIdPathPair* self, const CRntString* text) {
    s32 colon = fn_710007facc(text, ':', 0);
    if (colon > 0) {
        UnkTakeId(self->id, UnkTextId<g_unk_7101d47108>(UnkPairText(text->data, colon)));
        u32 start = colon + 1;
        u32 rest = text->length - start;
        UnkTakeId(self->path, UnkTextId<g_unk_7101d47200>(UnkPairText(text->data + start, rest)));
    } else {
        UnkResetId(self->id);
        UnkResetId(self->path);
    }
}

typedef base::UnkStrIdMemberOf<g_unk_7101d47200> UnkPathId;

// The table's value. The insert (fn_710001ab7c) builds the two tables at the start empty (the
// layout of base/UnkPairArrays.h, growth step 10, flags 0xe) and the list with flags 9.
struct UnkPathListRecord {
    u8 field_0x0[0x28];
    u8 field_0x28[0x28];
    base::UnkVector<CFilePathStrId> paths;  // 0x50
    UnkPathId path;                         // 0x78
};
static_assert(sizeof(UnkPathListRecord) == 0x80, "UnkPathListRecord layout");

// Values and keys in two arrays that share the count (the growth, fn_7100017948, takes the step
// at 0x18).
struct UnkPathListTable {
    UnkPathListRecord* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;

    UnkPathListRecord* Find(const CStrId& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return &values[count];
    }
    UnkPathListRecord* End() { return &values[count]; }
};

// Appends the key with an empty record and returns the record.
UnkPathListRecord* fn_710001ab7c(UnkPathListTable* self, const CStrId* key);

static inline UnkPathListRecord* UnkFindOrAdd(UnkPathListTable* self, const CStrId* key) {
    UnkPathListRecord* record = self->Find(*key);
    if (record == self->End())
        record = fn_710001ab7c(self, key);
    return record;
}

// Appends each of `paths` to the record of `key`.
void fn_710001acf8(UnkPathListTable* self, const CStrId* key,
                   const base::UnkVector<CFilePathStrId>* paths) {
    UnkPathListRecord* record = UnkFindOrAdd(self, key);
    const CFilePathStrId* end = paths->data + paths->count;
    for (const CFilePathStrId* it = paths->data; it != end; ++it) {
        base::UnkVector<CFilePathStrId>& list = record->paths;
        u32 n = list.count;
        u32 new_capacity = list.capacity;
        if (new_capacity <= n && list.step != 0)
            new_capacity = (n / list.step + 1) * list.step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_7100018450(reinterpret_cast<UnkResPathVec*>(&list), new_capacity);
        // Copied bitwise, then the reference taken: the pool is reloaded after the store.
        CFilePathStrId* slot = &list.data[list.count];
        UnkStringData* value = it->data;
        __builtin_memcpy(slot, &value, sizeof(value));
        if (&g_unk_7101d47200->empty != value)
            ++value->ref_count;
        ++list.count;
    }
}

// Sets the path of the record of `key`.
void fn_710001ae44(UnkPathListTable* self, const CStrId* key, const UnkPathId* path) {
    UnkFindOrAdd(self, key)->path = *path;
}
