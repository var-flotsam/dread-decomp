// CElectricReactionComponent's unit (tools/tu_map.py, unit 1233). Names and layouts are placeholders.
#include "game/CBossAIComponent.h"
#include "game/CElectricReactionComponent.h"
#include "types.h"

// As src/game/CBossAIComponent/CBossAIComponent_Doors_ovb0102.cpp declares them.
struct UnkZiplineLink_b49;
struct UnkZiplineLinkTarget_b49;
struct UnkDoorLock;
struct UnkWeakTarget;
UnkDoorLock* fn_71002af860(UnkWeakTarget*);
// As src/game/CCharClassUseSmartObjectEvent/CCharClassUseSmartObjectEvent.cpp declares it.
struct UnkSmartObjectActor;
struct UnkSmartObjectUserA;

struct UnkElectricLight_ovb0130 {
    u8 pad_0x0[0x64];
    bool off;  // 0x64
};

struct UnkElectricFader_ovb0130 {
    void** vtable;
    void vf_0x168(f32 t) { reinterpret_cast<void (*)(UnkElectricFader_ovb0130*, f32)>(vtable[0x168 / 8])(this, t); }
    void vf_0x170(f32 t) { reinterpret_cast<void (*)(UnkElectricFader_ovb0130*, f32)>(vtable[0x170 / 8])(this, t); }
};

struct UnkElectricLinkRef_ovb0130 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

UnkElectricLight_ovb0130* fn_71007cfcb0(void* target);
void fn_71008df190(UnkElectricLight_ovb0130* light, bool on);
void fn_71008d5e74(UnkDoorLock* door, bool on);
void fn_7100a9f764(UnkSmartObjectUserA* user, bool on);
UnkElectricFader_ovb0130* fn_71007cfe68(void* actor);
UnkElectricFader_ovb0130* fn_71007cff44(void* actor);
UnkElectricFader_ovb0130* fn_71007d0020(void* actor);

struct UnkElectricReaction_ovb0130 {
    u8 pad_0x0[0x30];
    void* actor;  // 0x30
};

static inline void SwitchLink_ovb0130(u8* link, bool on) {
    UnkElectricLinkRef_ovb0130* ref = reinterpret_cast<UnkElectricLinkRef_ovb0130*>(
        *fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(link)));
    if (!ref || !ref->target)
        return;
    if (UnkElectricLight_ovb0130* light = fn_71007cfcb0(ref->target)) {
        light->off = !on;
        fn_71008df190(light, on);
    }
}

// fn_71007cfb64: switches what the owner powers on or off: the door's two linked lights (+0x3c8, +0x388)
// and the door itself, the smart object user, and three faders (fully up, or down to zero).
void fn_71007cfb64(UnkElectricReaction_ovb0130* self, bool on) {
    if (UnkDoorLock* door = fn_71002af860(static_cast<UnkWeakTarget*>(self->actor))) {
        SwitchLink_ovb0130(reinterpret_cast<u8*>(door) + 0x3c8, on);
        SwitchLink_ovb0130(reinterpret_cast<u8*>(door) + 0x388, on);
        fn_71008d5e74(door, on);
    }
    if (UnkSmartObjectUserA* user = fn_71007cfd8c(static_cast<UnkSmartObjectActor*>(self->actor)))
        fn_7100a9f764(user, on);
    if (UnkElectricFader_ovb0130* f = fn_71007cfe68(self->actor)) {
        if (on)
            f->vf_0x168(1.0f);
        else
            f->vf_0x170(0.0f);
    }
    if (UnkElectricFader_ovb0130* f = fn_71007cff44(self->actor)) {
        if (on)
            f->vf_0x168(1.0f);
        else
            f->vf_0x170(0.0f);
    }
    if (UnkElectricFader_ovb0130* f = fn_71007d0020(self->actor)) {
        if (on)
            f->vf_0x168(0.0f);
        else
            f->vf_0x170(0.0f);
    }
}
