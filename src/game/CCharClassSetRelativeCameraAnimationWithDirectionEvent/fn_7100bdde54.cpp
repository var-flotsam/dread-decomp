// CCharClassSetRelativeCameraAnimationWithDirectionEvent's unit (tools/tu_map.py unit 1859).
// fn_7100bdde54: the animation to play for the actor's facing: the first set direction path (right,
// left, up, down) the actor's forward axis points along, else the base event's. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkDirectionEventActor_lbdde54 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkDirectionEventTarget_lbdde54 {
    UnkDirectionEventActor_lbdde54* actor;
};

struct UnkSetRelativeCameraAnimationWithDirectionEvent_lbdde54 {
    void* vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId path_0x10;  // the base event's animation
    u8 unk_0x18[0x58];
    CFilePathStrId sAnimationPathLookingRight;  // 0x70
    CFilePathStrId sAnimationPathLookingLeft;   // 0x78
    CFilePathStrId sAnimationPathLookingUp;     // 0x80
    CFilePathStrId sAnimationPathLookingDown;   // 0x88
};

// Whether the path is set and the forward axis lies within the cone around the direction.
static inline bool Looks(const CFilePathStrId& path, UnkVec3f forward, f32 dx, f32 dy, f32 dz) {
    return path.data != &g_unk_7101d47200->empty &&
           forward.x * dx + forward.y * dy + forward.z * dz >= 0.9f;
}

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
const CFilePathStrId* fn_7100bdde54(UnkSetRelativeCameraAnimationWithDirectionEvent_lbdde54* self,
                                    UnkDirectionEventTarget_lbdde54* target) {
    UnkMatrix44 matrix = target->actor->xform.GetMatrix();
    UnkVec3f forward(matrix.m[2], matrix.m[6], matrix.m[10]);
    if (Looks(self->sAnimationPathLookingRight, forward, 1.0f, 0.0f, 0.0f))
        return &self->sAnimationPathLookingRight;
    if (Looks(self->sAnimationPathLookingLeft, forward, -1.0f, 0.0f, 0.0f))
        return &self->sAnimationPathLookingLeft;
    if (Looks(self->sAnimationPathLookingUp, forward, 0.0f, 1.0f, 0.0f))
        return &self->sAnimationPathLookingUp;
    if (Looks(self->sAnimationPathLookingDown, forward, 0.0f, -1.0f, 0.0f))
        return &self->sAnimationPathLookingDown;
    return &self->path_0x10;
}
