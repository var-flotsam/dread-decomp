// CEmmyAIComponent's unit (tools/tu_map.py unit 811): the Emmy AI component's destructor. It removes its two
// render labels (fading out the sound of the second), then its members are destroyed and the base AI
// component's destructor runs. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/CAbilityGhostAura.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU363.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game/sound/UnkSoundHandle.h"
#include "game_s2/UnkOwnedList.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::UnkRefPtrMember;

struct UnkComponentBase_b0194;
struct UnkNode;
struct UnkWordRefPtrArray;
struct UnkGrabAttack_505744;
struct UnkEmmyNodeRecord_A3_b69;
struct UnkObject_71004f3584;
struct UnkObject_7100500614;
struct UnkHandle_7100566080;

extern void* g_unk_7101a482c8[];
extern char g_unk_7101a48b80[];
void fn_710053f35c(UnkComponentBase_b0194* self);
void fn_7100573128(UnkComponentBase_b0194* self);
void fn_71002b24e8(UnkNode* self);
void fn_71007051f0(UnkWordRefPtrArray* self);
void fn_7100505744(UnkGrabAttack_505744* self);
void fn_710053ff9c(UnkEmmyNodeRecord_A3_b69* self);
void fn_71004f3584(UnkObject_71004f3584* self);
void fn_7100500614(UnkObject_7100500614* self);
void fn_7100566080(UnkHandle_7100566080* handle, UnkHandle_7100566080** slot);

// A member whose destructor is out of line: `Dtor` destroys it.
template <class T, void (*Dtor)(T*), u32 Size>
struct alignas(8) UnkMember_l53f510 {
    u8 raw[Size];
    ~UnkMember_l53f510() { Dtor(reinterpret_cast<T*>(this)); }
};

// A growable array that frees its elements and storage when destroyed.
template <class T>
struct UnkVector_l53f510 : CRntVector<T> {
    ~UnkVector_l53f510() { this->Free(); }
};

// A fixed-capacity array: the count, then room for N elements.
template <class T, u32 N>
struct UnkFixedVector_l53f510 {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* items() { return reinterpret_cast<T*>(storage); }
    ~UnkFixedVector_l53f510() {
        for (u32 i = 0; i < count; ++i)
            items()[i].~T();
        count = 0;
    }
};

struct UnkNamedEntry_l53f510 {
    u8 pad_0x0[0x40];
    CStrId name;  // 0x40
};
static_assert(sizeof(UnkNamedEntry_l53f510) == 0x48, "UnkNamedEntry_l53f510 layout");

// An element with a virtual destructor.
struct UnkPolyEntry_l53f510 {
    virtual ~UnkPolyEntry_l53f510();
    u8 pad_0x8[0x38];
};

// Clears the word it points at when destroyed.
struct UnkSlot_l53f510 {
    u64* target;
    ~UnkSlot_l53f510() { *target = 0; }
};

// Registrations held through a handle, released when destroyed.
struct UnkTutoSlot_l53f510 {
    UnkTutoHandle* handle;
    u64 field_0x8;
    u64 field_0x10;
    ~UnkTutoSlot_l53f510() {
        if (handle) {
            fn_7100565ef0(handle, &handle);
            handle = nullptr;
        }
        field_0x10 = 0;
        field_0x8 = 0;
    }
};

struct UnkDeferredSlot_l53f510 {
    base::UnkDeferredSet<void>* set;
    u64 field_0x8;
    u64 field_0x10;
    ~UnkDeferredSlot_l53f510() {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        field_0x10 = 0;
        field_0x8 = 0;
    }
};

struct UnkHandleSlot_l53f510 {
    UnkHandle_7100566080* handle;
    u64 field_0x8;
    u64 field_0x10;
    ~UnkHandleSlot_l53f510() {
        if (handle) {
            fn_7100566080(handle, &handle);
            handle = nullptr;
        }
        field_0x10 = 0;
        field_0x8 = 0;
    }
};

