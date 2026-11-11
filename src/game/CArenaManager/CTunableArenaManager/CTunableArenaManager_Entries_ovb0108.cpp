// CArenaManager::CTunableArenaManager's unit (tools/tu_map.py): the destructor of the 0x80-byte
// arena entry that fn_7100e1480c (src/game/CArenaManager/CTunableArenaManager_VectorFree_b44.cpp)
// frees: a string-id table (+0x58), two-step string ids (+0x50, +0x30, +0x0), a refcounted
// pointer (+0x38) and an array of refcounted pointers (+0x8). Also the entry's "AllDead" update
// (fn_7100e14d74: the flag at +0x40 is set, stored in the scenario's blackboard section under the
// property's value id, and the timer at +0x38 stopped and released), and the lookups of an entry by
// name in the manager's entries (std::find_if over UnkIter, the key captured by value) that hand it
// on. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

#include <algorithm>

#include "../../../game_s7/UnkIter.h"

// An intrusively refcounted object; the count is a union member so its decrement forces the
// reload of the array's count, as in the original.
struct UnkArenaItemDef_ovb0108;
struct UnkArenaRefObj_ovb0108 {
    virtual void vf_0x0();
    virtual ~UnkArenaRefObj_ovb0108();
    union {
        s32 ref_count;
    };
    UnkArenaItemDef_ovb0108* def;  // 0x10
};

struct UnkArenaRefPtr_ovb0108 {
    UnkArenaRefObj_ovb0108* ptr;

