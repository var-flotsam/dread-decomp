// CYamplotXAIComponent's unit: whether the target (fn_7100d7c990 of the actor) has room above and below
// it: two segment queries (fn_7100d5c518, mask 0x80409) from its position, up and down by its radius
// (+0x37c) plus 30, must both report true. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The query's closest hit (0x48 bytes): two counted references, an id, three vectors and flags.
struct UnkHit_l6f29c4 {
    base::UnkRefPtrMember object;
    base::UnkRefPtrMember field_0x8;
    s32 id;
    CVector3D point;
    CVector3D field_0x20;
    CVector3D normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l6f29c4()
        : object{nullptr}, field_0x8{nullptr}, id(-1), point{0.0f, 0.0f, 0.0f}, field_0x20{0.0f, 0.0f, 0.0f},
          normal{0.0f, 0.0f, 0.0f}, field_0x38(0), flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkHit_l6f29c4) == 0x48, "UnkHit_l6f29c4 layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkSegmentQuery_l6f29c4 {
    void** vtable;
    UnkHit_l6f29c4 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l6f29c4(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l6f29c4() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l6f29c4 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l6f29c4(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkYamplotTarget_l6f29c4 {
    u8 pad_0x0[0x210];
    void* field_0x210;
};

struct UnkYamplotXAIComponent_l6f29c4 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

// Runs one segment query (mask 0x80409) and returns what it reports.
inline bool Probe_l6f29c4(UnkTree* tree, const UnkSegment_l6f29c4& segment, UnkSegmentQuery_l6f29c4& query) {
    return fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2);
}

// The point `length` along `dir` from `from`.
static inline CVector2D Toward_l6f29c4(const CVector2D& from, const CVector2D& dir, f32 length) {
    return CVector2D{from.x + length * dir.x, from.y + length * dir.y};
}

static inline CVector2D Position2D_l6f29c4(const UnkActor* actor) {
    return CVector2D{actor->field_0x44, actor->field_0x54};
}

static inline CVector3D Position_l6f29c4(const UnkActor* actor) {
    return CVector3D{actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

bool fn_71006f29c4(UnkYamplotXAIComponent_l6f29c4* self) {
    UnkActor* target = fn_7100d7c990(self->actor);
    if (!target || !reinterpret_cast<UnkYamplotTarget_l6f29c4*>(target)->field_0x210)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    const CVector2D up{0.0f, 1.0f};
    const CVector2D down{0.0f, -1.0f};

    UnkSegment_l6f29c4 above(Position2D_l6f29c4(target),
                             Toward_l6f29c4(Position2D_l6f29c4(target), up, target->field_0x37c + 30.0f));
    UnkSegmentQuery_l6f29c4 above_query(Position_l6f29c4(target));
    if (!Probe_l6f29c4(tree, above, above_query))
        return false;

    UnkSegment_l6f29c4 below(Position2D_l6f29c4(target),
                             Toward_l6f29c4(Position2D_l6f29c4(target), down, target->field_0x37c + 30.0f));
    UnkSegmentQuery_l6f29c4 below_query(Position_l6f29c4(target));
    if (!Probe_l6f29c4(tree, below, below_query))
        return false;
    return true;
}
