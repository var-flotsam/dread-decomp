// Unit 1176 (hint 'CCentralUnitComponent'; no reliable name): the array of 0x80-byte entries (two callback sets of
// vtable 0x7101a20950 each; src/game/CCentralUnitComponent/CCentralUnitComponent_b40.cpp has its Reserve and copies)
// with its element copy (fn_7100779d34), append (fn_710077a700), grow-and-default-construct (fn_710077ad70,
// fn_710077ae90), and the small state machine the central unit runs on its current state (+0x40), and the
// handle source at +0x540. Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CCharClassStartPointComponent.h"
#include "game/CInventoryComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU341.h"
#include "game/UnkTU367.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

using base::global::CStrId;

struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkCallbackBase {
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkCallbackBase() : field_0x30(0) {}
};

// The callback set of vtable 0x7101a20950 (0x40 bytes).
struct UnkCallbackSet_7101a20950 : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    UnkCallbackSet_7101a20950() : owner(nullptr) {}
    UnkCallbackSet_7101a20950(const UnkCallbackSet_7101a20950& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p != owner) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = owner) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            owner = p;
        }
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }

    UnkCallbackSet_7101a20950(UnkCallbackSet_7101a20950&& other);  // fn_71002458a4

    __attribute__((always_inline)) ~UnkCallbackSet_7101a20950() {
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkSetPair_b0230 {
    UnkCallbackSet_7101a20950 first;
    UnkCallbackSet_7101a20950 second;
    UnkSetPair_b0230() = default;
    __attribute__((always_inline)) UnkSetPair_b0230(const UnkSetPair_b0230& o) : first(o.first), second(o.second) {}
    __attribute__((always_inline)) UnkSetPair_b0230(UnkSetPair_b0230&& o)
        : first(static_cast<UnkCallbackSet_7101a20950&&>(o.first)),
          second(static_cast<UnkCallbackSet_7101a20950&&>(o.second)) {}
};

struct UnkPairVec_b0230 {
    UnkSetPair_b0230* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// As src/game/CCentralUnitComponent/CCentralUnitComponent_b40.cpp declares them.
// An entry of the array, destroyed out of line (as src/game/CCentralUnitComponent/CCentralUnitComponent_b40.cpp).
struct UnkEntry_cu {
    u8 data[0x80];
    ~UnkEntry_cu() asm("fn_7100778228");
};
void fn_710077a630(base::UnkVector<UnkEntry_cu>* self, u32 n);

__attribute__((always_inline)) static inline void DestroyEntry_b0230(UnkSetPair_b0230* pair) {
    reinterpret_cast<UnkEntry_cu*>(pair)->~UnkEntry_cu();
}
void fn_710077ae90(UnkPairVec_b0230* self, u32 n);

// Copies one entry into place (fn_7100779d34 as src/clones/clones_02.cpp declares it).
// fn_7100779d34 (src/clones/clones_02.cpp declares it with untyped pointers, which would change the code).
__attribute__((noinline)) void CopyPair_b0230(UnkSetPair_b0230* self, const UnkSetPair_b0230* other)
    asm("_Z13fn_7100779d34PvS_");
void CopyPair_b0230(UnkSetPair_b0230* self, const UnkSetPair_b0230* other) {
    new (self) UnkSetPair_b0230(*other);
}

void fn_710077a700(base::UnkVector<UnkEntry_cu>* vec, const UnkEntry_cu* element) {
    UnkPairVec_b0230* self = reinterpret_cast<UnkPairVec_b0230*>(vec);
    const UnkSetPair_b0230* value = reinterpret_cast<const UnkSetPair_b0230*>(element);
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkSetPair_b0230* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkSetPair_b0230*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkSetPair_b0230), 8, self->flags));
        if (self->data) {
            UnkSetPair_b0230* src = self->data;
            UnkSetPair_b0230* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkSetPair_b0230(static_cast<UnkSetPair_b0230&&>(*src));
                DestroyEntry_b0230(src);
            }
            self->allocator->vf_0x48(self->data);
        }
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    } else {
        p = self->data;
    }
    CopyPair_b0230(&p[n], value);
    self->count = self->count + 1;
}

