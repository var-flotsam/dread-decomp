// CCharClassSteamJetComponent's unit (tools/tu_map.py): the destructor (constructor fn_7100898fbc): the
// string-id list, the two listeners, the two callback lists and the four arrays, last first, then the
// base destructor (fn_7100877990). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CCharClassSteamJetComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100877990(char*);

struct UnkSteamJetCallbacks_l899154;
template <>
struct base::UnkCallbackTraits<UnkSteamJetCallbacks_l899154> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkSteamJetCallbackList_l899154 = base::UnkCallbackListShort<UnkSteamJetCallbacks_l899154>;

// An element of the arrays at +0x40 and +0x68.
struct UnkSteamJetEntry_l899154 {
    virtual ~UnkSteamJetEntry_l899154();
    u8 pad_0x8[0x38];
};

// The string-id list fn_7100893a1c builds.
struct UnkSteamJetIdList_l899154 {
    u8 field_0x0;
    base::UnkStrIdMember id;
    UnkStrIdTable table;

    ~UnkSteamJetIdList_l899154() {
        fn_71004f11f8(&table);
        if (table.keys)
            table.allocator->vf_0x48(table.keys);
        if (table.values)
            table.allocator->vf_0x48(table.values);
    }
};

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkSteamJetLink_l899154 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkSteamJetWeakListener_l899154 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkSteamJetLink_l899154* link;

    ~UnkSteamJetWeakListener_l899154() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkSteamJetLink_l899154* l = link) {
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
struct UnkSteamJetListener_l899154 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkSteamJetLink_l899154* link;

    ~UnkSteamJetListener_l899154() {
        vtable = &g_unk_7101a77000[2];
        if (UnkSteamJetLink_l899154* l = link) {
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

struct UnkSteamJetComponentD_l899154 {
    void** vtable;
    u8 pad_0x8[0x38];
    base::global::CRntVector<UnkSteamJetEntry_l899154> entries_a;  // 0x40
    base::global::CRntVector<UnkSteamJetEntry_l899154> entries_b;  // 0x68
    base::global::CRntVector<void*> list_a;                        // 0x90
    base::global::CRntVector<void*> list_b;                        // 0xb8
    u8 pad_0xe0[0x18];
    UnkSteamJetCallbackList_l899154 callbacks_a;   // 0xf8
    UnkSteamJetCallbackList_l899154 callbacks_b;   // 0x138
    UnkSteamJetWeakListener_l899154 listener_a;    // 0x178
    UnkSteamJetListener_l899154 listener_b;        // 0x198
    UnkSteamJetIdList_l899154 ids;                 // 0x1b8
};
static_assert(__builtin_offsetof(UnkSteamJetComponentD_l899154, callbacks_a) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkSteamJetComponentD_l899154, ids) == 0x1b8, "layout");

void fn_7100899154(UnkSteamJetComponentD_l899154* self) {
    self->vtable = g_unk_7101a7cad8 + 2;
    self->ids.~UnkSteamJetIdList_l899154();
    self->listener_b.~UnkSteamJetListener_l899154();
    self->listener_a.~UnkSteamJetWeakListener_l899154();
    self->callbacks_b.~UnkSteamJetCallbackList_l899154();
    self->callbacks_a.~UnkSteamJetCallbackList_l899154();
    self->list_b.Free();
    self->list_a.Free();
    self->entries_b.Free();
    self->entries_a.Free();
    fn_7100877990(reinterpret_cast<char*>(self));
}
