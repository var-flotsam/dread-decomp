// CPlayerInfo's unit (tools/tu_map.py unit 2592): sets a property of a timed entry: finds (or adds, starting
// at the frame timer's time) the entry under `name` in the map at +0x320, then finds (or adds) its value under
// `key` and hands it `value` (fn_710002fd10). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU0.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkTimedMap_ovb0071;
void CPlayerInfo_ReserveTimedEntryMap(UnkTimedMap_ovb0071* self, u32 capacity);
void fn_7100f0f980(UnkPropertyArrays* self, u32 capacity);

// The frame timer as this function reads it.
struct UnkFrameTimer_lf0e4e4 {
    u8 pad_0x0[0x30];
    f32 time;  // 0x30
};

struct UnkPlayerValue_lf0e4e4 {
    u8 raw[0xb0];
};

// A timed entry's properties: values (0xb0 bytes each) and their ids sharing a count.
struct UnkPlayerProps_lf0e4e4 {
    UnkPlayerValue_lf0e4e4* values;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;

    explicit UnkPlayerProps_lf0e4e4(base::IAllocator* allocator)
        : values(nullptr), ids(nullptr), count(0), capacity(0), step(10), flags(2), allocator(allocator) {}

    // Releases the ids and destroys the values, keeping the arrays.
    void Clear() {
        if (ids && count) {
            for (u64 i = 0; i < count; ++i) {
                UnkStringPool* pool = g_unk_7101d47108;
                if (!pool)
                    continue;
                CStrId* list = ids;
                UnkStringData* data = list[i].data;
                if (data && data != &pool->empty) {
                    --data->ref_count;
                    if (list[i].data->ref_count == 0) {
                        StringPool_Release(pool, list[i].data);
                        pool = g_unk_7101d47108;
                    }
                }
                list[i].data = &pool->empty;
            }
        }
        if (values) {
            for (u32 i = 0; i < count; ++i)
                fn_710002f67c(&values[i]);
        }
        count = 0;
    }

    UnkPlayerValue_lf0e4e4* Find(const CStrId& key) {
        for (u32 i = 0; i < count; ++i) {
            if (ids[i].data == key.data)
                return &values[i];
        }
        return &values[count];
    }

    UnkPlayerValue_lf0e4e4* Add(const CStrId& key) {
        UnkPlayerValue_lf0e4e4 value;
        fn_710002f4e4(&value);
        if (count == capacity)
            fn_7100f0f980(reinterpret_cast<UnkPropertyArrays*>(this), count + step);
        new (&ids[count]) CStrId(key);
        ConstructIdListCopyingNodesAndMovingCallbacks(&values[count], &value);
        u32 n = ++count;
        UnkPlayerValue_lf0e4e4* end = &values[n];
        UnkPlayerValue_lf0e4e4* added = n ? &end[-1] : end;
        fn_710002f67c(&value);
        return added;
    }
};

struct UnkTimedHead_lf0e4e4 {
    u8 counter;
    f32 time;
};

struct UnkTimedEntry_lf0e4e4 {
    UnkTimedHead_lf0e4e4 head;
    UnkPlayerProps_lf0e4e4 properties;  // 0x8

    explicit UnkTimedEntry_lf0e4e4(base::IAllocator* allocator) : head(), properties(allocator) {}
};

struct UnkTimedMap_lf0e4e4 {
    UnkTimedEntry_lf0e4e4* values;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;

    UnkTimedEntry_lf0e4e4* Find(const CStrId& key) {
        for (u32 i = 0; i < count; ++i) {
            if (ids[i].data == key.data)
                return &values[i];
        }
        return &values[count];
    }

    // Adds an entry under key whose properties allocate through llocator.
    UnkTimedEntry_lf0e4e4* Add(const CStrId& key, base::IAllocator* allocator) {
        if (count == capacity)
            CPlayerInfo_ReserveTimedEntryMap(reinterpret_cast<UnkTimedMap_ovb0071*>(this), count + step);
        new (&ids[count]) CStrId(key);
        new (&values[count]) UnkTimedEntry_lf0e4e4(allocator);
        u32 n = ++count;
        UnkTimedEntry_lf0e4e4* end = &values[n];
        return n ? &end[-1] : end;
    }
};

struct UnkPlayerInfoTimed_lf0e4e4 {
    u8 pad_0x0[0x320];
    UnkTimedMap_lf0e4e4 timed;  // 0x320
};

void fn_7100f0e4e4(void* a, void* b, void* c, void* d) {
    UnkPlayerInfoTimed_lf0e4e4* self = static_cast<UnkPlayerInfoTimed_lf0e4e4*>(a);
    const CStrId& name = *static_cast<const CStrId*>(b);
    const CStrId& key = *static_cast<const CStrId*>(c);
    UnkTimedMap_lf0e4e4& timed = self->timed;
    UnkTimedEntry_lf0e4e4* entry = timed.Find(name);
    if (entry == &timed.values[timed.count]) {
        entry = timed.Add(name, GetDefaultAllocator());
        entry->head.counter = 0;
        entry->properties.Clear();
        entry->head.time = reinterpret_cast<UnkFrameTimer_lf0e4e4*>(g_unk_7101d47328)->time;
    }
    UnkPlayerProps_lf0e4e4& properties = entry->properties;
    UnkPlayerValue_lf0e4e4* value = properties.Find(key);
    if (value == &properties.values[properties.count])
        value = properties.Add(key);
    fn_710002fd10(reinterpret_cast<UnkGuiKeyList*>(value), static_cast<CStrId*>(d));
    entry->head.counter = 0;
}
