// CThermalRoomConnectionFX unit (tools/tu_map.py, the name from reflection): stores the new factor
// (+0x7c), refreshing first (virtual slot 0x88) when not enabled (+0x78); then shows the actor's
// (+0x30) effect (fn_7100798eec, fn_710024fa0c) at a factor of 1 or more and hides it at 0 or less.
// Two identical copies. Names and layouts are placeholders.
#include "types.h"

struct UnkStruct_710024fa0c;
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);
UnkStruct_710024fa0c* fn_7100798eec(void* actor);

struct UnkThermalFX {
    virtual void vf_slots();  // slot 0x88 is reached through the vtable below
    u8 unk_0x8[0x28];
    void* actor;  // 0x30
    u8 unk_0x38[0x40];
    bool enabled;  // 0x78
    f32 factor;    // 0x7c
};

static inline __attribute__((always_inline)) void SetFactor(UnkThermalFX* self, f32 factor) {
    bool enabled = self->enabled;
    self->factor = factor;
    if (!enabled) {
        using Fn = void (*)(UnkThermalFX*);
        (*reinterpret_cast<Fn**>(self))[0x88 / 8](self);
    }
    if (self->actor == nullptr)
        return;
    UnkStruct_710024fa0c* fx = fn_7100798eec(self->actor);
    if (fx == nullptr)
        return;
    if (factor >= 1.0f)
        fn_710024fa0c(fx, true);
    else if (factor <= 0.0f)
        fn_710024fa0c(fx, false);
}

void fn_710087b7e0(UnkThermalFX* self, f32 factor) {
    SetFactor(self, factor);
}

void fn_710087b878(UnkThermalFX* self, f32 factor) {
    SetFactor(self, factor);
}
