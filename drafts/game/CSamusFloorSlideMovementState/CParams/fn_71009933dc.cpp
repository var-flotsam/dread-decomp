// CSamusFloorSlideMovementState (unit 1459): looks under Samus's front for the floor she slides on.
// A slope of about 45 or 26 degrees (or the flat floor ahead missing) picks the floor-slide track
// to play on the owner's animation and the state (+0x198) to go to. Names and layouts are placeholders.
#include <math.h>
#include "../../../game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2689.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_71010d15b4(UnkGroundPlayer_b52*);

extern UnkDesc_SpitSend g_unk_7172769764;

// A reference to a counted object, released when the count drops to zero.
struct UnkSlideHitRef_l9933dc {
    base::UnkRefCounted* ptr;

    UnkSlideHitRef_l9933dc() : ptr(nullptr) {}
    ~UnkSlideHitRef_l9933dc() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkSlideHitVec_l9933dc {
    f32 x, y, z;

    UnkSlideHitVec_l9933dc(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit: the object hit and a second reference, an id, the point, the reversed normal
// and the normal, and flags.
struct UnkSlideHit_l9933dc {
    UnkSlideHitRef_l9933dc object;
    UnkSlideHitRef_l9933dc field_0x8;
    s32 id;
    UnkSlideHitVec_l9933dc point;
    UnkSlideHitVec_l9933dc field_0x20;
    UnkSlideHitVec_l9933dc normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkSlideHit_l9933dc()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkSlideHit_l9933dc) == 0x48, "layout");

// The segment query's callback object: the closest hit so far and the segment's origin.
struct UnkSlideHitQuery_l9933dc {
    void** vtable;
    UnkSlideHit_l9933dc hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSlideHitQuery_l9933dc(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSlideHitQuery_l9933dc() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSlideSegment_l9933dc {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSlideSegment_l9933dc(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_ovb0004 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_ovb0004 {
    s64 id;
    UnkDesc16_ovb0004 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_ovb0004(UnkDesc16_ovb0004 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_ovb0004()
        : UnkMoveTransition_ovb0004(*reinterpret_cast<const UnkDesc16_ovb0004*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_ovb0004) == 0x30, "layout");

// The track that fills in the start-of-slide request.
struct UnkSlideInitTrack_l9933dc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkMoveTransition_ovb0004* request);
};

// What fn_71010d7aac finds for the movement: a box at +0x40 and a flag at +0x60.
struct UnkSlideCache_l9933dc {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    f32 min_y;
    f32 max_x;  // 0x48
    f32 max_y;  // 0x4c
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60

    void vf_0x98() { reinterpret_cast<void (*)(UnkSlideCache_l9933dc*)>(vtable[0x98 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkSlideCache_l9933dc, dirty) == 0x60, "layout");

struct UnkSlideGroundSub_l9933dc {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkSlideGround_l9933dc {
    u8 pad_0x0[0x128];
    UnkSlideGroundSub_l9933dc* field_0x128;
    u8 pad_0x130[0x24];
    CVector3D up;  // 0x154
};
static_assert(__builtin_offsetof(UnkSlideGround_l9933dc, up) == 0x154, "layout");

struct UnkSlideOwner_l9933dc {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x180];
    UnkSlideGround_l9933dc* ground;  // 0x1f8
    UnkPlayerMovementCache* movement;  // 0x200
    UnkGroundPlayer_b52* player;  // 0x208
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;  // 0x220
};
static_assert(__builtin_offsetof(UnkSlideOwner_l9933dc, animation) == 0x220, "layout");

struct UnkSlideState_l9933dc {
    u8 pad_0x0[0x30];
    UnkSlideOwner_l9933dc* owner;  // 0x30
    UnkMove_b152* move;  // 0x38
    u8 pad_0x40[0x158];
    s32 state;  // 0x198
};
static_assert(__builtin_offsetof(UnkSlideState_l9933dc, state) == 0x198, "layout");

static inline s32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

static inline f32 Dot(const CVector3D& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// The angle between two unit vectors.
static inline f32 Angle(f32 cosine) {
    return acosf(cosine > -1.0f ? (!(cosine >= 1.0f) ? cosine : 1.0f) : -1.0f);
}

static inline bool Near(f32 v, f32 target, f32 tolerance) {
    return !(v + tolerance < target) && !(v - tolerance > target);
}

// Sends `value` to the animation under `hash`, with the descriptors and the type id mask.
static inline void Emit(UnkAnimationComponent* animation, u64 hash, u32 value, const UnkDesc_SpitSend* b) {
    base::reflection::refl_EmitU32(animation, hash, 0xf4061ff25a2163d6, value, &g_unk_710164b958, b);
}

void fn_71009933dc(UnkSlideState_l9933dc* self) {
    if (self->state == 3)
        return;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return;
    UnkSlideGround_l9933dc* ground = self->owner->ground;
    if (!ground || !ground->field_0x128 || !ground->field_0x128->field_0x10)
        return;
    if (!self->owner->movement)
        return;
    UnkSlideCache_l9933dc* cache = static_cast<UnkSlideCache_l9933dc*>(fn_71010d7aac(self->owner->movement));
    if (!cache)
        return;
    UnkGroundPlayer_b52* player = self->owner->player;
    if (!player)
        return;
    if (cache->dirty)
        cache->vf_0x98();

    s32 facing = Sign(fn_710002614c(&self->owner->transform).m[2]);
    f32 y = self->owner->transform.m[7];
    CVector2D start = {self->owner->transform.m[3] + (facing < 0 ? -130.0f : 130.0f), y};
    CVector2D below = {start.x, start.y + -300.0f};
    UnkSlideSegment_l9933dc segment(start, below);
    UnkGameManager* gm = g_unk_7101d47310;
    UnkTree* tree = gm->field_0x608 ? static_cast<UnkTree*>(gm->field_0x608->field_0x820) : nullptr;
    UnkSlideHitQuery_l9933dc query(CVector3D{start.x, start.y, 0.0f});

    if (fn_7100d5c518(tree, 0x409, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        u64 out = 0;
        if (fn_71010d9638(self->move, reinterpret_cast<UnkHitInfo_b152*>(&query.hit), &out))
            return;
        CVector3D normal = {query.hit.normal.x, query.hit.normal.y, query.hit.normal.z};
        f32 slope = Angle(Dot(ground->up, normal));
        if (!(slope > 0.1f))
            return;
        f32 f = facing;
        CVector3D ahead = {f, 0.0f * f, 0.0f * f};
        if (!(Angle(Dot(normal, ahead)) < 1.6707964f))
            return;
        if (Near(slope, 0.78539819f, 0.1f)) {
            bool on_slope = fn_71010d1598(player);
            bool sliding = fn_710117b348(animation, fn_7100c1a968());
            if (on_slope) {
                if (!sliding)
                    return;
                if (self->state == 1) {
                    Emit(animation, 0xa2829c68ac092d1c, 1, &g_unk_7172769764);
                    return;
                }
                Emit(animation, 0xbb50d5bb5473c359, 0, &g_unk_7172769740);
            } else {
                if (sliding)
                    return;
                if (self->state == 1) {
                    UnkMoveTransition_ovb0004 request(*reinterpret_cast<const UnkDesc16_ovb0004*>(&g_unk_710164b958),
                                                      *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769764),
                                                      UnkAll_SpitSend().value);
                    request.id = 0x20ee4036297fa79b;
                    request.field_0x24 = 1;
                    UnkTrackQuery track = {fn_7100c57edc(), nullptr, -1.0f};
                    if (fn_710117b08c(animation, &track))
                        reinterpret_cast<UnkSlideInitTrack_l9933dc*>(track.result)->vf_0x18(&request);
                    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
                    return;
                }
                Emit(animation, 0x9e44db7c24e1f6cd, 0, &g_unk_7172769740);
            }
        } else {
            bool on_slope = fn_71010d15b4(player);
            bool sliding = fn_710117b348(animation, fn_7100c1a458());
            if (on_slope) {
                if (!sliding)
                    return;
                CVector3D side = {1.0f, 0.0f, 0.0f};
                if (!Near(Dot(normal, side), 0.0f, 0.1f))
                    return;
                if (self->state == 1) {
                    Emit(animation, 0xa2829c68ac092d1c, 1, &g_unk_7172769764);
                    return;
                }
                Emit(animation, 0x63a7d4cbea48447d, 0, &g_unk_7172769740);
            } else {
                if (sliding)
                    return;
                if (self->state == 1) {
                    Emit(animation, 0x135b897efe508968, 1, &g_unk_7172769764);
                    return;
                }
                Emit(animation, 0xb1411bd3eaea3152, 0, &g_unk_7172769740);
            }
        }
        self->state = 2;
        return;
    }

    // Nothing under the front: is there floor beyond the edge of the box?
    CVector2D edge = {facing > 0 ? cache->min_x : cache->max_x, cache->max_y};
    CVector2D beyond = {(facing > 0 ? 300.0f : -300.0f) + edge.x, edge.y + 0.0f};
    UnkSlideSegment_l9933dc ahead(edge, beyond);
    if (fn_7100d5c518(tree, 0x409, reinterpret_cast<const UnkBox*>(&ahead), &query, 2) &&
        !(query.hit.normal.y < 0.001f))
        return;
    if (fn_710117b348(animation, fn_7100d2fa08()))
        return;
    if (fn_710117b348(animation, fn_7100c1a968()))
        Emit(animation, 0xf85faf4733567310, 0, &g_unk_7172769740);
    else if (fn_710117b348(animation, fn_7100c1a458()))
        Emit(animation, 0x2e63b85dc34d3423, 0, &g_unk_7172769740);
    else
        return;
    self->state = 5;
}
