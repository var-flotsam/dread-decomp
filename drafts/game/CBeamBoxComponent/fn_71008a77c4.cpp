// CBeamBoxComponent unit: a hit on the beam box. The first two hits of the breaking kind only play the
// hit effect; the third measures how far the box slides (along its path, then stopped by collision),
// plays the break effects, turns its collision off and stores the end point in the blackboard.
// Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU372.h"
#include "game/UnkTU54.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTypedValue.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkTree;
struct UnkShape;
struct UnkMtx44;
struct Vec2;
bool SweepShapeAgainstCollisionTree(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

void fn_71008635a4(UnkCollisionContainer* self, const CStrId* id, const u32* extra);

struct UnkMagnetPathHolder_b52;
struct UnkActor;
UnkMagnetPathHolder_b52* fn_7100382150(UnkActor* actor);

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

struct UnkBox2D {
    f32 min_x, min_y, max_x, max_y;

    UnkBox2D(f32 min_x_, f32 min_y_, f32 max_x_, f32 max_y_)
        : min_x(min_x_), min_y(min_y_), max_x(max_x_), max_y(max_y_) {}
};

// The unit's data (its statics, merged into one block).
struct UnkBeamBoxTU_l8ac5f0 {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId light;       // 0x20
    CStrId collision;   // 0x28
    UnkReflType type;   // 0x30, the component's class
};
extern UnkBeamBoxTU_l8ac5f0 g_unk_717272eb48 __attribute__((visibility("hidden")));
extern UnkProperty g_unk_717272e168;

// Timeline effects: the hit (one per kind of beam), the start of the slide and the break.
extern u64 g_unk_710164afa8;
extern u64 g_unk_710164afb0;
extern u64 g_unk_710164afb8;
extern u64 g_unk_710164afc0;
extern u64 g_unk_710164afc8;
extern u64 g_unk_710164afd0;
extern u64 g_unk_710164af88;
extern u64 g_unk_710164af90;
extern u64 g_unk_710164af98;

struct UnkBeamStaticZero_l8a77c4 {
    u32 value;
    UnkBeamStaticZero_l8a77c4() : value(0) {}
};

inline const UnkBeamStaticZero_l8a77c4& UnkBeamStaticZero() {
    static UnkBeamStaticZero_l8a77c4 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkBeamDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkBeamStaticZero().value & 0xff;
    return s_mask;
}

static inline void PlayEffect(UnkTimelineComponent* timeline, const u64* id) {
    alignas(8) u32 mask = UnkBeamDefaultMask();
    fn_7100a2cb7c(timeline, id, &mask, false, -1.0f);
}

// A point on the box's path.
struct UnkPathPoint_l8a77c4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const f32* vf_0x18();  // the position
    virtual void vf_0x20();
    virtual bool vf_0x28();  // whether it has points
    virtual UnkPathPoint_l8a77c4* vf_0x30(s32 index);
};

struct UnkPathHolder_l8a77c4 {
    u8 pad_0x0[0x30];
    UnkPathPoint_l8a77c4 path;
};

struct UnkBeamCollider_l8a77c4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();  // the shape kind; 4 is a 2D box
    UnkMatrix44 transform;  // 0x8
    u8 pad_0x48[0xa8 - 0x48];
    f32 min_x;  // 0xa8
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

struct UnkBeamCollisionObject_l8a77c4 {
    u8 pad_0x0[0x158];
    UnkBeamCollider_l8a77c4* collider;
};

// A collision shape built from a 2D box (fn_7100d5e1f8) and destroyed by fn_7100d5e2c8.
struct UnkBeamShape_l8a77c4 {
    void** vtable;
    UnkMatrix44 transform;  // 0x8
    u8 pad_0x48[0xc0 - 0x48];

    void Update() { reinterpret_cast<void (*)(UnkBeamShape_l8a77c4*)>(vtable[0x80 / 8])(this); }
};

struct UnkBeamHitVec_l8a77c4 {
    f32 x, y, z;
    UnkBeamHitVec_l8a77c4() : x(0.0f), y(0.0f), z(0.0f) {}
};

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

struct __attribute__((packed)) UnkBeamHit_l8a77c4 {
    UnkRefObject* object;
    UnkRefObject* field_0x8;
    s32 id;
    UnkBeamHitVec_l8a77c4 point;  // 0x14
    UnkBeamHitVec_l8a77c4 field_0x20;
    UnkBeamHitVec_l8a77c4 normal;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 pad_0x44;

    UnkBeamHit_l8a77c4()
        : object(nullptr), field_0x8(nullptr), id(-1), field_0x38(0), field_0x3c(0), field_0x40(0) {}
};

