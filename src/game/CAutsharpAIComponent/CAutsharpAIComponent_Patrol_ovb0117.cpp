// CAutsharpAIComponent unit (tools/tu_map.py unit 572): the Autsharp's patrol logic. The per-frame update
// (fn_71003c94b0) dispatches on the state at +0x146d8 (1 walk, 2 wait, 3 turn); the walk and turn steps
// scale the tuning speed (+0x146d0, vf_0xb0) by the walk progress; a wall probe (fn_71003cc304) casts the
// facing (+0x14760) through the collision tree as src/game/CCharacterMovement.cpp does; a stuck check
// (fn_71003ca8a8) compares the owner's translation every 0.1 s; the movement hooks (fn_71003cc7dc,
// fn_71003cca50) adjust the velocity. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAutsharpOwner_b0117 {
    void** vtable;
    u8 pad_0x8[0x38 - 0x8];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    void* field_0x1f0;
    u8 pad_0x1f8[0x218 - 0x1f8];
    void* field_0x218;
    void* field_0x220;
    void* field_0x228;
    u8 pad_0x230[0x298 - 0x230];
    void* field_0x298;
    u8 pad_0x2a0[0x37c - 0x2a0];
    f32 radius;  // 0x37c
    u8 pad_0x380[0x18];
    void* field_0x398;
};
static_assert(__builtin_offsetof(UnkAutsharpOwner_b0117, field_0x398) == 0x398, "owner layout");

// The tuning object at +0x146d0: its vf_0xb0 gives a speed for the owner.
struct UnkAutsharpTuning_b0117 {
    void** vtable;
    u8 pad_0x8[0x530 - 0x8];
    f32 field_0x530;
    f32 field_0x534;
    f32 field_0x538;
    f32 field_0x53c;
    f32 field_0x540;
    f32 field_0x544;
    f32 field_0x548;

    f32 GetSpeed(UnkAutsharpOwner_b0117* owner) {
        return reinterpret_cast<f32 (*)(UnkAutsharpTuning_b0117*, UnkAutsharpOwner_b0117*)>(vtable[0xb0 / 8])(this,
                                                                                                              owner);
    }
};

struct UnkPair_b0117 {
    u64 first;
    u64 second;
};

struct UnkAutsharpAI_b0117 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAutsharpOwner_b0117* owner;  // 0x30
    u8 pad_0x38[0x30];
    CVector3D* points;  // 0x68
    u32 point_count;    // 0x70
    u8 pad_0x74[0x20];
    u8 open;  // 0x94
    u8 pad_0x95[0x146d0 - 0x95];
    UnkAutsharpTuning_b0117* tuning;  // 0x146d0
    s32 state;                        // 0x146d8
    u8 pad_0x146dc[4];
    UnkPair_b0117 field_0x146e0;
    u8 link[0x18];       // 0x146f0
    bool field_0x14708;
    bool field_0x14709;
    bool field_0x1470a;
    bool field_0x1470b;
    bool field_0x1470c;
    u8 pad_0x1470d[3];
    f32 field_0x14710;
    f32 timer;  // 0x14714
    u8 pad_0x14718[8];
    f32 field_0x14720;
    f32 field_0x14724;
    f32 field_0x14728;
    f32 field_0x1472c;
    f32 field_0x14730;
    u8 pad_0x14734[8];
    bool field_0x1473c;
    u8 pad_0x1473d[0xb];
    bool field_0x14748;
    u8 pad_0x14749[3];
    CVector2D field_0x1474c;
    CVector3D field_0x14754;
    UnkVec3f field_0x14760;
    CVector2D field_0x1476c;
    CVector2D field_0x14774;
    u32 field_0x1477c;
    bool field_0x14780;
    u8 pad_0x14781[4];
    bool field_0x14785;
    u8 pad_0x14786[2];
    f32 field_0x14788;
    UnkVec3f field_0x1478c;
    u8 pad_0x14798[0x11];
    bool field_0x147a9;
    bool field_0x147aa;
    bool field_0x147ab;
};
static_assert(__builtin_offsetof(UnkAutsharpAI_b0117, field_0x147ab) == 0x147ab, "layout");
static_assert(__builtin_offsetof(UnkAutsharpAI_b0117, field_0x14785) == 0x14785, "layout");

