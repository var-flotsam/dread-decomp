// The transform node of include/game/UnkXformNode.h, translation unit 282 of data/tu_map.csv
// (0x7100205bc4-0x710020694c, no class hint; its constructors and setters are the per-function
// sources in src/game_s1/). Holds fn_7100206078, which moves the node's target along a velocity.
// Names are placeholders inferred from the code.
#include "game/UnkXformNode.h"
#include "types.h"

// The node's virtual slots used here: its position (0x98) and an update taking a time step (0x20).
struct UnkXformNodeVtbl_7100206078 {
    u8 pad_0x0[0x20];
    void (*vf_0x20)(UnkXformNode* self, f32 dt);
    u8 pad_0x28[0x70];
    UnkVec3f (*vf_0x98)(UnkXformNode* self);
};

static inline UnkXformNodeVtbl_7100206078* UnkVtbl(UnkXformNode* self) {
    return *reinterpret_cast<UnkXformNodeVtbl_7100206078**>(self);
}

static inline UnkVec3f UnkScaled(const UnkVec3f& v, f32 s) {
    return UnkVec3f(v.x * s, v.y * s, v.z * s);
}

static inline UnkVec3f UnkSum(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

// The target's previous and next positions.
void fn_71001ba980(UnkXformTarget* target, const UnkVec3f* pos);
void fn_71001ba864(UnkXformTarget* target, const UnkVec3f* pos);

// Hands the target the node's position and `to`, marks the node and updates it twice with no time.
static inline void UnkMoveTo(UnkXformNode* self, const UnkVec3f& to) {
    UnkXformTarget* target = self->target_0x108;
    UnkVec3f from = UnkVtbl(self)->vf_0x98(self);
    fn_71001ba980(target, &from);
    fn_71001ba864(self->target_0x108, &to);
    self->flag_0xfb = 1;
    UnkVtbl(self)->vf_0x20(self, 0.0f);
    UnkVtbl(self)->vf_0x20(self, 0.0f);
}

// Moves the node's target from its position by ten times `velocity`.
// NON_MATCHING(minor): instruction scheduling and register allocation before the first virtual call
// (the original loads the velocity before the vtable and keeps 10.0 in s0).
void fn_7100206078(UnkXformNode* self, const UnkVec3f* velocity) {
    UnkMoveTo(self, UnkSum(UnkScaled(*velocity, 10.0f), UnkVtbl(self)->vf_0x98(self)));
}
