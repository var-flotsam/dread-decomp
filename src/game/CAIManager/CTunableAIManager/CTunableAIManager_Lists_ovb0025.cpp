// The CTunableAIManager unit (tools/tu_map.py, unit 2540): the AI manager's lists and the containers they
// are kept in. The manager (see fn_7100da7e2c, the constructor) holds a weak reference at +0x0, a list
// of registered items at +0x18 that its members at +0x80, +0x368, +0x3f0, +0x470 and +0x5e0 also hear
// about, a keyed table of named points at +0xbc8, six slot arrays from +0xbf8 (three kinds, two sides)
// and a timed weak reference at +0x2410. The pool lists of render items (0x18- and 0x20-byte nodes)
// drop each node's item from the render manager before freeing it. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CActorComponent.h"
#include "game/CAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU811.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflObject;

// ---- Pool lists of render items

extern char g_unk_7101aa92a8[];
extern char g_unk_7101aa92c8[];
extern char g_unk_7101aa92e8[];
extern char g_unk_7101aa9308[];

// Takes an item off the render manager's layers and forgets it.
static inline void DropItem(UnkItemBase_71011a91c4** item) {
    if (*item) {
        fn_71002199d8(g_unk_7101d47300, *item, 0);
        *item = nullptr;
    }
}

struct UnkItemNode20_b0025 {
    UnkItemBase_71011a91c4* item;
    u8 unk_0x8[0x10];
    UnkItemNode20_b0025* next;

    void Release() { DropItem(&item); }
};

struct UnkItemNode18_b0025 {
    UnkItemBase_71011a91c4* item;
    UnkRefObject* ref;
    UnkItemNode18_b0025* next;

    void Release() {
        DropItem(&item);
        if (UnkRefObject* r = ref) {
            if (--r->ref_count == 0)
                r->vf_0x10();
        }
    }
};

// The list's base class (its vtable is BaseVtable): releases and frees every node.
template <class Node, char* BaseVtable>
struct UnkItemListBase_b0025 {
    char* vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    void FreeNodes() {
        for (Node* n = head; n;) {
            Node* next = n->next;
            n->Release();
            allocator->vf_0x48(n);
            n = next;
        }
    }
    __attribute__((always_inline)) ~UnkItemListBase_b0025() {
        vtable = BaseVtable + 0x10;
        FreeNodes();
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// The list with its own block pool for the nodes (vtable Vtable).
template <class Node, char* BaseVtable, char* Vtable>
struct UnkItemList_b0025 : UnkItemListBase_b0025<Node, BaseVtable> {
    base::UnkBlockPool pool;

    __attribute__((always_inline)) ~UnkItemList_b0025() {
        this->vtable = Vtable + 0x10;
        pool.field_0x3c = true;
        this->FreeNodes();
        this->count = 0;
        this->head = nullptr;
        this->tail = nullptr;
        this->allocator = nullptr;
    }
};

using UnkItemListBase20_b0025 = UnkItemListBase_b0025<UnkItemNode20_b0025, g_unk_7101aa9308>;
using UnkItemListBase18_b0025 = UnkItemListBase_b0025<UnkItemNode18_b0025, g_unk_7101aa92c8>;
using UnkItemList20_b0025 = UnkItemList_b0025<UnkItemNode20_b0025, g_unk_7101aa9308, g_unk_7101aa92e8>;
using UnkItemList18_b0025 = UnkItemList_b0025<UnkItemNode18_b0025, g_unk_7101aa92c8, g_unk_7101aa92a8>;

void fn_7100da8534(UnkItemList20_b0025* self) {
    self->~UnkItemList20_b0025();
}

void fn_7100da8680(UnkItemList18_b0025* self) {
    self->~UnkItemList18_b0025();
}

void fn_7100dabda0(UnkItemListBase20_b0025* self) {
    self->~UnkItemListBase20_b0025();
}

void fn_7100dabbf4(UnkItemListBase18_b0025* self) {
    self->~UnkItemListBase18_b0025();
}

// The bases' deleting destructors.
void fn_7100dabe40(UnkItemListBase20_b0025* self) {
    self->vtable = g_unk_7101aa9308 + 0x10;
    self->FreeNodes();
    fn_7100083c00(self);
}

void fn_7100dabcc0(UnkItemListBase18_b0025* self) {
    self->vtable = g_unk_7101aa92c8 + 0x10;
    self->FreeNodes();
    fn_7100083c00(self);
}

// ---- Named points

// A named point: its name's string data (released in two steps, as CStrId::Reset does) and a position.
struct UnkNamedPoint_b0025 {
    UnkStringData* name;
    f32 x, y;
    f32 z;
    u32 unk_0x14;

    UnkNamedPoint_b0025(UnkNamedPoint_b0025&& other) : name(other.name) {
        other.name = &g_unk_7101d47108->empty;
        __builtin_memcpy(&x, &other.x, 8);
        z = other.z;
    }
    ~UnkNamedPoint_b0025() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (name && name != &pool->empty) {
                --name->ref_count;
                if (name->ref_count == 0) {
                    fn_71000803a4(pool, name);
                    pool = g_unk_7101d47108;
                }
            }
            name = &pool->empty;
        }
    }
};

