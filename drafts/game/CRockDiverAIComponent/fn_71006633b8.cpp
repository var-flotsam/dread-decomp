// CRockDiverAIComponent's unit (tools/tu_map.py): after the base update, resets the dive track and, when the
// linked actor has a path, aims at its last point: stores that point and the squared tunable radius, then
// the signed angle between the owner's forward axis and the way to the path's first point, mapped from
// [-45, 45] degrees to [0, 1]; both values go into the owner's property bag. Names are placeholders.
#include <math.h>

#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CGoToWallFronPathAction.h"
#include "types.h"

// The two value keys the track fills: the aim and the dive ratio.
extern const unk_prop_bag::CStrId g_unk_71726fd810[2] __attribute__((visibility("hidden")));

struct UnkRockDiverTunable_l6633b8 {
    u8 pad_0x0[0x50c];
    f32 radius;  // 0x50c
};

struct UnkRockDiverTrack_l6633b8 {
    UnkRockDiverTunable_l6633b8* tunable;
    UnkVec3f target;  // 0x8
    s32 state;        // 0x14
    f32 radius_sq;    // 0x18
    f32 ratio;        // 0x1c
    f32 aim;          // 0x20
};

struct UnkPathPoint_l6633b8;
struct UnkPathPointVtable_l6633b8 {
    void* slots_0x0[3];
    const UnkVec3f* (*GetPosition)(UnkPathPoint_l6633b8* self);  // 0x18
};
struct UnkPathPoint_l6633b8 {
    UnkPathPointVtable_l6633b8* vtable;
};

struct UnkPath_l6633b8;
struct UnkPathVtable_l6633b8 {
    void* slots_0x0[5];
    s32 (*GetCount)(UnkPath_l6633b8* self);                          // 0x28
    UnkPathPoint_l6633b8* (*GetPoint)(UnkPath_l6633b8* self, s32 i);  // 0x30
};
struct UnkPath_l6633b8 {
    UnkPathVtable_l6633b8* vtable;
};

struct UnkPathSet_l6633b8;
struct UnkPathSetVtable_l6633b8 {
    void* slots_0x0[6];
    UnkPath_l6633b8* (*GetPath)(UnkPathSet_l6633b8* self, s32 i);  // 0x30
};
struct UnkPathSet_l6633b8 {
    UnkPathSetVtable_l6633b8* vtable;
};

struct UnkPathHolder_l6633b8 {
    u8 pad_0x0[0x30];
    UnkPathSet_l6633b8 paths;  // 0x30
};

struct UnkPartLink_l6633b8 {
    u8 pad_0x0[0x10];
    UnkActor* object;
};

struct UnkRockDiverAI_l6633b8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x1350 - 0x38];
    u8 part_0x1350[0x146c8 - 0x1350];
    UnkRockDiverTrack_l6633b8 track;  // 0x146c8
};

static inline UnkActor* LinkedActor(void* part) {
    auto* link = reinterpret_cast<UnkPartLink_l6633b8*>(*fn_71002a76a4(static_cast<UnkBreakablePart*>(part)));
    return link ? link->object : nullptr;
}

void fn_71006633b8(UnkRockDiverAI_l6633b8* self) {
    UnkRockDiverTrack_l6633b8* track = &self->track;
    fn_7100369650(reinterpret_cast<char*>(self));
    track->ratio = 1.0f;
    track->aim = 0.5f;
    track->state = 0;

    if (UnkActor* linked = LinkedActor(self->part_0x1350)) {
        auto* holder = reinterpret_cast<UnkPathHolder_l6633b8*>(fn_7100382150(linked));
        UnkPathSet_l6633b8* paths = &holder->paths;
        UnkPath_l6633b8* path = paths->vtable->GetPath(paths, 0);
        s32 count = path->vtable->GetCount(path);
        UnkPathPoint_l6633b8* last = path->vtable->GetPoint(path, count - 1);
        track->target = *last->vtable->GetPosition(last);
        f32 radius = track->tunable->radius;
        track->radius_sq = radius * radius;

        UnkPathPoint_l6633b8* first = path->vtable->GetPoint(path, 0);
        const UnkVec3f* start = first->vtable->GetPosition(first);
        UnkActor* owner = self->owner;
        f32 sx = start->x;
        f32 sy = start->y;
        f32 sz = start->z;
        f32 ox = owner->field_0x44;
        f32 oy = owner->field_0x54;
        f32 oz = owner->field_0x64;
        f32 dx = sx - ox;
        f32 dy = sy - oy;
        f32 dz = sz - oz;
        f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
        f32 nx = dx * inv;
        f32 ny = dy * inv;
        f32 nz = dz * inv;

        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        f32 fx = m.m[2];
        f32 fy = m.m[6];
        f32 fz = m.m[10];
        f32 dot = fx * nx + ny * fy + nz * fz;
        f32 cosine = dot / (sqrtf(fx * fx + fy * fy + fz * fz) * sqrtf(nx * nx + ny * ny + nz * nz));
        f32 angle;
        if (cosine >= 1.0f)
            angle = 0.0f;
        else if (cosine <= -1.0f)
            angle = 180.0f;
        else
            angle = acosf(cosine) * 57.29578f;

        const UnkMatrix44& side = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        f32 across = nx * side.m[1] + ny * side.m[5] + nz * side.m[9];
        track->aim = ((across < 0.0f ? -angle : angle) + 45.0f) / 90.0f;
    }

    unk_prop_bag::fn_710033187c(
        reinterpret_cast<unk_prop_bag::UnkPropBag*>(
            static_cast<UnkAnimationComponent*>(self->owner->field_0x220)->field_0x80),
        g_unk_71726fd810[0], track->aim);
    unk_prop_bag::fn_710033187c(
        reinterpret_cast<unk_prop_bag::UnkPropBag*>(
            static_cast<UnkAnimationComponent*>(self->owner->field_0x220)->field_0x80),
        g_unk_71726fd810[1], track->ratio);
}
