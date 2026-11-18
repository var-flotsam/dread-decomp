// CEnemyMovement unit: moves the collider along this frame's displacement. Unless the player is dead or
// the animation ignores character-player collision, it first sweeps against the player's collision
// object; then, after the movement's own step (vf_0x398), it sweeps against the world with the
// character class's settings. Each sweep updates the target transform and the contact vectors.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharacterMovement.h"
#include "game/CLifeComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

extern f32 g_unk_710164baa0;

struct UnkSweepCounted_l0cb9c0 {
    virtual void vf_0x0();
    virtual ~UnkSweepCounted_l0cb9c0();
    s32 ref_count;
};

static inline void Release(UnkSweepCounted_l0cb9c0* p) {
    if (p && --p->ref_count == 0)
        delete p;
}

#pragma pack(push, 4)
// A hit of a sweep (0x48 bytes): the object hit and a second reference, an id, the point and more.
struct UnkSweepHit_l0cb9c0 {
    UnkSweepCounted_l0cb9c0* object;
    UnkSweepCounted_l0cb9c0* field_0x8;
    s32 id;
    f32 point[3];
    f32 field_0x20[3];
    f32 normal[3];
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;
    u32 field_0x44;

    UnkSweepHit_l0cb9c0()
        : object(nullptr), field_0x8(nullptr), id(-1), point(), field_0x20(), normal(), field_0x38(0), flags(0),
          field_0x40(0) {}
    ~UnkSweepHit_l0cb9c0() {
        Release(field_0x8);
        Release(object);
    }
};

// Up to 32 hits.
struct UnkSweepHits_l0cb9c0 {
    u32 count;
    u8 storage[32 * sizeof(UnkSweepHit_l0cb9c0)];

    UnkSweepHits_l0cb9c0() : count(0) {}
    UnkSweepHit_l0cb9c0* items() { return reinterpret_cast<UnkSweepHit_l0cb9c0*>(storage); }
    ~UnkSweepHits_l0cb9c0() {
        for (u64 i = 0; i < count; i++) {
            Release(items()[i].field_0x8);
            Release(items()[i].object);
        }
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkSweepHit_l0cb9c0) == 0x48, "hit");

// What the sweeps found: the hits and the closest one, and the velocity they were made with.
struct UnkSweepResult_l0cb9c0 {
    bool field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    CVector3D velocity;  // 0x14
    u32 field_0x20;
    u32 field_0x24;
    u64 field_0x28[6];
    UnkSweepHits_l0cb9c0 hits;  // 0x58
    u32 field_0x95c;
    UnkSweepHit_l0cb9c0 closest;  // 0x960

    UnkSweepResult_l0cb9c0()
        : field_0x0(false), field_0x8(0), field_0x10(0), velocity(), field_0x20(0), field_0x24(0), field_0x28(),
          field_0x95c(0) {}
};
static_assert(__builtin_offsetof(UnkSweepResult_l0cb9c0, closest) == 0x960, "result");

struct UnkSweepCallback_l0cb9c0 {
    void* owner;
    void (*fn)();
};

// Objects the sweep passes through.
struct UnkSweepIgnoreList_l0cb9c0 {
    u32 count = 0;
    void* items[2] __attribute__((packed, aligned(4)));

    void Add(void* item) {
        items[count] = item;
        count++;
    }
};

// What the collider is asked to do.
struct UnkSweepQuery_l0cb9c0 {
    explicit UnkSweepQuery_l0cb9c0(u32 k) {
        kind = k;
        down = UnkVec3f(0.0f, -1.0f, 0.0f);
    }

    u32 kind;
    bool field_0x4 = false;
    u32 field_0x8 = 1;
    u32 field_0xc = 0;
    UnkSweepCallback_l0cb9c0 filter = {};  // 0x10
    UnkSweepCallback_l0cb9c0 hit = {};     // 0x20
    u64 field_0x30 = 0;
    u64 field_0x38 = 0;
    u64 field_0x40 = 0;
    u64 field_0x48 = 0;
    UnkShape* shape = nullptr;  // 0x50
    UnkSweepIgnoreList_l0cb9c0 ignored;  // 0x58
    bool field_0x6c = true;
    u32 layers[3] = {};  // 0x70
    UnkVec3f down;  // 0x7c
    bool field_0x88 = true;
};
static_assert(__builtin_offsetof(UnkSweepQuery_l0cb9c0, field_0x88) == 0x88, "query");