// Removes the point at `index`, moving the later ones down.
void fn_7100dac138(base::UnkVector<UnkNamedPoint_b0025>* self, u32 index) {
    if (index < self->count) {
        self->data[index].~UnkNamedPoint_b0025();
        for (u32 i = index; i < self->count - 1; ++i) {
            UnkNamedPoint_b0025* e = &self->data[i];
            new (e) UnkNamedPoint_b0025(static_cast<UnkNamedPoint_b0025&&>(e[1]));
            (&self->data[i])[1].~UnkNamedPoint_b0025();
        }
        self->count = self->count - 1;
    }
}

// The point lists by key (values and keys in parallel arrays).
struct UnkPointMap_b0025 {
    base::UnkVector<UnkNamedPoint_b0025>* values;
    u32* keys;
    u32 count;
};

// Frees every list and empties the table.
void fn_7100daa178(UnkPointMap_b0025* self) {
    if (self->values) {
        for (u64 i = 0; i < self->count; ++i) {
            base::UnkVector<UnkNamedPoint_b0025>* points = &self->values[i];
            if (points->data) {
                for (u64 j = 0; j < points->count; ++j)
                    points->data[j].~UnkNamedPoint_b0025();
                points->count = 0;
                points->allocator->vf_0x48(points->data);
                points->capacity = 0;
            }
        }
    }
    self->count = 0;
}

// ---- Registered items

struct UnkMemberA_b0025;
struct UnkMemberB_b0025;
struct UnkMemberC_b0025;
struct UnkMemberD_b0025;

void fn_7100639f7c(UnkMemberA_b0025* self, void* item);
void fn_71006de93c(UnkMemberB_b0025* self, void* item);
void fn_71006bd124(UnkMemberC_b0025* self, void* item);
void fn_71006a20a0(UnkMemberD_b0025* self, void* item);

struct UnkItemLink_b0025 {
    void* item;
    UnkItemLink_b0025* next;
};

struct UnkNavOwner_b0025 {
    UnkWeakHandle* field_0x0;
    u8 unk_0x8[0x10];
    u32 count;
    UnkItemLink_b0025* head;
    UnkItemLink_b0025* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    u8 unk_0x3c[0x44];
    u8 field_0x80[0x2e8];
    u8 field_0x368[0x88];
    u8 field_0x3f0[0x80];
    u8 field_0x470[0x170];
    u8 field_0x5e0[0x8];
};
static_assert(__builtin_offsetof(UnkNavOwner_b0025, field_0x5e0) == 0x5e0, "UnkNavOwner_b0025 layout");

static inline s32 IndexOf(UnkNavOwner_b0025* self, void* item) {
    u32 i = 0;
    for (UnkItemLink_b0025* n = self->head; n; n = n->next, ++i) {
        if (n->item == item)
            return static_cast<s32>(i);
    }
    return -1;
}

