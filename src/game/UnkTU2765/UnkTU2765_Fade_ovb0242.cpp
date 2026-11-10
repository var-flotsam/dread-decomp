// Unit 2765 (tools/tu_map.py): a colour fade on a GUI object held through a weak handle. The constructor
// takes the weak handle of its owner; the update copies the object's colour (+0xa0) when it is flagged
// pending, then blends from it towards the target colour over the duration (fn_71011c1764 blends, the
// result goes to the object through fn_71011cfc24) and, once the time is up, parks the timer
// (elapsed 0, duration -1). Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "../../game_s7/UnkGuiS7.h"
#include "types.h"

extern void* g_unk_7101cb7168[];

struct UnkFadeColor_b0242 {
    f32 r, g, b, a;
};
UnkFadeColor_b0242 fn_71011c1764(const UnkFadeColor_b0242* from, const UnkFadeColor_b0242* to, f32 t);

// The values a target keeps at +0xa0 (a colour).
struct UnkFadeTarget_b0242 {
    u8 pad_0x0[0xa0];
    f32 field_0xa0[4];
};

struct UnkFade_b0242 {
    void** vtable;
    bool pending;  // 0x8
    u8 pad_0x9[3];
    f32 from[4];   // 0xc
    f32 to[4];     // 0x1c
    f32 elapsed;   // 0x2c
    f32 duration;  // 0x30
    u8 pad_0x34[4];
    UnkWeakHandle* handle;  // 0x38
};

void fn_7101202d1c(UnkFade_b0242* self, UnkWeakTarget* owner) {
    self->pending = false;
    self->vtable = &g_unk_7101cb7168[2];
    __builtin_memset(self->from, 0, sizeof(self->from));
    __builtin_memset(self->to, 0, sizeof(self->to));
    self->elapsed = 0.0f;
    self->duration = -1.0f;
    UnkWeakHandle* handle = GetWeakHandle(owner);
    self->handle = nullptr;
    AssignRef(&self->handle, handle);
}

void fn_7101202eec(UnkFade_b0242* self, f32 dt) {
    if (self->pending) {
        UnkFadeTarget_b0242* target = self->handle ? reinterpret_cast<UnkFadeTarget_b0242*>(self->handle->target) : nullptr;
        __builtin_memcpy(self->from, target->field_0xa0, sizeof(self->from));
        self->elapsed = 0.0f;
        self->pending = false;
    }
    f32 duration = self->duration;
    if (duration > 0.0f) {
        f32 elapsed = self->elapsed;
        elapsed += dt;
        self->elapsed = elapsed;
        f32 t = elapsed / duration;
        f32 clamped = !(t >= 1.0f) ? t : 1.0f;
        // (the compare is `hi`: true for a NaN too)
        f32 blend = (t > 0.0f || t != t) ? clamped : 0.0f;
        UnkFadeColor_b0242 color = fn_71011c1764(reinterpret_cast<const UnkFadeColor_b0242*>(self->from),
                                                 reinterpret_cast<const UnkFadeColor_b0242*>(self->to), blend);
        unk_gui::fn_71011cfc24(self->handle ? reinterpret_cast<unk_gui::UnkGuiObj*>(self->handle->target) : nullptr,
                               reinterpret_cast<const unk_gui::UnkColor&>(color), 0);
        if (self->elapsed > self->duration + -1.1920929e-7f) {
            self->elapsed = 0.0f;
            self->duration = -1.0f;
        }
    }
}
