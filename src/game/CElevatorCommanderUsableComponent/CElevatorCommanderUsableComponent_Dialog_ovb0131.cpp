// CElevatorCommanderUsableComponent's unit (tools/tu_map.py unit 1590). fn_7100a80930 asks the elevator
// commander question through the GUI composition (the game manager's +0x2258, its +0x10) with
// "#SKYBASE_ELEVATOR_COMMANDER", accept and back texts and two callbacks on the component (fn_7100a80e14,
// fn_7100a80e20); the same shape as fn_7100a86d00
// (src/game/CMapAcquisitionComponent/CMapAcquisitionComponent_Question_ovb0090.cpp). fn_7100a80ad8 shows or
// hides the commander dialog: when the owner's animation (+0x220) plays the use animation (the id at +0x30 of
// the record behind vtable slot 0x208), the environment manager (g_unk_7101d47368, made on first use as in
// src/game/CSaveStationUsableComponent/CSaveStationUsableComponent_Use_ovb0118.cpp) is notified
// (fn_7100d8e0a0) and the player's input (+0x1d8) is switched off, else on (fn_7100872460). Then the plain
// use (fn_7100aa2570), the user's (+0xe0) smart object (fn_710033dc00, fn_7100a0a08c), the owner's +0x2a0
// object (fn_7100861148), the player's +0x1f0 object gets a fixed position (vtable slot 0x1b0) and the
// owner's +0x228 object is switched off (fn_710024fa0c). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "game/CActorComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CInputComponent.h"
#include "game/CUsableComponent.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkElevatorCommander_ovb0131;
void fn_7100a80e14(UnkElevatorCommander_ovb0131* self);
void fn_7100a80e20(UnkElevatorCommander_ovb0131* self);

struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const CStrId* text, const CStrId* accept, const CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time);

// A callback: an object and a function to call on it.
struct UnkElevatorCallback_ovb0131 {
    UnkElevatorCommander_ovb0131* object;
    void (*function)(UnkElevatorCommander_ovb0131*);
};

struct UnkGuiRef_ovb0131 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

void fn_7100a80930(UnkElevatorCommander_ovb0131* self) {
    UnkGuiRef_ovb0131* ref = static_cast<UnkGuiRef_ovb0131*>(g_unk_7101d47310->field_0x2258);
    if (!ref)
        return;
    UnkGuiComposition_ovb0090* gui = ref->composition;
    if (!gui)
        return;
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    CStrId text("#SKYBASE_ELEVATOR_COMMANDER", true);
    UnkElevatorCallback_ovb0131 accept_callback = {self, fn_7100a80e14};
    UnkElevatorCallback_ovb0131 back_callback = {self, fn_7100a80e20};
    fn_7100b5fde0(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
}

struct UnkPlayer;
struct UnkInputComponent;
extern void* g_unk_7101d47368;
struct UnkUsableDialog_A6_b62;
struct UnkPlatformSm_b61;
struct UnkStruct_7100861148;
struct UnkStruct_710024fa0c;

struct UnkElevatorInputArg_ovb0131;
extern UnkElevatorInputArg_ovb0131 g_unk_7172750640 __attribute__((visibility("hidden")));
UnkPlatformSm_b61* fn_710033dc00(void* object);

struct UnkElevatorPosition_ovb0131 {
    f32 x, y, z;
    UnkElevatorPosition_ovb0131(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkElevatorTarget_ovb0131 {
    void** vtable;

    void vf_0x1b0(const UnkElevatorPosition_ovb0131* position) {
        reinterpret_cast<void (*)(UnkElevatorTarget_ovb0131*, const UnkElevatorPosition_ovb0131*)>(
            vtable[0x1b0 / 8])(this, position);
    }
};

struct UnkElevatorPlayer_ovb0131 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x10];
    UnkElevatorTarget_ovb0131* target;  // 0x1f0
};

struct UnkElevatorOwner_ovb0131 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    UnkStruct_710024fa0c* field_0x228;
    u8 pad_0x230[0x70];
    UnkStruct_7100861148* field_0x2a0;
};

struct UnkElevatorAnimId_ovb0131 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkElevatorUser_ovb0131 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkElevatorUsable_ovb0131 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkElevatorOwner_ovb0131* owner;  // 0x30
    u8 pad_0x38[0xa8];
    UnkElevatorUser_ovb0131* user;  // 0xe0

    UnkElevatorAnimId_ovb0131** vf_0x208() {
        return reinterpret_cast<UnkElevatorAnimId_ovb0131** (*)(UnkElevatorUsable_ovb0131*)>(vtable[0x208 / 8])(
            this);
    }
};

void fn_7100a80ad8(UnkElevatorUsable_ovb0131* self, bool show) {
    auto* player = reinterpret_cast<UnkElevatorPlayer_ovb0131*>(fn_7100dec788(g_unk_7101d47310));
    UnkInputComponent* input = player ? player->input : nullptr;
    UnkAnimationComponent* animation = self->owner->animation;
    if (fn_710117ae38(animation, (*self->vf_0x208())->id, false)) {
        void* manager = g_unk_7101d47368;
        if (!manager) {
            manager = fn_71000839a0(0x4c8, 0x3f);
            fn_7100d8ced8(manager);
            g_unk_7101d47368 = manager;
        }
        fn_7100d8e0a0(manager);
        if (input)
            fn_7100872460(input, false, false, &g_unk_7172750640);
    } else if (input) {
        fn_7100872460(input, true, false, &g_unk_7172750640);
    }
    fn_7100aa2570(reinterpret_cast<UnkUsableDialog_A6_b62*>(self), show);
    if (UnkPlatformSm_b61* smart = fn_710033dc00(self->user ? self->user->object : nullptr))
        fn_7100a0a08c(smart);
    if (UnkStruct_7100861148* object = self->owner->field_0x2a0)
        fn_7100861148(object);
    UnkElevatorTarget_ovb0131* target = player->target;
    UnkElevatorPosition_ovb0131 position(1300.0f, 2026.6f, 0.0f);
    target->vf_0x1b0(&position);
    fn_710024fa0c(self->owner->field_0x228, false);
}