// Registers `item` (once) and hands it to the members that track items.
void fn_7100da9e88(UnkNavOwner_b0025* self, void* item) {
    if (IndexOf(self, item) < 0) {
        auto* link = static_cast<UnkItemLink_b0025*>(self->allocator->vf_0x40(0x10, 8, self->flags));
        if (link) {
            link->item = item;
            link->next = nullptr;
            if (self->count) {
                // Linked through an untyped copy: the original reloads the count after it.
                __builtin_memcpy(&self->tail->next, &link, sizeof(link));
                link->next = nullptr;
                u32 count = self->count + 1;
                self->tail = link;
                self->count = count;
            } else {
                self->head = link;
                self->tail = link;
                self->count = 1;
            }
        }
    }
    fn_7100639f7c(reinterpret_cast<UnkMemberA_b0025*>(self->field_0x80), item);
    fn_71006de93c(reinterpret_cast<UnkMemberB_b0025*>(self->field_0x368), item);
    fn_71006bd124(reinterpret_cast<UnkMemberC_b0025*>(self->field_0x3f0), item);
    fn_71006bd124(reinterpret_cast<UnkMemberC_b0025*>(self->field_0x470), item);
    fn_71006a20a0(reinterpret_cast<UnkMemberD_b0025*>(self->field_0x5e0), item);
}

// ---- Weak references

struct UnkAIManagerRef_b0025 {
    u8 unk_0x0[0xbf0];
    UnkWeakHandle* field_0xbf0;
};

// Drops the reference at +0xbf0 when it refers to `target`.
void fn_7100daaee4(UnkAIManagerRef_b0025* self, UnkWeakTarget* target) {
    if (!target)
        return;
    UnkWeakHandle* handle = self->field_0xbf0;
    UnkWeakTarget* current = handle ? handle->target : nullptr;
    if (current == target)
        ResetRef(&self->field_0xbf0);
}

// The timed reference fn_7100da9018 (src/game/CAIManager/CTunableAIManager/fn_7100da9018.cpp) sets.
struct UnkTimedRef;

struct UnkTimedRefOwner_b0025 {
    u8 unk_0x0[0x2410];
    UnkWeakHandle* field_0x2410;
    f32 field_0x2418;
    u8 unk_0x241c[0xc];
    u8 field_0x2428[0x80];
    u8 field_0x24a8[0x28];
};

void* fn_71004067a4(void*);
void fn_7100874460(void* list, UnkWeakTarget** item);
void fn_71007aa604(void* component);

