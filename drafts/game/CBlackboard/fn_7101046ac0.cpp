// CBlackboard (unit 2653): the destructor. The sections are cleared, each entry of the map at 0x30 is
// deleted, the map is emptied, then the strings, the map and the section arrays are destroyed.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;

template <class Value>
struct UnkIdTable;
struct UnkReleasedStrId;

// The blackboard's vtable (the vtable pointer is its address + 0x10).
extern char g_unk_7101cac7c8[];

void fn_71000d99d0(UnkIdTable<UnkReleasedStrId>* self);
void fn_7101046d58(UnkKeyedStrIds* self);

// An entry's property arrays: cleared by fn_71000d99d0, then both arrays are freed.
struct UnkEntryProperties_l046ac0 {
    UnkPropertyArrays arrays;

    ~UnkEntryProperties_l046ac0() {
        fn_71000d99d0(reinterpret_cast<UnkIdTable<UnkReleasedStrId>*>(&arrays));
        if (arrays.ids)
            arrays.allocator->vf_0x48(arrays.ids);
        if (arrays.values)
            arrays.allocator->vf_0x48(arrays.values);
    }
};

struct UnkEntryIds_l046ac0 {
    UnkKeyedStrIds table;

    ~UnkEntryIds_l046ac0() { fn_7101046d58(&table); }
};

// What the map at 0x30 holds: a key, property arrays and a table of ids.
struct UnkBlackboardEntry_l046ac0 {
    u64 field_0x0;
    CStrId key;
    UnkEntryProperties_l046ac0 properties;
    UnkEntryIds_l046ac0 ids;
};

struct UnkBlackboardNode_l046ac0 {
    CStrId key;
    UnkBlackboardEntry_l046ac0* entry;
    UnkBlackboardNode_l046ac0* next;
};

struct UnkBlackboardMap_l046ac0 {
    base::IAllocator* allocator;
    u32 flags;
    u32 count;
    base::global::CRntVector<UnkBlackboardNode_l046ac0*> buckets;
    UnkBlackboardNode_l046ac0* last;
    UnkBlackboardNode_l046ac0* first;
};

struct UnkBlackboardLayout_l046ac0 {
    char* vtable;
    UnkKeyedStrIds sections;
    UnkBlackboardMap_l046ac0 map;
    u64 field_0x78;
    base::global::CRntString field_0x80;
    base::global::CRntString field_0xb0;
};
static_assert(__builtin_offsetof(UnkBlackboardLayout_l046ac0, map) == 0x30, "layout");
static_assert(__builtin_offsetof(UnkBlackboardLayout_l046ac0, field_0xb0) == 0xb0, "layout");

void fn_7101046c68(UnkBlackboardMap_l046ac0* map);

void fn_7101046ac0(CBlackboard* blackboard) {
    UnkBlackboardLayout_l046ac0* self = reinterpret_cast<UnkBlackboardLayout_l046ac0*>(blackboard);
    self->vtable = g_unk_7101cac7c8 + 0x10;
    fn_7101047ea8(self);
    for (UnkBlackboardNode_l046ac0* node = self->map.first; node; node = node->next) {
        if (node->entry) {
            delete node->entry;
            node->entry = nullptr;
        }
    }
    UnkBlackboardMap_l046ac0& map = self->map;
    if (map.count) {
        UnkBlackboardNode_l046ac0* node = map.first;
        while (node) {
            UnkBlackboardNode_l046ac0* next = node->next;
            node->key.Reset();
            map.allocator->vf_0x48(node);
            node = next;
        }
        memset(map.buckets.data, 0, map.buckets.capacity * u64(8));
        map.count = 0;
        map.last = nullptr;
        map.first = nullptr;
    }
    self->field_0x78 = 0;
    CRntString_Destroy(&self->field_0xb0);
    CRntString_Destroy(&self->field_0x80);
    fn_7101046c68(&self->map);
    fn_7101046d58(&self->sections);
}
