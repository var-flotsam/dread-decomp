// CPullOffGrapplePointComponent unit (tools/tu_map.py unit 1296): copies of the listener destructors in
// src/game/CAINavigationComponent/CAINavigationComponent_b39.cpp (fn_71006f4344, fn_71006f538c) with this
// unit's vtable and detach call (fn_7100a2d4d0). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a77000[];

struct UnkLinkOwner_pog;

struct UnkLink_pog {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;  // 0x8
    UnkLinkOwner_pog* owner;  // 0x10
};

struct UnkListener_pog {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkLink_pog* link;  // 0x18
};

void fn_7100a2d4d0(void* target, void* listener);

static inline __attribute__((always_inline)) void Destroy_pog(UnkListener_pog* self) {
    self->vtable = &g_unk_7101a77000[2];
    UnkLink_pog* link = self->link;
    if (!link)
        return;
    if (UnkLinkOwner_pog* owner = link->owner) {
        fn_7100a2d4d0(owner, self);
        link = self->link;
        if (!link)
            return;
    }
    if (--link->ref_count == 0)
        link->Release();
}

void fn_710082dad4(UnkListener_pog* self) { Destroy_pog(self); }

void fn_710082eec0(UnkListener_pog* self) {
    Destroy_pog(self);
    fn_7100083c00(self);
}
