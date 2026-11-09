// CScenario unit (tools/tu_map.py, the name from reflection): the scenario's actor lists (base/UnkSList.h)
// at +0x2a0, +0x310 and +0x380: adding an actor (fn_71011a2770) and taking one off all of them
// (fn_71011a2a24), the nearest actor to a point (fn_71011a2e54), the lookup of a key in one of the
// scenario's 0xc8-byte layers (fn_71011a3234) and a script call with the empty id's text
// (fn_71011a49ec). Names and layouts are placeholders inferred from the code.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkSList.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringEntry;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkScenarioListTag_ovb0032;
using UnkScenarioList_ovb0032 = base::UnkSList<UnkScenarioListTag_ovb0032>;

struct UnkScenarioObject_ovb0032 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x114];
    void* field_0x1a8;
};
static_assert(__builtin_offsetof(UnkScenarioObject_ovb0032, field_0x1a8) == 0x1a8, "UnkScenarioObject_ovb0032 layout");

struct UnkScenario_ovb0032 {
    u8 pad_0x0[0x2a0];
    UnkScenarioList_ovb0032 list_0x2a0;
    u8 pad_0x2d0[0x40];
    UnkScenarioList_ovb0032 list_0x310;
    u8 pad_0x340[0x40];
    UnkScenarioList_ovb0032 list_0x380;
    u8 pad_0x3b0[0x40];
    bool field_0x3f0;
};
static_assert(__builtin_offsetof(UnkScenario_ovb0032, field_0x3f0) == 0x3f0, "UnkScenario_ovb0032 layout");

// The moves between the lists at +0x310 and +0x380 (src/game/CScenario/CScenario_Lists_b52.cpp).
struct UnkScenarioLists_b52;
void fn_71011a28e8(UnkScenarioLists_b52* self, void* object);
void fn_71011a2c40(UnkScenarioLists_b52* self, void* object);
void fn_71002337d0(UnkScenarioObject_ovb0032* object, UnkScenario_ovb0032* scenario);

// Removes the first node holding `value` and frees it (as src/game/CScenario/CScenario_Lists_b52.cpp).
static inline __attribute__((always_inline)) void Remove_ovb0032(UnkScenarioList_ovb0032& list, u64 value) {
    UnkScenarioList_ovb0032::Node* node = list.head;
    for (; node; node = node->next) {
        if (node->value == value)
            break;
    }
    if (!node)
        return;
    UnkScenarioList_ovb0032::Node* prev = nullptr;
    UnkScenarioList_ovb0032::Node* cur = list.head;
    UnkScenarioList_ovb0032::Node* next;
    UnkScenarioList_ovb0032::Node* last = nullptr;
    for (;;) {
        if (!cur)
            return;
        prev = last;
        next = cur->next;
        if (cur == node)
            break;
        last = cur;
        cur = next;
    }
    if (prev) {
        prev->next = next;
        if (list.tail == node)
            list.tail = prev;
    } else {
        list.head = next;
    }
    if (--list.count == 0) {
        list.head = nullptr;
        list.tail = nullptr;
    }
    list.allocator->vf_0x48(node);
}

static inline __attribute__((always_inline)) s32 IndexOf_ovb0032(const UnkScenarioList_ovb0032& list, u64 value) {
    u32 i = 0;
    for (UnkScenarioList_ovb0032::Node* n = list.head; n; n = n->next, ++i)
        if (n->value == value)
            return (s32)i;
    return -1;
}

// Adds `object`: while +0x3f0 is set, moves it from +0x380 to +0x310 instead; otherwise takes it off
// +0x380 and, unless +0x1a8 is set, adds it to +0x2a0 once and tells it (fn_71002337d0).
void fn_71011a2770(UnkGameManagerSub_0x608* scenario, void* object_) {
    auto* self = reinterpret_cast<UnkScenario_ovb0032*>(scenario);
    auto* object = static_cast<UnkScenarioObject_ovb0032*>(object_);
    if (!object)
        return;
    if (self->field_0x3f0) {
        fn_71011a28e8(reinterpret_cast<UnkScenarioLists_b52*>(self), object);
        return;
    }
    u64 value = reinterpret_cast<u64>(object);
    Remove_ovb0032(self->list_0x380, value);
    if (object->field_0x1a8)
        return;
    if (IndexOf_ovb0032(self->list_0x2a0, value) < 0) {
        self->list_0x2a0.PushBack(value);
        fn_71002337d0(object, self);
    }
}

// Removes `object`: while +0x3f0 is set, moves it from +0x310 to +0x380 instead; otherwise takes it
// off all three lists and tells it (fn_71002337d0 with no scenario).
void fn_71011a2a24(UnkGameManagerSub_0x608* scenario, void* object_) {
    auto* self = reinterpret_cast<UnkScenario_ovb0032*>(scenario);
    auto* object = static_cast<UnkScenarioObject_ovb0032*>(object_);
    if (!object)
        return;
    if (self->field_0x3f0) {
        fn_71011a2c40(reinterpret_cast<UnkScenarioLists_b52*>(self), object);
        return;
    }
    u64 value = reinterpret_cast<u64>(object);
    Remove_ovb0032(self->list_0x2a0, value);
    Remove_ovb0032(self->list_0x380, value);
    Remove_ovb0032(self->list_0x310, value);
    fn_71002337d0(object, nullptr);
}

// --- The nearest actor ---

