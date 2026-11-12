// From unit 2728 of tools/tu_map.py (0x7101192628-0x7101192c5c): a timer that, once its period runs
// out while its target handle is gone, fades the music (fn_7100fd7af8 on the music manager singleton),
// notifies the game manager's object at +0x2278 and fires its stored callback once. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "game/GUI/CSubtitleComposition.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

extern UnkMgr_0xf18* g_unk_71726bc560;
#include "game/UnkGameManager.h"

struct UnkUnit2728Target;

struct UnkHandleTarget_7101192944 {
    void* obj;
    u64 id;
};

struct UnkNotified_7101192944 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};


// A one-shot callback: called with its argument, then cleared.
struct UnkCallback_7101192944 {
    void* arg;
    void (*fn)(void* arg);
};

struct UnkTimerF_7101192944 {
    UnkHandleTarget_7101192944* target;  // +0x0
    u64 id;                              // +0x8
    UnkCallback_7101192944 callback;     // +0x10
    f32 elapsed;                         // +0x20
    f32 period;                          // +0x24
    u64 field_0x28;
    f32 total;                           // +0x30
    f32 fade;                            // +0x34
};

f32 fn_7101192944(UnkTimerF_7101192944* self, f32 dt) {
    f32 t = self->elapsed + dt;
    self->elapsed = t;
    if (t < self->period)
        return t;
    self->elapsed = 0;
    UnkHandleTarget_7101192944* target = self->target;
    if (!target || target->id != self->id || self->id == (u64)-1 || !target->obj) {
        if (self->fade >= 0) {
            void* music = g_unk_71726bc560;
            if (!music) {
                music = fn_71000839a0(0xf18, 0x3f);
                fn_7100fd3804(music);
                g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(music);
            }
            fn_7100fd7af8(music, self->fade, 0.2f);
            self->fade = -1.0f;
        }
        UnkNotified_7101192944* n = *reinterpret_cast<UnkNotified_7101192944**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2278);
        if (n && n->field_0x10)
            fn_7100b8538c(static_cast<UnkUnit2728Target*>(n->field_0x10));
        self->field_0x28 = 0;
        if (void (*fn)(void*) = self->callback.fn) {
            UnkCallback_7101192944* cb = &self->callback;
            asm("" : "+r"(cb));
            fn(cb->arg);
            *cb = UnkCallback_7101192944{};
        }
    }
    self->total += dt;
    return self->total;
}
