// CDemolitionBlockComponent's unit (tools/tu_map.py 1211): restores the block from the blackboard. Reads the
// stored "used" flag of the current scenario's section; a used block tells its owner's source to stop, an unused
// one sends each holder's target its two messages and sets it enabled or not. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CDemolitionBlockComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game/UnkTU372.h"
#include "game/UnkTU437.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflRegistryMap;

extern UnkProperty g_unk_717271a040;

struct UnkReflObjectRef;
UnkReflObjectRef* fn_71009c0e24(void* owner, u64 id);
struct UnkDemolitionBlock_l7b046c;
void fn_71007b0948(UnkDemolitionBlock_l7b046c* self);

// What a holder's handle refers to: a counted reference to the holder's target.
struct UnkHolderRef_l7b046c : base::UnkRefCounted {
    void* target;  // 0x10
};

// A lazily resolved holder (0x40 bytes): the key is resolved to a handle on first use.
struct UnkHolder_l7b046c {
    u8 pad_0x0[0x8];
    u8 key[0x28];  // 0x8
    u32 state;     // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38

    void* Target() {
        if (state == 1) {
            {
                base::UnkRefPtrMember fetched = fn_7100353be8(key);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fetched));
            }
            state = 2;
        }
        if (base::UnkRefCounted* ref = handle.ptr)
            return static_cast<UnkHolderRef_l7b046c*>(ref)->target;
        return nullptr;
    }
};
static_assert(sizeof(UnkHolder_l7b046c) == 0x40, "layout");

struct UnkHolderList_l7b046c {
    void* data;
    u32 count;  // 0x8
};
UnkHolder_l7b046c* fn_71007b0e04(UnkHolderList_l7b046c* self, u32 index);

struct UnkDemolitionBlock_l7b046c {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x48];
    UnkReflRegistryMap properties;  // 0x80
    u8 pad_0xb0[0x8];
    bool used;  // 0xb8
    bool field_0xb9;
    u8 pad_0xba[0x26];
    UnkHolderList_l7b046c holders_0xe0;  // 0xe0
    u8 pad_0xf0[0x18];
    UnkHolderList_l7b046c holders_0x108;  // 0x108
};
static_assert(__builtin_offsetof(UnkDemolitionBlock_l7b046c, properties) == 0x80, "layout");
static_assert(__builtin_offsetof(UnkDemolitionBlock_l7b046c, used) == 0xb8, "layout");
static_assert(__builtin_offsetof(UnkDemolitionBlock_l7b046c, holders_0xe0) == 0xe0, "layout");
static_assert(__builtin_offsetof(UnkDemolitionBlock_l7b046c, holders_0x108) == 0x108, "layout");

// The name of `property` in the map's values (the value past the last when the map lacks it).
static inline const CStrId* FindName(UnkReflRegistryMap* map, const UnkProperty& property) {
    u64 index = 0;
    u32 count = map->count;
    if (count) {
        CStrId* keys = map->keys;
        for (; index != count; ++index) {
            if (keys[index].data == property.name.data)
                break;
        }
    }
    return reinterpret_cast<const CStrId*>(&map->values[index]);
}

void fn_71007b046c(UnkDemolitionBlock_l7b046c* self) {
    self->used = false;
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x608) {
        self->field_0xb9 = true;
    } else {
        CBlackboard* blackboard = &manager->field_0x968;
        {
            CStrId section_id = fn_71011a2fbc(manager->field_0x608);
            const CStrId* name = FindName(&self->properties, g_unk_717271a040);
            if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
                fn_710006d11c(section, name, &self->used);
        }
        self->field_0xb9 = !self->used;
        if (self->used) {
            {
                CStrId unused(base::global::UnkStrIdInline(), nullptr, false);
                fn_71007b0948(self);
            }
            if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner))) {
                if (UnkReflObjectRef* source = fn_71009c0e24(target, 0xb15b31b835c17820))
                    fn_71009bfda8(reinterpret_cast<UnkOwner*>(target), reinterpret_cast<UnkSource*>(source));
            }
            fn_7100232864(reinterpret_cast<UnkEntityState*>(self->owner), false);
            return;
        }
    }
    // Sends each holder's target the message and sets the target enabled or not.
    auto send = [](UnkHolderList_l7b046c* list, u64 id, bool enabled) {
        for (u32 i = 0; i < list->count; ++i) {
            if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(fn_71007b0e04(list, i)->Target()))) {
                if (UnkReflObjectRef* source = fn_71009c0e24(target, id))
                    fn_71009bfda8(reinterpret_cast<UnkOwner*>(target), reinterpret_cast<UnkSource*>(source));
            }
            fn_7100232864(reinterpret_cast<UnkEntityState*>(fn_71007b0e04(list, i)->Target()), enabled);
        }
    };
    send(&self->holders_0xe0, 0xb15b31b835c17820, false);
    send(&self->holders_0x108, 0x13ae275a1bcd6558, true);
}
