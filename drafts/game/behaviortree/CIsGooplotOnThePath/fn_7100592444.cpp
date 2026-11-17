// behaviortree::CIsGooplotOnThePath (tools/tu_map.py): whether another actor of the manager g_unk_71726af468
// (created on first use) is within the condition's radius of the owner, has a path crossing the owner's,
// and lies ahead of the owner along that path. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

#include <math.h>

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_71001263bc(char* p0, char* p1, f32 f0);

// The path of an AI component: its points, joined back to the first when `closed` is set.
struct UnkGooplotAI_l592444 {
    u8 pad_0x0[0x68];
    CVector3D* points;  // 0x68
    u32 point_count;  // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94: nonzero when the last point joins the first

    // An open path has one segment fewer than it has points.
    u32 SegmentCount() const { return point_count - ((point_count != 0) & (closed == 0)); }
    bool HasPath() const { return SegmentCount() != 0; }
    UnkSegment2D Path() const {
        const CVector3D& end = point_count == 1 ? points[0] : points[1];
        return UnkSegment2D(reinterpret_cast<const CVector2D&>(points[0]), reinterpret_cast<const CVector2D&>(end));
    }
};

struct UnkGooplotEntry_l592444 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkGooplotNode_l592444 {
    UnkGooplotEntry_l592444* entry;
    UnkGooplotNode_l592444* next;
};

// The manager's list of tracked actors.
struct UnkGooplotList_l592444 {
    u8 pad_0x0[0x1b8];
    u32 count;  // 0x1b8
    UnkGooplotNode_l592444* head;  // 0x1c0

    UnkGooplotEntry_l592444* At(u32 index) const {
        UnkGooplotNode_l592444* node = head;
        for (u32 i = 0; i < index; ++i)
            node = node->next;
        return node->entry;
    }
};

struct CIsGooplotOnThePath_l592444 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x18];
    bool reversed;  // 0x38
    f32 radius;  // 0x3c
};

static inline UnkGooplotAI_l592444* AI(UnkActor* actor) {
    return static_cast<UnkGooplotAI_l592444*>(actor->field_0x240);
}

static inline UnkManager_71726af468* Manager() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

static inline f32 DistanceSq(const UnkActor* a, const UnkActor* b) {
    f32 dx = a->field_0x44 - b->field_0x44;
    f32 dy = a->field_0x54 - b->field_0x54;
    f32 dz = a->field_0x64 - b->field_0x64;
    return dx * dx + dy * dy + dz * dz;
}

bool fn_7100592444(CIsGooplotOnThePath_l592444* self) {
    UnkGooplotAI_l592444* ai = AI(self->owner);
    if (!ai->HasPath())
        return false;
    UnkSegment2D path = ai->Path();
    f32 dir_x = path.end.x - path.start.x;
    f32 dir_y = path.end.y - path.start.y;
    f32 length = sqrtf(dir_x * dir_x + dir_y * dir_y);
    if (length != 0.0f) {
        dir_x /= length;
        dir_y /= length;
    }

    f32 radius = self->radius;
    UnkGooplotList_l592444* list = reinterpret_cast<UnkGooplotList_l592444*>(Manager());
    for (u32 i = 0; i < list->count; ++i) {
        UnkGooplotEntry_l592444* entry = list->At(i);
        UnkActor* other = entry ? entry->actor : nullptr;
        if ((DistanceSq(self->owner, other) < radius * radius) & (other != self->owner)) {
            UnkGooplotAI_l592444* other_ai = AI(other);
            if (!other_ai->HasPath())
                continue;
            UnkSegment2D other_path = other_ai->Path();
            if (!fn_71001263bc(reinterpret_cast<char*>(&other_path), reinterpret_cast<char*>(&path), 0.5f))
                continue;
            // Ahead of the owner along its path: against the path's direction unless `reversed` is set.
            f32 side_x = !self->reversed ? -dir_x : dir_x;
            f32 side_y = !self->reversed ? -dir_y : dir_y;
            f32 side_z = 0.0f;
            f32 to_x = other->field_0x44 - self->owner->field_0x44;
            f32 to_y = other->field_0x54 - self->owner->field_0x54;
            f32 to_z = other->field_0x64 - self->owner->field_0x64;
            if (side_x * to_x + side_y * to_y + side_z * to_z > 0.0f)
                return true;
        }
    }
    return false;
}