// Makes the array hold `n` entries, default-constructing the new ones (after fn_710077a630 has made room).
void fn_710077ae90(UnkPairVec_b0230* self, u32 n) {
    fn_710077a630(reinterpret_cast<base::UnkVector<UnkEntry_cu>*>(self), n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkSetPair_b0230();
    self->count = n;
}

// The entry at `index`; an index past the end first grows the array (to the next multiple of the step) and
// default-constructs the entries up to it.
UnkSetPair_b0230* fn_710077ad70(UnkPairVec_b0230* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                UnkSetPair_b0230* p = static_cast<UnkSetPair_b0230*>(
                    self->allocator->vf_0x40((u64)grown * sizeof(UnkSetPair_b0230), 8, self->flags));
                if (self->data) {
                    UnkSetPair_b0230* src = self->data;
                    UnkSetPair_b0230* dst = p;
                    for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                        new (dst) UnkSetPair_b0230(static_cast<UnkSetPair_b0230&&>(*src));
                        DestroyEntry_b0230(src);
                    }
                    self->allocator->vf_0x48(self->data);
                }
                self->data = p;
                self->capacity = grown;
            }
        }
        fn_710077ae90(self, index + 1);
    }
    return &self->data[index];
}


struct UnkStateNode_b0230 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(u32 previous_kind);  // entered
    virtual void vf_0x18(u32 next_kind);      // left
    u8 pad_0x8[8];
    u32 kind;  // 0x10
};

struct UnkStateMachine_b0230 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkStateNode_b0230* previous, UnkStateNode_b0230* next);
    virtual void vf_0x28(UnkStateNode_b0230* current, UnkStateNode_b0230* next);
    u8 pad_0x8[8];
};

struct __attribute__((packed, aligned(4))) UnkStateList_b0230 {
    u32 count;
    UnkStateNode_b0230* items[16];
};

struct UnkCentralFsm_b0230 {
    u8 pad_0x0[0x30];
    UnkStateMachine_b0230 machine;  // 0x30
    UnkStateNode_b0230* current;    // 0x40
    UnkStateNode_b0230* previous;   // 0x48
    UnkStateList_b0230 states;      // 0x50
    u32 field_0xd4;
    u8 field_0xd8;
    u8 pad_0xd9[0x538 - 0xd9];
    u64 field_0x538;
};
static_assert(__builtin_offsetof(UnkCentralFsm_b0230, field_0xd4) == 0xd4, "layout");
static_assert(__builtin_offsetof(UnkCentralFsm_b0230, field_0x538) == 0x538, "layout");

// Leaves the current state for the first listed state of kind 2.
__attribute__((always_inline)) static inline void SwitchToKind2(UnkCentralFsm_b0230* self) {
    UnkStateNode_b0230* current = self->current;
    UnkStateMachine_b0230* machine = &self->machine;
    if (current->kind != 2) {
        UnkStateNode_b0230* const* it = self->states.items;
        u64 n = (u64)self->states.count * sizeof(UnkStateNode_b0230*);
        // Keeps the byte count the loop is counted down in.
        asm("" : "+r"(n));
        UnkStateNode_b0230* next = nullptr;
        for (; n != 0; ++it, n -= sizeof(UnkStateNode_b0230*)) {
            if ((*it)->kind == 2) {
                next = *it;
                break;
            }
        }
        if (next) {
            current->vf_0x18(2);
            machine->vf_0x28(self->current, next);
            UnkStateNode_b0230* old = self->current;
            self->previous = old;
            self->current = next;
            next->vf_0x10(old ? old->kind : 8);
            machine->vf_0x20(self->previous, next);
        }
    }
    self->field_0xd4 = 8;
}

struct UnkCountedCentral_b0230 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkRef {
    UnkCountedCentral_b0230* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

// A callable entry of the handle source: its storage and the table of operations (+0x40 destroys it).
struct UnkCallable_b0230 {
    u64 storage;
    void (**ops)(UnkCallable_b0230* self);