static inline UnkWeakTarget* RefTarget(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

// Fires the timed reference: when its target has the component fn_71004067a4 looks for, the target
// goes through the lists at +0x2428 and +0x24a8 around fn_71007aa604; then the reference is dropped.
void fn_7100da9194(UnkTimedRef* ref) {
    auto* self = reinterpret_cast<UnkTimedRefOwner_b0025*>(ref);
    if (void* component = fn_71004067a4(RefTarget(self->field_0x2410))) {
        UnkWeakTarget* target = RefTarget(self->field_0x2410);
        fn_7100874460(self->field_0x2428, &target);
        fn_71007aa604(component);
        target = RefTarget(self->field_0x2410);
        fn_7100874460(self->field_0x24a8, &target);
    }
    ResetRef(&self->field_0x2410);
    self->field_0x2418 = 0.0f;
}

// ---- Slot arrays

struct UnkSlot_b0025 {
    UnkSwitchObject* object;
    bool free;
    u8 unk_0x9[7];
} __attribute__((packed, aligned(4)));

struct UnkSlotArray_b0025 {
    u32 count;
    UnkSlot_b0025 slots[64];
};
static_assert(sizeof(UnkSlotArray_b0025) == 0x404, "UnkSlotArray_b0025 layout");

struct UnkSlotOwner_b0025 {
    u8 unk_0x0[0xbf8];
    UnkSlotArray_b0025 groups[2][3];
};
static_assert(__builtin_offsetof(UnkSlotOwner_b0025, groups[1][0]) == 0x1804, "UnkSlotOwner_b0025 layout");

static inline UnkSlot_b0025* FindSlot(UnkSlotArray_b0025* array, UnkSwitchObject* object) {
    UnkSlot_b0025* slot = array->slots;
    u64 size = (u64)array->count * sizeof(UnkSlot_b0025);
    asm("" : "+r"(size));
    for (; size != 0; ++slot, size -= sizeof(UnkSlot_b0025)) {
        if (slot->object == object)
            return slot;
    }
    return nullptr;
}

// Gives `object` back: marks its slot free and switches it off.
void fn_7100da8f00(UnkSlotOwner_b0025* self, UnkSwitchObject* object) {
    for (u32 kind = 0; kind < 3; ++kind) {
        for (u32 side = 0; side < 2; ++side) {
            if (UnkSlot_b0025* slot = FindSlot(&self->groups[side][kind], object)) {
                slot->free = true;
                fn_7100145e94(object, false);
                return;
            }
        }
    }
}

// ---- Emmy queries

struct UnkListedItem;
base::UnkVector<UnkListedItem*>* fn_7100552d7c();
struct UnkEmmyAIComponent_b46a1;
bool fn_710112baa0(void* actor);

struct UnkListedEntry_b0025 {
    u8 unk_0x0[0x10];
    UnkReflObject* component;
};

struct UnkEmmyCheckActor_b0025 {
    u8 unk_0x0[0x350];
    bool field_0x350;
};

// False when some listed CEmmyAIComponent (not busy at +0x27720, or accepted by fn_71005436e8) answers
// fn_710024fbc8, for an actor with +0x350 set that fn_710112baa0 rejects; true otherwise.
bool fn_7100daa560(void* self, UnkEmmyCheckActor_b0025* actor) {
    if (!actor->field_0x350 || fn_710112baa0(actor))
        return true;
    base::UnkVector<UnkListedItem*>* items = fn_7100552d7c();
    UnkListedItem** it = items->data;
    UnkListedItem** end = it + items->count;
    for (; it != end; ++it) {
        auto* entry = reinterpret_cast<UnkListedEntry_b0025*>(*it);
        if (!entry)
            continue;
        UnkReflObject* component = entry->component;
        if (!component || !fn_710009d50c(component->vf_0x0(), fn_710053b934()))
            continue;
        bool flag = reinterpret_cast<u8*>(component)[0x27720];
        if (flag && !fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(component)))
            continue;
        if (fn_710024fbc8(component))
            return false;
    }
    return true;
}

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
bool fn_71009e12d0(void*);
struct UnkStruct_710117af10;
struct UnkCls_710117af10;

struct UnkFallMovement_b0025 {
    virtual void vf_0x0();
    bool IsBusy() {
        return reinterpret_cast<bool (*)(UnkFallMovement_b0025*)>((*reinterpret_cast<void***>(this))[0x3b8 / 8])(this);
    }
    u8 unk_0x8[0xf1];
    bool field_0xf9;
};

struct UnkFallPlayer_b0025 {
    u8 unk_0x0[0x208];
    UnkFallMovement_b0025* movement;
    u8 unk_0x210[0x10];
    UnkStruct_710117af10* animation;
};

// Whether the player is in a state where the "fallend" animation could play but its animation
// does not take it (fn_710117af10).
bool fn_7100dadf20() {
    auto* player = reinterpret_cast<UnkFallPlayer_b0025*>(fn_7100dec788(g_unk_7101d47310));
    UnkFallMovement_b0025* movement = player->movement;
    if (!movement || movement->field_0xf9 || movement->IsBusy() || fn_71009e12d0(movement))
        return false;
    UnkStruct_710117af10* animation = player->animation;
    CStrId name("fallend", true);
    return !fn_710117af10(animation, reinterpret_cast<UnkCls_710117af10**>(&name), false);
}
