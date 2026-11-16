// CCollisionComponent unit (tools/tu_map.py unit 1319): adds an item to the set kept under a key (at most
// eight per key), registers the key with the collision manager and hands the key to every record object and
// to the collider groups on the given layers. Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/CCollisionComponent.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::global::CRntSmallDictionary;
using base::global::CRntVector;

void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d49144(UnkManager_7100863a8c* self, u32 mask, u32 key);
void fn_7100d35b14(void* object, u32 key, void* owner);

struct UnkCollisionContainer_l864374;
struct UnkCollisionRecord_l864374;
void fn_7100864374(UnkCollisionContainer_l864374* self, u32 mask,
                   base::global::CRntVector<UnkCollisionRecord_l864374*>* out, bool all);

struct UnkCollisionNode_ovb0107;
void fn_71008633e0(UnkCollisionContainer* self, UnkCollisionNode_ovb0107* node, u32 flags);

static inline UnkManager_7100863a8c* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

// Up to eight items, stored in place right after the count.
#pragma pack(push, 4)
struct UnkItemSet_l8656d8 {
    u32 count;
    void* items[8];

    UnkItemSet_l8656d8() : count(0) {}
    void** begin() { return items; }
    void** end() { return items + count; }
};
#pragma pack(pop)
static_assert(sizeof(UnkItemSet_l8656d8) == 0x44, "UnkItemSet_l8656d8 layout");

using UnkItemMap_l8656d8 = CRntSmallDictionary<u32, UnkItemSet_l8656d8>;
void fn_7100866c74(UnkItemMap_l8656d8* self, u32 capacity);

struct UnkCollisionRecord_l8656d8 {
    u8 pad_0x0[0x28];
    void* object;  // 0x28
};

struct UnkCollisionGroup_l8656d8 {
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

struct UnkCollisionComponent_l8656d8 {
    u8 pad_0x0[0x40];
    u8 field_0x40[0x8];
    u8 pad_0x48[0x28];
    CRntVector<UnkCollisionGroup_l8656d8*> groups;  // 0x70
    u8 pad_0x98[0x40];
    UnkItemMap_l8656d8 itemsByKey;  // 0xd8
    bool field_0x100;               // 0x100
};
static_assert(__builtin_offsetof(UnkCollisionComponent_l8656d8, field_0x100) == 0x100,
              "UnkCollisionComponent_l8656d8 layout");

// The set under `key`, added empty when there is none.
static inline UnkItemSet_l8656d8& SetFor(UnkItemMap_l8656d8& map, u32 key) {
    if (map.Find(key) == map.End()) {
        if (map.count == map.capacity)
            fn_7100866c74(&map, map.capacity + map.step);
        map.keys[map.count] = key;
        new (&map.values[map.count]) UnkItemSet_l8656d8();
        ++map.count;
    }
    u64 i;
    for (i = 0; i < map.count; ++i)
        if (map.keys[i] == key)
            break;
    return map.values[i];
}

void fn_71008656d8(void* object, u32 mask, u32 key, void* item) {
    auto* self = static_cast<UnkCollisionComponent_l8656d8*>(object);
    UnkItemSet_l8656d8& set = SetFor(self->itemsByKey, key);
    void** end = set.end();
    void** it = set.begin();
    for (; it != end; ++it)
        if (*it == item)
            break;
    if (set.count > 7 || it != end)
        return;

    fn_7100d49144(Manager(), mask, key);
    set.items[set.count] = item;
    ++set.count;
    self->field_0x100 = true;

    CRntVector<UnkCollisionRecord_l864374*> records(0xf);
    fn_7100864374(reinterpret_cast<UnkCollisionContainer_l864374*>(self), mask, &records, true);
    UnkCollisionRecord_l864374** last = records.data + records.count;
    for (UnkCollisionRecord_l864374** r = records.data; r != last; ++r)
        fn_7100d35b14(reinterpret_cast<UnkCollisionRecord_l8656d8*>(*r)->object, key, self->field_0x40);

    for (UnkCollisionGroup_l8656d8** g = self->groups.data; g != self->groups.data + self->groups.count; ++g) {
        if ((*g)->GetLayers() & mask)
            fn_7100d35b14(*g, key, self->field_0x40);
        fn_71008633e0(reinterpret_cast<UnkCollisionContainer*>(self), reinterpret_cast<UnkCollisionNode_ovb0107*>(*g),
                      (*g)->GetLayers());
    }
    records.Free();
}
