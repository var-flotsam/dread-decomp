// Unit 2635: resets a music-state holder: returns its state machine to state 0 and, in mode 2, fades
// every music channel out. Names and layouts are placeholders.
#include "base/UnkLoadingMask.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkState_lfd0998 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previousId);
    virtual void Exit(s32 reason);
    u8 pad_0x8[0x8];
    u32 id;  // 0x10
};

#pragma pack(push, 4)
struct UnkStateMachine_lfd0998 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEnter(UnkState_lfd0998* previous, UnkState_lfd0998* next);
    virtual void OnExit(UnkState_lfd0998* current, UnkState_lfd0998* next);
    u8 pad_0x8[0x8];
    UnkState_lfd0998* current;   // 0x10
    UnkState_lfd0998* previous;  // 0x18
    u32 count;                   // 0x20
    u8 states[8];                // 0x24, `count` state pointers

    UnkState_lfd0998** begin() { return reinterpret_cast<UnkState_lfd0998**>(states); }
    UnkState_lfd0998** end() { return begin() + count; }

    UnkState_lfd0998* Find(u32 id) {
        for (UnkState_lfd0998* state : *this)
            if (state->id == id)
                return state;
        return nullptr;
    }

    void ChangeState(UnkState_lfd0998* next) {
        if (current) {
            current->Exit(0);
            OnExit(current, next);
        }
        previous = current;
        current = next;
        if (next) {
            next->Enter(previous ? previous->id : 0x7fffffff);
            OnEnter(previous, next);
        }
    }

    void SetState(u32 id) {
        if (current && current->id == id)
            return;
        ChangeState(Find(id));
    }
};
#pragma pack(pop)

struct UnkMusicOwner_lfd0998 {
    u8 pad_0x0[0x8];
    void* music;  // 0x8
};

struct UnkMusicState_lfd0998 {
    u8 pad_0x0[0x8];
    UnkMusicOwner_lfd0998* owner;  // 0x8
    u8 pad_0x10[0x20];
    UnkStateMachine_lfd0998 machine;  // 0x30
    u8 pad_0x5c[0x78];
    s32 field_0xd4;
    bool field_0xd8;
    u8 pad_0xd9[0xa87];
    u32 field_0xb60;
    bool field_0xb64;
};
static_assert(__builtin_offsetof(UnkMusicState_lfd0998, field_0xd4) == 0xd4, "");
static_assert(__builtin_offsetof(UnkMusicState_lfd0998, field_0xb60) == 0xb60, "");

inline const UnkLoadingMask& MaskNone4_lfd0998() {
    static UnkLoadingMask s_mask;
    return s_mask;
}
inline const UnkLoadingMask& MaskAll4_lfd0998() {
    static UnkLoadingMask s_mask(~MaskNone4_lfd0998().bits & 0xf);
    return s_mask;
}
inline const UnkLoadingMask& MaskNone9_lfd0998() {
    static UnkLoadingMask s_mask;
    return s_mask;
}
inline const UnkLoadingMask& MaskAll9_lfd0998() {
    static UnkLoadingMask s_mask(~MaskNone9_lfd0998().bits & 0x1ff);
    return s_mask;
}

void fn_7100fd0998(UnkMusicState_lfd0998* self, s32 mode) {
    self->field_0xb64 = false;
    self->field_0xb60 = self->machine.current->id;
    self->field_0xd4 = 0;
    if (!self->field_0xd8) {
        self->machine.SetState(0);
        self->field_0xd4 = 0x7fffffff;
    }
    if (mode == 2) {
        void* music = self->owner ? self->owner->music : nullptr;
        fn_7100fd7b44(music, MaskAll4_lfd0998(), MaskAll9_lfd0998(), 0.0f);
        self->field_0xb60 = 0;
    }
}
