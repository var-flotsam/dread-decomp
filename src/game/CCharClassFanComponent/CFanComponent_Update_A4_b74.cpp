// CCharClassFanComponent unit (tools/tu_map.py, unit 1336): the update. After the base update, when either effect
// (+0xae8, +0xaf0) is missing, takes the owner's (+0x30) object at +0x280 into +0xae0 and, when there is one,
// creates them (fn_710087f138, fn_710087f22c); starts both effects on it (fn_7101152c0c), then runs the four
// per-frame steps. Names and layouts are placeholders.
#include "types.h"

struct UnkObject;
void fn_7101152c0c(UnkObject* self, void* effect, u32 on, u32 restart);
struct UnkFan_A4_b74;
void fn_7100a474f8(UnkFan_A4_b74* self, f32 dt);
void fn_710087f138(UnkFan_A4_b74* self);
void fn_710087f22c(UnkFan_A4_b74* self);
void fn_710087e378(UnkFan_A4_b74* self);
void fn_710087e648(UnkFan_A4_b74* self, f32 dt);
void fn_710087ea38(UnkFan_A4_b74* self);
void fn_710087eba4(UnkFan_A4_b74* self, f32 dt);

struct UnkFan_A4_b74 {
    u8 pad_0x0[0x30];
    char* owner;
    u8 pad_0x38[0xae0 - 0x38];
    UnkObject* object;  // 0xae0
    void* effect_a;     // 0xae8
    void* effect_b;     // 0xaf0
};

void fn_710087e2b4(UnkFan_A4_b74* self, f32 dt) {
    fn_7100a474f8(self, dt);
    if (!self->effect_a || !self->effect_b) {
        self->object = self->owner ? *reinterpret_cast<UnkObject**>(self->owner + 0x280) : nullptr;
        if (self->object) {
            fn_710087f138(self);
            fn_710087f22c(self);
        }
    }
    if (self->effect_b)
        fn_7101152c0c(self->object, self->effect_b, 1, 1);
    if (self->effect_a)
        fn_7101152c0c(self->object, self->effect_a, 1, 1);
    fn_710087e378(self);
    fn_710087e648(self, dt);
    fn_710087ea38(self);
    fn_710087eba4(self, dt);
}
