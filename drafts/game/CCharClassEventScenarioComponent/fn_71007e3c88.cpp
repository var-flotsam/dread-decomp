// CCharClassEventScenarioComponent's unit (tools/tu_map.py unit 1251): fn_71007e3c88, the per-frame update.
// Opens or closes the event when its conditions start or stop holding, runs the open and close timers (each
// fires fn_71007e3a8c when it runs out), then the base update. Names and layouts are placeholders.
#include "game/CActorComponent.h"
#include "types.h"

// As src/game/CCharClassEventScenarioComponent/CCharClassEventScenarioComponent_Scenario_ovb0097.cpp and
// src/game/CCharClassEventScenarioComponent/CEventScenarioComponent_b49.cpp declare them.
struct UnkEventScenario_ovb0097;
struct UnkEventScenario_b49a2;
void fn_71007e3a8c(UnkEventScenario_ovb0097* self);
void fn_71007e41b8(UnkEventScenario_b49a2* self);

struct UnkEventScenario_l7e3c88 {
    u8 pad_0x0[0x1d0];
    f32 open_time;      // 0x1d0
    f32 close_time;     // 0x1d4
    f32 open_elapsed;   // 0x1d8
    f32 close_elapsed;  // 0x1dc
    u8 pad_0x1e0[4];
    bool field_0x1e4;
    u8 pad_0x1e5[8];
    bool field_0x1ed;
    bool open;  // 0x1ee
};
static_assert(__builtin_offsetof(UnkEventScenario_l7e3c88, open) == 0x1ee, "layout");

bool fn_71007e3db8(UnkEventScenario_l7e3c88* self);
bool fn_71007e3f6c(UnkEventScenario_l7e3c88* self);
void fn_71007e35fc(UnkEventScenario_l7e3c88* self, f32 dt);

void fn_71007e3c88(UnkEventScenario_l7e3c88* self, f32 dt) {
    if (!fn_710024fbc8(self))
        return;
    bool active = fn_71007e3db8(self) || fn_71007e3f6c(self);
    bool open = self->open;
    if (active && !open) {
        fn_71007e41b8(reinterpret_cast<UnkEventScenario_b49a2*>(self));
    } else if (!active && open) {
        self->close_elapsed = 0.0f;
        self->open = false;
        if (!self->field_0x1e4 && self->close_time < 0.0f)
            self->field_0x1ed = false;
    }
    if (!self->field_0x1e4) {
        if (self->open_time > 0.0f && self->open && self->open_time > self->open_elapsed) {
            self->open_elapsed += dt;
            if (self->open_elapsed >= self->open_time)
                fn_71007e3a8c(reinterpret_cast<UnkEventScenario_ovb0097*>(self));
        }
        if (self->close_time > 0.0f && !self->open && self->close_time > self->close_elapsed) {
            self->close_elapsed += dt;
            if (self->close_elapsed >= self->close_time)
                fn_71007e3a8c(reinterpret_cast<UnkEventScenario_ovb0097*>(self));
        }
    }
    fn_71007e35fc(self, dt);
}