// ---- the collision-tree segment query (as src/game/CCharacterMovement.cpp spells it) ----

struct UnkHitRef_b0117 {
    base::UnkRefCounted* ptr;

    UnkHitRef_b0117() : ptr(nullptr) {}
    ~UnkHitRef_b0117() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_b0117 {
    f32 x, y, z;
    UnkHitVec_b0117(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_b0117 {
    UnkHitRef_b0117 object;
    UnkHitRef_b0117 field_0x8;
    s32 id;
    UnkHitVec_b0117 point;
    UnkHitVec_b0117 field_0x20;
    UnkHitVec_b0117 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_b0117()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

extern void* g_unk_7101a20fa8[];
extern void* g_unk_7101a20ed8[];
struct UnkSegmentHitQuery_b0117 {
    void** vtable;
    UnkCollisionHit_b0117 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_b0117(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_b0117() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkHitSegment_b0117 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_b0117(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkTree;
struct UnkBox;
bool fn_7100d5c518(UnkTree* self, u32 mask, const UnkBox* box, void* arg, u32 arg2);

// ---- reflection casts ----

template <const base::reflection::CClass* (*GetClass)()>
static inline base::reflection::UnkReflObject* CastTo_b0117(base::reflection::UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), GetClass()) ? obj : nullptr;
}

// The ratio-scaled speed: the speed times the ratio clamped to [0, 1], blended against zero.
static inline f32 ScaledSpeed_b0117(f32 speed, f32 t) {
    if (t >= 1.0f)
        return speed;
    if (t < 0.0f)
        return 0.0f;
    f32 from = (1.0f - t) * 0.0f;
    return speed * t + from;
}

struct UnkAutsharpSpawnSlot_b0117 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};

// ---- the effect parameter blocks ----

struct UnkEffectSink;
void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags);

// The block handed to the owner's +0x218 (fn_71010c7e6c); the vtable tells the kinds apart.
struct UnkEffectParams_b0117 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 field_0x51;
    u8 field_0x52;
};

// The kind-6 block handed to fn_71006f4b0c.
struct UnkParams6_b0117 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    f32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    UnkPair_b0117 field_0x20;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
};

// The movement at the owner's +0x1f0: a flag at +0xf9 with its change handler (vf_0x260).
struct UnkAutsharpMovement_b0117 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;

    void vf_0x260() { reinterpret_cast<void (*)(UnkAutsharpMovement_b0117*)>(vtable[0x260 / 8])(this); }
};

// The kind-5 block: all clear.
template <class Self>
inline void SendKind5_b0117(Self* self) {
    UnkParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x30 = 0;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    fn_71006f4b0c(self->owner->field_0x298, &params, 0);
}

struct UnkAutsharpSpawnPoint_b0117 {
    u8 pad_0x0[0x3b8];
    s32 side;
};

static inline void FaceBack_b0117(UnkAutsharpAI_b0117* self) {
    const UnkMatrix44& n = self->owner->xform.GetMatrix();
    self->field_0x14760 = UnkVec3f(-n.m[2], -n.m[6], -n.m[10]);
}

void fn_71003c930c(UnkAutsharpAI_b0117* self) {
    const UnkMatrix44& m = self->owner->xform.GetMatrix();
    self->field_0x14760 = UnkVec3f(m.m[2], m.m[6], m.m[10]);
    auto* slot = static_cast<UnkAutsharpSpawnSlot_b0117*>(self->owner->field_0x398);
    auto* spawn = reinterpret_cast<UnkAutsharpSpawnPoint_b0117*>(CastTo_b0117<fn_7100a13bd0>(slot->field_0x10));
    switch (spawn->side) {
    case 1:
        self->field_0x14708 = true;
        break;
    case 0:
        self->field_0x14708 = false;
        FaceBack_b0117(self);
        break;
    default:
        if (fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x44 < self->owner->xform.m[0][3]) {
            self->field_0x14708 = false;
            FaceBack_b0117(self);
        } else {
            self->field_0x14708 = true;
        }
        break;
    }
    if (self->field_0x14760.x < 0.0f) {
        UnkAutsharpOwner_b0117* owner = self->owner;
        UnkVec3f angle = owner->xform.GetAngle();
        UnkVec3f flipped = angle;
        flipped.y = -angle.y;
        owner = self->owner;
        reinterpret_cast<void (*)(UnkAutsharpOwner_b0117*, const UnkVec3f*, bool)>(owner->vtable[0xd8 / 8])(
            owner, &flipped, true);
    }
}

