// CCollisionComponent unit (tools/tu_map.py unit 1319): removes an item from the set kept under a key,
// dropping the key when its set empties, and takes the key back from every record object and from the
// collider groups on the given layers. Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/CCollisionComponent.h"
#include "types.h"

using base::global::CRntSmallDictionary;
using base::global::CRntVector;

void fn_7100d35be4(void* object, u32 key, void* owner);

struct UnkCollisionContainer_l864374;
struct UnkCollisionRecord_l864374;
void fn_7100864374(UnkCollisionContainer_l864374* self, u32 mask,
                   base::global::CRntVector<UnkCollisionRecord_l864374*>* out, bool all);

// Up to eight items, stored in place right after the count.
#pragma pack(push, 4)
struct UnkItemSet_l8659c4 {
    u32 count;
    void* items[8];

    UnkItemSet_l8659c4(const UnkItemSet_l8659c4& other) {
        __builtin_memcpy(items, other.items, other.count * sizeof(void*));
        count = other.count;
    }
    ~UnkItemSet_l8659c4() { count = 0; }

    void** begin() { return items; }
    void** end() { return items + count; }

    // Grows with null items or shrinks to `n` items.
    void Resize(u32 n) {
        if (n > count)
            __builtin_memset(&items[count], 0, (u64)(n - count) * sizeof(void*));
        count = n;
    }

    // Removes the item at `it`, moving the last one into its place.
    void Erase(void** it) {
        void** last = end() - 1;
        if (last != it)
            *it = *last;
        Resize(count - 1);
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkItemSet_l8659c4) == 0x44, "UnkItemSet_l8659c4 layout");

using UnkItemMap_l8659c4 = CRntSmallDictionary<u32, UnkItemSet_l8659c4>;

// Removes `key` and its set, moving the last entry into its place.
static inline void EraseKey(UnkItemMap_l8659c4& map, u32 key) {
    UnkItemSet_l8659c4* value = map.Find(key);
    if (value == map.End())
        return;
    u32 i = value - map.values;
    map.values[i].~UnkItemSet_l8659c4();
    u32 last = map.count - 1;
    if (last != i) {
        new (&map.values[i]) UnkItemSet_l8659c4(map.values[last]);
        map.values[map.count - 1].~UnkItemSet_l8659c4();
        map.keys[i] = map.keys[map.count - 1];
    }
    --map.count;
}

struct UnkCollisionRecord_l8659c4 {
    u8 pad_0x0[0x28];
    void* object;  // 0x28
};

struct UnkCollisionGroup_l8659c4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 GetLayers();  // vf_0x58
};

struct UnkCollisionComponent_l8659c4 {
    u8 pad_0x0[0x40];
    u8 field_0x40[0x8];
    u8 pad_0x48[0x28];
    CRntVector<UnkCollisionGroup_l8659c4*> groups;  // 0x70
    u8 pad_0x98[0x40];
    UnkItemMap_l8659c4 itemsByKey;  // 0xd8
    bool field_0x100;               // 0x100
};
static_assert(__builtin_offsetof(UnkCollisionComponent_l8659c4, field_0x100) == 0x100,
              "UnkCollisionComponent_l8659c4 layout");

void fn_71008659c4(UnkCollisionContainer* container, u32 mask, u32 key, void* item) {
    auto* self = reinterpret_cast<UnkCollisionComponent_l8659c4*>(container);
    UnkItemMap_l8659c4& map = self->itemsByKey;
    if (map.Find(key) != map.End()) {
        u32 i;
        for (i = 0; i < map.count; ++i)
            if (map.keys[i] == key)
                break;
        UnkItemSet_l8659c4& set = map.values[i];
        void** end = set.end();
        void** it = set.begin();
        for (; it != end; ++it)
            if (*it == item)
                break;
        if (it != end) {
            set.Erase(it);
            if (set.count == 0)
                EraseKey(map, key);

            CRntVector<UnkCollisionRecord_l864374*> records(7);
            fn_7100864374(reinterpret_cast<UnkCollisionContainer_l864374*>(self), mask, &records, true);
            UnkCollisionRecord_l864374** last = records.data + records.count;
            for (UnkCollisionRecord_l864374** r = records.data; r != last; ++r)
                fn_7100d35be4(reinterpret_cast<UnkCollisionRecord_l8659c4*>(*r)->object, key, self->field_0x40);
            for (UnkCollisionGroup_l8659c4** g = self->groups.data; g != self->groups.data + self->groups.count;
                 ++g) {
                if ((*g)->GetLayers() & mask)
                    fn_7100d35be4(*g, key, self->field_0x40);
            }
            records.Free();
        }
    }
    self->field_0x100 = map.count != 0;
}
