// Reserve of maps from string ids to owned lists (two parallel arrays, as in
// src/game_s7/fn_7100aee918.cpp's UnkIdMap): grows both arrays, moving the lists and the ids.
// The same code for 14 maps; fn_7100b48944 (src/game_aca57c/fn_7100eb1824.cpp) is another copy.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    // The pointer is stored bytewise (char alias info), so the destructor of the moved-from id
    // reloads the pool.
    UnkReleasedStrId(UnkReleasedStrId&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

template <class V>
struct UnkIdMap {
    V* values;
    UnkReleasedStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Grows both arrays, moving the values (destroying the old ones with Destroy) and the keys.
template <class V, void (*Destroy)(V*)>
__attribute__((always_inline)) static inline void Reserve(UnkIdMap<V>* self, u32 capacity) {
    if (self->capacity < capacity) {
        V* values = static_cast<V*>(self->allocator->vf_0x40((u64)capacity * sizeof(V), 8, self->flags));
        auto* keys = static_cast<UnkReleasedStrId*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkReleasedStrId), 8, self->flags));
        if (V* src = self->values) {
            V* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) V(static_cast<V&&>(*src));
                Destroy(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (UnkReleasedStrId* src = self->keys) {
            UnkReleasedStrId* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkReleasedStrId(static_cast<UnkReleasedStrId&&>(*src));
                src->~UnkReleasedStrId();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}


inline void DestroyList(UnkOwnedList* list) { list->~UnkOwnedList(); }


void fn_71001358e0(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_710027ee38(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_710027efe8(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_710027f198(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_710027f54c(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_710027f6fc(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_710027fe8c(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_7100760774(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_71007ed0ac(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_71009ba838(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_71009c4158(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_7100e06048(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_710102bb48(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_710102c0e8(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
void fn_7101157f00(UnkIdMap<UnkOwnedList>* self, u32 capacity) { Reserve<UnkOwnedList, DestroyList>(self, capacity); }
