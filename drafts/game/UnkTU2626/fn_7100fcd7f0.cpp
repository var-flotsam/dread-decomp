// Unit 2626: drops every entry of a keyed sound-handle table whose sound has ended, gathering the
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
struct UnkSoundHandleTable_2626 {
    u8 pad_0x0[0x10];
    UnkSoundHandle* handles;  // 0x10
    u64* keys;                // 0x18
    u32 count;                // 0x20

    UnkSoundHandle* begin() { return handles; }
    UnkSoundHandle* end() { return handles + count; }
};

// The keys gathered for removal, kept between calls so the array is reused.
struct UnkKeyList_2626 {
    base::global::CRntVector<u64> keys;

    UnkKeyList_2626() : keys(0xf) {}
    ~UnkKeyList_2626();  // fn_7100fca440
};

// The object an entry's handle names, if it is still the one the handle was made for.
static inline UnkSoundObject_b50* Get_2626(const UnkSoundHandle& handle) {
    if (!handle.ref)
        return nullptr;
    UnkSoundObject_b50* object = nullptr;
    if (handle.serial != -1 && handle.ref->serial == handle.serial)
        object = handle.ref->object;
    return object;
}

// The object a handle names while it is still the one the handle was made for.
static inline UnkSoundObject_b50* Resolve_2626(const UnkSoundHandle& handle) {
    if (handle.ref && handle.ref->serial == handle.serial && handle.serial != -1)
        return handle.ref->object;
    return nullptr;
}

// The handle stored under `key`, or one past the last on a miss.
static inline UnkSoundHandle* Find_2626(UnkSoundHandleTable_2626* table, u64 key) {
    for (u32 i = 0; i < table->count; ++i)
        if (table->keys[i] == key)
            return &table->handles[i];
    return &table->handles[table->count];
}

// Removes entry i by moving the last entry into its place.
static inline void EraseAt_2626(UnkSoundHandleTable_2626* table, u32 i) {
    u32 last = table->count - 1;
    if (i != last) {
        table->handles[i] = table->handles[last];
        table->keys[i] = table->keys[table->count - 1];
    }
    table->count = table->count - 1;
}

void fn_7100fcd7f0(void* self) {
    UnkSoundHandleTable_2626* table = static_cast<UnkSoundHandleTable_2626*>(self);
    static UnkKeyList_2626 s_ended;
    base::global::CRntVector<u64>& ended = s_ended.keys;

    ended.count = 0;
    if (table->count == 0)
        return;

    UnkSoundHandle* end = table->end();
    for (UnkSoundHandle* it = table->begin(); it < end; ++it) {
        if (Get_2626(*it) == Resolve_2626(g_unk_7108b6c460)) {
            u32 index = it - table->handles;
            ended.PushBack(table->keys[index]);
        }
    }

    for (u64 key : ended) {
        UnkSoundHandle* found = Find_2626(table, key);
        if (found != &table->handles[table->count])
            EraseAt_2626(table, found - table->handles);
    }
}
