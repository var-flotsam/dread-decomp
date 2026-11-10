// CBillboardCollisionComponent's unit (tools/tu_map.py): taking an entry out of the flat map at +0x140 (as src/game/CBillboardCollisionComponent/CBillboardCollisionComponent_Move_b66a4.cpp moves one) and the per-frame placement of the colliders.
// Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkClone_fn_710008c504;
using UnkCounter_b66a4 = UnkCloneElem<UnkClone_fn_710008c504, 4>;
extern template UnkCounter_b66a4& base::UnkVector<UnkCounter_b66a4>::GrowAt(u32);

struct UnkBillboardInfo_ovb0137 {
    u8 pad_0x0[0x20];
    u32 id;  // 0x20
};

struct UnkBillboardKey_ovb0137 {
    u8 pad_0x0[0x10];
    UnkBillboardInfo_ovb0137* info;  // 0x10
};

// A flat map: values, their keys and the shared count.
struct UnkBillboardMap_ovb0137 {
    UnkEntry** values;
    UnkBillboardKey_ovb0137** keys;
    u32 count;

    UnkEntry** Find(const UnkEntry* value) {
        for (u64 i = 0; i != count; ++i) {
            if (values[i] == value)
                return &values[i];
        }
        return values + count;
    }
};

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};
UnkMatrix44_7100078c9c fn_7100078cd8(const UnkVec3f& t);

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};
struct UnkStruct_71007455dc;
UnkVec3Result fn_71007455dc(UnkStruct_71007455dc* self);
struct UnkSwarmMember_ovb6;
bool fn_7100744e1c(UnkSwarmMember_ovb6* member);

// One billboard of the set (0x148 bytes): shown (+0x24), its position (+0xa0) and a word at +0xe0.
struct UnkBillboardItem_ovb0137 {
    u8 pad_0x0[0x24];
    bool shown;
    u8 pad_0x25[0x7b];
    UnkVec3f pos;  // 0xa0
    u8 pad_0xac[0x34];
    u32 field_0xe0;
    u8 pad_0xe4[0x64];
};
static_assert(sizeof(UnkBillboardItem_ovb0137) == 0x148, "UnkBillboardItem_ovb0137 layout");

struct UnkBillboardSetData_ovb0137 {
    u8 pad_0x0[0x50];
    UnkBillboardItem_ovb0137* items;  // 0x50
};

struct UnkBillboardSet_ovb0137 {
    u8 pad_0x0[0x10];
    UnkBillboardSetData_ovb0137* data;  // 0x10
};

struct UnkBillboardInfoSet_ovb0137 {
    u8 pad_0x0[0x110];
    s32 count;  // 0x110
};

struct UnkBillboardOwner_ovb0137 {
    u8 pad_0x0[0x230];
    UnkBillboardInfoSet_ovb0137* set;  // 0x230
};

struct UnkBillboardObject_ovb0137 {
    void** vtable;
    void vf_0x78(const UnkMatrix44_7100078c9c* m) {
        reinterpret_cast<void (*)(UnkBillboardObject_ovb0137*, const UnkMatrix44_7100078c9c*)>(vtable[0x78 / 8])(this,
                                                                                                             m);
    }
};

struct UnkBillboardCollision_ovb0137 {
    u8 pad_0x0[0x30];
    UnkBillboardOwner_ovb0137* owner;  // 0x30
    u8 pad_0x38[0x10];
    base::UnkVector<UnkCollisionRecord> records;  // 0x48
    u8 pad_0x70[0x58];
    bool enabled;  // 0xc8
    u8 pad_0xc9[0x67];
    UnkBillboardSet_ovb0137* billboards;  // 0x130
    UnkBillboardSet_ovb0137* field_0x138;
    UnkBillboardMap_ovb0137 map;  // 0x140
    u8 pad_0x158[0x10];
    base::UnkVector<UnkEntry*> removed;  // 0x168
    base::UnkVector<UnkCounter_b66a4> counters;  // 0x190
};

// Takes the entry at `p` out of the map: it goes on the removed list, its bucket's counter drops, it
// is switched off, and the last pair takes its place.
static inline __attribute__((always_inline)) void RemoveAt(UnkBillboardCollision_ovb0137* self, UnkEntry** p,
                                                           UnkEntry* entry) {
    UnkBillboardMap_ovb0137& map = self->map;
    self->removed.PushBack(entry);
    --*reinterpret_cast<u32*>(&self->counters.GrowAt(map.keys[(u32)(p - map.values)]->info->id));
    fn_7100d35a98(entry, false);
    u32 index = (u32)(p - map.values);
    if (map.count - 1 != index) {
        __builtin_memcpy(&map.values[index], &map.values[map.count - 1], sizeof(UnkEntry*));
        __builtin_memcpy(&map.keys[index], &map.keys[map.count - 1], sizeof(UnkBillboardKey_ovb0137*));
        index = map.count - 1;
    }
    map.count = index;
}

// Takes `entry` out of the map once.
void fn_7100725610(UnkBillboardCollision_ovb0137* self, UnkEntry* entry) {
    UnkEntry** p = self->map.Find(entry);
    if (p == self->map.values + self->map.count)
        return;
    RemoveAt(self, p, entry);
}

// Places the colliders: each billboard's record gets the billboard's position and, when shown (and,
// with a second set, its word at +0xe0 set), flags 0x802180 if it is active; then every map entry is
// moved to its key's position, or taken out when the key is gone or refuses.
void fn_71007251e0(UnkBillboardCollision_ovb0137* self) {
    if (self->billboards && self->billboards->data && self->enabled) {
        UnkBillboardInfoSet_ovb0137* set = self->owner->set;
        for (s32 i = 0; i < set->count; ++i) {
            UnkCollisionRecord& record = fn_71007253b8(&self->records, i);
            UnkBillboardItem_ovb0137* item = &self->billboards->data->items[i];
            if (!item->shown || (self->field_0x138 && self->field_0x138->data && !item->field_0xe0)) {
                *reinterpret_cast<u32*>(&record.field_0x20) = 0;
                UnkMatrix44_7100078c9c m = fn_7100078cd8(UnkVec3f(item->pos));
                reinterpret_cast<UnkBillboardObject_ovb0137*>(record.field_0x28)->vf_0x78(&m);
            } else if (*reinterpret_cast<bool*>(&record.field_0x18)) {
                UnkMatrix44_7100078c9c m = fn_7100078cd8(UnkVec3f(item->pos));
                *reinterpret_cast<u32*>(&record.field_0x20) = 0x802180;
                reinterpret_cast<UnkBillboardObject_ovb0137*>(record.field_0x28)->vf_0x78(&m);
            }
        }
    }
    for (u32 n = self->map.count; n; --n) {
        UnkEntry* entry = self->map.values[n - 1];
        if (!entry)
            continue;
        UnkBillboardKey_ovb0137* key = self->map.keys[n - 1];
        if (!key || !fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(key))) {
            fn_7100725610(self, entry);
            continue;
        }
        UnkVec3Result pos = fn_71007455dc(reinterpret_cast<UnkStruct_71007455dc*>(key));
        UnkMatrix44_7100078c9c m = fn_7100078cd8(reinterpret_cast<const UnkVec3f&>(pos));
        reinterpret_cast<UnkBillboardObject_ovb0137*>(entry)->vf_0x78(&m);
    }
}
