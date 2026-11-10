// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009ab200 tells the surface's owner (its sliding
// block, fn_710054a2cc's slot 0x170; fn_71005b6320's and fn_7100692bb0's objects) that the actor (+0x30) left,
// stamps the time and the tunable's +0x3c, keeps the target's slot and id, fades out the loop rumble and runs the
// base state's step (fn_71009b0ce4). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkHandleTarget_ovb0082 {
    void* target;
    u64 serial;
};

struct UnkHandle {
    UnkHandleTarget_ovb0082* target;
    u64 serial;

    void* Get() const {
        if (target && target->serial == serial && serial != ~0ull)
            return target->target;
        return nullptr;
    }
};

extern UnkHandle g_unk_7101d586c0;
void fn_7100072328(void*, f32, f32);
extern UnkTunable g_unk_717273c650;
struct UnkSlidingBlockMovement;
UnkSlidingBlockMovement* fn_710054a2cc(void*);
struct UnkSmTarget_b60;
UnkSmTarget_b60* fn_71005b6320(void*);
void* fn_7100692bb0(void* owner);
void fn_710092467c(void* target, void* actor);
void fn_710092567c(void* target, void* actor);
struct UnkAssignedSlot;
void fn_71002a5d1c(UnkAssignedSlot*, const UnkAssignedSlot*);
struct CSamusAirMovementState;
void fn_71009b0ce4(CSamusAirMovementState*, void*);

struct UnkMagnetSurfaceRef_ovb0082 {
    u8 pad_0x0[0x10];
    void* owner;
};

struct UnkMagnetMovement_ovb0082 {
    u8 pad_0x0[0x104];
    bool field_0x104;
    u8 pad_0x105[0xfb];
    f32 field_0x200;
};

struct UnkMagnetSource_ovb0082 {
    u8 pad_0x0[0xc0];
    f32 field_0xc0;
};

struct UnkMagnetEnterState_ovb0082 {
    u8 pad_0x0[0x30];
    void* actor;
    UnkMagnetMovement_ovb0082* movement;  // 0x38
    UnkMagnetSource_ovb0082* source;      // 0x40
    u8 pad_0x48[0x160];
    f32 field_0x1a8;
    f32 field_0x1ac;
    UnkAssignedSlot* field_0x1b0;
    u32 field_0x1b8;
    u8 pad_0x1bc[0x28];
    u32 field_0x1e4;
    UnkMagnetSurfaceRef_ovb0082* surface;  // 0x1e8
    UnkAssignedSlot* field_0x1f0;
    u8 pad_0x1f8[0x140];
    UnkHandle rumble;  // 0x338
};
static_assert(__builtin_offsetof(UnkMagnetEnterState_ovb0082, rumble) == 0x338, "layout");

static inline void* SurfaceOwner(UnkMagnetEnterState_ovb0082* self) {
    return self->surface ? self->surface->owner : nullptr;
}

void fn_71009ab200(UnkMagnetEnterState_ovb0082* self, u32 arg) {
    if (self->surface && self->surface->owner) {
        if (void* sliding = fn_710054a2cc(self->surface->owner)) {
            void* actor = self->actor;
            void** vtable = *static_cast<void***>(sliding);
            reinterpret_cast<void (*)(void*, void*)>(vtable[0x170 / 8])(sliding, actor);
        }
        if (void* target = fn_71005b6320(SurfaceOwner(self)))
            fn_710092467c(target, self->actor);
        if (void* target = fn_7100692bb0(SurfaceOwner(self)))
            fn_710092567c(target, self->actor);
    }
    const UnkAssignedSlot* from = reinterpret_cast<const UnkAssignedSlot*>(&self->field_0x1f0);
    self->movement->field_0x200 = self->source->field_0xc0 + 1.1920929e-07f;
    self->field_0x1a8 = *reinterpret_cast<f32*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2444);
    self->field_0x1ac = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_717273c650) + 0x3c);
    fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(&self->field_0x1b0), from);
    self->field_0x1b8 = self->field_0x1e4;
    self->movement->field_0x104 = true;
    if (self->rumble.Get() != g_unk_7101d586c0.Get())
        fn_7100072328(self->rumble.Get(), 0.2f, 0.0f);
    // The base state's step takes a 32-bit value here.
    reinterpret_cast<void (*)(UnkMagnetEnterState_ovb0082*, u32)>(fn_71009b0ce4)(self, arg);
}
