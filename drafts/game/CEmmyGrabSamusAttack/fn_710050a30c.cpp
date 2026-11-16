// CEmmyGrabSamusAttack's unit: ends the grab. Resets the attack, moves its state machine to its first
// state, drops the weak handle it listens on and the object at +0x7a8, tells the game, the scenario script
// (CurrentScenario.OnEmmyGrabEnd) and the central-unit registry. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/UnkAttack.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/lua_lib.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkUnit2547;
void fn_7100ddbb7c(UnkUnit2547* self, u32 expected);

struct UnkGrabEndInfo_l50a30c;
extern UnkGrabEndInfo_l50a30c g_unk_71726de5c0 __attribute__((visibility("hidden")));
void fn_71010e4a80(void* self, UnkGrabEndInfo_l50a30c* info);

struct UnkGrabState_l50a30c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The grab's state machine; its states are only 4-aligned after the count.
struct UnkGrabStateMachine_l50a30c {
    static const s32 kNone = 0xe;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkGrabState_l50a30c* previous, UnkGrabState_l50a30c* next);  // after entering
    virtual void vf_0x28(UnkGrabState_l50a30c* current, UnkGrabState_l50a30c* next);   // after leaving

    u64 field_0x8;
    UnkGrabState_l50a30c* current;
    UnkGrabState_l50a30c* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkGrabState_l50a30c*)];
    s32 pending;
    bool field_0xa8;

    UnkGrabState_l50a30c** items() { return reinterpret_cast<UnkGrabState_l50a30c**>(storage); }

    UnkGrabState_l50a30c* Find(s32 id) {
        u64 size = count * sizeof(UnkGrabState_l50a30c*);
        for (UnkGrabState_l50a30c** p = items(); size != 0; ++p, size -= sizeof(UnkGrabState_l50a30c*)) {
            if ((*p)->id == id)
                return *p;
        }
        return nullptr;
    }

    void Request(s32 id) {
        pending = id;
        if (field_0xa8)
            return;
        UnkGrabState_l50a30c* cur = current;
        if (!cur || cur->id != id) {
            if (UnkGrabState_l50a30c* next = Find(id)) {
                if (cur) {
                    cur->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : kNone);
                vf_0x20(previous, next);
            }
        }
        pending = kNone;
    }
};
static_assert(__builtin_offsetof(UnkGrabStateMachine_l50a30c, pending) == 0xa4, "layout");

struct UnkEmmyGrabAttack_l50a30c : UnkAttackBase {
    u8 pad_0x20[0x8];
    UnkGrabStateMachine_l50a30c state_machine;  // 0x28
    u8 pad_0xd8[0x8];
    u8 link[0x18];                               // 0xe0
    UnkWeakHandle* handle;                       // 0xf8
    u8 pad_0x100[0x7a8 - 0x100];
    UnkRefObject* field_0x7a8;
};
static_assert(__builtin_offsetof(UnkEmmyGrabAttack_l50a30c, handle) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkEmmyGrabAttack_l50a30c, field_0x7a8) == 0x7a8, "layout");

struct UnkGrabReleasable_l50a30c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();  // released
};

static inline UnkUnit2547* Registry() {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    return reinterpret_cast<UnkUnit2547*>(reg);
}

void fn_710050a30c(UnkEmmyGrabAttack_l50a30c* self) {
    fn_71003609b8(self);
    self->state_machine.Request(1);
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target)
            fn_710117b3f0(handle->target, self->link);
        ResetRef(&self->handle);
    }
    if (self->field_0x7a8) {
        reinterpret_cast<UnkGrabReleasable_l50a30c*>(self->field_0x7a8)->vf_0xe0();
        self->field_0x7a8 = nullptr;
    }
    if (void* game = fn_7100defcec(g_unk_7101d47310))
        fn_71010e4a80(game, &g_unk_71726de5c0);
    if (fn_71010c0bb0(g_unk_7172a01d18, "CurrentScenario.OnEmmyGrabEnd")) {
        fn_7100fbc4e0(g_unk_7172a01d18, self->field_0x8);
        char error[0x100];
        fn_71010c0d80(g_unk_7172a01d18, 1, error, sizeof(error), nullptr, nullptr);
    }
    fn_7100ddbb7c(Registry(), 0);
}
