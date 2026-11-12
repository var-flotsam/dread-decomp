// Unit 2595 (tools/tu_map.py; no class name known): updates of the music trigger's state machine
// (src/game/UnkStateMachineSet_b61.cpp's template, the machine at +0x58 of the trigger) that wait on the
// unit's singleton (fn_7100ec7aec, g_unk_71729dc210) before asking for their state. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2595.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2725.h"
#include "game/UnkTU2744.h"
#include "game/malloc.h"
#include "game/nn/time.h"
#include "game/sound/CMusicManager.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkSingleton2595_b0225 {
    u8 pad_0x0[0x58];
    bool field_0x58;
    bool field_0x59;
    u8 pad_0x5a[0x6];

    UnkSingleton2595_b0225();  // fn_7100ec7aec
};

void fn_7100ec9e38(UnkSingleton2595_b0225* self, f32 dt);

struct UnkSingleton2595_b49a6;

static inline UnkSingleton2595_b0225* Singleton2595() {
    UnkSingleton2595_b0225*& slot = reinterpret_cast<UnkSingleton2595_b0225*&>(g_unk_71729dc210);
    if (!slot) {
        void* mem = fn_71000839a0(sizeof(UnkSingleton2595_b0225), 0x3f);
        slot = new (mem) UnkSingleton2595_b0225();
    }
    return slot;
}

struct UnkSmState_b0225 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b0225* __attribute__((aligned(4))) UnkSmStatePtr_b0225;

struct UnkSmMachine_b0225 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b0225* previous, UnkSmState_b0225* next);
    virtual void vf_0x28(UnkSmState_b0225* current, UnkSmState_b0225* next);

    u64 field_0x8;
    UnkSmState_b0225* current;
    UnkSmState_b0225* previous;
    u32 count;
    UnkSmStatePtr_b0225 states[16];
    u32 pending;
    bool busy;
};

struct UnkSmOwner_b0225 {
    u8 pad_0x0[0x58];
    UnkSmMachine_b0225 machine;  // 0x58
    u8 pad_0x108[0x150];
    f32 timer;  // 0x258
};
static_assert(__builtin_offsetof(UnkSmOwner_b0225, timer) == 0x258, "owner layout");

__attribute__((always_inline)) static inline bool FindState_b0225(UnkSmMachine_b0225* self, u32 id,
                                                                 UnkSmState_b0225*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b0225);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b0225* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b0225)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` and clears the pending id.
__attribute__((always_inline)) static inline void SwitchState_b0225(UnkSmMachine_b0225* self, u32 id,
                                                                   UnkSmState_b0225* current) {
    if (!current || current->id != id) {
        UnkSmState_b0225* found;
        UnkSmState_b0225* next;
        u32 enter;
        if (FindState_b0225(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            asm("" : "+r"(enter));
            next = nullptr;
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_b0225*, s32);
            s32 previous_id = self->previous ? self->previous->id : 0x7fffffff;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = 0x7fffffff;
}

__attribute__((always_inline)) static inline void RequestState_b0225(UnkSmMachine_b0225* self, u32 id) {
    u32* pending = &self->pending;
    *pending = id;
    if (self->busy)
        return;
    SwitchState_b0225(self, id, self->current);
}

// Counts the timer down while the singleton's flag is set, and asks for state 2 once the effect player
// lets go and the timer has run out (at once without the flag).
void fn_7100f13e50(UnkSmOwner_b0225* self, f32 dt) {
    if (Singleton2595()->field_0x59) {
        self->timer -= dt;
        if (!fn_7101053a98(g_unk_7101d47310->field_0x1040) && self->timer <= 0.0f)
            RequestState_b0225(&self->machine, 2);
    } else {
        RequestState_b0225(&self->machine, 2);
    }
}

// Steps the singleton while its flag is set, and asks for state 3 once it and the effect player are done
// (at once without the flag).
void fn_7100f14118(UnkSmOwner_b0225* self, f32 dt) {
    if (Singleton2595()->field_0x59) {
        fn_7100ec9e38(Singleton2595(), dt);
        if (!Singleton2595()->field_0x58 && !fn_7101053a98(g_unk_7101d47310->field_0x1040))
            RequestState_b0225(&self->machine, 3);
    } else {
        RequestState_b0225(&self->machine, 3);
    }
}

struct UnkSoundHolder;
extern UnkSoundHolder* g_unk_7101d47340;
struct UnkMgr_0xf18;
extern UnkMgr_0xf18* g_unk_71726bc560;
extern base::global::CRntString g_unk_71729dc240 __attribute__((visibility("hidden")));
struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};
using base::global::CRntString;
void fn_7100fd90d8(f32 a, f32 b, void* music, const CRntString* name, u32 c, const UnkMusicMode* d, u32 e,
                   u32 f, void* g);

// The state's entry: the base's step (which also takes the argument), the sound holder, an "Ingame" string id built and dropped, the GUI root
// told, state 1 requested and the "SamusStory" music started.
void fn_7100f135d8(UnkSmOwner_b0225* self, void* arg) {
    fn_7100132aa0(true);
    reinterpret_cast<void (*)(char*, void*)>(fn_71011867ec)(reinterpret_cast<char*>(self), arg);
    if (!g_unk_7101d47340) {
        UnkSoundHolder* p = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(p);
        g_unk_7101d47340 = p;
    }
    base::global::CStrId("Ingame", true);
    fn_71011b3768(g_unk_717271e8f8);
    RequestState_b0225(&self->machine, 1);
    void* music = reinterpret_cast<void*&>(g_unk_71726bc560);
    if (!music) {
        music = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(music);
        reinterpret_cast<void*&>(g_unk_71726bc560) = music;
    }
    UnkMusicMode mode;
    mode.kind = 0x4e;
    fn_7100fd90d8(-1.0f, -1.0f, music, &g_unk_71729dc240, 7, &mode, 0, 0, nullptr);
}
