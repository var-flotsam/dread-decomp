// CPullOffGrapplePointComponent's unit (tools/tu_map.py): the destructor. The listener at +0x110,
// the callback list at +0xc8, the string-id table at +0xa0 and the name at +0x98, then the base
// destructor fn_710082ca68. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "game/CTimelineComponent.h"
#include "types.h"

extern void* g_unk_7101a76dc0[];   // the class's vtable
extern void* g_unk_7101a77000[];  // the listener's vtable
extern void* g_unk_7101a20998[];  // the callback list's base vtable
extern void* g_unk_7101a33fa0[];  // the callback list's vtable

void fn_710082ca68(char*);

struct UnkPullOffCallbacks_ovb0138;
template <>
struct base::UnkCallbackTraits<UnkPullOffCallbacks_ovb0138> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkPullOffCallbackList_ovb0138 = base::UnkCallbackListShort<UnkPullOffCallbacks_ovb0138>;

struct UnkPullOffLink_ovb0138 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

// A listener on a counted link; it detaches itself from the link's owner on destruction.
struct UnkPullOffListener_ovb0138 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkPullOffLink_ovb0138* link;  // 0x18

    ~UnkPullOffListener_ovb0138() {
        vtable = &g_unk_7101a77000[2];
        if (UnkPullOffLink_ovb0138* l = link) {
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

struct UnkPullOffTable_ovb0138 : UnkStrIdTable {
    ~UnkPullOffTable_ovb0138() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

struct UnkPullOffComponent_ovb0138 {
    char* vtable;
    u8 pad_0x8[0x90];
    base::UnkStrIdMember name;                 // 0x98
    UnkPullOffTable_ovb0138 table;             // 0xa0
    UnkPullOffCallbackList_ovb0138 callbacks;  // 0xc8
    u8 pad_0x108[0x8];
    UnkPullOffListener_ovb0138 listener;       // 0x110
};
static_assert(__builtin_offsetof(UnkPullOffComponent_ovb0138, listener) == 0x110, "layout");

void fn_710082d908(UnkPullOffComponent_ovb0138* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a76dc0) + 0x10;
    self->listener.~UnkPullOffListener_ovb0138();
    self->callbacks.~UnkPullOffCallbackList_ovb0138();
    self->table.~UnkPullOffTable_ovb0138();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    fn_710082ca68(reinterpret_cast<char*>(self));
}