// The sweep: a time, where it starts and its rotation, the contact vectors and the displacement.
struct UnkSweepProbe_l0cb9c0 {
    f32 time;
    UnkVec3f start;     // 0x4
    UnkVec3f angle;     // 0x10
    UnkVec3f field_0x1c;
    UnkVec3f field_0x28;
    UnkVec3f field_0x34;
    UnkVec3f move;      // 0x40
    UnkVec3f move_too;  // 0x4c
    f32 field_0x58;
    f32 field_0x5c;
};
static_assert(sizeof(UnkSweepProbe_l0cb9c0) == 0x60, "probe");

struct UnkSweepOut_l0cb9c0 {
    UnkVec3f position = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f field_0xc = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f field_0x18 = UnkVec3f(0.0f, 0.0f, 0.0f);
};

void fn_7100d4c198(void* collider, UnkSweepQuery_l0cb9c0* query, const UnkSweepProbe_l0cb9c0* probe,
                   UnkSweepOut_l0cb9c0* out, UnkSweepResult_l0cb9c0* result);

// The transforms the movement moves between.
struct UnkSweepTransforms_l0cb9c0 {
    u32 field_0x0;
    UnkXform current;  // 0x4
    u8 pad_0x60[0xc];
    CVector3D velocity;  // 0x6c
    u8 pad_0x78[0x40];
    UnkXform next;  // 0xb8
};
static_assert(__builtin_offsetof(UnkSweepTransforms_l0cb9c0, next) == 0xb8, "transforms");

struct UnkSweepClass_l0cb9c0 {
    u8 pad_0x0[0x1b9];
    bool field_0x1b9;
    u8 pad_0x1ba[2];
    u32 query_kind;    // 0x1bc
    bool field_0x1c0;
    bool field_0x1c1;
};

struct UnkSweepIgnore_l0cb9c0 {
    u8 pad_0x0[0x1b0];
    void* ignored;  // 0x1b0
};
struct UnkSweepLifeData_l0cb9c0 {
    u8 pad_0x0[0x10];
    UnkSweepIgnore_l0cb9c0* field_0x10;
};
struct UnkSweepLife_l0cb9c0 {
    u8 pad_0x0[0x48];
    UnkSweepLifeData_l0cb9c0* data;  // 0x48
};

struct UnkSweepOwner_l0cb9c0 {
    u8 pad_0x0[0x1b8];
    UnkSweepLife_l0cb9c0* life;  // 0x1b8
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x78];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkSweepPlayer_l0cb9c0 {
    u8 pad_0x0[0x1b8];
    void* life;  // 0x1b8
};

struct UnkEnemyMovement_l0cb9c0;
struct UnkEnemyMovementVtbl_l0cb9c0 {
    void* slots_0x0[0x1a8 / 8];
    UnkVec3f (*Down)(UnkEnemyMovement_l0cb9c0* self);  // 0x1a8
    void* slots_0x1b0[(0x290 - 0x1b0) / 8];
    f32 (*SweepMargin)(UnkEnemyMovement_l0cb9c0* self);  // 0x290
    void* slots_0x298[(0x398 - 0x298) / 8];
    void (*Step)(UnkEnemyMovement_l0cb9c0* self, UnkSweepTransforms_l0cb9c0* xf, f32 dt,
                 UnkSweepResult_l0cb9c0* result);  // 0x398
};

struct UnkEnemyMovement_l0cb9c0 {
    UnkEnemyMovementVtbl_l0cb9c0* vtable;
    u8 pad_0x8[0x28];
    UnkSweepOwner_l0cb9c0* owner;  // 0x30
    u8 pad_0x38[0x1d8 - 0x38];
    UnkVec3f field_0x1d8;
    UnkVec3f field_0x1e4;
    u8 pad_0x1f0[0x18];
    UnkVec3f field_0x208;
    u8 pad_0x214[0x3b0 - 0x214];
    u8 collider[0x18];  // 0x3b0
    u32 layer;          // 0x3c8
    u8 pad_0x3cc[0xc];
    UnkSweepCallback_l0cb9c0 filter;  // 0x3d8
    UnkSweepCallback_l0cb9c0 hit;     // 0x3e8
    u8 pad_0x3f8[0x1be0 - 0x3f8];
    bool field_0x1be0;
    u8 pad_0x1be1[0x1c18 - 0x1be1];
    UnkSweepClass_l0cb9c0* char_class;  // 0x1c18
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0cb9c0, char_class) == 0x1c18, "movement");

static inline CVector3D Scaled(f32 s, const CVector3D& v) {
    CVector3D r = {s * v.x, s * v.y, s * v.z};
    return r;
}