// Four tutorial slots and the arrays that track them (cleared by the component's destructor).
struct UnkTutoSet_l53f510 {
    UnkTutoSlot_l53f510 slot_0;  // 0x0
    UnkTutoSlot_l53f510 slot_1;  // 0x18
    UnkTutoSlot_l53f510 slot_2;  // 0x30
    UnkTutoSlot_l53f510 slot_3;  // 0x48
    CRntVector<UnkSlot_l53f510> clears;  // 0x60
    CRntVector<void*> field_0x88;
    CRntVector<void*> field_0xb0;
    u8 pad_0xd8[0x30];

};
static_assert(sizeof(UnkTutoSet_l53f510) == 0x108, "UnkTutoSet_l53f510 layout");

// A listener linked into its owner's list; it unlinks itself and drops the owner when destroyed.
struct UnkListener_l53f510 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* owner;  // 0x18

    ~UnkListener_l53f510() {
        vtable = g_unk_7101a22d48 + 2;
        if (owner) {
            if (owner->target)
                fn_710117b3f0(owner->target, this);
            if (owner && --owner->ref_count == 0)
                owner->vf_0x10();
        }
    }
};

// A list of nodes linked at +0x20, freed through its allocator.
struct UnkListNode_l53f510 {
    u8 pad_0x0[0x20];
    UnkListNode_l53f510* next;  // 0x20
};

struct UnkList_l53f510 {
    void** vtable;
    u32 count;
    UnkListNode_l53f510* head;
    UnkListNode_l53f510* tail;
    base::UnkAllocator* allocator;

