// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py; no reliable unit name), batch b0201: the
// frame set-up of the map's draw pass (fn_7100e931b0), loading the room's model (fn_7100e99e18), the
// 0x118-byte record's move and copy assignments (fn_7100eac82c, fn_7100eac64c), the keyed array of 0x100-byte
// records and its Reserve (fn_7100ea9934), the minimap's cell texture (fn_7100e95480), and the owner's removal
// of a record by key (fn_7100ea5a48) and of all records (fn_7100ea5f30), which erase the record's two nodes
// from the hashed set at +0xd64210 (fn_7100ea5c40). Names and layouts are placeholders.
#include <new>
#include <stdio.h>
#include <string.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "game/CBlackboard.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU127.h"
#include "game/UnkTU2566.h"
#include "game/UnkXformNode.h"
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkPoolIter;

extern void* g_unk_7101a1b5a0[];

// ---- the 0x118-byte record's assignments ----

using UnkString20_b = UnkBufferString<0x20, g_unk_7101a1b5a0>;

struct UnkTail_b {
    u64 a;
    u64 b;
    u32 c;
    u32 pad_0x14;
    CRntString field_0x18;
    u32 field_0x48;
    u32 pad_0x4c;
    u64 field_0x50;
    u32 field_0x58;
    u32 pad_0x5c;
    UnkString20_b field_0x60;
    u32 field_0xc8;
    u32 pad_0xcc;
    u32 field_0xd0;
    u16 field_0xd4;
    u16 pad_0xd6;

    __attribute__((always_inline)) void AssignMove(UnkTail_b& o) {
        u64 av = o.a;
        u64 bv = o.b;
        u32 cv = o.c;
        c = cv;
        b = bv;
        a = av;
        fn_710007eaa0(&field_0x18, &o.field_0x18);
        field_0x48 = o.field_0x48;
        u64 x = o.field_0x50;
        u32 y = o.field_0x58;
        field_0x58 = y;
        field_0x50 = x;
        fn_710007eaa0(field_0x60.Str(), o.field_0x60.Str());
        field_0xc8 = o.field_0xc8;
        u32 z = o.field_0xd0;
        u16 w = o.field_0xd4;
        field_0xd4 = w;
        field_0xd0 = z;
    }

    __attribute__((always_inline)) void AssignCopy(const UnkTail_b& o) {
        u64 av = o.a;
        u64 bv = o.b;
        u32 cv = o.c;
        a = av;
        c = cv;
        b = bv;
        fn_710007eaa0(&field_0x18, &o.field_0x18);
        field_0x48 = o.field_0x48;
        u64 x = o.field_0x50;
        u32 y = o.field_0x58;
        field_0x58 = y;
        field_0x50 = x;
        fn_710007eaa0(field_0x60.Str(), o.field_0x60.Str());
        field_0xc8 = o.field_0xc8;
        u32 z = o.field_0xd0;
        u16 w = o.field_0xd4;
        field_0xd4 = w;
        field_0xd0 = z;
    }
};

struct UnkRecord118_b {
    u64 field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    u32 field_0x20[7];
    u32 pad_0x3c;
    UnkTail_b tail;
};
static_assert(sizeof(UnkRecord118_b) == 0x118, "UnkRecord118_b layout");

// The ids are taken over (the other's become empty), the rest copied.
UnkRecord118_b* fn_7100eac82c(UnkRecord118_b* self, UnkRecord118_b* other) {
    self->field_0x0 = other->field_0x0;
    self->field_0x8.ReleaseAndTake(other->field_0x8);
    self->field_0x10.ReleaseAndTake(other->field_0x10);
    self->field_0x18.ReleaseAndTake(other->field_0x18);
    for (int i = 0; i < 7; ++i)
        self->field_0x20[i] = other->field_0x20[i];
    self->tail.AssignMove(other->tail);
    return self;
}

UnkRecord118_b* fn_7100eac64c(UnkRecord118_b* self, const UnkRecord118_b* other) {
    self->field_0x0 = other->field_0x0;
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    for (int i = 0; i < 7; ++i)
        self->field_0x20[i] = other->field_0x20[i];
    self->tail.AssignCopy(other->tail);
    return self;
}

