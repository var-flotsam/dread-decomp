// CArenaManager::CTunableArenaManager's unit (tools/tu_map.py): makes `item` (a spawn group) the arena
// entry's current group: stops the old one, points the entry at the new one, turns its members off and
// back on by the blackboard flag, and stores the group's name in the scenario's section.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/PrimitiveType.h"
#include "game/CAbilityShinespark.h"
#include "game/CActorComponent.h"
#include "game/CArenaManager/CTunableArenaManager.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#include "../../../../src/game_aca57c/UnkBufferString.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::PrimitiveType;

namespace base::global {
class TRntString256;
}  // namespace base::global
extern template const CType& PrimitiveType<base::global::TRntString256>::Get();

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

struct UnkArenaItemName_ovb0108 {
    u8 unk_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkArenaItemDef_ovb0108 {
    u8 unk_0x0[0x30];
    UnkArenaItemName_ovb0108* name;  // 0x30
};

struct UnkSwifterComponent_ovb0114;

struct UnkSwifterActor_ovb0114 {
    u8 pad_0x0[0x10];
    UnkSwifterComponent_ovb0114* component;  // 0x10
};

// A spawned member (0x40 bytes); fn_71003df2d0 gives its actor.
struct UnkSwifterMember_ovb0114 {
    u8 pad_0x0[0x40];
};
UnkSwifterActor_ovb0114** fn_71003df2d0(UnkSwifterMember_ovb0114* member);

struct UnkComponent_71011199cc;
void fn_71011199cc(UnkComponent_71011199cc* self);
void* fn_7100e161c0(void* self, const base::global::CStrId& id);

// A spawned member's entity, as far as its name goes.
struct UnkArenaMemberEntity_le15cec {
    u8 unk_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkArenaMemberComponent_le15cec {
    u8 unk_0x0[0x30];
    UnkArenaMemberEntity_le15cec* entity;  // 0x30
};

// The spawn group an entry points at through its weak handle.
struct UnkArenaGroup_le15cec : UnkWeakTarget {
    u8 unk_0x8[0x28];
    UnkArenaItemName_ovb0108* name;  // 0x30
    u8 unk_0x38[0x30];
    UnkSwifterMember_ovb0114* members;  // 0x68
    u32 memberCount;                    // 0x70
};

static inline UnkArenaMemberEntity_le15cec* EntityOf(UnkSwifterMember_ovb0114* member) {
    return reinterpret_cast<UnkArenaMemberComponent_le15cec*>((*fn_71003df2d0(member))->component)->entity;
}

// Whether the id's text contains `str` (an empty id contains nothing).
static inline bool Contains(const CStrId& id, const char* str) {
    ++id.data->reads;
    if (id.data == &g_unk_7101d47108->empty)
        return false;
    return CRntString_Contains(&id.data->String(), str);
}

// The index of a key, or the count if it isn't there.
static inline u64 IndexOf(const UnkStrIdTable& table, const UnkPropertyName& key) {
    u32 count = table.count;
    for (u64 i = 0; i != count; ++i) {
        if (table.keys[i].data == key.data)
            return i;
    }
    return count;
}

// Stores `name` in the scenario's blackboard section under the property's value id.
static inline void StoreName(UnkArenaEntry_ovb0108* entry, UnkGameManagerSub_0x608* owner, CBlackboard* blackboard,
                             const CStrId& name) {
    UnkTypedValue v;
    CStrId section_id = fn_71011a2fbc(owner);
    UnkStrIdTable& table = entry->field_0x58.table;
    u64 index = IndexOf(table, g_unk_71727c0948.name);
    UnkStrIdTableValue* value = &table.values[index];
    const char* text = name.Text();
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            UnkBufferString<0x100, g_unk_7101a194b8> str(text);
            v.type = &PrimitiveType<base::global::TRntString256>::Get();
            v.value = &str;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(value), &v);
        }
    }
}

void fn_7100e15cec(UnkArenaEntry_ovb0108* entry, UnkGameManagerSub_0x608* owner, CBlackboard* blackboard,
                   UnkArenaItemDef_ovb0108* item) {
    UnkWeakHandle** current = reinterpret_cast<UnkWeakHandle**>(&entry->field_0x38.ptr);
    if (*current && (*current)->target)
        fn_7101119a50((*current)->target);

    UnkWeakTarget* target = reinterpret_cast<UnkWeakTarget*>(item);
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(current, handle);

    if (UnkArenaGroup_le15cec* group = handle ? static_cast<UnkArenaGroup_le15cec*>(handle->target) : nullptr) {
        for (UnkArenaRefPtr_ovb0108* it = entry->field_0x8.begin(); it != entry->field_0x8.end(); ++it) {
            if (it->ptr && it->ptr->def) {
                UnkArenaItemDef_ovb0108* def = it->ptr->def;
                if (void* component = fn_7100e161c0(def, def->name->name))
                    fn_710024fa0c(static_cast<UnkStruct_710024fa0c*>(component), false);
            }
        }

        group = static_cast<UnkArenaGroup_le15cec*>((*current)->target);
        UnkSwifterMember_ovb0114* end = group->members + group->memberCount;
        for (UnkSwifterMember_ovb0114* it = group->members; it != end; ++it)
            fn_7100232864(reinterpret_cast<UnkEntityState*>(EntityOf(it)), false);

        bool flag = false;
        {
            CStrId section_id = fn_71011a2fbc(owner);
            UnkStrIdTable& table = entry->field_0x58.table;
            u64 index = IndexOf(table, g_unk_71727c0998.name);
            UnkStrIdTableValue* value = &table.values[index];
            if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                fn_710006d11c(s, reinterpret_cast<const CStrId*>(value), &flag);
        }

        UnkArenaMemberEntity_le15cec* found = nullptr;
        for (UnkSwifterMember_ovb0114* it = group->members; it != end; ++it) {
            UnkArenaMemberEntity_le15cec* entity = EntityOf(it);
            if (Contains(entity->name, g_unk_71727c0a58.data) == flag) {
                found = entity;
                break;
            }
        }
        if (found)
            fn_7100232864(reinterpret_cast<UnkEntityState*>(found), true);

        UnkArenaGroup_le15cec* now = *current ? static_cast<UnkArenaGroup_le15cec*>((*current)->target) : nullptr;
        if (flag) {
            if (void* component = fn_7100e161c0(now, now->name->name))
                fn_710024fa0c(static_cast<UnkStruct_710024fa0c*>(component), true);
        } else {
            fn_71011199cc(reinterpret_cast<UnkComponent_71011199cc*>(now));
        }
    }

    StoreName(entry, owner, blackboard, item ? item->name->name : g_unk_7101d470e0);
}