// The hits of a collision query: up to eight after the count.
struct __attribute__((packed)) UnkBeamHitList_l8a77c4 {
    void** vtable;
    u32 count;
    u8 storage[8 * sizeof(UnkBeamHit_l8a77c4)];

    UnkBeamHit_l8a77c4* items() { return reinterpret_cast<UnkBeamHit_l8a77c4*>(storage); }

    UnkBeamHitList_l8a77c4() {
        vtable = g_unk_7101a23478 + 2;
        count = 0;
    }
    ~UnkBeamHitList_l8a77c4() {
        vtable = g_unk_7101a23478 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items()[i].field_0x8);
            ReleaseRef(items()[i].object);
        }
    }
    // The first hit, added empty if there is none.
    UnkBeamHit_l8a77c4& First() {
        if (count == 0) {
            new (&items()[0]) UnkBeamHit_l8a77c4();
            count = 1;
        }
        return items()[0];
    }
};

struct UnkBeamSource_l8a77c4 {
    u8 pad_0x0[0x30];
    u64 kind;
};

struct UnkBeamShot_l8a77c4 {
    u8 pad_0x0[0x20];
    UnkBeamSource_l8a77c4* source;
};

struct UnkBeamHitMessage_l8a77c4 {
    u8 pad_0x0[0x60];
    UnkBeamShot_l8a77c4* shot;
    u8 pad_0x68[0x78 - 0x68];
    f32 direction;
    u8 pad_0x7c[0x94 - 0x7c];
    u8 field_0x94;
    u8 pad_0x95[0xb4 - 0x95];
    u32 flags;
};

struct UnkBeamOwner_l8a77c4 {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x2a0 - 0x94];
    UnkCollisionContainer* collision;
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* timeline;
};

struct UnkBeamValue_l8a77c4 {
    UnkPropertyName name;
    u8 pad_0x8[8];
};

struct UnkBeamBoxComponent_l8a77c4 {
    u8 pad_0x0[0x30];
    UnkBeamOwner_l8a77c4* owner;
    u8 pad_0x38[0x240 - 0x38];
    u32 mask;
    u8 pad_0x244[0x3c0 - 0x244];
    UnkBeamValue_l8a77c4* values;
    UnkStringData** keys;
    u32 count;
    u8 pad_0x3d4[0x428 - 0x3d4];
    u32 field_0x428;
    u8 pad_0x42c[0x434 - 0x42c];
    s32 hits;
    bool field_0x438;
    bool active;  // 0x439
    bool broken;  // 0x43a
    UnkVec3f slide;  // 0x43c
    UnkVec3f start;  // 0x448
    UnkVec3f end;    // 0x454
};
static_assert(__builtin_offsetof(UnkBeamBoxComponent_l8a77c4, end) == 0x454, "");

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

// Plays the effect for the kind of beam that hit.
static inline void PlayHitEffect(UnkTimelineComponent* timeline, UnkBeamHitMessage_l8a77c4* msg, const u64* a,
                                 const u64* b, const u64* c) {
    UnkBeamShot_l8a77c4* shot = msg->shot;
    if (shot->source->kind == 0x480772a0506c844aull)
        PlayEffect(timeline, a);
    if (shot->source->kind == 0xfb293e1e53cbdb25ull)
        PlayEffect(timeline, b);
    if (shot->source->kind == 0xbc8344f7f82c2411ull)
        PlayEffect(timeline, c);
}