    ~UnkList_l53f510() {
        vtable = reinterpret_cast<void**>(g_unk_7101a48bc0) + 2;
        UnkListNode_l53f510* node = head;
        while (node) {
            UnkListNode_l53f510* next = node->next;
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkListOwner_l53f510 {
    void** vtable;
    UnkList_l53f510 list;
    ~UnkListOwner_l53f510() { vtable = reinterpret_cast<void**>(g_unk_7101a48b80) + 2; }
};

// What a render label adds to the render manager: the item at +8.
struct UnkRenderLabel_l53f510 {
    void** vtable;
    u8 item[8];
    UnkItemBase_71011a91c4* Item() { return reinterpret_cast<UnkItemBase_71011a91c4*>(item); }
};

// The handle's view of the sound manager's reference block.
struct UnkSoundRefView_l53f510 {
    UnkSound* sound;
    s64 serial;
};

static inline UnkSound* Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_l53f510* ref = reinterpret_cast<const UnkSoundRefView_l53f510*>(handle.ref);
    if (ref && ref->serial == handle.serial && handle.serial != -1)
        return ref->sound;
    return nullptr;
}

struct UnkAIComponentBase_l53f510 {
    void** vtable;
    u8 pad_0x8[0x146d0 - 0x8];
};

using UnkSubComponent_l53f510 = UnkMember_l53f510<UnkComponentBase_b0194, fn_710053f35c, 0x100>;
using UnkSubComponentBase_l53f510 = UnkMember_l53f510<UnkComponentBase_b0194, fn_7100573128, 0x70>;
using UnkMoveRecord_l53f510 = UnkMember_l53f510<void, fn_710002f67c, 0xb0>;
using UnkMoveSlot_l53f510 = UnkMember_l53f510<UnkMoveSlot_0xb0, fn_7100030a50, 0xb0>;

struct UnkEmmyAIComponent_l53f510 : UnkAIComponentBase_l53f510 {
    UnkVector_l53f510<UnkVector_l53f510<u32>> lists;  // 0x146d0
    u8 pad_0x146f8[0x20];
    UnkListOwner_l53f510 list_owner;  // 0x14718
    u8 pad_0x14748[0x18];
    u64 field_0x14760;
    u64 field_0x14768;
    CStrId name;  // 0x14770
    u8 pad_0x14778[0x8];
    UnkSubComponent_l53f510 sub_0x14780;
    UnkSubComponent_l53f510 sub_0x14880;
    UnkSubComponentBase_l53f510 sub_0x14980;
    UnkSubComponentBase_l53f510 sub_0x149f0;
    UnkSubComponentBase_l53f510 sub_0x14a60;
    UnkSubComponentBase_l53f510 sub_0x14ad0;
    UnkRefPtrMember ref_0x14b40;
    u8 pad_0x14b48[0x18];
    UnkRefPtrMember ref_0x14b60;
    u8 pad_0x14b68[0x58];
    UnkMember_l53f510<UnkMsg_b152, fn_7100287954, 0x10> msg;  // 0x14bc0
    UnkRefPtrMember ref_0x14bd0;
    u8 pad_0x14bd8[0x10];
    UnkRenderLabel_l53f510* label_a;  // 0x14be8
    UnkRenderLabel_l53f510* label_b;  // 0x14bf0
    u8 pad_0x14bf8[0x68];
    UnkRefPtrMember ref_0x14c60;
    UnkHandleSlot_l53f510 handle_0x14c68;
    UnkHandleSlot_l53f510 handle_0x14c80;
    UnkMember_l53f510<UnkObject_71002ad0dc, fn_71002ad0dc, 0x8> path;  // 0x14c98
    u8 pad_0x14ca0[0x27724 - 0x14ca0];
    UnkFixedVector_l53f510<UnkNamedEntry_l53f510, 100> entries;  // 0x27724
    u8 pad_0x29348[0x8];
    UnkMoveRecord_l53f510 record_0x29350;
    UnkDeferredSlot_l53f510 deferred_0x29400;
    UnkMoveRecord_l53f510 record_0x29418;
    UnkDeferredSlot_l53f510 deferred_0x294c8;
    u8 pad_0x294e0[0x48];
    UnkSoundHandle sound;  // 0x29528
    u8 pad_0x29538[0x20];
    UnkRefPtrMember ref_0x29558;
    UnkMoveSlot_l53f510 move_0x29560;
    UnkMoveSlot_l53f510 move_0x29610;
    u8 pad_0x296c0[0x20];
    UnkVector_l53f510<u32> field_0x296e0;
    UnkMember_l53f510<UnkGrabAttack_505744, fn_7100505744, 0x828> grab_attack;  // 0x29708
    UnkTutoSet_l53f510 tutorials;  // 0x29f30
    UnkMember_l53f510<UnkEmmyNodeRecord_A3_b69, fn_710053ff9c, 0x198> nodes;  // 0x2a038
    UnkMember_l53f510<UnkObject_7100500614, fn_7100500614, 0x48> field_0x2a1d0;
    UnkMember_l53f510<UnkObject_71004f3584, fn_71004f3584, 0x50> field_0x2a218;
    UnkMember_l53f510<UnkWordRefPtrArray, fn_71007051f0, 0x30> field_0x2a268;
    UnkMember_l53f510<UnkNode, fn_71002b24e8, 0x68> node_0x2a298;
    UnkMember_l53f510<UnkNode, fn_71002b24e8, 0x78> node_0x2a300;
    UnkMoveRecord_l53f510 record_0x2a378;
    UnkListener_l53f510 listener;  // 0x2a428
    UnkMoveSlot_l53f510 move_0x2a448;
    UnkMoveRecord_l53f510 record_0x2a4f8;
    UnkFixedVector_l53f510<CStrId, 12> names;  // 0x2a5a8
    UnkOwnedList owned_0x2a610;
    UnkOwnedList owned_0x2a638;
    UnkOwnedList owned_0x2a660;
    UnkOwnedList owned_0x2a688;
    u8 pad_0x2a6b0[0x18];
    UnkVector_l53f510<UnkPolyEntry_l53f510> entries_0x2a6c8;

};
static_assert(__builtin_offsetof(UnkEmmyAIComponent_l53f510, name) == 0x14770, "layout");
static_assert(__builtin_offsetof(UnkEmmyAIComponent_l53f510, label_a) == 0x14be8, "layout");
static_assert(__builtin_offsetof(UnkEmmyAIComponent_l53f510, entries) == 0x27724, "layout");
static_assert(__builtin_offsetof(UnkEmmyAIComponent_l53f510, sound) == 0x29528, "layout");
static_assert(__builtin_offsetof(UnkEmmyAIComponent_l53f510, tutorials) == 0x29f30, "layout");
static_assert(__builtin_offsetof(UnkEmmyAIComponent_l53f510, listener) == 0x2a428, "layout");
static_assert(__builtin_offsetof(UnkEmmyAIComponent_l53f510, names) == 0x2a5a8, "layout");
static_assert(__builtin_offsetof(UnkEmmyAIComponent_l53f510, entries_0x2a6c8) == 0x2a6c8, "layout");

template <class T>
static inline void Destroy(T& member) {
    member.~T();
}

// The component's destructor: its own clean-up, then its members from the last to the first, then the base's.
void fn_710053f510(UnkComponent* component) {
    UnkEmmyAIComponent_l53f510* self = reinterpret_cast<UnkEmmyAIComponent_l53f510*>(component);
    self->vtable = g_unk_7101a482c8 + 2;
    if (self->label_b) {
        fn_71002199d8(g_unk_7101d47300, self->label_b->Item(), 0);
        self->label_b = nullptr;
        if (UnkSound* playing = Playing(self->sound))
            fn_7100105a80(playing, 0.2f, 0.0f);
    }
    if (self->label_a) {
        fn_71002199d8(g_unk_7101d47300, self->label_a->Item(), 0);
        self->label_a = nullptr;
    }
    self->field_0x14760 = 0;
    self->field_0x14768 = 0;

    Destroy(self->entries_0x2a6c8);
    Destroy(self->owned_0x2a688);
    Destroy(self->owned_0x2a660);
    Destroy(self->owned_0x2a638);
    Destroy(self->owned_0x2a610);
    Destroy(self->names);
    Destroy(self->record_0x2a4f8);
    Destroy(self->move_0x2a448);
    Destroy(self->listener);
    Destroy(self->record_0x2a378);
    Destroy(self->node_0x2a300);
    Destroy(self->node_0x2a298);
    Destroy(self->field_0x2a268);
    Destroy(self->field_0x2a218);
    Destroy(self->field_0x2a1d0);
    Destroy(self->nodes);
    {
        UnkTutoSet_l53f510& tutorials = self->tutorials;
        UnkSlot_l53f510* end = tutorials.clears.data + tutorials.clears.count;
        for (UnkSlot_l53f510* it = tutorials.clears.data; it != end; ++it)
            it->~UnkSlot_l53f510();
        tutorials.clears.count = 0;
        tutorials.field_0x88.Clear();
        tutorials.field_0xb0.Clear();
        tutorials.field_0xb0.Free();
        tutorials.field_0x88.Free();
        if (tutorials.clears.data) {
            tutorials.clears.count = 0;
            tutorials.clears.allocator->vf_0x48(tutorials.clears.data);
            tutorials.clears.capacity = 0;
        }
        Destroy(tutorials);
    }
    Destroy(self->grab_attack);
    Destroy(self->field_0x296e0);
    Destroy(self->move_0x29610);
    Destroy(self->move_0x29560);
    Destroy(self->ref_0x29558);
    Destroy(self->deferred_0x294c8);
    Destroy(self->record_0x29418);
    Destroy(self->deferred_0x29400);
    Destroy(self->record_0x29350);
    Destroy(self->entries);
    Destroy(self->path);
    Destroy(self->handle_0x14c80);
    Destroy(self->handle_0x14c68);
    Destroy(self->ref_0x14c60);
    Destroy(self->ref_0x14bd0);
    Destroy(self->msg);
    Destroy(self->ref_0x14b60);
    Destroy(self->ref_0x14b40);
    Destroy(self->sub_0x14ad0);
    Destroy(self->sub_0x14a60);
    Destroy(self->sub_0x149f0);
    Destroy(self->sub_0x14980);
    Destroy(self->sub_0x14880);
    Destroy(self->sub_0x14780);
    Destroy(self->name);
    Destroy(self->list_owner);
    Destroy(self->lists);
    fn_7100369380(self);
}
