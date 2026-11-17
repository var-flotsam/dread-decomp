// CMorphBallMovement's unit: one movement step of the ball through the collider. Builds the query
// (the shape, the layers, the three callbacks and the up direction for the current mode, negated),
// the probe (time step, start, angles and the ball's vectors) and runs it; the out transform takes
// the position the probe ends at and the ball its new vectors, then the hits are handed on.
// Names and layouts are placeholders.
#include "game/CCharacterMovement.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkGrabQuery_l13cf3c;
struct UnkGrabProbe_l13cf3c;
struct UnkGrabHit_l13cf3c;
struct UnkMorphBallMovement_l949b7c;

extern f32 g_unk_710164baa0;

void fn_7100d4c198(void* collider, UnkGrabQuery_l13cf3c* query, const UnkGrabProbe_l13cf3c* probe,
                   UnkGrabHit_l13cf3c* hit, void* extra);
UnkVec3f fn_7100949b7c(UnkMorphBallMovement_l949b7c* self);

static inline void ReleaseRef_9db0(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

struct UnkMorphBallCallback_9db0 {
    void* owner;
    void (*fn)();
};

struct UnkMorphBallMovement_9db0 {
    void** vtable;
    u8 pad_0x8[0x1d8 - 0x8];
    UnkVec3f velocity;     // 0x1d8
    UnkVec3f field_0x1e4;  // 0x1e4
    u8 pad_0x1f0[0x208 - 0x1f0];
    UnkVec3f field_0x208;  // 0x208
    u8 pad_0x214[0x3b0 - 0x214];
    u8 collider[0x18];  // 0x3b0
    u32 layer;          // 0x3c8
    u32 field_0x3cc;
    u32 field_0x3d0;
    u8 pad_0x3d4[4];
    UnkMorphBallCallback_9db0 filter;  // 0x3d8
    UnkMorphBallCallback_9db0 hit;     // 0x3e8
    u8 pad_0x3f8[0x2288 - 0x3f8];
    s32 mode;  // 0x2288
    u8 pad_0x228c[0x248c - 0x228c];
    UnkVec3f field_0x248c;
    u8 pad_0x2498[0x25d0 - 0x2498];
    UnkMorphBallCallback_9db0 field_0x25d0;

    f32 vf_0x290() { return reinterpret_cast<f32 (*)(UnkMorphBallMovement_9db0*)>(vtable[0x290 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkMorphBallMovement_9db0, field_0x25d0) == 0x25d0, "layout");

bool fn_71010d32b8(UnkMorphBallMovement_9db0* self);
struct UnkMorphBallHitList_9db0;
void fn_7100979514(UnkMorphBallMovement_9db0* self, UnkMorphBallHitList_9db0* hits);

// The step's transforms: the one it starts from (+0x4) and the one it writes (+0xb8).
struct UnkMorphBallStep_9db0 {
    u32 field_0x0;
    UnkXform from;  // 0x4
    UnkVec3f field_0x60;
    UnkVec3f field_0x6c;
    u8 pad_0x78[0xb8 - 0x78];
    UnkXform to;  // 0xb8
};
static_assert(__builtin_offsetof(UnkMorphBallStep_9db0, to) == 0xb8, "layout");

// What the collider is asked to do.
struct UnkMorphBallQuery_9db0 {
    UnkMorphBallQuery_9db0() {
        kind = 3;
        field_0x88 = false;
    }

    u32 kind;
    bool field_0x4;
    u32 field_0x8 = 1;
    u32 field_0xc;
    UnkMorphBallCallback_9db0 filter = {};  // 0x10
    UnkMorphBallCallback_9db0 hit = {};     // 0x20
    UnkMorphBallCallback_9db0 field_0x30 = {};
    u64 field_0x40 = 0;
    u64 field_0x48 = 0;
    UnkShape* shape = nullptr;  // 0x50
    u32 field_0x58 = 0;
    u8 field_0x5c[0x10];
    bool field_0x6c = true;
    u32 layers[3] = {};  // 0x70
    UnkVec3f down = UnkVec3f(0.0f, -1.0f, 0.0f);  // 0x7c
    bool field_0x88;
};
static_assert(__builtin_offsetof(UnkMorphBallQuery_9db0, field_0x88) == 0x88, "");

// The path: the time step, where it starts, the angles and the ball's vectors.
struct UnkMorphBallProbe_9db0 {
    f32 dt;
    UnkVec3f start;        // 0x4
    UnkVec3f angle = UnkVec3f(0.0f, 0.0f, 0.0f);       // 0x10
    UnkVec3f field_0x1c = UnkVec3f(0.0f, 0.0f, 0.0f);  // 0x1c
    UnkVec3f velocity = UnkVec3f(0.0f, 0.0f, 0.0f);    // 0x28
    UnkVec3f field_0x34 = UnkVec3f(0.0f, 0.0f, 0.0f);  // 0x34
    UnkVec3f field_0x40 = UnkVec3f(0.0f, 0.0f, 0.0f);  // 0x40
    UnkVec3f field_0x4c = UnkVec3f(0.0f, 0.0f, 0.0f);  // 0x4c
    f32 field_0x58 = 0.0f;
    f32 field_0x5c;
};
static_assert(sizeof(UnkMorphBallProbe_9db0) == 0x60, "");

struct UnkMorphBallResult_9db0 {
    UnkVec3f position = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f velocity = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f field_0x18 = UnkVec3f(0.0f, 0.0f, 0.0f);
};
static_assert(sizeof(UnkMorphBallResult_9db0) == 0x24, "");

#pragma pack(push, 4)
struct UnkMorphBallHit_9db0 {
    UnkRefObject* first;
    UnkRefObject* second;
    u8 pad_0x10[0x38];
};

struct UnkMorphBallHitList_9db0 {
    u32 count = 0;
    UnkMorphBallHit_9db0 items[32];  // 0x4

    ~UnkMorphBallHitList_9db0() {
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_9db0(items[i].second);
            ReleaseRef_9db0(items[i].first);
        }
    }
};

struct UnkMorphBallRef_9db0 {
    UnkRefObject* p = nullptr;
    ~UnkMorphBallRef_9db0() { ReleaseRef_9db0(p); }
};

// What the probe hit.
struct UnkMorphBallHits_9db0 {
    bool field_0x0 = false;
    u8 pad_0x1[7];
    u64 field_0x8[10] = {};
    UnkMorphBallHitList_9db0 list;  // 0x58
    u32 field_0x95c = 0;
    UnkMorphBallRef_9db0 field_0x960;
    UnkMorphBallRef_9db0 field_0x968;
    s32 field_0x970 = -1;
    UnkVec3f field_0x974 = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f field_0x980 = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f field_0x98c = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f field_0x998 = UnkVec3f(0.0f, 0.0f, 0.0f);
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkMorphBallHits_9db0, field_0x95c) == 0x95c, "");
static_assert(sizeof(UnkMorphBallHits_9db0) == 0x9a4, "");

void fn_7100949db0(UnkMorphBallMovement_9db0* self, UnkMorphBallStep_9db0* step, f32 dt) {
    u32 mask = fn_71010d32b8(self) ? self->field_0x3d0 : self->field_0x3cc;
    UnkMorphBallQuery_9db0 query;
    query.shape = fn_7100976588(reinterpret_cast<UnkFlagged*>(self));
    query.layers[0] = mask;
    query.layers[1] = mask;
    query.layers[2] = self->layer;
    query.field_0x4 = false;
    query.field_0xc = 2;
    query.hit = self->hit;
    query.field_0x30 = self->field_0x25d0;
    query.filter = self->filter;

    UnkVec3f up;
    switch (self->mode) {
    case 2:
    case 4:
    case 5:
        up = fn_7100949b7c(reinterpret_cast<UnkMorphBallMovement_l949b7c*>(self));
        break;
    case 3:
        up = -self->field_0x248c;
        break;
    default:
        up = UnkVec3f(0.0f, 1.0f, 0.0f);
        break;
    }
    query.down = -up;

    UnkMorphBallProbe_9db0 probe;
    probe.field_0x58 = -0.3f;
    probe.field_0x5c = g_unk_710164baa0;
    probe.dt = dt;
    probe.start = step->from.GetTranslation();
    UnkVec3f angle = step->from.GetAngle();
    probe.angle = UnkVec3f(angle.x, angle.y, 0.0f);
    probe.field_0x34 = self->field_0x208;
    probe.velocity = self->velocity;
    probe.field_0x1c = self->field_0x1e4;
    probe.field_0x40 = step->field_0x60;
    probe.field_0x4c = step->field_0x6c;
    probe.field_0x58 = self->vf_0x290();

    UnkMorphBallResult_9db0 result;
    UnkMorphBallHits_9db0 hits;
    fn_7100d4c198(self->collider, reinterpret_cast<UnkGrabQuery_l13cf3c*>(&query),
                  reinterpret_cast<const UnkGrabProbe_l13cf3c*>(&probe), reinterpret_cast<UnkGrabHit_l13cf3c*>(&result),
                  &hits);
    step->to.SetTranslation(result.position);
    self->velocity = result.velocity;
    self->field_0x208 = result.field_0x18;
    fn_7100979514(self, &hits.list);
}