bool fn_71009e12d0(void*);
void fn_71003c95d8(UnkAutsharpAI_b0117* self);
void fn_71003c97ec(UnkAutsharpAI_b0117* self);
void fn_71003c9b7c(UnkAutsharpAI_b0117* self, bool flag);
void fn_71003c9eac(UnkAutsharpAI_b0117* self, bool flag);
void fn_71003c9fd8(UnkAutsharpAI_b0117* self, f32 dt);
void fn_71003ca118(UnkAutsharpAI_b0117* self, f32 dt);
void fn_71003ca3e0(UnkAutsharpAI_b0117* self, f32 dt);

static inline UnkActor* Target(UnkAutsharpAI_b0117* self) {
    return fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
}

void fn_71003c94b0(UnkAutsharpAI_b0117* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    fn_71003c95d8(self);
    if (!self->field_0x147a9)
        fn_71003c97ec(self);
    bool found = false;
    if (Target(self)->field_0x208)
        found = fn_71009e12d0(Target(self)->field_0x208);
    if (self->field_0x147a9)
        fn_71003c9b7c(self, found);
    if (self->field_0x147ab) {
        fn_71003c9eac(self, false);
        self->field_0x147ab = false;
    }
    switch (self->state) {
    case 1:
        fn_71003c9fd8(self, dt);
        break;
    case 2:
        fn_71003ca118(self, dt);
        break;
    case 3:
        fn_71003ca3e0(self, dt);
        break;
    }
}

extern void* g_unk_7101a24db8[];
extern void* g_unk_7101a25fb0[];

void fn_71003c9eac(UnkAutsharpAI_b0117* self, bool flag) {
    reinterpret_cast<void (*)(UnkAutsharpAI_b0117*)>(self->vtable[0x3d8 / 8])(self);
    UnkEffectSink* sink = static_cast<UnkEffectSink*>(self->owner->field_0x218);
    if (!sink)
        return;
    UnkEffectParams_b0117 effect;
    effect.field_0xc = 0;
    effect.field_0x14 = 0;
    __builtin_memset(effect.field_0x18, 0, 13);
    __builtin_memset(effect.field_0x28, 0, 13);
    effect.vtable = g_unk_7101a24db8 + 2;
    effect.field_0x3d = 0;
    effect.field_0x41 = 0;
    __builtin_memset(effect.field_0x44, 0, 13);
    effect.field_0x38 = 1;
    effect.field_0x3e = 1;
    effect.field_0x3f = 1;
    effect.field_0x51 = flag;
    effect.field_0x52 = 1;
    fn_71010c7e6c(sink, &effect, 1);
    auto* movement = static_cast<UnkAutsharpMovement_b0117*>(self->owner->field_0x1f0);
    if (movement->field_0xf9) {
        movement->field_0xf9 = false;
        movement->vf_0x260();
    }
    UnkParams6_b0117 params;
    __builtin_memset(params.field_0x8, 0, 13);
    params.vtable = g_unk_7101a25fb0 + 2;
    params.field_0x34 = 6;
    params.field_0x38 = self->field_0x14708;
    params.field_0x18 = 1.0f;
    params.field_0x39 = 1;
    params.field_0x1c = 1;
    params.field_0x20 = self->field_0x146e0;
    params.field_0x30 = 1;
    fn_71006f4b0c(self->owner->field_0x298, &params, 1);
}

