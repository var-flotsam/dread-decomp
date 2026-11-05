// CPlaythrough's unit: replaces the owned record stored for a key in a flat map (parallel value
// and key arrays) with the one passed in, then destroys the old record (its owned object at +0x30,
// its string id at +0x8) and frees it. Names are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkOwnedB56 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRecordB56 {
    u8 pad_0x0[0x8];
    UnkStringData* name;  // 0x8
    u8 pad_0x10[0x20];
    UnkOwnedB56* owned;  // 0x30
};

// A 64-bit key, passed by pointer: a reference (dereferenceable) has its load hoisted above the keys array.
struct UnkKeyB56 {
    u64 value;
    bool operator==(const UnkKeyB56& o) const { return value == o.value; }
};

struct UnkRecordMapB56 {
    UnkRecordB56** values;  // 0x0
    UnkKeyB56* keys;        // 0x8
    u32 count;              // 0x10
};

static inline UnkRecordB56** Find(UnkRecordMapB56* map, const UnkKeyB56* key) {
    for (u64 i = 0; i != map->count; ++i) {
        if (map->keys[i] == *key) {
            return &map->values[i];
        }
    }
    return map->values + map->count;
}

void fn_71002eefd8(UnkRecordMapB56* map, const UnkKeyB56* key, UnkRecordB56*& record) {
    UnkRecordB56** it = Find(map, key);
    if (it == map->values + map->count) {
        return;
    }
    UnkRecordB56* incoming = record;
    record = nullptr;
    UnkRecordB56* old = *it;
    *it = incoming;
    if (old == nullptr) {
        return;
    }
    UnkOwnedB56* owned = old->owned;
    old->owned = nullptr;
    if (owned != nullptr) {
        owned->vf_0x10();
    }
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData* name = old->name;
        if (name && name != &pool->empty) {
            --old->name->ref_count;
            if (old->name->ref_count == 0) {
                fn_71000803a4(pool, old->name);
            }
        }
    }
    fn_7100083c00(old);
}
