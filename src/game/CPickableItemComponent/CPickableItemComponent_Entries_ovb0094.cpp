// CPickableItemComponent's unit (tools/tu_map.py): the item's entries registered with their roots, kept
// with their keys in parallel arrays (adding fn_71009d4940, growing fn_71009d4a10, removing
// fn_71009d42e8, dropping those whose objects are flagged fn_71009cf780, dropping one by key
// fn_71009d2be0), placing the item on a breakable tile (fn_71009d0404), its timer (fn_71009d0570) and
// marking it picked in the blackboard (fn_71009d383c). Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkTree;
void fn_71009d3dc4(void* root, UnkTree* tree);
void fn_71009d4418(void* root, UnkTree* tree);

// An entry registered with its root (fn_71009d4418 adds it, fn_71009d3dc4 removes it).
struct UnkPickEntry_ovb0094 {
    void* root;
    void* field_0x8;
    void* field_0x10;

    void Reset() {
        if (root) {
            fn_71009d3dc4(root, reinterpret_cast<UnkTree*>(this));
            root = nullptr;
        }
        field_0x8 = nullptr;
        field_0x10 = nullptr;
    }
    UnkPickEntry_ovb0094(UnkPickEntry_ovb0094& other)
        : root(other.root), field_0x8(other.field_0x8), field_0x10(other.field_0x10) {
        other.Reset();
        if (root)
            fn_71009d4418(root, reinterpret_cast<UnkTree*>(this));
    }
    ~UnkPickEntry_ovb0094() { Reset(); }
};

// Entries and keys in parallel arrays.
struct UnkPickPool_ovb0094 {
    UnkPickEntry_ovb0094* entries;  // 0x0
    u64* keys;                      // 0x8
    u32 count;                      // 0x10
    u32 capacity;                   // 0x14
    u32 grow;                       // 0x18
    u32 flags;                      // 0x1c
    base::UnkAllocator* allocator;  // 0x20
};

void fn_71009d4a10(UnkPickPool_ovb0094* self, u32 n) {
    if (self->capacity < n) {
        auto* entries = static_cast<UnkPickEntry_ovb0094*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkPickEntry_ovb0094), 8, self->flags));
        auto* keys = static_cast<u64*>(self->allocator->vf_0x40((u64)n * sizeof(u64), 8, self->flags));
        if (UnkPickEntry_ovb0094* old = self->entries) {
            UnkPickEntry_ovb0094* dst = entries;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                new (dst) UnkPickEntry_ovb0094(*old);
                old->~UnkPickEntry_ovb0094();
            }
            self->allocator->vf_0x48(self->entries);
        }
        if (self->keys) {
            __builtin_memcpy(keys, self->keys, (u64)self->count * sizeof(u64));
            self->allocator->vf_0x48(self->keys);
        }
        self->entries = entries;
        self->keys = keys;
        self->capacity = n;
    }
}

UnkPickEntry_ovb0094* fn_71009d4940(UnkPickPool_ovb0094* self, const u64& key, UnkPickEntry_ovb0094& value) {
    if (self->count == self->capacity)
        fn_71009d4a10(self, self->count + self->grow);
    __builtin_memcpy(&self->keys[self->count], &key, sizeof(u64));
    new (&self->entries[self->count]) UnkPickEntry_ovb0094(value);
    ++self->count;
    UnkPickEntry_ovb0094* end = self->entries + self->count;
    return self->count ? end - 1 : self->entries;
}

UnkPickEntry_ovb0094* fn_71009d42e8(UnkPickPool_ovb0094* self, UnkPickEntry_ovb0094* entry);

// Removes the entry (swapping the last one into its place).
UnkPickEntry_ovb0094* fn_71009d42e8(UnkPickPool_ovb0094* self, UnkPickEntry_ovb0094* entry) {
    u32 index = entry - self->entries;
    self->entries[index].Reset();
    u32 last = self->count - 1;
    if (last != index) {
        new (&self->entries[index]) UnkPickEntry_ovb0094(self->entries[last]);
        self->entries[self->count - 1].Reset();
        __builtin_memcpy(&self->keys[index], &self->keys[self->count - 1], sizeof(u64));
    }
    --self->count;
    return &self->entries[index];
}

void* fn_7100751520(void*);

struct UnkPickTarget_ovb0094 {
    u8 unk_0x0[0x39];
    bool field_0x39;
};

struct UnkPickItem_ovb0094 {
    u8 unk_0x0[0x138];
    UnkPickPool_ovb0094 pool;  // 0x138
};

static inline UnkPickEntry_ovb0094* Find_ovb0094(UnkPickPool_ovb0094* pool, const u64& key) {
    for (u32 i = 0; i < pool->count; ++i)
        if (pool->keys[i] == key)
            return &pool->entries[i];
    return &pool->entries[pool->count];
}