struct UnkAutsharpAI_b61a2;
void fn_71003ca540(UnkAutsharpAI_b61a2* self, bool on);
void fn_71003ca6e4(UnkAutsharpAI_b0117* self, f32 dt, f32 speed);
void fn_7100a145a8(base::reflection::UnkReflObject* spawn_point);

void fn_71003c9fd8(UnkAutsharpAI_b0117* self, f32 dt) {
    f32 speed = self->tuning->GetSpeed(self->owner) * self->field_0x1472c;
    speed = ScaledSpeed_b0117(speed, self->field_0x14710 / self->field_0x14720);
    self->field_0x14710 += dt;
    if (!(self->field_0x14710 >= self->field_0x14720)) {
        fn_71003ca6e4(self, dt, speed);
        return;
    }
    if (self->state != 2)
        self->state = 2;
    fn_71003c9eac(self, true);
    base::reflection::UnkReflObject* spawn = nullptr;
    auto* slot = static_cast<UnkAutsharpSpawnSlot_b0117*>(self->owner->field_0x398);
    if (slot && slot->field_0x10)
        spawn = CastTo_b0117<fn_7100a13bd0>(slot->field_0x10);
    fn_7100a145a8(spawn);
    fn_71003ca540(reinterpret_cast<UnkAutsharpAI_b61a2*>(self), false);
}

bool fn_71003caa58(UnkAutsharpAI_b0117* self);
bool fn_71003cb4f8(UnkAutsharpAI_b0117* self);
void fn_71003cb250(UnkAutsharpAI_b0117* self);

void fn_71003ca3e0(UnkAutsharpAI_b0117* self, f32 dt) {
    self->timer += dt;
    if (self->timer >= self->tuning->field_0x530) {
        self->timer = 0.0f;
        fn_71003cb250(self);
        if (self->state != 4)
            self->state = 4;
        return;
    }
    if (fn_71003caa58(self)) {
        self->timer = 0.0f;
        self->field_0x14728 = self->tuning->GetSpeed(self->owner);
    } else if (fn_71003cb4f8(self)) {
        f32 speed = self->tuning->GetSpeed(self->owner) * self->field_0x1472c;
        fn_71003ca6e4(self, dt, ScaledSpeed_b0117(speed, self->field_0x14710 / self->field_0x14720));
        return;
    } else {
        self->timer = 0.0f;
        self->field_0x14728 = self->tuning->GetSpeed(self->owner);
        fn_71003c9eac(self, true);
    }
    if (self->state != 2)
        self->state = 2;
}

void fn_71003cb864(UnkAutsharpAI_b0117* self);

static inline bool Near_b0117(f32 a, f32 b) {
    return a - 0.1f <= b && b <= a + 0.1f;
}

void fn_71003ca8a8(UnkAutsharpAI_b0117* self, f32 dt) {
    self->field_0x14788 += dt;
    if (!(self->field_0x14788 >= 0.1f))
        return;
    self->field_0x14788 = 0.0f;
    UnkVec3f pos = self->owner->xform.GetTranslation();
    if (!Near_b0117(pos.x, self->field_0x1478c.x) || !Near_b0117(pos.y, self->field_0x1478c.y) ||
        !Near_b0117(pos.z, self->field_0x1478c.z)) {
        self->field_0x1478c = pos;
        return;
    }
    if (fn_71003cb4f8(self)) {
        if (self->owner->field_0x218)
            SendKind5_b0117(self);
        if (self->state != 3)
            self->state = 3;
        return;
    }
    if (!fn_71003caa58(self))
        fn_71003cb864(self);
}

bool fn_71003cc304(UnkAutsharpAI_b0117* self);

