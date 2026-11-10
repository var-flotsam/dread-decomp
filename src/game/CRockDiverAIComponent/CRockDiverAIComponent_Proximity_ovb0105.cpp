// CRockDiverAIComponent's unit (tools/tu_map.py): after the base update, while the state at +0x146dc is 1
// and the owner is within the tunable radius (+0x50c) of the point at +0x146d0, the squared distance over
// the scale at +0x146e0, clamped to [0, 1], goes to +0x146e4 and into the owner's property bag.
// Names are placeholders.
#include "game/UnkBehaviorTree.h"
#include "types.h"
#include "../../game_s7/UnkPropBagS7.h"

extern char g_unk_71726fd818[] __attribute__((visibility("hidden")));

struct UnkRockDiverTunable_b0105 {
    u8 pad_0x0[0x50c];
    f32 radius;  // 0x50c
};

struct UnkRockDiverOwner_b0105 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x220 - 0x68];
    char* bag;  // 0x220
};

struct UnkRockDiverTrack_b0105 {
    UnkRockDiverTunable_b0105* tunable;
    f32 x, y, z;
    s32 state;  // 0x14
    f32 scale;  // 0x18
    f32 ratio;  // 0x1c
};

struct UnkRockDiverAI_b0105 {
    u8 pad_0x0[0x30];
    UnkRockDiverOwner_b0105* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkRockDiverTrack_b0105 track;  // 0x146c8
};

void fn_710066369c(UnkRockDiverAI_b0105* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    if (self->track.state != 1)
        return;
    UnkRockDiverOwner_b0105* owner = self->owner;
    UnkRockDiverTrack_b0105* track = &self->track;
    f32 pos[3] = {owner->x, owner->y, owner->z};
    f32 dx = pos[0] - track->x;
    f32 dy = pos[1] - track->y;
    f32 xy = dx * dx + dy * dy;
    f32 dz = pos[2] - track->z;
    f32 dist2 = xy + dz * dz;
    f32 radius = track->tunable->radius;
    if (!(dist2 <= radius * radius))
        return;
    f32 t = dist2 / track->scale;
    f32 ratio;
    if (t >= 1.0f)
        ratio = 1.0f;
    else if (t < 0.0f)
        ratio = 0.0f;
    else
        ratio = t + (1.0f - t) * 0.0f;
    track->ratio = ratio;
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(owner->bag + 0x80),
                                *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_71726fd818), ratio);
}