static inline void Fill(UnkSweepProbe_l0cb9c0* probe, UnkEnemyMovement_l0cb9c0* self, UnkSweepTransforms_l0cb9c0* xf,
                        const UnkSweepResult_l0cb9c0& result, f32 dt) {
    probe->time = dt;
    probe->start = xf->current.GetTranslation();
    probe->angle = xf->current.GetAngle();
    probe->field_0x34 = self->field_0x208;
    probe->field_0x28 = self->field_0x1d8;
    probe->field_0x1c = self->field_0x1e4;
    UnkVec3f move(result.velocity.x * dt, result.velocity.y * dt, result.velocity.z * dt);
    probe->move = move;
    probe->move_too = move;
}

static inline void Apply(UnkEnemyMovement_l0cb9c0* self, UnkSweepTransforms_l0cb9c0* xf, const UnkSweepOut_l0cb9c0& out) {
    xf->next.SetTranslation(out.position);
    self->field_0x1d8 = out.field_0xc;
    self->field_0x208 = out.field_0x18;
}

void fn_71010cb9c0(UnkEnemyMovement_l0cb9c0* self, UnkSweepTransforms_l0cb9c0* xf, f32 dt) {
    UnkSweepResult_l0cb9c0 result;
    result.velocity = Scaled(1.0f / dt, xf->velocity);
    UnkVec3f down = self->vtable->Down(self);

    UnkSweepPlayer_l0cb9c0* player = reinterpret_cast<UnkSweepPlayer_l0cb9c0*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->life && !fn_7101161088(player->life)) {
        UnkAnimationComponent* animation = self->owner->animation;
        if (!animation || !fn_710117b348(animation, fn_7100d070a8())) {
            UnkSweepQuery_l0cb9c0 query(1);
            query.shape = reinterpret_cast<UnkShape*>(fn_7100864194(
                self->owner->collision, reinterpret_cast<const base::global::CStrId*>(
                                            reinterpret_cast<const u8*>(&GameManagerIdTable()) + 0xaa0)));
            if (query.shape) {
                query.layers[0] = 0x10;
                query.layers[1] = 0x10;
                query.layers[2] = self->layer;
                query.field_0x4 = true;
                query.down = down;
                query.hit = self->hit;
                query.filter = self->filter;
                query.field_0x88 = self->char_class->field_0x1b9;

                UnkSweepProbe_l0cb9c0 probe = UnkSweepProbe_l0cb9c0();
                probe.field_0x58 = -0.3f;
                probe.field_0x5c = g_unk_710164baa0;
                Fill(&probe, self, xf, result, dt);
                UnkSweepOut_l0cb9c0 out;
                fn_7100d4c198(self->collider, &query, &probe, &out, &result);
                Apply(self, xf, out);
            }
        }
    }

    self->vtable->Step(self, xf, dt, &result);

    UnkSweepQuery_l0cb9c0 query(1);
    query.shape = fn_7100976588(reinterpret_cast<UnkFlagged*>(self));
    if (!query.shape)
        return;
    UnkSweepClass_l0cb9c0* char_class = self->char_class;
    query.kind = char_class->query_kind;
    u32 layer = self->layer;
    query.layers[0] = layer;
    query.layers[1] = layer;
    query.layers[2] = layer;
    query.field_0x4 = true;
    query.hit = self->hit;
    query.filter = self->filter;
    query.field_0xc = char_class->field_0x1c0 ? 1 : 2;
    query.down = down;
    query.field_0x88 = char_class->field_0x1b9;
    if (query.kind <= 1 && self->field_0x1be0) {
        query.field_0xc = 1;
        query.kind = 2;
    }
    query.field_0x6c = char_class->field_0x1c1;
    if (UnkSweepLife_l0cb9c0* life = self->owner->life) {
        if (!life->data || !life->data->field_0x10)
            fn_710115fe20(reinterpret_cast<UnkHolder*>(life));
        if (life->data && life->data->field_0x10) {
            if (void* ignored = life->data->field_0x10->ignored)
                query.ignored.Add(ignored);
        }
    }

    UnkSweepProbe_l0cb9c0 probe = UnkSweepProbe_l0cb9c0();
    probe.field_0x58 = -0.3f;
    probe.field_0x5c = g_unk_710164baa0;
    Fill(&probe, self, xf, result, dt);
    probe.field_0x58 = self->vtable->SweepMargin(self);
    UnkSweepOut_l0cb9c0 out;
    fn_7100d4c198(self->collider, &query, &probe, &out, &result);
    Apply(self, xf, out);
}
