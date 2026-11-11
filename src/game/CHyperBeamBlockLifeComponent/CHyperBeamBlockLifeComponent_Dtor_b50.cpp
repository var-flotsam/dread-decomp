// CHyperBeamBlockLifeComponent unit (tools/tu_map.py, the name from reflection): two identical
// destructors (the same shape as fn_71008c97f8 in
// src/game/CDoorCentralUnitLifeComponent/CDoorCentralUnitLifeComponent_Dtor_b50.cpp): this class's
// vtable (g_unk_7101a7ee28); the listener at +0x3a0 (vtable g_unk_7101a77000) detaches itself from the
// owner of its counted link (fn_7100a2d4d0) and drops the link's reference; the handle at +0x388 is
// released (fn_7100565ef0) and the slot cleared; then the base destructor fn_71008e05e8. Names and
// layouts are placeholders.
#include "game/CItemLifeComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkTU811.h"
#include "types.h"

extern void* g_unk_7101a7ee28[];
extern void* g_unk_7101a77000[];

struct UnkTutoHandle;

struct UnkBlockLink {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

struct UnkBlockListener {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkBlockLink* link;  // 0x18
};

struct UnkHyperBeamBlockLife {
    void** vtable;
    u8 unk_0x8[0x380];
    UnkTutoHandle* handle;  // 0x388
    u64 field_0x390;
    u64 field_0x398;
    UnkBlockListener listener;  // 0x3a0
};

static inline __attribute__((always_inline)) void Destroy(UnkHyperBeamBlockLife* self) {
    self->vtable = &g_unk_7101a7ee28[2];
    UnkBlockListener* l = &self->listener;
    l->vtable = &g_unk_7101a77000[2];
    if (UnkBlockLink* link = l->link) {
        bool held = true;
        if (void* owner = link->owner) {
            fn_7100a2d4d0(owner, l);
            link = l->link;
            held = link != nullptr;
        }
        if (held && --link->ref_count == 0)
            link->Release();
    }
    UnkTutoHandle** slot = &self->handle;
    if (UnkTutoHandle* handle = *slot) {
        fn_7100565ef0(handle, slot);
        *slot = nullptr;
    }
    self->field_0x398 = 0;
    self->field_0x390 = 0;
    fn_71008e05e8(self);
}

void fn_71008dfacc(UnkHyperBeamBlockLife* self) {
    Destroy(self);
}

void fn_71008dfd40(UnkHyperBeamBlockLife* self) {
    Destroy(self);
}

// The deleting form of fn_71008dfacc.
void fn_71008dfb78(UnkHyperBeamBlockLife* self) {
    Destroy(self);
    operator delete(self);
}
