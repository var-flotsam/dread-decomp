// CBeamBoxComponent's unit (tools/tu_map.py unit 1355): the component's destructor (constructor fn_71008a5fb0):
// its members last first (the string id, the registration in a deferred set, the two listeners, the id list
// and the array of callback-set entries), then the life component base destructor (fn_71008e05e8).
// Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a7d4e0[];  // the class's vtable

struct UnkBeamSetBase_l8a6128 {
    char* vtable;
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    ~UnkBeamSetBase_l8a6128() { vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10; }
};

// A callback set with the refcounted owner it belongs to.
struct UnkBeamSet_l8a6128 : UnkBeamSetBase_l8a6128 {
    base::UnkRefHandle owner;

    ~UnkBeamSet_l8a6128() {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        if (base::UnkRefCounted* p = owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkBeamEntry_l8a6128 {
    UnkBeamSet_l8a6128 set;
    base::UnkStrIdMember id;
};
static_assert(sizeof(UnkBeamEntry_l8a6128) == 0x48, "entry size");

// The string-id list fn_71008a99e4 builds.
struct UnkBeamIdList_l8a6128 {
    u8 field_0x0;
    base::UnkStrIdMember id;
    UnkStrIdTable table;

    ~UnkBeamIdList_l8a6128() {
        fn_71004f11f8(&table);
        if (table.keys)
            table.allocator->vf_0x48(table.keys);
        if (table.values)
            table.allocator->vf_0x48(table.values);
    }
};

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkBeamLink_l8a6128 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkBeamWeakListener_l8a6128 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkBeamLink_l8a6128* link;

    ~UnkBeamWeakListener_l8a6128() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkBeamLink_l8a6128* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

// A listener that unregisters from its link's owner (fn_7100a2d4d0) on destruction.
struct UnkBeamListener_l8a6128 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkBeamLink_l8a6128* link;

    ~UnkBeamListener_l8a6128() {
        vtable = &g_unk_7101a77000[2];
        if (UnkBeamLink_l8a6128* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_7100a2d4d0(owner, this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

struct UnkBeamBox_l8a6128 {
    void** vtable;
    u8 pad_0x8[0x380];
    base::global::CRntVector<UnkBeamEntry_l8a6128> entries;  // 0x388
    UnkBeamIdList_l8a6128 list;                                // 0x3b0
    UnkBeamWeakListener_l8a6128 listener_0x3e8;
    UnkBeamListener_l8a6128 listener_0x408;
    u8 pad_0x428[0x38];
    base::UnkDeferredSet<void>* registered_in;  // 0x460
    void* callback_object;
    void* callback;
    base::UnkStrIdMember id;  // 0x478
};
static_assert(__builtin_offsetof(UnkBeamBox_l8a6128, list) == 0x3b0, "layout");
static_assert(__builtin_offsetof(UnkBeamBox_l8a6128, registered_in) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkBeamBox_l8a6128, id) == 0x478, "layout");

void fn_71008a6128(UnkBeamBox_l8a6128* self) {
    self->vtable = &g_unk_7101a7d4e0[2];
    using base::UnkStrIdMember;
    self->id.~UnkStrIdMember();
    if (self->registered_in) {
        fn_71004f3ea4(self->registered_in, &self->registered_in);
        self->registered_in = nullptr;
    }
    self->callback = nullptr;
    self->callback_object = nullptr;
    self->listener_0x408.~UnkBeamListener_l8a6128();
    self->listener_0x3e8.~UnkBeamWeakListener_l8a6128();
    self->list.~UnkBeamIdList_l8a6128();
    self->entries.Free();
    fn_71008e05e8(self);
}
