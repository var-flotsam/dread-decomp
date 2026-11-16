// Unit 2602: looks a string id up in a hashed table, then, when it is not there, in the music manager's
// own table (+0x1c0; the manager g_unk_71726bc560 is created on first use); gives the entry's value or
// null. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkHashedStr_lf89818 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkEntryValue_lf89818 {
    u8 pad_0x0[0x78];
};

struct UnkEntry_lf89818 {
    UnkStringData* key;
    UnkEntryValue_lf89818 value;  // 0x8
    UnkEntry_lf89818* next;       // 0x80
};

struct UnkTable_lf89818 {
    u8 pad_0x0[0x10];
    UnkEntry_lf89818** buckets;
    u32 count;
    u32 bucket_count;

    UnkEntry_lf89818** BucketFor(const CStrId* key) {
        if (count == 0)
            return nullptr;
        const UnkHashedStr_lf89818* h = reinterpret_cast<const UnkHashedStr_lf89818*>(key->data);
        u32 index = (h->hash_lo ^ h->hash_hi) % bucket_count;
        return buckets + index;
    }
    UnkEntry_lf89818* Find(const CStrId* key) {
        UnkEntry_lf89818** slot = BucketFor(key);
        if (!slot)
            return nullptr;
        for (UnkEntry_lf89818* n = *slot; n; n = n->next)
            if (n->key == key->data)
                return n;
        return nullptr;
    }
    bool Contains(const CStrId* key) { return Find(key) != nullptr; }
    UnkEntryValue_lf89818* Get(const CStrId* key) { return &Find(key)->value; }
};

struct UnkMusicManager_lf89818 {
    u8 pad_0x0[0x1c0];
    UnkTable_lf89818 table;  // 0x1c0
};

// The music manager, created on first use.
static inline UnkMusicManager_lf89818* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return static_cast<UnkMusicManager_lf89818*>(manager);
}

void* fn_7100f89818(void* object, const CStrId* key) {
    UnkTable_lf89818* table = static_cast<UnkTable_lf89818*>(object);
    if (table->Contains(key))
        return table->Get(key);
    if (MusicManager()->table.Contains(key))
        return MusicManager()->table.Get(key);
    return nullptr;
}