    UnkArenaRefPtr_ovb0108(const UnkArenaRefPtr_ovb0108& other) : ptr(other.ptr) {
        if (ptr)
            ++ptr->ref_count;
    }
    ~UnkArenaRefPtr_ovb0108() {
        if (UnkArenaRefObj_ovb0108* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkArenaRefArray_ovb0108 {
    UnkArenaRefPtr_ovb0108* data;
    u32 count;
    u32 capacity;
    u8 unk_0x10[0x10];
    base::UnkAllocator* allocator;  // 0x20

    UnkArenaRefPtr_ovb0108* begin() { return data; }
    UnkArenaRefPtr_ovb0108* end() { return data + count; }

    ~UnkArenaRefArray_ovb0108() {
        if (data) {
            if (count) {
                u64 i = 0;
                do {
                    data[i].~UnkArenaRefPtr_ovb0108();
                } while (++i < count);
            }
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkArenaTable_ovb0108 {
    UnkStrIdTable table;

    ~UnkArenaTable_ovb0108() {
        fn_71004f11f8(&table);
        if (table.keys)
            table.allocator->vf_0x48(table.keys);
        if (table.values)
            table.allocator->vf_0x48(table.values);
    }
};

struct UnkArenaEntry_ovb0108 {
    base::UnkStrIdMember field_0x0;
    UnkArenaRefArray_ovb0108 field_0x8;
    base::UnkStrIdMember field_0x30;
    base::UnkRefPtrMember field_0x38;
    bool all_dead;  // 0x40
    u8 unk_0x41[0xf];
    base::UnkStrIdMember field_0x50;
    UnkArenaTable_ovb0108 field_0x58;
};
static_assert(sizeof(UnkArenaEntry_ovb0108) == 0x80, "UnkArenaEntry_ovb0108 layout");

__attribute__((flatten)) void fn_7100e167d4(char* self) {
    reinterpret_cast<UnkArenaEntry_ovb0108*>(self)->~UnkArenaEntry_ovb0108();
}

using base::global::CStrId;

struct UnkArenaTimer_ovb0108 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
extern UnkProperty g_unk_71727c0970;  // "AllDead"

// The index of a key, or the count if it isn't there.
static inline u64 IndexOf(const UnkStrIdTable& table, const UnkPropertyName& key) {
    u32 count = table.count;
    for (u64 i = 0; i != count; ++i) {
        if (table.keys[i].data == key.data)
            return i;
    }
    return count;
}

void fn_7100e14d74(UnkArenaEntry_ovb0108* self, UnkGameManagerSub_0x608* owner, CBlackboard* blackboard) {
    self->all_dead = true;
    {
        UnkTypedValue v;
        CStrId section_id = fn_71011a2fbc(owner);
        UnkStrIdTable& table = self->field_0x58.table;
        u64 index = IndexOf(table, g_unk_71727c0970.name);
        UnkStrIdTableValue* value = &table.values[index];
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!blackboard->field_0xe8) {
                v.type = fn_710000d228();
                v.value = &self->all_dead;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(value), &v);
            }
        }
    }
    UnkArenaTimer_ovb0108* timer = static_cast<UnkArenaTimer_ovb0108*>(self->field_0x38.ptr);
    if (timer && timer->field_0x10) {
        fn_7101119a50(timer->field_0x10);
        if (base::UnkRefCounted* p = self->field_0x38.ptr) {
            if (--p->ref_count == 0)
                delete p;
            self->field_0x38.ptr = nullptr;
        }
    }
}

struct UnkArenaEntries_ovb0108 {
    UnkGameManagerSub_0x608* owner;
    UnkArenaEntry_ovb0108* data;  // 0x8
    u32 count;                    // 0x10

    UnkArenaEntry_ovb0108* begin() { return data; }
    UnkArenaEntry_ovb0108* end() { return data + count; }
    // The entry named `key`, or end().
    UnkArenaEntry_ovb0108* Find(const CStrId& key) {
        return std::find_if(UnkIter<UnkArenaEntry_ovb0108>{begin()}, UnkIter<UnkArenaEntry_ovb0108>{end()},
                            [key](const UnkArenaEntry_ovb0108& e) { return e.field_0x0.data == key.data; })
            .p;
    }
};

struct UnkArenaActor_ovb0108 {
    u8 unk_0x0[0x1c0];
    CStrId name;  // 0x1c0
};

void fn_7100e14fc8(UnkArenaEntry_ovb0108* entry, UnkGameManagerSub_0x608* owner, CBlackboard* blackboard);

void fn_7100e14c8c(UnkArenaEntries_ovb0108* self, UnkArenaActor_ovb0108* actor) {
    if (actor->name.data == &g_unk_7101d47108->empty)
        return;
    UnkArenaEntry_ovb0108* it = self->Find(actor->name);
    fn_7100e14d74(it == self->end() ? nullptr : it, self->owner, &g_unk_7101d47310->field_0x968);
}

void fn_7100e14ec0(UnkArenaEntries_ovb0108* self, const CStrId& key) {
    UnkArenaEntry_ovb0108* it = self->Find(key);
    if (it && it != self->end())
        fn_7100e14fc8(it, self->owner, &g_unk_7101d47310->field_0x968);
}

struct UnkArenaItemName_ovb0108 {
    u8 unk_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkArenaItemDef_ovb0108 {
    u8 unk_0x0[0x30];
    UnkArenaItemName_ovb0108* name;  // 0x30
};

UnkArenaItemDef_ovb0108* fn_7100e15bb0(UnkArenaEntry_ovb0108* self, const CStrId& name);
void fn_7100e15cec(UnkArenaEntry_ovb0108* entry, UnkGameManagerSub_0x608* owner, CBlackboard* blackboard,
                   UnkArenaItemDef_ovb0108* item);

void fn_7100e1593c(UnkArenaEntries_ovb0108* self, const CStrId& key, const CStrId& value) {
    UnkArenaEntry_ovb0108* it = self->Find(key);
    if (__builtin_expect(!it || self->end() == it, 0))
        return;
    it->field_0x30 = reinterpret_cast<const base::UnkStrIdMember&>(value);
}

void fn_7100e15a84(UnkArenaEntries_ovb0108* self, const CStrId& key, const CStrId& item_name) {
    UnkArenaEntry_ovb0108* it = self->Find(key);
    if (it && it != self->end()) {
        if (UnkArenaItemDef_ovb0108* item = fn_7100e15bb0(it, item_name))
            fn_7100e15cec(it, self->owner, &g_unk_7101d47310->field_0x968, item);
    }
}
