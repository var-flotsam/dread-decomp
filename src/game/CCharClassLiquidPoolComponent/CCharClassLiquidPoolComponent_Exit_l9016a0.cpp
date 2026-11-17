// CCharClassLiquidPoolComponent's unit (tools/tu_map.py unit 1394): an actor leaves the pool (after the base
// trigger's handler, fn_7100a45cf0). For the player while the pool is in state 1, the player info is told and
// the surface ripple placed at the position; otherwise a moving actor is told, the ripple placed and the
// splash played, or an AI is put in mode 1 by the pool's kind. Names and layouts are placeholders, as in
// CCharClassLiquidPoolComponent_Pool_ovb0075.cpp (fn_7100901dd4, the entering twin).
#include "game/CLiquidSimulationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game/CCharClassBaseDamageTriggerComponent/CCharClassBaseDamageTriggerComponent_Update_ovb0119.cpp
// defines it.
struct UnkDamageTrigger_b0119;
void fn_7100a45cf0(UnkDamageTrigger_b0119* self, void* arg);

// As src/game/CCharClassLiquidPoolComponent/CCharClassLiquidPoolComponent_Pool_ovb0075.cpp declares them.
struct UnkLiquidPoolEnter_ovb0075;
struct UnkLiquidPoolActor_ovb0075;
struct UnkLiquidPoolMover_ovb0075;
void fn_71009014a8(UnkLiquidPoolEnter_ovb0075* self, UnkVec3f pos);
UnkLiquidPoolMover_ovb0075* fn_71009019bc(UnkLiquidPoolActor_ovb0075* actor);
struct UnkLiquidPoolActorPart_ovb0075;
void fn_71010ff50c(UnkLiquidPoolActorPart_ovb0075* part, bool a, u32);

struct UnkLiquidPoolExit_l9016a0;
s32 fn_7100f0d5c0(void* list, UnkLiquidPoolExit_l9016a0* self, const UnkVec3f* pos, s32 kind, u32 param);

struct UnkLiquidPoolOwner_l9016a0 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
};

struct UnkLiquidPoolAI_l9016a0 {
    void** vtable;

    bool vf_0x638() { return reinterpret_cast<bool (*)(UnkLiquidPoolAI_l9016a0*)>(vtable[0x638 / 8])(this); }
};

struct UnkLiquidPoolActor_l9016a0 {
    u8 pad_0x0[0x240];
    UnkLiquidPoolAI_l9016a0* ai;  // 0x240
};

struct UnkLiquidPoolMover_l9016a0 {
    void** vtable;

    void vf_0x2e8(const UnkVec3f* pos, s32 kind, u32 param) {
        reinterpret_cast<void (*)(UnkLiquidPoolMover_l9016a0*, const UnkVec3f*, s32, u32)>(vtable[0x2e8 / 8])(
            this, pos, kind, param);
    }
};

struct UnkLiquidPoolFx_l9016a0 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkLiquidPoolExit_l9016a0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLiquidPoolOwner_l9016a0* owner;  // 0x30
    u8 pad_0x38[0x9b8 - 0x38];
    s32 field_0x9b8;
    u8 pad_0x9bc[0x24c];
    u32 field_0xc08;
    u8 pad_0xc0c[0x7c];
    UnkLiquidPoolFx_l9016a0* fx;  // 0xc88

    UnkVec3f vf_0x178() { return reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolExit_l9016a0*)>(vtable[0x178 / 8])(this); }
    UnkVec3f vf_0x180() { return reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolExit_l9016a0*)>(vtable[0x180 / 8])(this); }
    s32 vf_0x258() { return reinterpret_cast<s32 (*)(UnkLiquidPoolExit_l9016a0*)>(vtable[0x258 / 8])(this); }

    // Places the surface ripple at `height` across the pool (fn_7100901900, inlined).
    void SetRatio(const f32* height, u32 a, bool b) {
        f32 h = *height;
        UnkVec3f pos = vf_0x180();
        f32 base = pos.x;
        UnkVec3f ext = vf_0x178();
        f32 extent = ext.x;
        UnkVec3f size = vf_0x178();
        if (fx != nullptr && fx->field_0x10 != nullptr) {
            f32 width = size.x;
            f32 ratio = (h - (base + extent * -0.5f)) / width;
            fn_7100905d8c(fx->field_0x10, ratio, a, b);
        }
    }
};
static_assert(__builtin_offsetof(UnkLiquidPoolExit_l9016a0, fx) == 0xc88, "layout");

void fn_71009016a0(UnkLiquidPoolExit_l9016a0* self, UnkLiquidPoolActor_l9016a0* actor, const UnkVec3f* pos) {
    fn_7100a45cf0(reinterpret_cast<UnkDamageTrigger_b0119*>(self), actor);
    if (self->field_0x9b8 == 1 && !self->owner->field_0x1b5 && fn_7100df5c38(g_unk_7101d47310, actor)) {
        if (fn_7100f0d5c0(reinterpret_cast<char*>(g_unk_7101d47310) + 0x38, self, pos, self->vf_0x258(),
                          self->field_0xc08) == 1)
            self->SetRatio(&pos->x, 0, true);
        return;
    }
    auto* mover = reinterpret_cast<UnkLiquidPoolMover_l9016a0*>(
        fn_71009019bc(reinterpret_cast<UnkLiquidPoolActor_ovb0075*>(actor)));
    if (mover) {
        mover->vf_0x2e8(pos, self->vf_0x258(), self->field_0xc08);
        self->SetRatio(&pos->x, 1, true);
        fn_71009014a8(reinterpret_cast<UnkLiquidPoolEnter_ovb0075*>(self), *pos);
    } else if (UnkLiquidPoolAI_l9016a0* ai = actor->ai) {
        if (ai->vf_0x638())
            fn_71010ff50c(reinterpret_cast<UnkLiquidPoolActorPart_ovb0075*>(ai), true, self->vf_0x258());
    }
}
