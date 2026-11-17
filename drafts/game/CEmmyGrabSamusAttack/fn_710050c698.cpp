// CEmmyGrabSamusAttack unit (tools/tu_map.py): one grab-hold update. Without the flag at +0x700 it runs
// fn_7100507a28 and hands the player's +0x1c0 part a string id; otherwise it counts the timers and, once
// the countdown at +0x6bc runs out, asks the state machine at +0x28 for state 5 when the E.M.M.I. AI's
// value (fn_7100552ee4) is positive, else state 8. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;

struct UnkSmState_l50c698 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_l50c698* __attribute__((aligned(4))) UnkSmStatePtr_l50c698;

// A small state machine: states 0..13, 14 meaning none.
struct UnkSmMachine_l50c698 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_l50c698* previous, UnkSmState_l50c698* next);
    virtual void vf_0x28(UnkSmState_l50c698* current, UnkSmState_l50c698* next);

    u64 field_0x8;
    UnkSmState_l50c698* current;
    UnkSmState_l50c698* previous;
    u32 count;
    UnkSmStatePtr_l50c698 states[16];
    u32 pending;
    bool busy;

    static const u32 kLast = 13;
    static const u32 kNone = 14;

    UnkSmState_l50c698* Find(u32 state) {
        UnkSmStatePtr_l50c698* p = states;
        for (u64 size = (u64)count * sizeof(*p); size != 0; size -= sizeof(*p), ++p)
            if ((*p)->id == state)
                return *p;
        return nullptr;
    }

    void Switch(u32 state) {
        if (!current || current->id != state) {
            UnkSmState_l50c698* next = Find(state);
            if (next || state == 0) {
                if (current) {
                    current->vf_0x18(state);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                if (next) {
                    next->vf_0x10(previous ? previous->id : kNone);
                    vf_0x20(previous, next);
                }
            }
        }
        pending = kNone;
    }

    void Request(u32 state) {
        pending = state;
        if (state > kLast)
            return;
        if (busy)
            return;
        Switch(state);
    }
};

struct UnkGrabPart_l50c698;
struct UnkGrabPlayer_l50c698 {
    u8 pad_0x0[0x1c0];
    UnkGrabPart_l50c698* field_0x1c0;
};

struct UnkGrabAI_l50c698 {
    void** vtable;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGrabAI_l50c698*)>(vtable[0])(this);
    }
};

struct UnkGrabHold_l50c698 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x28 - 0x10];
    UnkSmMachine_l50c698 machine;
    u8 pad_0xd8[0x6b4 - 0xd8];
    f32 timer;
    u8 pad_0x6b8[4];
    f32 countdown;
    u8 pad_0x6c0[0x700 - 0x6c0];
    bool holding;
};
static_assert(__builtin_offsetof(UnkGrabHold_l50c698, machine.pending) == 0xcc, "layout");

struct UnkGrabSm_b61;
struct UnkGrabAttack_b58a3;
struct UnkEmmyAI_b61;
void fn_710050c490(UnkGrabSm_b61* self);
void fn_7100507a28(UnkGrabAttack_b58a3* self);
void fn_710116a160(UnkGrabPart_l50c698* self, const CStrId* id, f32 value);
extern CStrId g_unk_71726de5c8 __attribute__((visibility("hidden")));

void fn_710050c698(UnkGrabHold_l50c698* self, f32 dt) {
    if (!self->holding) {
        fn_7100507a28(reinterpret_cast<UnkGrabAttack_b58a3*>(self));
        auto* player = reinterpret_cast<UnkGrabPlayer_l50c698*>(fn_7100dec788(g_unk_7101d47310));
        if (player && player->field_0x1c0)
            fn_710116a160(player->field_0x1c0, &g_unk_71726de5c8, 1.0f);
        return;
    }
    self->timer += dt;
    UnkActor* actor = reinterpret_cast<UnkActor*>(fn_7100defcd4(g_unk_7101d47310));
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(actor->field_0x220), fn_7100d0ebf8()))
        return;
    fn_710050c490(reinterpret_cast<UnkGrabSm_b61*>(self));
    self->countdown -= dt;
    if (!(self->countdown <= 0.0f))
        return;
    auto* ai = static_cast<UnkGrabAI_l50c698*>(self->owner->field_0x240);
    auto* emmy = ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? reinterpret_cast<UnkEmmyAI_b61*>(ai) : nullptr;
    if (fn_7100552ee4(emmy) > 0.0f)
        self->machine.Request(5);
    else
        self->machine.Request(8);
}