void fn_71003cb148(UnkAutsharpAI_b0117* self) {
    if (!self->field_0x14780)
        return;
    UnkAutsharpOwner_b0117* owner = self->owner;
    CVector2D p;
    f32 x = owner->xform.m[0][3];
    f32 y = owner->xform.m[1][3];
    f32 z = owner->xform.m[2][3];
    CVector3D pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    fn_710007b430(&p, &pos);
    f32 dx = self->field_0x14774.x - p.x;
    f32 dy = self->field_0x14774.y - p.y;
    f32 len = sqrtf(dx * dx + dy * dy);
    if (len != 0.0f) {
        dx /= len;
        dy /= len;
    }
    self->field_0x1476c.x = dx;
    self->field_0x1476c.y = dy;
    if (fn_71003cc304(self) && !self->field_0x147aa)
        self->field_0x147aa = true;
    if (self->field_0x147aa && !fn_71003cc304(self)) {
        self->field_0x1470a = true;
        self->field_0x147aa = false;
        self->field_0x14780 = false;
        self->field_0x1477c = 0;
    }
}

void fn_71003cbd2c(UnkAutsharpAI_b61a2* self, bool on);
extern void* g_unk_7101a2e1c0[];

struct UnkEffectParams2_b0117 {
    void** vtable;
    f32 field_0x8;
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
};

static inline const CVector2D& Point_b0117(UnkAutsharpAI_b0117* self, u32 index) {
    return reinterpret_cast<const CVector2D&>(self->points[index]);
}

bool fn_71003cbb18(UnkAutsharpAI_b0117* self) {
    if (self->state != 2)
        return false;
    if (fn_71003cb4f8(self)) {
        if (self->owner->field_0x218)
            SendKind5_b0117(self);
        if (self->state != 3)
            self->state = 3;
        return true;
    }
    fn_71003cbd2c(reinterpret_cast<UnkAutsharpAI_b61a2*>(self), true);
    auto* sink = static_cast<UnkAutsharpMovement_b0117*>(self->owner->field_0x218);
    if (sink) {
        self->field_0x14748 = false;
        u32 count = self->point_count;
        u32 closed = (self->open == 0) & (count != 0);
        u32 last = count - closed;
        if (last != 0) {
            self->field_0x14748 = true;
            if (self->field_0x14708) {
                UnkSegment2D seg(Point_b0117(self, last - 1), closed ? Point_b0117(self, last) : Point_b0117(self, 0));
                self->field_0x1474c.x = seg.end.x;
                self->field_0x1474c.y = seg.end.y;
            } else {
                UnkSegment2D seg(Point_b0117(self, 0), count == 1 ? Point_b0117(self, 0) : Point_b0117(self, 1));
                self->field_0x1474c.x = seg.start.x;
                self->field_0x1474c.y = seg.start.y;
            }
        }
        if (sink->field_0xf9 != true) {
            sink->field_0xf9 = true;
            sink->vf_0x260();
        }
        reinterpret_cast<u8*>(sink)[0x1c10] = 0;
        UnkEffectParams2_b0117 params;
        params.field_0x14 = 0;
        __builtin_memset(params.field_0x18, 0, 13);
        __builtin_memset(params.field_0x28, 0, 13);
        params.vtable = g_unk_7101a2e1c0 + 2;
        params.field_0x38 = 2;
        params.field_0x8 = 1.0f;
        params.field_0xc = 1;
        fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(sink), &params, 0);
    }
    self->field_0x14709 = true;
    if (fn_71003caa58(self))
        self->field_0x14709 = false;
    return true;
}

bool fn_71003cc304(UnkAutsharpAI_b0117* self) {
    if (self->field_0x14780) {
        f32 dx = self->field_0x14760.x;
        f32 dy = self->field_0x14760.y;
        UnkAutsharpOwner_b0117* owner = self->owner;
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        CVector2D start = {owner->xform.m[0][3], owner->xform.m[1][3]};
        CVector2D end = {start.x + dx * 160.0f, start.y + dy * 160.0f};
        UnkHitSegment_b0117 segment(start, end);
        UnkSegmentHitQuery_b0117 query(CVector3D{owner->xform.m[0][3], owner->xform.m[1][3], owner->xform.m[2][3]});
        if (fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2))
            return true;
    }
    return false;
}

struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
void fn_71010d08d4(UnkMovement_71010d08d4*, const UnkMovementVec_71010d08d4*, bool);

