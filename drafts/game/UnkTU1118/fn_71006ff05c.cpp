// Unit 1118: picks the nearest item of the list on the side the owner faces (ahead of or behind it
// along x, by the facing angle's y), falling back to the other side. Names and layouts are placeholders.
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

// An item of the list: its position.
struct UnkPickItem_l6ff05c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3f* vf_0x18();
};

// A list-like object: ready, size and element accessors.
struct UnkPickList_l6ff05c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    virtual s32 vf_0x28();
    virtual UnkPickList_l6ff05c* vf_0x30(s32 index);

    UnkPickItem_l6ff05c* Item(s32 index) { return reinterpret_cast<UnkPickItem_l6ff05c*>(vf_0x30(index)); }
};

// A component of the owner that knows the direction it faces.
struct UnkFacingSource_l6ff05c {
    void** vtable;

    UnkVec3f vf_0x370() {
        return reinterpret_cast<UnkVec3f (*)(UnkFacingSource_l6ff05c*)>(vtable[0x370 / 8])(this);
    }
};

struct UnkPickOwner_l6ff05c {
    u8 pad_0x0[0x38];
    UnkXform xform;                       // 0x38
    u8 pad_0x94[0x184];
    UnkFacingSource_l6ff05c* facing;      // 0x218

};
static_assert(__builtin_offsetof(UnkPickOwner_l6ff05c, facing) == 0x218, "facing");

struct UnkPickState_l6ff05c {
    u8 pad_0x0[0x30];
    UnkPickOwner_l6ff05c* owner;          // 0x30
    u8 pad_0x38[0x100];
    UnkPickList_l6ff05c* list;            // 0x138
    u8 has_list;                          // 0x140
    u8 pad_0x141[0x3b];
    UnkVec3f dir;                         // 0x17c
    u8 has_dir;                           // 0x188
    u8 pad_0x189[0x12b07];
    UnkVec3f offset;                      // 0x12c90
};
static_assert(__builtin_offsetof(UnkPickState_l6ff05c, offset) == 0x12c90, "offset");

// fn_710007a7f0 takes its direction by reference (the original passes the caller's own vector).
static inline UnkVec3f DirectionToAngle(const UnkVec3f& dir) {
    return ((UnkVec3f(*)(const UnkVec3f&))fn_710007a7f0)(dir);
}

static const s32 kNoItem = -0x7fffffff;

s32 fn_71006ff05c(UnkPickState_l6ff05c* self, UnkPickList_l6ff05c* list, const UnkVec3f* angle) {
    if (!list && self->has_list)
        list = self->list;

    s32 ahead = kNoItem;
    s32 behind = kNoItem;
    f32 aheadDist = 3.4028235e38f;
    f32 behindDist = 3.4028235e38f;
    UnkPickList_l6ff05c* items = nullptr;
    s32 count = 0;
    if (list && list->vf_0x20() && list->vf_0x28()) {
        list->vf_0x28();
        items = list->vf_0x30(0);
        count = items->vf_0x28();
    }
    {
        for (s32 i = 0; i < count; i++) {
            const UnkVec3f* pos = items->Item(i)->vf_0x18();
            UnkXform& xform = self->owner->xform;
            f32 refX = xform.m[0][3] + self->offset.x;
            f32 refY = xform.m[1][3] + self->offset.y;
            f32 refZ = xform.m[2][3] + self->offset.z;
            f32 dx = pos->x - refX;
            f32 dy = pos->y - refY;
            f32 dz = pos->z - refZ;
            f32 dist = dx * dx + dy * dy + dz * dz;
            if (pos->x > refX) {
                if (dist < aheadDist) {
                    aheadDist = dist;
                    ahead = i;
                }
            } else {
                if (dist < behindDist) {
                    behindDist = dist;
                    behind = i;
                }
            }
        }
    }

    // The facing angle's y: the given angle's, else the angle of the state's direction or the owner's.
    f32 yaw;
    if (angle) {
        yaw = angle->y;
    } else {
        UnkVec3f dir(0.0f, 0.0f, 0.0f);
        UnkPickOwner_l6ff05c* owner;
        if (self->has_dir) {
            dir = self->dir;
            yaw = DirectionToAngle(dir).y;
        } else if ((owner = self->owner)->facing) {
            yaw = DirectionToAngle(owner->facing->vf_0x370()).y;
        } else {
            yaw = owner->xform.GetAngle().y;
        }
    }

    s32 result;
    if (yaw > 1.0f)
        result = ahead != kNoItem ? ahead : behind;
    else if (yaw < -1.0f)
        result = behind != kNoItem ? behind : ahead;
    else
        result = aheadDist < behindDist ? ahead : behind;
    return result != kNoItem ? result : 0;
}