// Drops the entries whose objects (fn_7100751520 on the key) have their flag at +0x39 set.
void fn_71009cf780(UnkPickItem_ovb0094* self) {
    for (u64* it = self->pool.keys; it != self->pool.keys + self->pool.count;) {
        auto* target = static_cast<UnkPickTarget_ovb0094*>(fn_7100751520(reinterpret_cast<void*>(*it)));
        if (target && target->field_0x39) {
            UnkPickEntry_ovb0094* entry = Find_ovb0094(&self->pool, *it);
            if (entry != &self->pool.entries[self->pool.count])
                fn_71009d42e8(&self->pool, entry);
        } else {
            ++it;
        }
    }
}

struct UnkEntityState;
void fn_7100232864(UnkEntityState*, bool);
struct UnkStruct_71002334fc;
void fn_71002334fc(UnkStruct_71002334fc*, bool);
void fn_7100232c68(void*);
struct UnkSpawnGroupKey;
void fn_7100de9730(UnkGameManager* manager, UnkSpawnGroupKey* key, void* object);
extern u64 g_unk_717273ffc8 __attribute__((visibility("hidden")));

struct UnkPickTimed_ovb0094;
void fn_71009cd8e0(UnkPickTimed_ovb0094* self, f32 dt);
void fn_71009d0644(UnkPickTimed_ovb0094* self);

struct UnkPickTimed_ovb0094 {
    u8 unk_0x0[0x30];
    void* entity;  // 0x30
    u8 unk_0x38[0xf4];
    f32 duration;  // 0x12c
    void* field_0x130;
    u8 unk_0x138[0x28];
    bool field_0x160;
    u8 unk_0x161[0x51];
    bool field_0x1b2;
    bool registered;  // 0x1b3
    f32 elapsed;      // 0x1b4
};
static_assert(__builtin_offsetof(UnkPickTimed_ovb0094, elapsed) == 0x1b4, "UnkPickTimed_ovb0094 layout");

// Advances the timer at +0x1b4; once it reaches the duration, finishes the item.
void fn_71009d0570(UnkPickTimed_ovb0094* self, f32 dt) {
    f32 elapsed = self->elapsed + dt;
    f32 duration = self->duration;
    self->elapsed = elapsed;
    if (duration > 0.0f && elapsed < duration)
        return;
    if (!self->field_0x160) {
        fn_71009cd8e0(self, dt);
        return;
    }
    if (!self->field_0x1b2)
        fn_71009d0644(self);
    if (!self->field_0x130) {
        fn_7100232864(static_cast<UnkEntityState*>(self->entity), false);
        fn_7100232c68(self->entity);
        return;
    }
    if (self->registered) {
        fn_7100de9730(g_unk_7101d47310, reinterpret_cast<UnkSpawnGroupKey*>(&g_unk_717273ffc8), self);
        self->registered = false;
    }
    fn_7100232864(static_cast<UnkEntityState*>(self->entity), true);
    fn_71002334fc(static_cast<UnkStruct_71002334fc*>(self->entity), false);
}

using base::global::CStrId;

CStrId fn_7100de9c30(UnkGameManager*);
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection*, UnkPropertyName*, const UnkTypedValue*);
extern UnkProperty g_unk_717273fe08;
struct UnkPickList_ovb0094;
extern u8 g_unk_717273fe60[];

struct UnkPickTile_ovb0094 {
    u8 unk_0x0[0x3c];
    bool field_0x3c;
};

struct UnkPickValue_ovb0094 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkPickPicked_ovb0094 {
    u8 unk_0x0[0x30];
    void* entity;  // 0x30
    u8 unk_0x38[0xf3];
    bool field_0x12b;
    UnkPickTile_ovb0094* tile;  // 0x130
    UnkPickPool_ovb0094 pool;   // 0x138
    u8 unk_0x160[0x18];
    UnkPickValue_ovb0094* values;  // 0x178
    u64* keys;                     // 0x180
    u32 count;                     // 0x188
    u8 unk_0x18c[0xc4];
    u8 field_0x250[0x10];
};
static_assert(__builtin_offsetof(UnkPickPicked_ovb0094, field_0x250) == 0x250, "UnkPickPicked_ovb0094 layout");

void fn_710082d0ac(UnkPickList_ovb0094* list, void* const& item);

// Stores true under the property g_unk_717273fe08 in the current scenario's section.
static inline void StorePicked_ovb0094(UnkPickPicked_ovb0094* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_7100de9c30(manager);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717273fe08.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkPickValue_ovb0094* values = self->values;
    UnkTypedValue v;
    bool value = true;
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            UnkPropertyName* name = &values[index].name;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(section, name, &v);
        }
    }
}

// The same, with the typed value declared first (its own stack layout). Stores true under the property g_unk_717273fe08 in the current scenario's section.
static inline void StorePickedFirst_ovb0094(UnkPickPicked_ovb0094* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkTypedValue v;
    CStrId section_id = fn_7100de9c30(manager);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717273fe08.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkPickValue_ovb0094* values = self->values;
    bool value = true;
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            UnkPropertyName* name = &values[index].name;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(section, name, &v);
        }
    }
}