// ---- the keyed array of 0x100-byte records: Reserve ----

template <u32 N>
struct UnkBlock_d {
    u8 data[N];

    __attribute__((always_inline)) UnkBlock_d(const UnkBlock_d& other) { __builtin_memcpy(this, &other, N); }
};

// Padding a move leaves alone.
struct UnkPad_d {
    u32 data;

    __attribute__((always_inline)) UnkPad_d(const UnkPad_d&) {}
};

struct UnkRecord100_d {
    UnkBlock_d<0xc> head;
    UnkPad_d pad_0xc;
    UnkBufferString<0x20, g_unk_7101a1b5a0> field_0x10;
    u32 field_0x78;
    UnkPad_d pad_0x7c;
    UnkBlock_d<0xc> field_0x80;
    UnkPad_d pad_0x8c;
    UnkBufferString<0x20, g_unk_7101a1b5a0> field_0x90;
    u32 field_0xf8;
    UnkPad_d pad_0xfc;

    // The strings are copied and the source's destroyed afterwards.
    __attribute__((always_inline)) UnkRecord100_d(UnkRecord100_d&& o)
        : head(o.head), pad_0xc(o.pad_0xc), field_0x10(o.field_0x10), field_0x78(o.field_0x78),
          pad_0x7c(o.pad_0x7c), field_0x80(o.field_0x80), pad_0x8c(o.pad_0x8c), field_0x90(o.field_0x90),
          field_0xf8(o.field_0xf8), pad_0xfc(o.pad_0xfc) {}
    __attribute__((always_inline)) ~UnkRecord100_d() = default;
};
static_assert(sizeof(UnkRecord100_d) == 0x100, "UnkRecord100_d layout");

struct UnkKeyed100Array_d {
    UnkRecord100_d* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100ea9934(UnkKeyed100Array_d* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    auto* values = static_cast<UnkRecord100_d*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkRecord100_d), 8, self->flags));
    auto* keys = static_cast<u32*>(self->allocator->vf_0x40((u64)capacity * sizeof(u32), 8, self->flags));
    if (UnkRecord100_d* src = self->values) {
        UnkRecord100_d* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) UnkRecord100_d(static_cast<UnkRecord100_d&&>(*src));
            src->~UnkRecord100_d();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        memcpy(keys, self->keys, (u64)self->count * sizeof(u32));
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}

// ---- the minimap's cell texture ----

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

extern const f32 g_unk_710164be30;
extern u32 g_unk_7101aa9e38[][1024];
extern const f32 g_unk_7101647bec[4] HIDDEN;

struct UnkMinimapDrawList {
    void* data;
    u32 count;
    u8 unk_0xc[0x1c];
};

// The unit's statics, which the compiler merged into one block (addressed from one base), as
// src/game/CMinimapManager.cpp declares it.
struct UnkMinimapStatics {
    u8 unk_0x0[0x4];
    f32 cell_size;
    u8 unk_0x8[0x68];
    u64 cell_size_guard;
    u8 unk_0x78[0x88];
    UnkMinimapDrawList lists[4];
};

struct UnkLinkedCell_e;
void fn_7100e78180(UnkLinkedCell_e* self);
void fn_7100e78060(UnkLinkedCell_e* self);

struct UnkGridCell_e {
    f32 x;
    f32 y;
    u8 kind;
    u8 unk_0x9[0x36];
    u8 field_0x3f;
    u8 field_0x40;
    bool dirty;
    bool field_0x42;
};

struct UnkMinimapOrigin_e {
    u8 unk_0x0[0x68];
    f32 x;
    f32 y;
};

struct UnkMinimapOwner_e {
    u8 unk_0x0[0x68];
    UnkMinimapOrigin_e* map;
    u8 unk_0x70[0xa8];
    u8 field_0x118[1];
};

UnkGridCell_e* fn_7100e796a8(void* grid, u32 index);