struct UnkAutsharpMove_b0117 {
    u8 pad_0x0[0x1d8];
    UnkVec3f velocity;  // 0x1d8
    u8 pad_0x1e4[0x24];
    UnkVec3f field_0x208;

    void Apply(const UnkVec3f& v) {
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(this),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&v), true);
    }
};

void fn_71003cc7dc(UnkAutsharpAI_b0117* self, UnkAutsharpMove_b0117* move) {
    if (self->field_0x14780) {
        UnkVec3f v = move->velocity;
        UnkVec3f w = move->field_0x208;
        switch (self->field_0x1477c) {
        case 1:
            v.x = 0.0f;
            v.y = -800.0f;
            w.x = 0.0f;
            w.y = -800.0f;
            break;
        case 2:
            v.x *= 0.5f;
            v.y = -600.0f;
            w.x *= 0.5f;
            w.y = -600.0f;
            break;
        }
        move->Apply(v);
        move->field_0x208 = w;
    } else if (self->field_0x14785) {
        UnkVec3f v = move->velocity;
        v.x = 0.0f;
        move->Apply(v);
        move->field_0x208.x = 0.0f;
    } else if (self->field_0x1470a) {
        UnkVec3f v = move->velocity;
        v.x *= 0.5f;
        move->Apply(v);
        move->field_0x208.x *= 0.5f;
    } else if (!self->field_0x14709 && !self->field_0x1470c) {
        UnkVec3f v = move->velocity;
        v.y = 0.0f;
        move->Apply(v);
        move->field_0x208.y = 0.0f;
    }
}

s32 fn_7100661e24(UnkAutsharpAI_b0117* self, void* event);
extern void* g_unk_7101a2e208[];

struct UnkAutsharpEvent_b0117 {
    u8 pad_0x0[0xb4];
    u8 flags;
};

struct UnkEffectParams3_b0117 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    UnkVec3f field_0x28;
    u8 field_0x34;
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
};

s32 fn_71003cca50(UnkAutsharpAI_b0117* self, UnkAutsharpEvent_b0117* event) {
    s32 result = fn_7100661e24(self, event);
    if (!(event->flags & 0x10))
        return result;
    if (!self->field_0x1473c && (self->state & ~1) != 2)
        return result;
    if (!self->field_0x1470c) {
        self->field_0x1470c = true;
        self->field_0x1473c = true;
        auto* sink = static_cast<UnkAutsharpMovement_b0117*>(self->owner->field_0x218);
        if (sink) {
            UnkEffectParams3_b0117 params;
            params.field_0xc = 0;
            params.field_0x14 = 0;
            __builtin_memset(params.field_0x18, 0, 13);
            params.vtable = g_unk_7101a2e208 + 2;
            params.field_0x3d = 0;
            params.field_0x38 = 3;
            params.field_0x28 = reinterpret_cast<const UnkVec3f&>(self->field_0x14760);
            params.field_0x34 = 1;
            fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(sink), &params, 1);
            reinterpret_cast<u8*>(sink)[0x1c10] = 1;
            if (sink->field_0xf9 != true) {
                sink->field_0xf9 = true;
                sink->vf_0x260();
            }
            const UnkMatrix44& m = self->owner->xform.GetMatrix();
            f32 ux = m.m[1];
            f32 uy = m.m[5];
            f32 uz = m.m[9];
            f32 up_scale = self->tuning->field_0x544;
            f32 side_scale = self->tuning->field_0x548 * (self->field_0x14708 ? 1.0f : -1.0f);
            f32 sx = uy * 1.0f - uz * 0.0f;
            f32 sy = uz * 0.0f - ux * 1.0f;
            f32 sz = ux * 0.0f - uy * 0.0f;
            CVector3D v;
            v.x = ux * up_scale + sx * side_scale;
            v.y = uy * up_scale + sy * side_scale;
            v.z = uz * up_scale + sz * side_scale;
            fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(sink),
                          reinterpret_cast<const UnkMovementVec_71010d08d4*>(&v), true);
        }
    }
    return 6;
}