// A string id's data as the name read sees it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_ovb0032 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 uses;  // 0x44
    };
};

static inline const CRntString* Text_ovb0032(const CStrId& id) {
    ++reinterpret_cast<UnkStrIdText_ovb0032*>(id.data)->uses;
    return &reinterpret_cast<UnkStrIdText_ovb0032*>(id.data)->str;
}

void* fn_7100774014(UnkScenarioObject_ovb0032* actor);
bool fn_710007fc80(const CRntString* str, const CRntString* prefix);

// A function-local static id and its guard.
struct UnkStaticIdSlot_ovb0032 {
    CStrId id;
    u64 guard;
};
extern UnkStaticIdSlot_ovb0032 g_unk_7172a0bfb8 __attribute__((visibility("hidden")));

static inline const CStrId& DebugPrefix_ovb0032() {
    UnkStaticIdSlot_ovb0032& slot = g_unk_7172a0bfb8;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&slot.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&slot.guard)) {
            new (&slot.id) CStrId("ST_DEBUG_", true);
            fn_7100000250(fn_710000051c, &slot.id, &g_unk_7101a19000);
            __cxa_guard_release(&slot.guard);
        }
    }
    return slot.id;
}

// As src/game/CGameManager.cpp declares them.
struct UnkStartPoint;
struct UnkStartPointKey;

// The nearest actor of the list at +0x2a0 to the point `key` that fn_7100774014 accepts and whose
// name doesn't start with "ST_DEBUG_", or null.
UnkStartPoint* fn_71011a2e54(UnkGameManagerSub_0x608* scenario, const UnkStartPointKey* key) {
    auto* self = reinterpret_cast<UnkScenario_ovb0032*>(scenario);
    auto* point = reinterpret_cast<const UnkVec3f*>(key);
    const CStrId& prefix = DebugPrefix_ovb0032();
    UnkScenarioObject_ovb0032* best = nullptr;
    f32 best_distance = 1.7446743745805418e+19f;
    for (UnkScenarioList_ovb0032::Node* n = self->list_0x2a0.head; n; n = n->next) {
        auto* actor = reinterpret_cast<UnkScenarioObject_ovb0032*>(n->value);
        if (!fn_7100774014(actor))
            continue;
        if (fn_710007fc80(Text_ovb0032(actor->name), Text_ovb0032(prefix)))
            continue;
        UnkVec3f t = actor->xform.GetTranslation();
        f32 dx = t.x - point->x;
        f32 dy = t.y - point->y;
        f32 dz = t.z - point->z;
        f32 d = dx * dx + dy * dy + dz * dz;
        if (d < best_distance) {
            best_distance = d;
            best = actor;
        }
    }
    return reinterpret_cast<UnkStartPoint*>(best);
}

// --- The layers ---

struct UnkScenarioKey_ovb0032 {
    u8 pad_0x0[0x30];
    u32 lo;  // 0x30
    u32 hi;  // 0x34
};

struct UnkScenarioNode_ovb0032 {
    UnkScenarioKey_ovb0032* key;
    u8 value[0x38];  // 0x8
    UnkScenarioNode_ovb0032* next;  // 0x40
};

// A layer: something fn_710025bda0 brings up to date at +0x20 and a chained map (base/UnkChainedMap.h)
// whose buckets are at +0xb0.
struct UnkScenarioLayer_ovb0032 {
    u8 pad_0x0[0x20];
    u8 field_0x20[0x90];
    UnkScenarioNode_ovb0032** buckets;  // 0xb0
    u32 bucket_count;  // 0xb8
    u32 modulus;  // 0xbc
    u8 pad_0xc0[0x8];
};
static_assert(sizeof(UnkScenarioLayer_ovb0032) == 0xc8, "UnkScenarioLayer_ovb0032 layout");

void fn_710025bda0(void* self);

// The value stored under `*key` in layer `index`, or null.
void* fn_71011a3234(UnkScenarioLayer_ovb0032* layers, u32 index, UnkScenarioKey_ovb0032* const* key) {
    UnkScenarioLayer_ovb0032* layer = &layers[index];
    fn_710025bda0(layer->field_0x20);
    UnkScenarioNode_ovb0032* node = nullptr;
    if (layer->bucket_count) {
        UnkScenarioKey_ovb0032* k = *key;
        u32 hash = k->lo ^ k->hi;
        u32 slot = hash % layers[index].modulus;
        UnkScenarioNode_ovb0032** bucket = &layers[index].buckets[slot];
        if (bucket) {
            for (node = *bucket; node; node = node->next) {
                if (node->key == k)
                    break;
            }
        }
    }
    return node ? node->value : nullptr;
}

// --- The script call ---

// The text of a string id, counting the use; "" when it has none.
static inline const char* TextOrEmpty_ovb0032(const CStrId& id) {
    UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(id.data);
    ++entry->tail.field_0x44;
    const char* text = entry->buffer.data;
    return text ? text : g_unk_710162e2ed;
}

struct UnkScenarioScript_ovb0032 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};

void fn_710119fe50(UnkScenarioScript_ovb0032* self, const char* a, const char* b);

void fn_71011a49ec(UnkScenarioScript_ovb0032* self) {
    fn_710119fe50(self, TextOrEmpty_ovb0032(g_unk_7101d470e0), TextOrEmpty_ovb0032(g_unk_7101d470e0));
    self->vf_0x18();
}