    ~UnkCallable_b0230() {
        if (ops) {
            ops[0x40 / 8](this);
            ops = nullptr;
        }
    }
};

struct UnkItems_b0230 {
    UnkCallable_b0230* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Destroys the callables and frees the array.
    void Free() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkCallable_b0230();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkSource_b0230 {
    void* vtable;
    UnkItems_b0230 items;
    u32 state;
    union {
        UnkRefObject* handle;            // 0x38
        base::UnkRefHandle handle_slot;  // the same pointer, as fn_710031d208 takes it
    };
};
static_assert(sizeof(UnkSource_b0230) == 0x40, "UnkSource_b0230 layout");

struct UnkWeakTarget;
void fn_7100772eb0(UnkSource_b0230* self, UnkWeakTarget* target);

struct UnkCentral_b0230 {
    u8 pad_0x0[0x540];
    UnkSource_b0230 source;  // 0x540
};

void fn_7100772cf8(UnkCentral_b0230* self, const CStrId* name) {
    UnkSource_b0230* src = &self->source;
    if (src->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(&src->items);
            fn_710031d208(&src->handle_slot, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        src->state = 2;
    }
    if (auto* h = static_cast<UnkCountedCentral_b0230*>(src->handle_slot.ptr))
        if (h->field_0x10)
            return;
    alignas(8) u32 kind;
    UnkWeakTarget* found;
    UnkGameManager* manager = g_unk_7101d47310;
    u32 kind_value = g_unk_71727bebb8;
    {
        u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(name->data) + 0x30);
        kind = kind_value;
        found = fn_7100debbc8(manager, &key, &kind);
    }
    if (!found)
        return;
    UnkSource_b0230 tmp;
    fn_7100772eb0(&tmp, found);
    const base::UnkRefHandle* from_handle = &tmp.handle_slot;
    base::UnkRefHandle* to_handle = reinterpret_cast<base::UnkRefHandle*>(reinterpret_cast<u8*>(self) + 0x578);
    fn_710031d208(to_handle, from_handle);
    u32 tmp_state = tmp.state;
    UnkErasedVector* to = reinterpret_cast<UnkErasedVector*>(reinterpret_cast<u8*>(self) + 0x548);
    const UnkErasedVector* from = reinterpret_cast<const UnkErasedVector*>(&tmp.items);
    __builtin_memcpy(reinterpret_cast<u8*>(self) + 0x570, &tmp_state, 4);
    fn_710023b29c(to, from);
    tmp.state = 0;
    fn_7100ad3588(reinterpret_cast<UnkErasedVector*>(&tmp.items));
    if (UnkRefObject* p = tmp.handle) {
        if (--p->ref_count == 0)
            p->vf_0x10();
        tmp.handle = nullptr;
    }
    tmp.vtable = g_unk_7101a20998 + 2;
    tmp.items.Free();
}

void fn_71007726e4(UnkCentralFsm_b0230* self) {
    UnkStateNode_b0230* current = self->current;
    if (!current || current->kind != 1)
        return;
    const u32 entering = 2;
    __builtin_memcpy(&self->field_0xd4, &entering, sizeof(entering));
    if (self->field_0xd8)
        return;
    SwitchToKind2(self);
}

struct UnkDoorLock;

struct UnkCentralSources_b0230 {
    u8 pad_0x0[0x640];
    UnkSource_b0230* sources;  // 0x640
    u32 source_count;          // 0x648
};

// Resolves each source's handle on its first pass, then runs `act` on the door lock behind it.
template <class Act>
__attribute__((always_inline)) static inline void ForEachSource_b0230(UnkCentralSources_b0230* self, Act act) {
    UnkSource_b0230* it = self->sources;
    alignas(8) u8 storage[sizeof(UnkRef)];
    for (UnkSource_b0230* end = it + self->source_count; it != end; ++it) {
        if (it->state == 1) {
            UnkRef* fresh = new (storage) UnkRef(fn_7100353be8(&it->items));
            fn_710031d208(&it->handle_slot, reinterpret_cast<const base::UnkRefHandle*>(fresh));
            fresh->~UnkRef();
            it->state = 2;
        }
        if (auto* h = static_cast<UnkCountedCentral_b0230*>(it->handle_slot.ptr)) {
            if (void* target = h->field_0x10) {
                if (UnkDoorLock* door = fn_71002af860(static_cast<UnkWeakTarget*>(target))) {
                    CStrId id = g_unk_7101d470e0;
                    act(door, &id);
                }
            }
        }
    }
}

void fn_7100772a04(UnkCentralSources_b0230* self) {
    ForEachSource_b0230(self, [](UnkDoorLock* door, CStrId* id) { fn_71008d586c(door, false, 0, id); });
}

void fn_7100772b50(UnkCentralSources_b0230* self) {
    ForEachSource_b0230(self, [](UnkDoorLock* door, CStrId* id) { fn_71008d564c(door, 0, id); });
}

struct UnkPlayer;
struct UnkScenarioObject_ovb0032;
struct UnkGeneratorTarget_ovb0120;

struct UnkPositioned_b0230 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkCentralFollow_b0230 {
    u8 pad_0x0[0x18];
    UnkPositioned_b0230* target;  // 0x18
};

// Places the target's link at the player's offset from the target, turned by their yaw difference.
void fn_7100773ed0(UnkCentralFollow_b0230* self) {
    void* link = fn_7100774014(reinterpret_cast<UnkScenarioObject_ovb0032*>(self->target));
    if (!link)
        return;
    UnkPositioned_b0230* player = reinterpret_cast<UnkPositioned_b0230*>(fn_7100dec788(g_unk_7101d47310));
    UnkPositioned_b0230* target = self->target;
    UnkVec3f offset(player->xform.m[0][3] - target->xform.m[0][3], player->xform.m[1][3] - target->xform.m[1][3],
                    player->xform.m[2][3] - target->xform.m[2][3]);
    UnkPositioned_b0230* player_again = reinterpret_cast<UnkPositioned_b0230*>(fn_7100dec788(g_unk_7101d47310));
    f32 player_yaw = player_again->xform.GetAngle().y;
    f32 target_yaw = self->target->xform.GetAngle().y;
    UnkVec3f turn(0.0f, player_yaw - target_yaw, 0.0f);
    fn_710116c128(static_cast<UnkGeneratorTarget_ovb0120*>(link), &offset, &turn, true);
}

struct UnkCentralUnit_b49;
void fn_71007717c0(UnkCentralUnit_b49* self);
void fn_7100541e34(char* self, bool value);
void fn_7100541690(char* self, bool value);
struct UnkManager;

struct UnkCentralLinkEntity_b0230 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;  // 0x240
};

struct UnkCentralLinkNode_b0230 {
    u8 pad_0x0[0x10];
    UnkCentralLinkEntity_b0230* entity;  // 0x10
};

struct UnkCentralRegistered_b0230 {
    u8 pad_0x0[0x3f0];
    u32 mode;  // 0x3f0
    u8 pad_0x3f4[0x6d8 - 0x3f4];
    UnkCentralLinkNode_b0230* link;  // 0x6d8
    u8 pad_0x6e0[0x7c8 - 0x6e0];
    base::UnkDeferredSet<void>* registered;  // 0x7c8
};

// The linked entity's Emmy AI component, if it has one.
static inline base::reflection::UnkReflObject* LinkedEmmyAI_b0230(UnkCentralRegistered_b0230* self) {
    if (UnkCentralLinkNode_b0230* link = self->link) {
        if (UnkCentralLinkEntity_b0230* entity = link->entity) {
            if (base::reflection::UnkReflObject* ai = entity->ai) {
                if (fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
                    return ai;
            }
        }
    }
    return nullptr;
}

// Re-registers the unit with the sub-area manager's deferred set and tells the linked Emmy AI the mode it is in.
void fn_71007749d4(UnkCentralUnit_b49* unit) {
    UnkCentralRegistered_b0230* self = reinterpret_cast<UnkCentralRegistered_b0230*>(unit);
    fn_71007717c0(unit);
    base::UnkDeferredSet<void>** slot = &self->registered;
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    if (*slot) {
        fn_71004f3ea4(*slot, slot);
        *slot = nullptr;
    }
    base::UnkDeferredSet<void>* set = reinterpret_cast<base::UnkDeferredSet<void>*>(static_cast<u8*>(manager) + 8);
    self->registered = set;
    fn_71004f3cf0(set, slot);
    u32 mode = self->mode;
    if (mode - 2 < 2 || mode == 0) {
        if (base::reflection::UnkReflObject* ai = LinkedEmmyAI_b0230(self))
            fn_7100541e34(reinterpret_cast<char*>(ai), true);
    } else if (mode == 1) {
        if (base::reflection::UnkReflObject* ai = LinkedEmmyAI_b0230(self)) {
            fn_7100541e34(reinterpret_cast<char*>(ai), false);
            fn_7100541690(reinterpret_cast<char*>(ai), true);
        }
    }
}

// Constructs the handle source of `target`: an empty callable array and the handle target's weak handle
// (created on first use), in state 2.
__attribute__((noinline)) void fn_7100772eb0(UnkSource_b0230* self, UnkWeakTarget* target) {
    self->vtable = g_unk_7101a20998 + 2;
    self->items.data = nullptr;
    self->items.count = 0;
    self->items.capacity = 0;
    self->items.step = 10;
    self->items.field_0x14 = true;
    self->items.flags = 0xf;
    self->items.allocator = fn_7100083990();
    self->state = 0;
    self->vtable = g_unk_7101a33fa0 + 2;
    self->handle = nullptr;
    UnkWeakHandle* weak = GetWeakHandle(target);
    UnkRefObject* handle = nullptr;
    if (weak) {
        ++weak->ref_count;
        handle = weak;
    }
    fn_710031d208(&self->handle_slot, reinterpret_cast<const base::UnkRefHandle*>(&handle));
    self->state = 2;
    if (handle && --handle->ref_count == 0)
        handle->vf_0x10();
}

struct UnkCentralUnit;
void fn_71007754b8(UnkCentralUnit_b49* unit);
void fn_71007717c0(UnkCentralUnit_b49* self);
void fn_71007727f0(UnkCentralUnit* unit);
struct UnkMapData;

struct UnkCavesHolder_b0230 {
    u8 pad_0x0[0x10];
    base::global::CRntString name;  // 0x10
    u8 pad_0x40[4];
    union {
        s32 ref_count;  // 0x44
    };
};

struct UnkCavesActor_b0230 {
    u8 pad_0x0[0x30];
    UnkCavesHolder_b0230* holder;  // 0x30
};

struct UnkCavesUnit_b0230 {
    u8 pad_0x0[0x18];
    UnkCavesActor_b0230* actor;  // 0x18
    u8 pad_0x20[0x3f0 - 0x20];
    u32 mode;  // 0x3f0
};

void fn_7100775358(UnkCentralUnit_b49* unit) {
    UnkCavesUnit_b0230* self = reinterpret_cast<UnkCavesUnit_b0230*>(unit);
    fn_71007717c0(unit);
    fn_71007727f0(reinterpret_cast<UnkCentralUnit*>(unit));
    UnkCavesActor_b0230* actor = self->actor;
    bool in_caves;
    {
        base::global::CRntString caves("PRP_CV_CentralUnitCaves");
        ++actor->holder->ref_count;
        in_caves = actor->holder->name.key == caves.key;
    }
    if (in_caves) {
        static_cast<u8*>(g_unk_71726af0b0)[0x1270] = 1;
        UnkGameManager* manager = g_unk_7101d47310;
        alignas(8) u32 kind;
        u64 key;
        UnkWeakTarget* found;
        {
            base::global::CStrId id("Door015 (CG-CG)", true);
            u32 kind_value = g_unk_71727bebb8;
            key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id.data) + 0x30);
            kind = kind_value;
            found = static_cast<UnkWeakTarget*>(fn_7100deba8c(manager, &key, &kind));
        }
        if (found) {
            if (UnkDoorLock* door = fn_71002af860(found))
                fn_71008cdf84(door);
        }
    }
    if (self->mode == 2)
        fn_71007754b8(unit);
    UnkMapData* value = fn_7100e8d8b0();
    fn_7100e8f9a0(reinterpret_cast<char*>(value), true,
                  reinterpret_cast<CStrId*>(reinterpret_cast<u8*>(self->actor) + 0x30));
}

