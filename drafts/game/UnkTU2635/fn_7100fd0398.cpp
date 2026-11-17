// Unit 2635: per-frame update of a music-state holder: outside the "INGAME" scene's hold, counts a delay
// down to a requested state, then applies pending state requests and runs the current state, at most ten
// times. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;

struct UnkMachine_lfd0398;

struct UnkState_lfd0398 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previousId);
    virtual void Exit(s32 nextId);
    virtual void Update(f32 dt);
    UnkMachine_lfd0398* machine;  // 0x8
    u32 id;                       // 0x10
};

#pragma pack(push, 4)
struct UnkMachine_lfd0398 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEnter(UnkState_lfd0398* previous, UnkState_lfd0398* next);
    virtual void OnExit(UnkState_lfd0398* current, UnkState_lfd0398* next);
    void* owner;                 // 0x8
    UnkState_lfd0398* current;   // 0x10
    UnkState_lfd0398* previous;  // 0x18
    u32 count;                   // 0x20
    u32 states[0x20];            // 0x24, UnkState_lfd0398* entries, 4-byte aligned
    UnkState_lfd0398** States() { return reinterpret_cast<UnkState_lfd0398**>(states); }

    UnkState_lfd0398* Find(u32 id) {
        UnkState_lfd0398** begin = States();
        UnkState_lfd0398** end = begin + count;
        for (UnkState_lfd0398** p = begin; p != end; ++p)
            if ((*p)->id == id)
                return *p;
        return nullptr;
    }

    // Switches to the state with `id`; id 0 with no such state leaves the machine with none.
    void SetState(u32 id) {
        if (current && current->id == id)
            return;
        UnkState_lfd0398* next = Find(id);
        if (next || id == 0)
            ChangeState(next);
    }

    void ChangeState(UnkState_lfd0398* next) {
        if (current) {
            current->Exit(next ? next->id : 0);
            OnExit(current, next);
        }
        previous = current;
        current = next;
        if (next) {
            next->Enter(previous ? previous->id : 0x7fffffff);
            OnEnter(previous, next);
        }
    }
};
#pragma pack(pop)

enum UnkMusicRequest_lfd0398 : u32 {
    UnkMusicRequest_lfd0398_None = 0x7fffffff,
};

struct UnkScene_lfd0398 {
    u8 pad_0x0[0x260];
    u32 field_0x260;
};

struct UnkMusicState_lfd0398 {
    u8 pad_0x0[0x30];
    UnkMachine_lfd0398 machine;      // 0x30
    UnkMusicRequest_lfd0398 request;  // 0xd4
    bool updating;                   // 0xd8
    u8 pad_0xd9[0xb68 - 0xd9];
    u32 delayed_state;               // 0xb68
    f32 delay;                       // 0xb6c
    f32 timer;                       // 0xb70

    // Takes the pending request now, if it names a state.
    void ApplyRequest() {
        u32 id = request;
        if (id < 8) {
            machine.SetState(id);
            request = UnkMusicRequest_lfd0398_None;
        }
    }

    void Request(u32 id) {
        request = static_cast<UnkMusicRequest_lfd0398>(id);
        if (id < 8 && !updating) {
            machine.SetState(id);
            request = UnkMusicRequest_lfd0398_None;
        }
    }
};
static_assert(__builtin_offsetof(UnkMusicState_lfd0398, request) == 0xd4, "layout");
static_assert(__builtin_offsetof(UnkMusicState_lfd0398, delayed_state) == 0xb68, "layout");

void fn_7100fd0398(UnkMusicState_lfd0398* self, f32 dt) {
    UnkScene_lfd0398* scene =
        static_cast<UnkScene_lfd0398*>(fn_7100deab70(g_unk_7101d47310, CRntString("INGAME")));
    if (scene->field_0x260 == 0 && self->timer >= 0.0f && self->delay >= 0.0f && self->delayed_state != 0) {
        u32 id = self->delayed_state;
        self->timer += dt;
        if (self->timer >= self->delay)
            self->Request(id);
    }

    self->updating = true;
    self->ApplyRequest();
    // At most ten requests a frame: a state may request another while it updates.
    s32 steps = 0;
    while (self->machine.current) {
        self->machine.current->Update(dt);
        if (self->request == UnkMusicRequest_lfd0398_None)
            break;
        self->ApplyRequest();
        if (++steps == 10)
            break;
    }
    self->updating = false;
}
