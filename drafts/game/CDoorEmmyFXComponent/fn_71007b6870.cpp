// CDoorEmmyFXComponent unit (tools/tu_map.py). Catches the effect up on the time since it started
// (+0x128 against the game time): through the first phase (+0x124), then the second (+0x120), then
// the rest, stepping itself (fn_71007b583c) and the owner's two effect objects; then refreshes the
// effect (fn_71007b5f58) and, when the key list at +0x70 is empty, the owner's timeline key.
// Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU39.h"
#include "game/UnkTimelineComponent.h"
#include <math.h>
#include <float.h>
#include "types.h"

extern const u64 g_unk_710164aac0;

// An effect object of the owner, stepped through its virtual slots.
struct UnkDoorEmmyEffect_b6870 {
    void** vtable;
    void Call(u32 offset, f32 dt) {
        reinterpret_cast<void (*)(UnkDoorEmmyEffect_b6870*, f32)>(vtable[offset / 8])(this, dt);
    }
};

struct UnkDoorEmmyOwner_b6870 {
    u8 pad_0x0[0x288];
    UnkDoorEmmyEffect_b6870* field_0x288;
    u8 pad_0x290[0x58];
    UnkDoorEmmyEffect_b6870* field_0x2e8;
};

struct UnkDoorEmmyFX_b6870 {
    u8 pad_0x0[0x30];
    UnkDoorEmmyOwner_b6870* owner;
    u8 pad_0x38[0x38];
    u8 field_0x70[0xb0];
    f32 field_0x120;  // the second phase's length
    f32 field_0x124;  // the first phase's length
    f32 field_0x128;  // when the effect started; 0 once caught up
};

void fn_71007b583c(UnkDoorEmmyFX_b6870* self, f32 dt);
void fn_71007b5f58(UnkDoorEmmyFX_b6870* self, u32 a, u32 b);

static inline void Advance(UnkDoorEmmyFX_b6870* self, UnkDoorEmmyEffect_b6870* main,
                           UnkDoorEmmyEffect_b6870* extra, f32 dt) {
    if (dt > 0.0f) {
        fn_71007b583c(self, dt);
        main->Call(0xa8, dt);
        main->Call(0xb0, dt);
        if (extra) {
            extra->Call(0x98, dt);
            extra->Call(0xa8, dt);
            extra->Call(0xb0, dt);
        }
    }
}

void fn_71007b6870(UnkDoorEmmyFX_b6870* self) {
    UnkDoorEmmyOwner_b6870* owner = self->owner;
    UnkDoorEmmyEffect_b6870* main = owner->field_0x2e8;
    if (main && self->field_0x128 > 0.0f) {
        UnkDoorEmmyEffect_b6870* extra = owner->field_0x288;
        f32 remaining = g_unk_7101d47310->field_0x2444 - self->field_0x128;
        f32 first = self->field_0x124;
        if (first > 0.0f) {
            if (first >= remaining) {
                fn_71007b583c(self, remaining);
                remaining = 0.0f;
            } else {
                f32 step = nextafterf(first, FLT_MAX);
                Advance(self, main, extra, step);
                remaining -= step;
            }
        }
        f32 second = self->field_0x120;
        if (second > 0.0f && !(second >= remaining)) {
            f32 step = nextafterf(second, FLT_MAX);
            Advance(self, main, extra, step);
            remaining -= step;
        }
        Advance(self, main, extra, remaining);
        self->field_0x128 = 0.0f;
    }
    fn_71007b5f58(self, 1, 0);
    if (!fn_710002fb70(self->field_0x70))
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164aac0, false);
}