// Inserts a copy of `value` at `index` (appending when the index is the count), moving the later entries up.
void fn_710077a96c(UnkPairVec_b0230* self, const UnkSetPair_b0230* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_710077a700(reinterpret_cast<base::UnkVector<UnkEntry_cu>*>(self), reinterpret_cast<const UnkEntry_cu*>(value));
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown) {
            UnkSetPair_b0230* p = static_cast<UnkSetPair_b0230*>(
                self->allocator->vf_0x40((u64)grown * sizeof(UnkSetPair_b0230), 8, self->flags));
            if (self->data) {
                UnkSetPair_b0230* src = self->data;
                UnkSetPair_b0230* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                    new (dst) UnkSetPair_b0230(static_cast<UnkSetPair_b0230&&>(*src));
                    DestroyEntry_b0230(src);
                }
                self->allocator->vf_0x48(self->data);
            }
            n = self->count;
            self->data = p;
            self->capacity = grown;
        }
    }
    for (u32 i = n - 1; i >= index; --i) {
        UnkSetPair_b0230* e = &self->data[i];
        new (e + 1) UnkSetPair_b0230(static_cast<UnkSetPair_b0230&&>(*e));
        DestroyEntry_b0230(&self->data[i]);
        if (i == 0)
            break;
    }
    CopyPair_b0230(&self->data[index], value);
    self->count = self->count + 1;
}

