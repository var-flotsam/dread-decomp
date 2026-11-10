// Unit 282 (tools/tu_map.py): a transform node's update: follows its target's position when the target asks
// for it, refreshes the sphere at +0x80 when dirty, then turns to face away from the target's reference point
// and moves the target to the node's position. Names and layouts are placeholders.
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The node's virtual slots used here: set the position (0x30) and the angles from a direction (0x38), the position (0x98).
struct UnkXformNodeVtbl2_ovo0303 {
    u8 pad_0x0[0x30];
    void (*vf_0x30)(UnkXformNode* self, const UnkVec3f* position);
    void (*vf_0x38)(UnkXformNode* self, const UnkVec3f* angles);
    u8 pad_0x40[0x58];
    UnkVec3f (*vf_0x98)(UnkXformNode* self);
};

// What the node's target holds: its position, a reference point and the flag that lets it drive the node.
struct UnkTargetData_ovo0303 {
    u8 pad_0x0[0xa8];
    UnkVec3f position_0xa8;
    u8 pad_0xb4[0x24];
    UnkVec3f reference_0xd8;
    u8 pad_0xe4[0x199];
    bool field_0x27d;
};

UnkVec3f fn_710007a7f0(UnkVec3f dir);
void fn_71001ba980(UnkXformTarget* target, const UnkVec3f* pos);
void fn_71001ba374(UnkXformTarget* target, f32 value);

static inline UnkXformNodeVtbl2_ovo0303* Vtbl2_ovo0303(UnkXformNode* self) {
    return *reinterpret_cast<UnkXformNodeVtbl2_ovo0303**>(self);
}

inline void FaceDirection_ovo0303(UnkXformNode* self, const UnkVec3f& dir) {
    UnkVec3f angles = ((UnkVec3f(*)(const UnkVec3f&))fn_710007a7f0)(dir);
    Vtbl2_ovo0303(self)->vf_0x38(self, &angles);
}

inline void MoveTarget_ovo0303(UnkXformNode* self) {
    UnkXformTarget* tgt = self->target_0x108;
    UnkVec3f there = Vtbl2_ovo0303(self)->vf_0x98(self);
    fn_71001ba980(tgt, &there);
}

// Brings the node's position and target up to date with whichever of the two changed; returns whether it is active.
bool fn_7100205e6c(UnkXformNode* self) {
    if (self->active_0xf9) {
        UnkTargetData_ovo0303* target = reinterpret_cast<UnkTargetData_ovo0303*>(self->target_0x108);
        if (target->field_0x27d) {
            const UnkVec3f* position = &target->position_0xa8;
            Vtbl2_ovo0303(self)->vf_0x30(self, position);
            self->flag_0xfb = 1;
        }
        if (self->dirty_0xfa) {
            self->dirty_0xfa = 0;
            UnkSphere* sphere = UnkXformNodeSphere(self);
            {
                UnkVec3f pos = Vtbl2_ovo0303(self)->vf_0x98(self);
                fn_7100216978(sphere, &pos);
            }
            self->flag_0xfb = 1;
        } else if (!self->flag_0xfb) {
            return self->active_0xf9;
        }
        UnkVec3f dir;
        UnkVec3f here = Vtbl2_ovo0303(self)->vf_0x98(self);
        UnkTargetData_ovo0303* t = reinterpret_cast<UnkTargetData_ovo0303*>(self->target_0x108);
        f32 dx = here.x - t->reference_0xd8.x;
        f32 dy = here.y - t->reference_0xd8.y;
        f32 dz = here.z - t->reference_0xd8.z;
        dir.x = dx;
        dir.y = dy;
        dir.z = dz;
        FaceDirection_ovo0303(self, dir);
        MoveTarget_ovo0303(self);
        fn_71001ba374(self->target_0x108, 0.0f);
        self->flag_0xfb = 0;
    }
    return self->active_0xf9;
}
