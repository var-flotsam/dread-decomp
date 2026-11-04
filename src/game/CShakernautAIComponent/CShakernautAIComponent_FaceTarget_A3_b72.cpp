// CShakernautAIComponent unit (tools/tu_map.py, the name from reflection): fn_71006b113c runs the base update
// (fn_7100369d80), then turns the 2D direction at +0x14810 around when it points away from the target
// (fn_7100d7c990): the dot product with the normalised (1 / (length + 1e-4)) offset from the owner to the
// target is negative. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkActor.h"
#include "types.h"

// Declared as src/game/CChozoWarriorAIComponent/CChozoWarriorAIComponent_Send_b58.cpp does.
struct UnkWarriorSend_b58;
void fn_7100369d80(UnkWarriorSend_b58*, void*);

struct UnkShakernautAI_A3_b72 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14810 - 0x38];
    f32 dir_x;  // 0x14810
    f32 dir_y;
};

void fn_71006b113c(UnkShakernautAI_A3_b72* self, void* arg) {
    fn_7100369d80(reinterpret_cast<UnkWarriorSend_b58*>(self), arg);
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return;
    UnkActor* owner = self->owner;
    f32 tx = target->field_0x44, ty = target->field_0x54, tz = target->field_0x64;
    f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
    f32 dx = tx - ox;
    f32 dy = ty - oy;
    f32 dz = tz - oz;
    f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    f32 nx = dx * inv;
    f32 ny = dy * inv;
    f32 nz = dz * inv;
    if (self->dir_x * nx + self->dir_y * ny + nz * 0.0f < 0.0f) {
        self->dir_x = -self->dir_x;
        self->dir_y = -self->dir_y;
    }
}
