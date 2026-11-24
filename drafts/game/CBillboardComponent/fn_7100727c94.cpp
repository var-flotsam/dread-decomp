// CBillboardComponent's unit (tools/tu_map.py): takes the owner out of the scene's list of
// billboard owners, then detaches both of the billboard's entries (ids at +0x48 and +0x4c) in the
// billboard manager g_unk_71726da048, freeing the entry array at +0x50. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2560.h"
#include "types.h"

void fn_7100e1c9d8(void* manager, u32 id);

// Declared as src/game/CBillboardComponent/CBillboardComponent_b40.cpp does: the destructor of
// that unit's 0x148-byte entries.
struct UnkEntry_bbd {
    u8 data[0x148];
    ~UnkEntry_bbd() asm("fn_7100745b24");
};

// A fixed-capacity list of owners (the count, then the items).
struct UnkOwnerList_l727c94 {
    u32 count;
    u8 storage[32 * sizeof(void*)];

    void** items() { return reinterpret_cast<void**>(storage); }

    // Grows or shrinks to `n` items, the new ones null.
    void Resize(u32 n) {
        u32 old = count;
        if (old < n) {
            void** p = items();
            __builtin_memset(p + old, 0, (u64)(n - old) * sizeof(void*));
        } else if (old == n) {
            return;
        }
        count = n;
    }

    void** begin() { return items(); }
    void** end() { return items() + count; }

    void** Find(void* item) {
        void** it = begin();
        for (void** last = end(); it != last; ++it)
            if (*it == item)
                break;
        return it;
    }

    // Removes the item at `it`, moving the ones after it down (copied bitwise, as the engine's
    // arrays copy their elements).
    void Erase(void** it) {
        for (void** next = it + 1; next != end(); ++next)
            __builtin_memcpy(next - 1, next, sizeof(void*));
        Resize(count - 1);
    }

    void Remove(void* item) {
        void** it = Find(item);
        if (it != end())
            Erase(it);
    }
};

struct UnkScene_l727c94 {
    u8 pad_0x0[0x2b0];
    struct UnkBillboardOwners_l727c94* field_0x2b0;
};

struct UnkBillboardOwners_l727c94 {
    u8 pad_0x0[0x848];
    UnkOwnerList_l727c94 list;
};

struct UnkBillboardActor_l727c94 {
    u8 pad_0x0[0x2a0];
    void* field_0x2a0;
};

struct UnkBillboardEntry_l727c94 {
    u8 pad_0x0[0x10];
    void* data;  // 0x10
    u8 pad_0x18[0x18];
    u64 field_0x30[4];
};

struct UnkBillboard_l727c94 {
    u8 pad_0x0[0x30];
    UnkBillboardActor_l727c94* owner;
    u8 pad_0x38[0x10];
    u32 id[2];  // 0x48
    UnkEntry_bbd* entries;  // 0x50
};

static inline void* BillboardManager() {
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        ConstructBillboardManager(manager);
        g_unk_71726da048 = manager;
    }
    return manager;
}

static inline void Detach(UnkBillboard_l727c94* self, u32* id) {
    auto* entry = reinterpret_cast<UnkBillboardEntry_l727c94*>(fn_7100e1caf0(BillboardManager(), *id));
    if (!entry)
        return;
    entry->field_0x30[0] = 0;
    entry->field_0x30[1] = 0;
    entry->field_0x30[2] = 0;
    entry->field_0x30[3] = 0;
    if (entry->data) {
        if (self->entries) {
            delete[] self->entries;
            self->entries = nullptr;
        }
        fn_7100e1c9d8(BillboardManager(), *id);
        *id = 0xffffffff;
        entry->data = nullptr;
    }
}

void fn_7100727c94(UnkBillboard_l727c94* self) {
    auto* scene = reinterpret_cast<UnkScene_l727c94*>(fn_7100defcd4(g_unk_7101d47310));
    if (scene) {
        if (UnkBillboardOwners_l727c94* owners = scene->field_0x2b0)
            owners->list.Remove(self->owner->field_0x2a0);
    }
    Detach(self, &self->id[0]);
    Detach(self, &self->id[1]);
}