bool fn_71008a77c4(UnkBeamBoxComponent_l8a77c4* self, UnkBeamHitMessage_l8a77c4* msg) {
    if (self->field_0x438 || self->broken || !self->active)
        return false;
    if (!(msg->flags & self->mask))
        return false;
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return false;
    if (!(*fn_7100864018(collision, msg->shot) == g_unk_717272eb48.light))
        return false;
    if (!(msg->field_0x94 & 0x20))
        return false;
    if (Sign(self->owner->xform.GetMatrix().m[0]) != Sign(msg->direction))
        return false;

    if (!(msg->flags & 0x400)) {
        UnkTimelineComponent* timeline = self->owner->timeline;
        if (!timeline)
            return false;
        PlayHitEffect(timeline, msg, &g_unk_710164afc0, &g_unk_710164afc8, &g_unk_710164afd0);
        return false;
    }
    if (++self->hits <= 2) {
        UnkTimelineComponent* timeline = self->owner->timeline;
        if (!timeline)
            return false;
        PlayHitEffect(timeline, msg, &g_unk_710164afa8, &g_unk_710164afb0, &g_unk_710164afb8);
        return false;
    }

    self->start = self->owner->xform.GetTranslation();
    if (UnkPathHolder_l8a77c4* holder =
            reinterpret_cast<UnkPathHolder_l8a77c4*>(fn_7100382150(reinterpret_cast<UnkActor*>(self->owner)))) {
        UnkPathPoint_l8a77c4* path = &holder->path;
        if (path->vf_0x28()) {
            UnkPathPoint_l8a77c4* first = path->vf_0x30(0);
            f32 x = self->start.x;
            f32 distance = fabsf(x - *first->vf_0x30(1)->vf_0x18());
            self->end.x = self->start.x + self->slide.x * distance;
            self->end.y = self->start.y + self->slide.y * distance;
            self->end.z = self->start.z + self->slide.z * distance;
        }
    }

    UnkGameManagerSub_0x608* tracker = g_unk_7101d47310->field_0x608;
    if (tracker && tracker->field_0x820 && self->owner->collision) {
        UnkTree* tree = static_cast<UnkTree*>(tracker->field_0x820);
        if (UnkCollisionObject* object = fn_7100864194(self->owner->collision, &g_unk_717272eb48.collision)) {
            base::math::CVector2D delta;
            {
                base::math::CVector3D offset;
                offset.x = self->end.x - self->start.x;
                offset.y = self->end.y - self->start.y;
                offset.z = self->end.z - self->start.z;
                fn_710007b430(&delta, &offset);
            }
            UnkBeamCollider_l8a77c4* collider =
                reinterpret_cast<UnkBeamCollisionObject_l8a77c4*>(object)->collider;
            UnkBeamCollider_l8a77c4* box = collider->vf_0x18() == 4 ? collider : nullptr;
            f32 cx = (box->min_x + box->max_x) * 0.5f;
            f32 cy = (box->min_y + box->max_y) * 0.5f;
            UnkBeamShape_l8a77c4 shape;
            {
                UnkBox2D shrunk(cx + (box->min_x - cx) * 0.99f, cy + (box->min_y - cy) * 0.99f,
                                cx + (box->max_x - cx) * 0.99f, cy + (box->max_y - cy) * 0.99f);
                fn_7100d5e1f8(reinterpret_cast<UnkCollisionShape*>(&shape), &shrunk);
            }
            UnkCopyMatrix(&shape.transform, box->transform);
            shape.Update();
            {
                UnkBeamHitList_l8a77c4 hits;
                UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(self->start);
                if (SweepShapeAgainstCollisionTree(tree, 1, reinterpret_cast<UnkShape*>(&shape), reinterpret_cast<const UnkMtx44*>(&xf),
                                  reinterpret_cast<const Vec2*>(&delta), &hits, false)) {
                    f32 halfWidth = fabsf((box->min_x + box->max_x) * 0.5f - box->min_x);
                    self->end.x = hits.First().point.x - halfWidth * self->slide.x;
                }
            }
            fn_7100d5e2c8(reinterpret_cast<UnkCollisionShape*>(&shape));
        }
    }

    UnkTimelineComponent* timeline = self->owner->timeline;
    if (!timeline) {
        self->active = false;
        self->field_0x428 = 0;
        self->hits = 0;
        return false;
    }
    fn_7100a2b7b8(timeline, &g_unk_710164afa8);
    fn_7100a2b7b8(timeline, &g_unk_710164afb0);
    fn_7100a2b7b8(timeline, &g_unk_710164afb8);
    PlayEffect(timeline, &g_unk_710164af88);
    PlayEffect(timeline, &g_unk_710164af90);
    PlayEffect(timeline, &g_unk_710164af98);
    {
        u32 flags = 0x200000;
        fn_71008635a4(collision, &g_unk_717272eb48.light, &flags);
    }
    {
        u32 flags = 0x200000;
        fn_71008635a4(collision, &g_unk_717272eb48.collision, &flags);
    }
    self->active = false;
    self->broken = true;
    self->field_0x428 = 0;
    self->hits = 0;

    UnkGameManager* gm = g_unk_7101d47310;
    CBlackboard* blackboard = &gm->field_0x968;
    {
        CStrId section = fn_7100de9c30(gm);
        u64 index = 0;
        while (index != self->count && self->keys[index] != g_unk_717272e168.name.data)
            index++;
        UnkBeamValue_l8a77c4* values = self->values;
        base::math::CVector2D end;
        fn_710007b430(&end, reinterpret_cast<const base::math::CVector3D*>(&self->end));
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
            if (!blackboard->field_0xe8) {
                UnkTypedValue v;
                v.type = fn_7100024504();
                v.value = &end;
                fn_7100332c80(s, &values[index].name, &v);
            }
        }
    }

    if (void* switcher = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner))) {
        fn_71009c0f1c(switcher, 0xb15b31b835c17820ull);
        fn_71009c0974(switcher, true);
    }
    return false;
}
