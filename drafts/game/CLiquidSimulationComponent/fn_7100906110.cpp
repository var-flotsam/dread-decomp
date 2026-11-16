// CLiquidSimulationComponent's unit (tools/tu_map.py unit 1395): one step of the liquid surface's
// spring simulation: each column's offset and speed are integrated, then each column pulls on the
// speeds of its two neighbours. Names and layouts are placeholders.
#include "types.h"

struct CLiquidSimulationComponent;  // src/game/CLiquidSimulationComponent.cpp

// The columns are written through a may-alias type, as the unit's other functions see them.
typedef f32 __attribute__((may_alias)) UnkLiquidValue_l906110;

// The component as the simulation step sees it.
struct UnkLiquidSim_l906110 {
    u8 unk_0x0[0x44];
    f32 stiffness;  // 0x44
    f32 damping;    // 0x48
    f32 spread;     // 0x4c
    u8 unk_0x50[4];
    UnkLiquidValue_l906110 columns[2][0x200];  // 0x54: each column's offset, then its speed
    u32 columnCount;                           // 0x1054

    UnkLiquidValue_l906110& Offset(u32 i) { return columns[0][i]; }
    UnkLiquidValue_l906110& Speed(u32 i) { return columns[1][i]; }
};
static_assert(__builtin_offsetof(UnkLiquidSim_l906110, columns) == 0x54, "layout");
static_assert(__builtin_offsetof(UnkLiquidSim_l906110, columnCount) == 0x1054, "layout");

void fn_7100906110(CLiquidSimulationComponent* component, f32 dt) {
    UnkLiquidSim_l906110* self = reinterpret_cast<UnkLiquidSim_l906110*>(component);
    for (u32 i = 0; i < self->columnCount; ++i) {
        f32 offset = self->Offset(i);
        f32 speed = self->Speed(i);
        f32 accel = -offset * self->stiffness - speed * self->damping;
        self->Offset(i) = offset + speed * dt * 62.5f;
        self->Speed(i) = speed + accel * dt * 62.5f;
    }
    for (u32 i = 0; i < self->columnCount; ++i) {
        if (i > 0)
            self->Speed(i - 1) += self->spread * (self->Offset(i) - self->Offset(i - 1));
        if (i < self->columnCount - 1)
            self->Speed(i + 1) += self->spread * (self->Offset(i) - self->Offset(i + 1));
    }
}
