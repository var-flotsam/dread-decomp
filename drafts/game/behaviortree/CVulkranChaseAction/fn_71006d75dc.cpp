// behaviortree::CVulkranChaseAction's unit: runs the AI component's path side search (fn_710035e8d8) from
// the owner toward its target and, when it finds a point on both sides, returns it and whether it was
// blocked, and clears the component's pending flag. Names and layouts are placeholders.
#include <functional>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The follower at +0x68 of the component: it is ready when its count is above one, or one with
// its flag clear.
struct UnkAIPathFollower {
    u8 pad_0x0[0x8];
    u32 count;
    u8 pad_0xc[0x20];
    u8 flag;

    bool IsReady() const { return count != (u32)((count != 0) & !flag); }
};

// What a side search fills (0x14 bytes).
struct UnkAIPathSearchOut {
    u64 a;
    u64 b;
    f32 distance;

    UnkAIPathSearchOut() : a(0), b(0), distance(-1.0f) {}
};

// The side search from `from` toward `to` (fn_710035e8d8): `blocked` tells whether it stopped on
// a side, `point` where.
bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);

struct UnkVulkranChaseAI_l6d75dc {
    u8 pad_0x0[0x68];
    UnkAIPathFollower follower;
    u8 pad_0x98[0x14732 - 0x98];
    bool field_0x14732;
    bool pending;  // 0x14733
};
static_assert(__builtin_offsetof(UnkVulkranChaseAI_l6d75dc, field_0x14732) == 0x14732, "layout");

struct UnkVulkranChaseTarget_l6d75dc {
    u8 pad_0x0[0x58];
    CVector3D field_0x58;
};

struct UnkVulkranChaseAction_l6d75dc {
    u8 pad_0x0[0x18];
    UnkActor* actor;
    u8 pad_0x20[0x10];
    UnkVulkranChaseAI_l6d75dc* ai;
};

// Ready as IsReady, with the flag read first.
static inline bool Ready(const UnkAIPathFollower* f) { return f->count != (u32)(!f->flag & (f->count != 0)); }

static inline CVector3D Translation(const UnkActor* actor) {
    return CVector3D{actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

bool fn_71006d75dc(UnkVulkranChaseAction_l6d75dc* self, CVector3D* outPoint, bool* outSide) {
    if (!fn_7100d7c990(self->actor))
        return false;
    UnkVulkranChaseAI_l6d75dc* ai = self->ai;
    if (!ai || !self->actor->field_0x298 || !self->actor->field_0x218)
        return false;

    s32 found = 0;
    if (Ready(&ai->follower)) {
        UnkAIPathFollower* follower = &ai->follower;
        CVector2D from;
        fn_710007b430(&from, &static_cast<const CVector3D&>(Translation(self->actor)));
        CVector2D to;
        fn_710007b430(&to, &static_cast<const CVector3D&>(Translation(fn_7100d7c990(self->actor))));
        CVector2D dirA;
        fn_710007b430(&dirA, &static_cast<UnkVulkranChaseTarget_l6d75dc*>(self->actor->field_0x218)->field_0x58);
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
        CVector2D dirB;
        fn_710007b430(&dirB, &static_cast<const CVector3D&>(CVector3D{m.m[2], m.m[6], m.m[10]}));

        CVector2D point;
        point.x = point.y = 0.0f;
        bool side = false;
        bool blocked = false;
        s32 indexA = -1;
        s32 indexB = -1;
        f32 distance = -1.0f;
        UnkAIPathSearchOut out;
        // The shared declaration types the side flag and the last three outputs otherwise.
        bool ok = fn_710035e8d8(follower, &from, &to, &dirA, &dirB, self->ai->field_0x14732, &point,
                                reinterpret_cast<u32*>(&side), 0.0f, &blocked, reinterpret_cast<f32*>(&indexA),
                                reinterpret_cast<f32*>(&indexB), reinterpret_cast<u32*>(&distance),
                                &out, std::function<void()>());
        if (ok && indexA != -1 && indexB != -1) {
            outPoint->x = point.x;
            outPoint->y = point.y;
            outPoint->z = 0.0f;
            *outSide = side;
            self->ai->pending = false;
            found = 1;
        }
    }
    return found;
}
