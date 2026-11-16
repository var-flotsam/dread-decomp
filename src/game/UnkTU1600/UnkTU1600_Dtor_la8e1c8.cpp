// CTeleporterUsableComponent's unit (tools/tu_map.py unit 1600): the destructor of the usable component
// fn_7100a8de20 builds: the callback set at +0x590, the listener at +0x568, the five string ids at +0x540,
// then the usable base destructor (fn_7100a9e6bc). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a92b00[];

struct UnkUsableCallbacks_la8e1c8;
template <>
struct base::UnkCallbackTraits<UnkUsableCallbacks_la8e1c8> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkUsableCallbackList_la8e1c8 = base::UnkCallbackListShort<UnkUsableCallbacks_la8e1c8>;

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkUsableLink_la8e1c8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkUsableListener_la8e1c8 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkUsableLink_la8e1c8* link;

    ~UnkUsableListener_la8e1c8() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkUsableLink_la8e1c8* l = link) {
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

struct UnkUsable_la8e1c8 {
    void** vtable;
    u8 pad_0x8[0x540 - 0x8];
    base::UnkStrIdMember ids[3];  // 0x540
    base::UnkStrIdMember pair[2];  // 0x558
    UnkUsableListener_la8e1c8 listener;  // 0x568
    u8 pad_0x588[0x8];
    UnkUsableCallbackList_la8e1c8 callbacks;  // 0x590
};
static_assert(__builtin_offsetof(UnkUsable_la8e1c8, callbacks) == 0x590, "layout");

void fn_7100a8e1c8(UnkUsable_la8e1c8* self) {
    self->vtable = &g_unk_7101a92b00[2];
    self->callbacks.~UnkUsableCallbackList_la8e1c8();
    self->listener.~UnkUsableListener_la8e1c8();
    using base::UnkStrIdMember;
    for (int i = 2; i-- > 0;)
        self->pair[i].~UnkStrIdMember();
    for (int i = 3; i-- > 0;)
        self->ids[i].~UnkStrIdMember();
    fn_7100a9e6bc(reinterpret_cast<char*>(self));
}
