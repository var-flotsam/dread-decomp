// CDemolitionBlockLifeComponent unit (tools/tu_map.py unit 1374): the destructor. The three arrays at
// +0x3e8 (the first one's entries point to slots that are cleared), the callback list at +0x3a8, the
// listeners at +0x330 and +0x310, the three id and path pairs at +0x298, the string-id table at +0x268 and
// the name at +0x260, then the base destructor fn_710115f91c. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CLifeComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkTU32.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a7e3c8[];

struct UnkDemolitionCallbacks_l8c4dc4;
template <>
struct base::UnkCallbackTraits<UnkDemolitionCallbacks_l8c4dc4> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkDemolitionCallbackList_l8c4dc4 = base::UnkCallbackListShort<UnkDemolitionCallbacks_l8c4dc4>;

struct UnkDemolitionLink_l8c4dc4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    UnkWeakTarget* owner;    // 0x10
};

// A listener on a counted link; it detaches itself from the link's owner (Kind::Detach) on
// destruction and drops the link.
template <class Kind>
struct UnkDemolitionListener_l8c4dc4 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkDemolitionLink_l8c4dc4* link;  // 0x18

    ~UnkDemolitionListener_l8c4dc4() {
        vtable = &Kind::vtable()[2];
        if (UnkDemolitionLink_l8c4dc4* l = link) {
            if (UnkWeakTarget* owner = l->owner) {
                Kind::Detach(owner, this);
                l = link;
                if (!l)
                    return;
            }
            if (--l->ref_count == 0)
                l->Release();
        }
    }
};

// The timeline's listener.
struct UnkDemolitionTimelineKind_l8c4dc4 {
    static void** vtable() { return g_unk_7101a77000; }
    static void Detach(UnkWeakTarget* owner, void* listener) { fn_7100a2d4d0(owner, listener); }
};

// The weak handle's listener.
struct UnkDemolitionWeakKind_l8c4dc4 {
    static void** vtable() { return g_unk_7101a22d48; }
    static void Detach(UnkWeakTarget* owner, void* listener) { fn_710117b3f0(owner, listener); }
};

struct UnkDemolitionTable_l8c4dc4 : UnkStrIdTable {
    ~UnkDemolitionTable_l8c4dc4() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// Releases an id and path pair with its resource (fn_7100026f70).
static inline void ReleasePath(UnkStrIdPathResource* path) {
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(path));
}

struct UnkDemolitionBlockLife_l8c4dc4 {
    void** vtable;
    u8 pad_0x8[0x258];
    base::UnkStrIdMember name;              // 0x260
    UnkDemolitionTable_l8c4dc4 table;       // 0x268
    u8 pad_0x290[0x8];
    UnkStrIdPathResource path_0x298;
    UnkStrIdPathResource path_0x2b0;
    UnkStrIdPathResource path_0x2c8;
    u8 pad_0x2e0[0x30];
    UnkDemolitionListener_l8c4dc4<UnkDemolitionTimelineKind_l8c4dc4> listener_0x310;
    UnkDemolitionListener_l8c4dc4<UnkDemolitionWeakKind_l8c4dc4> listener_0x330;
    u8 pad_0x350[0x58];
    UnkDemolitionCallbackList_l8c4dc4 callbacks;  // 0x3a8
    base::global::CRntVector<void**> lists[3];  // 0x3e8
};
static_assert(__builtin_offsetof(UnkDemolitionBlockLife_l8c4dc4, callbacks) == 0x3a8, "layout");
static_assert(__builtin_offsetof(UnkDemolitionBlockLife_l8c4dc4, lists) == 0x3e8, "layout");

void fn_71008c4dc4(UnkDemolitionBlockLife_l8c4dc4* self) {
    self->vtable = &g_unk_7101a7e3c8[2];
    base::global::CRntVector<void**>& links = self->lists[0];
    for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
        **it = nullptr;
    for (base::global::CRntVector<void**>& list : self->lists)
        list.Clear();
    self->lists[2].Free();
    self->lists[1].Free();
    self->lists[0].Free();
    self->callbacks.~UnkDemolitionCallbackList_l8c4dc4();
    self->listener_0x330.~UnkDemolitionListener_l8c4dc4();
    self->listener_0x310.~UnkDemolitionListener_l8c4dc4();
    ReleasePath(&self->path_0x2c8);
    ReleasePath(&self->path_0x2b0);
    ReleasePath(&self->path_0x298);
    self->table.~UnkDemolitionTable_l8c4dc4();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    fn_710115f91c(self);
}