struct UnkCentralUnit;
struct UnkPowerUpLifeRef_b52;
struct UnkPowerUpLifeEntry_b52;

struct UnkSourcePair_b0230 {
    UnkSource_b0230 first;
    UnkSource_b0230 second;
};

struct UnkTargetOwner_b0230 {
    u8 pad_0x0[0x18];
    void* field_0x18;
};

struct UnkCentralTracked_b0230 {
    u8 pad_0x0[0x580];
    UnkSourcePair_b0230* entries;  // 0x580
    u32 entry_count;               // 0x588
    u8 pad_0x58c[0x6f8 - 0x58c];
    UnkRefObject* tracked;  // 0x6f8
};

__attribute__((always_inline)) static inline void ResolveSource2_b0230(UnkSource_b0230* source) {
    if (source->state == 1) {
        {
            UnkRefPtr fresh = fn_710023b580(&source->items);
            fn_7100234f78(&source->handle_slot, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        source->state = 2;
    }
}

// Makes the unit track the handle of the second source of the entry whose first source points at `owner`'s
// target (dropping what it tracked before).
void fn_7100775918(UnkCentralUnit* unit, void* arg) {
    UnkCentralTracked_b0230* self = reinterpret_cast<UnkCentralTracked_b0230*>(unit);
    UnkTargetOwner_b0230* owner = static_cast<UnkTargetOwner_b0230*>(arg);
    ResetRef(&self->tracked);
    if (!owner)
        return;
    u8* entries = reinterpret_cast<u8*>(self->entries);
    for (u64 offset = 0, end = (u64)self->entry_count << 7; offset != end; offset += sizeof(UnkSourcePair_b0230)) {
        UnkSourcePair_b0230* it = reinterpret_cast<UnkSourcePair_b0230*>(entries + offset);
        void* want = owner->field_0x18;
        ResolveSource2_b0230(&it->first);
        UnkCountedCentral_b0230* first = static_cast<UnkCountedCentral_b0230*>(it->first.handle_slot.ptr);
        if ((first ? first->field_0x10 : nullptr) != want)
            continue;
        ResolveSource2_b0230(&it->second);
        if (auto* second = static_cast<UnkCountedCentral_b0230*>(it->second.handle_slot.ptr)) {
            if (second->field_0x10) {
                // Keeps the second read of the target pointer.
                asm("" : "+r"(second));
                UnkRefObject* found = reinterpret_cast<UnkRefObject*>(
                    fn_7100232f18(static_cast<UnkPowerUpLifeEntry_b52*>(second->field_0x10)));
                AssignRef(&self->tracked, found);
                return;
            }
        }
    }
}

struct UnkInventory;
struct UnkEnergyWaveFxComp_b49;
extern char g_unk_710164aa98[];
extern char g_unk_710164aaa0[];

struct UnkEnergyPlayer_b0230 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    UnkEnergyWaveFxComp_b49* fx;  // 0x228
    u8 pad_0x230[0x270 - 0x230];
    UnkInventory* inventory;  // 0x270
};

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId_b0230(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkEnergyRange_b0230 {
    u64 field_0x0;
    u32 field_0x8;
};

// Shows the central unit energy effects for the amount the player holds, and sends the player's class a request when
// there is some.
void fn_710077045c() {
    UnkEnergyPlayer_b0230* player = reinterpret_cast<UnkEnergyPlayer_b0230*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    UnkInventory* inventory = player->inventory;
    if (!inventory)
        return;
    f32 energy = fn_7101142928(inventory, ItemId_b0230(g_unk_717275ac18));
    if (UnkEnergyWaveFxComp_b49* fx = player->fx) {
        fn_71011738f8(fx, g_unk_710164aa98, energy == 0.0f);
        fn_71011738f8(fx, g_unk_710164aaa0, energy != 0.0f);
    }
    if (energy == 0.0f)
        return;
    if (void* target = player->field_0x220) {
        base::reflection::UnkReflTypeIdArg value;
        value.id = 0x80;
        UnkEnergyRange_b0230 range;
        range.field_0x0 = 0;
        range.field_0x8 = 0x40000000;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0xa218d351a57ec697, 0x5d523009b8f43e5f, &value, &g_unk_710164b958, &range, &mask);
    }
}

struct UnkCentralUnit;
struct UnkStrIdHandle;
extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT
struct UnkOwner;

struct UnkCentralMode_b0230 {
    u8 pad_0x0[0x3f0];
    u32 mode;  // 0x3f0
};

// Takes the central unit's items from the player's inventory, and tells the owner's effect component and
// character class the unit is in use.
void fn_71007727f0(UnkCentralUnit* unit) {
    UnkCentralMode_b0230* self = reinterpret_cast<UnkCentralMode_b0230*>(unit);
    UnkEnergyPlayer_b0230* player = reinterpret_cast<UnkEnergyPlayer_b0230*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    UnkInventory* inventory = player->inventory;
    if (!inventory)
        return;
    fn_71011437bc(inventory, reinterpret_cast<const UnkStrIdHandle&>(ItemId_b0230(g_unk_717275ac18)));
    if (self->mode == 1)
        fn_71011437bc(player->inventory, reinterpret_cast<const UnkStrIdHandle&>(ItemId_b0230(g_unk_717275ac58)));
    UnkEnergyPlayer_b0230* owner = reinterpret_cast<UnkEnergyPlayer_b0230*>(fn_7100defcd4(g_unk_7101d47310));
    if (!owner)
        return;
    if (UnkEnergyWaveFxComp_b49* fx = owner->fx) {
        fn_71011738f8(fx, g_unk_710164aa98, 0);
        fn_71011738f8(fx, g_unk_710164aaa0, 1);
    }
    if (void* target = owner->field_0x220) {
        base::reflection::UnkReflTypeIdArg value;
        value.id = 0x82;
        UnkEnergyRange_b0230 range;
        range.field_0x0 = 0;
        range.field_0x8 = 0x40000000;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0xbe3a59363755d886, 0x5d523009b8f43e5f, &value, &g_unk_710164b968, &range, &mask);
    }
}

struct UnkPairArray_b0230 {
    UnkSetPair_b0230* data;
    u32 count;
};

void fn_710077afa4(UnkPairArray_b0230* self, base::UnkCollectionIter<UnkSetPair_b0230>* out) {
    *out = base::UnkCollectionIter<UnkSetPair_b0230>(0, self->data);
}

void fn_710077b1e8(UnkPairArray_b0230* self, base::UnkCollectionIter<UnkSetPair_b0230>* out) {
    *out = base::UnkCollectionIter<UnkSetPair_b0230>(self->count, self->data + self->count);
}
