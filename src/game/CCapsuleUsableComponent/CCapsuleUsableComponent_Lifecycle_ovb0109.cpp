// CCapsuleUsableComponent (unit 1584 of tools/tu_map.py, the name from reflection): the constructor
// (fn_7100a78ca0) and destructor (fn_7100a78054). After the base (fn_7100a819ac / fn_7100a81c20) come
// a string (+0x578), two callback lists (+0x5a8 and +0x5e8; vtable g_unk_7101a33fa0 over
// g_unk_7101a20998), a listener (+0x628, vtable g_unk_7101a22d48) and a flag (+0x650); the constructor
// ends with the post-construction virtual call (slot 3). The constructor sees the lists field by field,
// the destructor as the shared list type. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU57.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100a819ac(void* self);
extern void* g_unk_7101a90770[];  // CCapsuleUsableComponent's vtable

struct UnkCapsuleUsableComponent_ovb0109 {
    void** vtable;
    u8 pad_0x8[0x570];
    u8 name[0x30];  // 0x578: a CRntString
    // Two callback lists (vtable, array laid out as base::UnkVector, a word and the owner).
    void** list_a_vtable;  // 0x5a8
    void* list_a_data;
    u64 list_a_count;  // count and capacity
    u32 list_a_step;
    bool list_a_grow;
    u32 list_a_flags;
    base::UnkAllocator* list_a_allocator;
    u32 list_a_field_0x30;
    void* list_a_owner;
    void** list_b_vtable;  // 0x5e8
    void* list_b_data;
    u64 list_b_count;
    u32 list_b_step;
    bool list_b_grow;
    u32 list_b_flags;
    base::UnkAllocator* list_b_allocator;
    u32 list_b_field_0x30;
    void* list_b_owner;
    // A listener link: vtable, two words and the weak handle of what it listens to.
    void** link_vtable;  // 0x628
    u64 link_field_0x8;
    u64 link_field_0x10;
    void* link_handle;  // 0x640
    u8 pad_0x648[0x8];
    bool field_0x650;
};
static_assert(__builtin_offsetof(UnkCapsuleUsableComponent_ovb0109, link_vtable) == 0x628, "layout");
static_assert(__builtin_offsetof(UnkCapsuleUsableComponent_ovb0109, field_0x650) == 0x650, "layout");

#define INIT_LIST_OVB0109(o)                            self->o##_vtable = g_unk_7101a20998 + 2;           self->o##_data = nullptr;                          self->o##_count = 0;                               self->o##_step = 10;                               self->o##_grow = true;                             self->o##_flags = 0xf;                             self->o##_allocator = fn_7100083990();             self->o##_field_0x30 = 0;                          self->o##_vtable = g_unk_7101a33fa0 + 2;           self->o##_owner = nullptr

void fn_7100a78ca0(UnkCapsuleUsableComponent_ovb0109* self) {
    fn_7100a819ac(self);
    self->vtable = g_unk_7101a90770 + 2;
    fn_710007e6c4(self->name);
    INIT_LIST_OVB0109(list_a);
    INIT_LIST_OVB0109(list_b);
    self->link_vtable = g_unk_7101a22d48 + 2;
    __builtin_memset(&self->link_field_0x8, 0, 0x18);
    self->field_0x650 = false;
    reinterpret_cast<void (*)(UnkCapsuleUsableComponent_ovb0109*)>(self->vtable[3])(self);
}

struct UnkCapsuleCallbacks_ovb0109;
template <>
struct base::UnkCallbackTraits<UnkCapsuleCallbacks_ovb0109> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkCapsuleCallbackList_ovb0109 = base::UnkCallbackListShort<UnkCapsuleCallbacks_ovb0109>;

// A listener holding a weak handle to what it listens to; it unregisters on destruction.
struct UnkCapsuleListener_ovb0109 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* link;  // 0x18

    ~UnkCapsuleListener_ovb0109() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkWeakHandle* l = link) {
            if (UnkWeakTarget* target = l->target) {
                fn_710117b3f0(target, this);
                l = link;
            }
            if (l && --l->ref_count == 0)
                l->vf_0x10();
        }
    }
};

struct UnkCapsuleUsableBase_ovb0109;
void fn_7100a81c20(UnkCapsuleUsableBase_ovb0109* self);

struct UnkCapsuleUsableComponentD_ovb0109 {
    void** vtable;
    u8 pad_0x8[0x570];
    base::global::CRntString name;           // 0x578
    UnkCapsuleCallbackList_ovb0109 list_a;   // 0x5a8
    UnkCapsuleCallbackList_ovb0109 list_b;   // 0x5e8
    UnkCapsuleListener_ovb0109 listener;     // 0x628
};
static_assert(__builtin_offsetof(UnkCapsuleUsableComponentD_ovb0109, listener) == 0x628, "layout");

void fn_7100a78054(UnkCapsuleUsableComponentD_ovb0109* self) {
    self->vtable = g_unk_7101a90770 + 2;
    self->listener.~UnkCapsuleListener_ovb0109();
    self->list_b.~UnkCapsuleCallbackList_ovb0109();
    self->list_a.~UnkCapsuleCallbackList_ovb0109();
    fn_710007ecd4(&self->name);
    fn_7100a81c20(reinterpret_cast<UnkCapsuleUsableBase_ovb0109*>(self));
}
