// CAnimationNavMeshItemStage unit: fn_71009b8910 is where the stage's animation ends up: the motion of
// the item's animation (or the default one) over its length, moved by the owner's transform and
// projected to 2D; the zero point when there is none. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CAnimationComponent.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::math::CVector2D;
using base::math::CVector3D;

extern const CVector2D g_unk_7101647780;

struct UnkNavMeshAnimOwner_ov110;
struct UnkNavMeshAnimDefault_ov110;

struct UnkAnimationNavMeshItem_ov110 {
    u8 pad_0x0[0x30];
    UnkNavMeshAnimOwner_ov110* owner;  // 0x30
    u8 pad_0x38[0x188];
    UnkNavMeshAnimDefault_ov110* fallback;  // 0x1c0
};

// The id the animation lookup returns: starts as the pool's empty entry (making the pool if there is
// none yet) and is released like a CStrId.
struct UnkAnimId_ov110 {
    UnkStringData* data;

    UnkAnimId_ov110() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkAnimId_ov110() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    // Takes a reference to `id`, releasing the one held (CStrId::ReleaseAndTake on a copy).
    void Assign(const CStrId& id) {
        CStrId copy(id);
        reinterpret_cast<CStrId*>(this)->ReleaseAndTake(copy);
    }
};

UnkAnimId_ov110 fn_71009b7c9c(UnkAnimationNavMeshItem_ov110* self);
UnkAnimationNavMeshItem_ov110* fn_71009b8c78(UnkActor* owner);

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

struct UnkFlags_l9b8910 {
    u32 bits;
    UnkFlags_l9b8910() : bits(0) {}
    UnkFlags_l9b8910(u32 b) : bits(b) {}
    UnkFlags_l9b8910(const UnkFlags_l9b8910& o) : bits(o.bits) {}
};

inline const UnkFlags_l9b8910& UnkFlagsNone_l9b8910() {
    static UnkFlags_l9b8910 s_none;
    return s_none;
}

inline const UnkFlags_l9b8910& UnkFlagsAll_l9b8910() {
    static UnkFlags_l9b8910 s_all(~UnkFlagsNone_l9b8910().bits & 7);
    return s_all;
}

// The motion fn_71007142e8 sets up: its matrix first, and whether it moved at +0x5c.
struct UnkNavMeshMotion_l9b8910 {
    UnkMatrix44 matrix;
    u8 pad_0x40[0x1c];
    u32 moved;  // 0x5c
    u8 pad_0x60[0x10];
};

// A transform's rotation rows with no translation part, then its translation row.
struct UnkNavMeshMatrix_l9b8910 {
    f32 m[16];

    explicit UnkNavMeshMatrix_l9b8910(const UnkMatrix44& src) {
        m[0] = src.m[0];
        m[1] = src.m[1];
        m[2] = src.m[2];
        m[3] = 0.0f;
        m[4] = src.m[4];
        m[5] = src.m[5];
        m[6] = src.m[6];
        m[7] = 0.0f;
        m[8] = src.m[8];
        m[9] = src.m[9];
        m[10] = src.m[10];
        m[11] = 0.0f;
        m[12] = src.m[12];
        m[13] = src.m[13];
        m[14] = src.m[14];
        m[15] = src.m[15];
    }
};

// A 3D point's x and y (fn_710007b430).
struct UnkNavMeshPoint_l9b8910 : CVector2D {
    UnkNavMeshPoint_l9b8910() {}
    UnkNavMeshPoint_l9b8910(const UnkNavMeshPoint_l9b8910& o) {
        x = o.x;
        y = o.y;
    }
    void Set(const CVector3D& v) { fn_710007b430(this, &v); }
    void Set(const CVector2D& o) {
        x = o.x;
        y = o.y;
    }
};

struct UnkNavMeshStageObject_l9b8910 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

struct UnkNavMeshItemStage_l9b8910 {
    u8 pad_0x0[0x8];
    UnkNavMeshStageObject_l9b8910* object;  // 0x8
};

UnkNavMeshPoint_l9b8910 fn_71009b8910(UnkNavMeshItemStage_l9b8910* self) {
    CVector3D pos = g_unk_710164779c;
    UnkNavMeshPoint_l9b8910 result;
    bool found = false;
    if (self->object && self->object->owner) {
        CStrId name(g_unk_7101d470e0);
        if (UnkAnimationNavMeshItem_ov110* item = fn_71009b8c78(self->object->owner)) {
            UnkAnimId_ov110 id = fn_71009b7c9c(item);
            name.ReleaseAndTake(reinterpret_cast<CStrId&>(id));
        }
        if (UnkWeakTarget* target = fn_71007e4924(self->object->owner)) {
            u64 key = name.data->Key();
            UnkTimeline* timeline = static_cast<UnkTimeline*>(
                fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(target), reinterpret_cast<void*>(key)));
            if (timeline) {
                UnkAnimationComponent* animation = reinterpret_cast<UnkAnimationComponent*>(target);
                UnkNavMeshMotion_l9b8910 motion;
                fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(&motion));
                {
                // Up to how far the animation has played, or over its whole length.
                bool play = true;
                f32 end;
                UnkFlags_l9b8910 flags;
                if (fn_710117ae38(animation, key, false)) {
                    end = fn_7101177ea0(animation, 0xf4061ff25a2163d6ull);
                    play = end != 0.0f;
                    flags = UnkFlags_l9b8910(2);
                } else {
                    flags = UnkFlagsAll_l9b8910();
                    end = timeline->frame_count;
                }
                if (play)
                    fn_7100b8b3bc(timeline, &motion, timeline->tracks, &flags.bits, 0.0f, end);
                }
                UnkNavMeshMatrix_l9b8910 world(
                    fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->object->owner->field_0x38)));
                if (motion.moved) {
                    const UnkMatrix44& m = fn_710002614c(&motion.matrix);
                    pos.x = m.m[3];
                    pos.y = m.m[7];
                    pos.z = m.m[11];
                    UnkVec3f moved = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&world),
                                                   reinterpret_cast<const UnkVec3f*>(&pos));
                    pos.x = moved.x;
                    pos.y = moved.y;
                    pos.z = moved.z;
                    result.Set(pos);
                    found = true;
                }
            }
        }
    }
    if (!found)
        result.Set(g_unk_7101647780);
    return result;
}
