// Unit 2645: drops every entry of a keyed sound-handle table whose sound has ended, gathering the
// keys first in a static list. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

struct UnkSoundObject_b50;

struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

// Sound handles kept in one array and their keys in a parallel one.
struct UnkSoundHandleTable_2645 {
    u8 pad_0x0[0x10];
    UnkSoundHandle* handles;  // 0x10
    u64* keys;                // 0x18
    u32 count;                // 0x20

    UnkSoundHandle* begin() { return handles; }
    UnkSoundHandle* end() { return handles + count; }
};

// The keys gathered for removal, kept between calls so the array is reused.
struct UnkKeyList_2645 {
    base::global::CRntVector<u64> keys;

    UnkKeyList_2645() : keys(0xf) {}
    ~UnkKeyList_2645();  // fn_7100fca440
};

// Whether the handle still names the object it was made for.
static inline bool IsLive_2645(const UnkSoundHandle& handle) {
    return handle.ref && handle.serial != -1 && handle.ref->serial == handle.serial;
}

// The object a handle names while it is still the one the handle was made for.
static inline UnkSoundObject_b50* Resolve_2645(const UnkSoundHandle& handle) {
    if (handle.ref && handle.ref->serial == handle.serial && handle.serial != -1)
        return handle.ref->object;
    return nullptr;
}

// The handle stored under `key`, or one past the last on a miss.
static inline UnkSoundHandle* Find_2645(UnkSoundHandleTable_2645* table, u64 key) {
    for (u32 i = 0; i < table->count; ++i)
        if (table->keys[i] == key)
            return &table->handles[i];
    return &table->handles[table->count];
}

// Removes entry i by moving the last entry into its place.
static inline void EraseAt_2645(UnkSoundHandleTable_2645* table, u32 i) {
    u32 last = table->count - 1;
    if (i != last) {
        table->handles[i] = table->handles[last];
        table->keys[i] = table->keys[table->count - 1];
    }
    table->count = table->count - 1;
}

void fn_710101e7d8(void* self) {
    UnkSoundHandleTable_2645* table = static_cast<UnkSoundHandleTable_2645*>(self);
    static UnkKeyList_2645 s_ended;
    base::global::CRntVector<u64>& ended = s_ended.keys;

    ended.count = 0;
    if (table->count == 0)
        return;

    UnkSoundHandle* it = table->begin();
    UnkSoundHandle* end = table->end();
    do {
        UnkSoundObject_b50* object = IsLive_2645(*it) ? it->ref->object : nullptr;
        if (object == Resolve_2645(g_unk_7108b6c460)) {
            u32 index = it - table->handles;
            ended.PushBack(table->keys[index]);
        }
    } while (++it != end);

    for (u64 key : ended) {
        UnkSoundHandle* found = Find_2645(table, key);
        if (found != &table->handles[table->count])
            EraseAt_2645(table, found - table->handles);
    }
}