// Paints one pixel per grid cell into the 1024 by 512 texture, then hands it and a second parameter
// to the render passes.
void fn_7100e95480(UnkMinimapOwner_e* self) {
    u64* guard = reinterpret_cast<u64*>(reinterpret_cast<u8*>(&g_unk_71729cdaa0) + 0x98);
    f32* cell_size = reinterpret_cast<f32*>(reinterpret_cast<u8*>(&g_unk_71729cdaa0) + 8);
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(guard)) {
            *cell_size = g_unk_710164be30 + g_unk_710164be30;
            __cxa_guard_release(guard);
        }
    }
    memset(g_unk_7101aa9e38, 0, 0x80000);
    for (u32 i = 0; i != 0x80000; ++i) {
        UnkGridCell_e* cell = fn_7100e796a8(self->field_0x118, i);
        if (!cell)
            continue;
        u8 kind = cell->kind;
        if (cell->dirty)
            fn_7100e78180(reinterpret_cast<UnkLinkedCell_e*>(cell));
        u8 links = cell->field_0x3f;
        if (cell->field_0x42)
            fn_7100e78060(reinterpret_cast<UnkLinkedCell_e*>(cell));
        f32 cx = (cell->x - self->map->x) / *cell_size;
        u32 xi = (u32)cx;
        f32 cy = (cell->y - self->map->y) / *cell_size;
        u32 yi = (u32)cy;
        if (xi >= 0x3ff)
            xi = 0x3ff;
        if (yi >= 0x1ff)
            yi = 0x1ff;
        u32 color = (kind > 1 ? 0xffu : 0u) | (kind == 6 ? 0xff00u : 0u);
        color |= (u32)links << 16;
        color |= (u32)cell->field_0x40 << 24;
        g_unk_7101aa9e38[yi][xi] = color;
    }
    fn_7100219294(g_unk_7101d47300, 0x6fc641f145352069, reinterpret_cast<const f32*>(g_unk_7101aa9e38), 6);
    f32 tmp[4];
    memcpy(tmp, g_unk_7101647bec, sizeof(tmp));
    fn_7100219294(g_unk_7101d47300, 0x7bc15ce967f525d2, tmp, 6);
}

// ---- removing records from the owner ----

