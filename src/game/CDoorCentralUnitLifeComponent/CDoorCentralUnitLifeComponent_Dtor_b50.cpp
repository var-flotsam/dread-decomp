// CDoorCentralUnitLifeComponent unit (tools/tu_map.py, the name from reflection): the destructor: this
// class's vtable (g_unk_7101a7e628); the listener at +0xc28 (vtable g_unk_7101a22d48) detaches itself
// from the owner of its counted link and drops the link's reference (as in
// src/game/CAbilityGhostAura/UnkListenerDtors_b40.cpp); the handle at +0xc10 is released
// (fn_7100565ef0) and the slot cleared; then the base destructor fn_71008ceda0. Names and layouts are
// placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a7e628[];
extern void* g_unk_7101a22d48[];

struct UnkTutoHandle;
void fn_7100565ef0(UnkTutoHandle* handle, UnkTutoHandle** slot);
void fn_71008ceda0(void* self);

struct UnkDoorLink {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

struct UnkDoorListener {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkDoorLink* link;  // 0x18
};

struct UnkDoorCentralUnitLife {
    void** vtable;
    u8 unk_0x8[0xc08];
    UnkTutoHandle* handle;  // 0xc10
    u64 field_0xc18;
    u64 field_0xc20;
    UnkDoorListener listener;  // 0xc28
};

static inline __attribute__((always_inline)) void Destroy(UnkDoorCentralUnitLife* self) {
    self->vtable = &g_unk_7101a7e628[2];
    UnkDoorListener* l = &self->listener;
    l->vtable = &g_unk_7101a22d48[2];
    if (UnkDoorLink* link = l->link) {
        bool held = true;
        if (void* owner = link->owner) {
            fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), l);
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
    self->field_0xc20 = 0;
    self->field_0xc18 = 0;
    fn_71008ceda0(self);
}

void fn_71008c97f8(UnkDoorCentralUnitLife* self) {
    Destroy(self);
}

// The deleting form.
void fn_71008c98a4(UnkDoorCentralUnitLife* self) {
    Destroy(self);
    operator delete(self);
}
