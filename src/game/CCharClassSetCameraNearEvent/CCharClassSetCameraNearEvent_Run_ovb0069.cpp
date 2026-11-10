// CCharClassSetCameraNearEvent's unit (tools/tu_map.py): the event's run: with the game manager's
// camera (fn_7100defcec), unless the actor's facing (its matrix's +0x8, fn_710002614c on its transform
// at +0x38) rules it out for the event's side (+0x18: 1 only when facing left, 2 only when facing
// right), the camera gets the event's near settings (+0x10) and blend time (+0xc) (fn_71010e4a78).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkSetCameraNearEvent_ovb0069 {
    u8 pad_0x0[0xc];
    f32 time;      // 0xc
    u8 near[0x8];  // 0x10
    s32 side;      // 0x18
};

struct UnkCameraNearActor_ovb0069 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

void fn_71010e4a78(void* camera, const void* settings, f32 time);

void fn_7100cd1bd4(UnkSetCameraNearEvent_ovb0069* self, UnkCameraNearActor_ovb0069** actor) {
    void* camera = fn_7100defcec(g_unk_7101d47310);
    if (!camera)
        return;
    if (UnkCameraNearActor_ovb0069* a = *actor) {
        switch (self->side) {
        case 1:
            if (!(fn_710002614c(&a->transform).m[2] < 0.0f))
                return;
            break;
        case 2:
            if (!(fn_710002614c(&a->transform).m[2] > 0.0f))
                return;
            break;
        }
    }
    fn_71010e4a78(camera, self->near, self->time);
}