// Hands the item to its two lists.
static inline void AddToLists_ovb0094(UnkPickPicked_ovb0094* self) {
    fn_710082d0ac(reinterpret_cast<UnkPickList_ovb0094*>(self->field_0x250), self);
    fn_710082d0ac(reinterpret_cast<UnkPickList_ovb0094*>(g_unk_717273fe60 + 0x80), self);
}

// Marks the item picked in the blackboard, shows the entity and hands the item to its lists.
void fn_71009d383c(UnkPickPicked_ovb0094* self) {
    StorePicked_ovb0094(self);
    fn_71002334fc(static_cast<UnkStruct_71002334fc*>(self->entity), true);
    fn_7100232864(static_cast<UnkEntityState*>(self->entity), true);
    AddToLists_ovb0094(self);
}

namespace base::reflection {
struct UnkReflObject;
}
struct UnkTileQuery_A3_b64;
UnkTileQuery_A3_b64* fn_7100ca1ecc(base::reflection::UnkReflObject*, const void*, void*, bool);

struct UnkPickTiles_ovb0094 {
    u8 unk_0x0[0x90];
    base::reflection::UnkReflObject** groups;  // 0x90
    u8 unk_0x98[0x8];
    u32 group_count;  // 0xa0
};

struct UnkPickEntity_ovb0094 {
    u8 unk_0x0[0x44];
    f32 x;  // 0x44
    u8 unk_0x48[0xc];
    f32 y;  // 0x54
    u8 unk_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkPickPoint_ovb0094 {
    f32 x, y, z;
};

struct UnkPickPlaced_ovb0094 {
    void** vtable;
    u8 unk_0x8[0x28];
    UnkPickEntity_ovb0094* entity;  // 0x30
    u8 unk_0x38[0x60];
    CStrId field_0x98;
    u8 unk_0xa0[0x88];
    bool field_0x128;
    bool field_0x129;
    u8 unk_0x12a[0x6];
    UnkPickTile_ovb0094* tile;  // 0x130
    u8 unk_0x138[0x78];
    bool field_0x1b0;
    u8 unk_0x1b1[0x2];
    bool registered;  // 0x1b3
    u8 unk_0x1b4[0x4];
    UnkPickEntry_ovb0094 tree;  // 0x1b8

    void Register() {
        if (tile && !registered) {
            fn_7100de9540(g_unk_7101d47310, &g_unk_717273ffc8, reinterpret_cast<UnkWeakTarget*>(this));
            registered = true;
        }
    }
};
static_assert(__builtin_offsetof(UnkPickPlaced_ovb0094, tree) == 0x1b8, "UnkPickPlaced_ovb0094 layout");

// Finds the breakable tile under the entity, registers the item and hooks it to the game manager.
void fn_71009d0404(UnkPickPlaced_ovb0094* self) {
    if (!self->field_0x1b0 && self->field_0x128 && self->field_0x129 &&
        self->field_0x98.data != &g_unk_7101d47108->empty)
        return;
    UnkPickEntity_ovb0094* entity = self->entity;
    f32 x = entity->x;
    f32 y = entity->y;
    f32 z = entity->z;
    UnkPickPoint_ovb0094 point = {x, y, z};
    UnkGameManagerSub_0x1af8* tiles = g_unk_7101d47310->field_0x1af8;
    UnkPickTiles_ovb0094* list = reinterpret_cast<UnkPickTiles_ovb0094*>(tiles);
    for (base::reflection::UnkReflObject **it = list->groups, **end = it + list->group_count; it != end; ++it) {
        if (UnkTileQuery_A3_b64* hit = fn_7100ca1ecc(*it, &point, nullptr, false)) {
            self->tile = reinterpret_cast<UnkPickTile_ovb0094*>(hit);
            // Stored as a plain byte: the original reloads the tile afterwards.
            *reinterpret_cast<char*>(&self->tile->field_0x3c) = true;
            self->Register();
            break;
        }
    }
    self->Register();
    UnkGameManager* manager = g_unk_7101d47310;
    if (self->tree.root) {
        fn_71004f3ea4(static_cast<base::UnkDeferredSet<void>*>(self->tree.root), &self->tree);
        self->tree.root = nullptr;
    }
    self->tree.root = reinterpret_cast<u8*>(manager) + 0x21a8;
    fn_71004f3cf0(static_cast<base::UnkDeferredSet<void>*>(self->tree.root), &self->tree);
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    reinterpret_cast<void (*)(UnkPickPlaced_ovb0094*, CBlackboard*)>(self->vtable[0x140 / 8])(self, blackboard);
}

// Drops the entry for the key; once none are left and there is no tile, the item counts as picked.
bool fn_71009d2be0(UnkPickPicked_ovb0094* self, u64 key) {
    UnkPickEntry_ovb0094* entry = Find_ovb0094(&self->pool, key);
    if (entry != &self->pool.entries[self->pool.count])
        fn_71009d42e8(&self->pool, entry);
    if (self->pool.count == 0 && !self->tile) {
        self->field_0x12b = true;
        StorePickedFirst_ovb0094(self);
        AddToLists_ovb0094(self);
    }
    return false;
}