struct UnkKeyObject_p {
    u8 unk_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

struct UnkNode_p {
    UnkKeyObject_p* key;
    u8 unk_0x8[0x15f0];
    UnkNode_p* next;
};

struct UnkEntry_p {
    u8 unk_0x0[8];
    UnkKeyObject_p* key_a;
    UnkKeyObject_p* key_b;
    u8 unk_0x18[0x118 - 0x18];
};

struct UnkEntryList_p {
    UnkEntry_p* data;
    u32* keys;
    u32 count;
};

// As src/game/CMinimapManager/fn_7100ea5c40.cpp and src/game_aca57c/fn_7100e9bf60.cpp declare them.
struct UnkRecord;
struct UnkRecordBuckets;
void fn_7100ea5c40(char* set, UnkPoolIter* it);
struct UnkRecord118;

// As src/game/CMinimapManager/CTunableMinimapManager_KeyedFind_b55.cpp and the same unit's other batch declare them.
struct UnkMmOwner_b0199;

struct UnkMinimapOwner_p {
    u8 unk_0x0[0xd64210];
    u8 set[0x1e0];
    UnkEntryList_p entries;
};
static_assert(__builtin_offsetof(UnkMinimapOwner_p, entries) == 0xd643f0, "UnkMinimapOwner_p layout");

// Erases the node of the hashed set at +0xd64210 that the key names, if there is one. The key is read
// through its slot each time (the original reloads it after the bucket call) and the iterator lives in
// storage of the caller (each caller keeps one per key).
__attribute__((always_inline)) static inline void EraseNode(UnkMinimapOwner_p* self,
                                                           UnkKeyObject_p* const* key_slot, void* slot) {
    char* set = reinterpret_cast<char*>(self->set);
    u32 buckets = *reinterpret_cast<u32*>(set + 8);
    if (buckets == 0)
        return;
    u32 hash = (*key_slot)->hash_a ^ (*key_slot)->hash_b;
    u32 bucket = hash % *reinterpret_cast<u32*>(set + 0xc);
    if (bucket >= buckets)
        return;
    UnkNode_p* node = *reinterpret_cast<UnkNode_p**>(fn_7100eb3814(reinterpret_cast<UnkRecordBuckets*>(set), bucket));
    if (!node)
        return;
    UnkKeyObject_p* want = *key_slot;
    UnkNode_p* prev;
    if (node->key == want) {
        prev = nullptr;
    } else {
        for (;;) {
            prev = node;
            node = node->next;
            if (!node)
                return;
            if (node->key == want)
                break;
        }
    }
    auto* it = static_cast<UnkPoolIter*>(slot);
    u32 version = *reinterpret_cast<u32*>(set + 0x48);
    it->node = reinterpret_cast<char*>(node);
    it->prev = reinterpret_cast<char*>(prev);
    it->ref.set = set;
    it->ref.bucket = bucket;
    it->ref.version = version;
    fn_7100ea5c40(set, it);
}

// The record whose key is the given one, or the end.
__attribute__((always_inline)) static inline UnkEntry_p* FindEntry(UnkEntryList_p& list, u32 key) {
    u32 n = list.count;
    if (n != 0) {
        u32* k = list.keys;
        for (u64 i = 0; i != n; ++i, ++k) {
            if (*k == key)
                return list.data + i;
        }
    }
    return list.data + n;
}

// Removes the record with the given key: its two nodes leave the set, then it leaves the list.
void fn_7100ea5a48(UnkMinimapOwner_p* self, u32 key) {
    UnkEntryList_p& list = self->entries;
    alignas(8) u8 slot_a[sizeof(UnkPoolIter)];
    alignas(8) u8 slot_b[sizeof(UnkPoolIter)];
    UnkEntry_p* it = FindEntry(list, key);
    if (it != list.data + list.count) {
        EraseNode(self, &it->key_a, slot_a);
        EraseNode(self, &it->key_b, slot_b);
        fn_7100ea5d7c(reinterpret_cast<char*>(&list), it);
    }
    fn_7100ea585c(reinterpret_cast<UnkMmOwner_b0199*>(self));
}

// Removes all records: their nodes leave the set, the records are destroyed.
void fn_7100ea5f30(UnkMinimapOwner_p* self) {
    UnkEntryList_p& list = self->entries;
    alignas(8) u8 slot_a[sizeof(UnkPoolIter)];
    alignas(8) u8 slot_b[sizeof(UnkPoolIter)];
    UnkEntry_p* it = list.data;
    if (list.count != 0) {
        for (UnkEntry_p* end = it + list.count; it != end; ++it) {
            EraseNode(self, &it->key_a, slot_a);
            EraseNode(self, &it->key_b, slot_b);
        }
    }
    if (list.data && list.count) {
        fn_7100ea7034(reinterpret_cast<UnkRecord118*>(&list.data[0]));
        for (u64 i = 1; i < list.count; ++i)
            fn_7100ea7034(reinterpret_cast<UnkRecord118*>(&list.data[i]));
    }
    list.count = 0;
    fn_7100ea585c(reinterpret_cast<UnkMmOwner_b0199*>(self));
}

// ---- the map's draw pass ----

// As src/game/CMinimapManager.cpp declares the owner (struct UnkMinimapOwner) and fn_7100ea3f7c.
struct UnkMinimapOwner;
void fn_7100ea39a4(UnkMinimapOwner* self, CBlackboard* blackboard, CBlackboard::CSection* section);
void fn_7100ea3f7c(UnkMinimapOwner* self);
void fn_7100ea40b4(UnkMinimapOwner* self, CBlackboard* blackboard, CBlackboard::CSection* section);
void fn_7100ea4504(UnkMinimapOwner* self);
void fn_7100ea4740(UnkMinimapOwner* self, CBlackboard* blackboard, CBlackboard::CSection* section);
void fn_7100ea4ee0(UnkMinimapOwner* self);
void fn_7100148148(UnkSceneObject* self, u32 layer, const UnkSceneSpan& a, const UnkSceneSpan& b);

// A draw list as the scene object takes it.
struct UnkSpan_u : UnkSceneSpan {
    UnkSpan_u(const UnkMinimapDrawList& list) : UnkSceneSpan{list.data, list.count} {}
};

struct UnkMinimapOwner_u {
    u8 unk_0x0[0x68];
    void* field_0x68;
    u8 unk_0x70[0xa0];
    CStrId field_0x110;
    u8 unk_0x118[0xd63f88 - 0x118];
    UnkSceneObject* object;
    u8 unk_0xd63f90[0x38];
    u32 toggle;
};
static_assert(__builtin_offsetof(UnkMinimapOwner_u, toggle) == 0xd63fc8, "UnkMinimapOwner_u layout");

// Turns the pass object's slots over (the other buffer is shown), empties three draw lists, lets the
// current area's section of the blackboard fill them and hands them to the object.
void fn_7100e931b0(UnkMinimapOwner_u* self) {
    if (!self->object || !self->field_0x68)
        return;
    fn_71001be430(self->object->field_0x130, 0, false);
    fn_71001be430(self->object->field_0x130, 1, false);
    self->toggle = self->toggle == 0;
    UnkMinimapDrawList* lists = g_unk_71729cdaa0.lists;
    lists[0].count = 0;
    lists[1].count = 0;
    lists[3].count = 0;
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    CStrId id = fn_7100dfbda4(g_unk_7101d47310, self->field_0x110);
    CBlackboard::CSection* section = fn_7101047414(blackboard, id);
    UnkMinimapOwner* owner = reinterpret_cast<UnkMinimapOwner*>(self);
    fn_7100ea39a4(owner, blackboard, section);
    fn_7100ea3f7c(owner);
    fn_7100ea40b4(owner, blackboard, section);
    fn_7100ea4504(owner);
    fn_7100ea4740(owner, blackboard, section);
    fn_7100ea4ee0(owner);
    fn_7100148148(self->object, self->toggle, UnkSpan_u(lists[0]), UnkSpan_u(lists[1]));
    fn_7100148178(self->object, self->toggle, UnkSpan_u(lists[3]), true);
    fn_71001be430(self->object->field_0x130, self->toggle, true);
    fn_7100149490(self->object, true);
}

// ---- the room's model ----

struct UnkModel;

struct UnkStrPair_0x10 {
    void* a;
    void* b;
};
struct UnkModel_ovb0032;

struct UnkModelData_v {
    u8 unk_0x0[0x18];
    u32 field_0x18;
};

struct UnkModel {
    u8 unk_0x0[0x138];
    UnkModelData_v* field_0x138;
};

// The pooled data behind a file-path id as its text accessor sees it.
struct UnkIdTextData_v {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    s32 reads;
};
typedef u32 __attribute__((may_alias)) UnkAliasU32_v;

static inline const char* PathText(const CFilePathStrId& id) {
    ++*reinterpret_cast<UnkAliasU32_v*>(&reinterpret_cast<UnkIdTextData_v*>(id.data)->reads);
    const char* s = reinterpret_cast<UnkIdTextData_v*>(id.data)->str.data;
    return s ? s : g_unk_710162e2ed;
}

struct UnkMinimapOwner_v {
    u8 unk_0x0[0xd63f80];
    UnkModel* model;
};

void fn_7100e99e18(UnkMinimapOwner_v* self, const CFilePathStrId* room) {
    if (self->model) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(reinterpret_cast<char*>(self->model) + 8), 6);
        self->model = nullptr;
    }
    char name[0x100];
    sprintf(name, "system/minimap/maproom/%s/%s.bcmdl", PathText(*room), PathText(*room));
    CFilePathStrId path(name, true);
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    bool known;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, &path);
        known = fn_7100087850(reg, reinterpret_cast<const UnkStrPair_0x10*>(&s));
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    if (known) {
        UnkRenderManager* manager = g_unk_7101d47300;
        {
            UnkModel* model = nullptr;
            fn_71006cae2c(manager->field_0x218, path, &model, &g_unk_7101d470e0);
            fn_7100219ae0(manager, model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr, 6);
            self->model = model;
        }
        if (self->model) {
            fn_710014364c(self->model, 1);
            *reinterpret_cast<UnkAliasU32_v*>(&self->model->field_0x138->field_0x18) = 3;
            fn_71001436a0(self->model, 1);
            UnkModel* shown = self->model;
            UnkVec3f v(0.0f, 0.0f, 200.0f);
            fn_7100143578(reinterpret_cast<UnkModel_ovb0032*>(shown), &v);
        }
    }
}
