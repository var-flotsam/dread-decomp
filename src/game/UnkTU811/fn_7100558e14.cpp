#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkEmmyRelaxAI_b59;
struct UnkPlayer;

struct UnkTargetAI_b0193 {
    u8 unk_0x0[0x30];
    UnkActor* field_0x30;
};

struct UnkPoint3_b0193 {
    f32 x, y, z;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers.
void fn_7100558e14(UnkEmmyRelaxAI_b59* self, UnkTargetAI_b0193* other) {
    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    UnkActor* actor = other->field_0x30;
    f32 dx = player->field_0x44 - actor->field_0x44;
    f32 dy = player->field_0x54 - actor->field_0x54;
    f32 dz = player->field_0x64 - actor->field_0x64;
    f32 along = dx * 1.0f + dy * 0.0f + dz * 0.0f;
    UnkPoint3_b0193 point;
    f32 offset = along > 0.0f ? -200.0f : 200.0f;
    f32 px = actor->field_0x44;
    px += offset;
    point.x = px;
    point.y = actor->field_0x54 + 150.0f;
    point.z = actor->field_0x64 + 0.0f;
    fn_7100546a30(self, 1, player, &point);
    fn_71005479a0(self);
}
