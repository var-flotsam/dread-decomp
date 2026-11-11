// Unit 2573 (no reliable name): sweeps an attack from one point to another, collects what it touches (or the
// player's shield object when one is up), sorts the hits by distance and hands each to the attack's hit
// handlers until one stops it. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern "C" f32 sqrtf(f32);

using Vec2 = base::math::CVector2D;

// A 3-float vector copied one component at a time.
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
    Vec3() {}
    Vec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    Vec3(const Vec3& o) : x(o.x), y(o.y), z(o.z) {}
    Vec3& operator=(const Vec3& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

static inline Vec3 operator-(const Vec3& a, const Vec3& b) { return Vec3{a.x - b.x, a.y - b.y, a.z - b.z}; }
static inline Vec3 operator+(const Vec3& a, const Vec3& b) { return Vec3{a.x + b.x, a.y + b.y, a.z + b.z}; }
static inline Vec3 operator*(const Vec3& a, f32 s) { return Vec3{a.x * s, a.y * s, a.z * s}; }
static inline f32 Dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline f32 LengthSq(const Vec3& a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
static inline Vec3 Normalized(const Vec3& a) { return a * (1.0f / (sqrtf(LengthSq(a)) + 0.0001f)); }

struct UnkSweepOwner_lecd1a4 {
    u8 pad_0x0[0x1f0];
    void* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkSweepLink_lecd1a4 {
    u8 pad_0x0[0x10];
    UnkSweepOwner_lecd1a4* target;  // 0x10
};

struct UnkSweepAttack_lecd1a4 {
    u8 pad_0x0[0x8];
    s32 type;  // 0x8
    s32 field_0xc;
    u8 pad_0x10[0x10];
    u32 flags;  // 0x20
    u8 pad_0x24[0x18];
    Vec3 field_0x3c;
    u8 pad_0x48[0x28];
    UnkSweepLink_lecd1a4* field_0x70;
    u8 pad_0x78[0x40];
    Vec3 normal;  // 0xb8
    u8 pad_0xc4[0xc];
    Vec3 field_0xd0;
    Vec3 field_0xdc;
};
static_assert(__builtin_offsetof(UnkSweepAttack_lecd1a4, field_0xdc) == 0xdc, "layout");

struct UnkSweepComponent_lecd1a4 {
    virtual const base::reflection::CClass* vf_0x0() const;
    u8 pad_0x8[0x8];
    UnkSweepComponent_lecd1a4* field_0x10;
    u8 pad_0x18[0x18];
    void* field_0x30;
    u8 pad_0x38[0x51];
    u8 field_0x89;
};

struct UnkSweepKind_lecd1a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};

struct UnkSweepActor_lecd1a4 {
    u8 pad_0x0[0x10];
    UnkSweepComponent_lecd1a4* component;  // 0x10
    u8 pad_0x18[0x140];
    UnkSweepKind_lecd1a4* kind;  // 0x158
};

struct UnkSweepActorHandle_lecd1a4 : UnkRefObject {
    UnkSweepActor_lecd1a4* target;
};

struct __attribute__((packed)) UnkSweepHit_lecd1a4 {
    UnkSweepActorHandle_lecd1a4* object;  // 0x0
    UnkRefObject* field_0x8;
    s32 field_0x10;
    Vec3 pos;  // 0x14
    u64 field_0x20;
    u32 field_0x28;
    Vec3 normal;  // 0x2c
    f32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 pad_0x44;

    UnkSweepHit_lecd1a4() {}
    UnkSweepHit_lecd1a4(const UnkSweepHit_lecd1a4& o) {
        object = nullptr;
        if (o.object) {
            ++o.object->ref_count;
            object = o.object;
        }
        field_0x8 = nullptr;
        AssignRef(&field_0x8, o.field_0x8);
        field_0x10 = o.field_0x10;
        pos.x = o.pos.x;
        pos.y = o.pos.y;
        pos.z = o.pos.z;
        *reinterpret_cast<u32*>(&field_0x20) = *reinterpret_cast<const u32*>(&o.field_0x20);
        reinterpret_cast<u32*>(&field_0x20)[1] = reinterpret_cast<const u32*>(&o.field_0x20)[1];
        field_0x28 = o.field_0x28;
        normal.x = o.normal.x;
        normal.y = o.normal.y;
        normal.z = o.normal.z;
        field_0x38 = o.field_0x38;
        field_0x3c = o.field_0x3c;
        field_0x40 = o.field_0x40;
    }
    ~UnkSweepHit_lecd1a4() {
        if (field_0x8 && --field_0x8->ref_count == 0)
            field_0x8->vf_0x10();
        if (object && --object->ref_count == 0)
            object->vf_0x10();
    }
    u32 Strength() const { return (field_0x3c & 0x40) ? field_0x40 : 0; }
};

extern void* g_unk_7101a70eb0[];
extern void* g_unk_7101a96350[];

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

struct __attribute__((packed)) UnkSweepHitList_lecd1a4 {
    void** vtable;
    u32 count;
    u8 storage[32 * sizeof(UnkSweepHit_lecd1a4)];

    UnkSweepHit_lecd1a4* items() { return reinterpret_cast<UnkSweepHit_lecd1a4*>(storage); }

    UnkSweepHitList_lecd1a4() {
        vtable = g_unk_7101a70eb0 + 2;
        count = 0;
    }
    ~UnkSweepHitList_lecd1a4() {
        vtable = g_unk_7101a70eb0 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items()[i].field_0x8);
            ReleaseRef(items()[i].object);
        }
    }
};

// The hit list that keeps the point its hits are sorted from.
struct __attribute__((packed)) UnkSweepSortedHits_lecd1a4 : UnkSweepHitList_lecd1a4 {
    Vec3 origin;

    explicit UnkSweepSortedHits_lecd1a4(const Vec3& from) {
        vtable = g_unk_7101a96350 + 2;
        origin = from;
    }
};

struct UnkSweepCompare_lecd1a4 {
    Vec3 origin;
};

struct UnkSweepSegment_lecd1a4 {
    Vec2 a;
    Vec2 b;
    Vec2 delta;
    UnkSweepSegment_lecd1a4(f32 ax, f32 ay, f32 bx, f32 by) {
        a.x = ax;
        a.y = ay;
        b.x = bx;
        b.y = by;
        delta.x = bx - ax;
        delta.y = by - ay;
    }
};

struct UnkTree;
struct UnkBox;
struct UnkShape;
struct UnkMtx44;

void* fn_7100ed77a0(void* player);
extern void* g_unk_71726af0b0;
bool fn_7100d5c518(UnkTree* self, u32 mask, const UnkBox* box, void* arg, u32 arg2);
bool fn_7100d5b484(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, void* arg, u16 arg2);
bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);
UnkMatrix44 fn_7100d36a2c(UnkShape* shape);
void fn_7100abbd40(UnkSweepHit_lecd1a4* first, UnkSweepHit_lecd1a4* last, UnkSweepCompare_lecd1a4& compare);
s32 fn_7100ecee50(UnkSweepAttack_lecd1a4* self, void* arg, const Vec3* from, const Vec3* delta, UnkSweepHit_lecd1a4* hit,
                  UnkSweepActor_lecd1a4* actor);
bool fn_7100ecebf4(UnkSweepAttack_lecd1a4* self, UnkSweepComponent_lecd1a4* group, const Vec3* from, const Vec3* delta,
                   UnkSweepHit_lecd1a4* hit, void* arg, u8* out);
void fn_7100ece7fc(UnkSweepAttack_lecd1a4* self, UnkSweepHit_lecd1a4* hit, u32 strength);

__attribute__((visibility("hidden"))) extern const u32 g_unk_710164beac[];

struct UnkSweepShape_lecd1a4 {
    u8 storage[0xb0];
};
void fn_7100d6169c(UnkSweepShape_lecd1a4* self, const void* source, f32 size);
void fn_7100d61750(UnkSweepShape_lecd1a4* self);

struct UnkSweepSphere_lecd1a4 : UnkSweepShape_lecd1a4 {
    UnkSweepSphere_lecd1a4(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkSweepSphere_lecd1a4() { fn_7100d61750(this); }
};

// The player's shield object: a weak target with a dirty flag and its extent.
struct UnkSweepShield_lecd1a4 {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    u8 pad_0x44[0x4];
    f32 field_0x48;
    u8 pad_0x4c[0x14];
    u8 dirty;  // 0x60

    void Refresh() { reinterpret_cast<void (*)(UnkSweepShield_lecd1a4*)>(vtable[0x98 / 8])(this); }
};

struct UnkSweepHandleRef_lecd1a4 {
    UnkWeakHandle* handle;
    explicit UnkSweepHandleRef_lecd1a4(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkSweepHandleRef_lecd1a4() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

static inline bool IsA(UnkSweepComponent_lecd1a4* c, const base::reflection::CClass* (*getter)()) {
    const base::reflection::CClass* own = c->vf_0x0();
    return fn_710009d50c(own, getter());
}

// Fills `hits` with what the sweep touches. False when there is nothing to handle.
static inline bool Collect(UnkSweepAttack_lecd1a4* self, const Vec3& from, const Vec3& to, const Vec3& delta,
                           const Vec3& dir, UnkSweepSortedHits_lecd1a4& hits) {
    UnkSweepShield_lecd1a4* shield = static_cast<UnkSweepShield_lecd1a4*>(fn_7100ed77a0(g_unk_71726af0b0));
    bool tracked = false;
    if (self->field_0x70 && self->field_0x70->target && self->field_0x70->target->animation)
        tracked = fn_710117b348(self->field_0x70->target->animation, fn_7100bf4fa8());

    if (!shield && !tracked) {
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        if (!tree)
            return false;
        UnkSweepSegment_lecd1a4 segment(from.x, from.y, delta.x + to.x, delta.y + to.y);
        bool any = fn_7100d5c518(tree, g_unk_710164beac[self->type] | 0x80400, reinterpret_cast<const UnkBox*>(&segment),
                                 &hits, 0);
        f32 back;
        if (self->type == 4)
            back = 5.0f;
        else
            back = self->field_0xc == 1 ? 30.0f : 10.0f;
        UnkMatrix44 xf;
        __builtin_memset(&xf, 0, sizeof(xf));
        xf.m[15] = 1.0f;
        xf.m[10] = 1.0f;
        xf.m[5] = 1.0f;
        xf.m[0] = 1.0f;
        xf.m[3] = from.x - dir.x * back;
        xf.m[7] = from.y - dir.y * back;
        xf.m[11] = from.z - dir.z * back;
        UnkSweepSphere_lecd1a4 shape(&g_unk_710164779c, back);
        bool hit = fn_7100d5b484(tree, 0x1080, reinterpret_cast<UnkShape*>(&shape),
                                 reinterpret_cast<const UnkMtx44*>(&xf), &hits, 0);
        if (hit)
            return true;
        any |= hit;
        if (self->type == 4) {
            any |= fn_7100d5c518(tree, 0x1080, reinterpret_cast<const UnkBox*>(&segment), &hits, 0);
        } else {
            Vec3 twice = delta + delta;
            Vec2 flat;
            fn_710007b430(&flat, reinterpret_cast<const base::math::CVector3D*>(&twice));
            any |= fn_7100d5babc(tree, 0x1080, reinterpret_cast<UnkShape*>(&shape),
                                 reinterpret_cast<const UnkMtx44*>(&xf), &flat, &hits, true);
        }
        return any;
    }

    if (shield->dirty)
        shield->Refresh();
    f32 width = shield->field_0x48 - shield->field_0x40;
    UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(shield));
    Vec3 centre(m.m[3], m.m[7], m.m[11]);
    UnkSweepHandleRef_lecd1a4 ref(GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(shield)));
    f32 half = width * 0.5f;
    Vec3 normal = self->normal;
    Vec3 point = from;
    bool found = true;
    if (tracked) {
        point = centre - Normalized(centre - from) * (half + 25.0f);
    } else if (!(LengthSq(from - centre) <= half * half)) {
        if (!(LengthSq(to - centre) <= half * half)) {
            if (!(Dot(centre - to, normal) < 0.0f))
                found = false;
            else
                point = delta * 0.5f + from;
        } else {
            point = to;
        }
    }
    if (!found)
        return found;
    if (static_cast<s32>(32 - hits.count) > 0) {
        UnkSweepHit_lecd1a4* hit = &hits.items()[hits.count];
        hit->object = nullptr;
        AssignRef(reinterpret_cast<UnkWeakHandle**>(&hit->object), ref.handle);
        hit->field_0x8 = nullptr;
        hit->field_0x10 = -1;
        hit->pos = point;
        hit->field_0x20 = 0;
        hit->field_0x28 = 0;
        hit->normal.x = -normal.x;
        hit->normal.y = -normal.y;
        hit->normal.z = -normal.z;
        hit->field_0x38 = 0.0f;
        hit->field_0x3c = 0x18;
        hit->field_0x40 = 0;
        ++hits.count;
    }
    UnkSweepCompare_lecd1a4 compare{Vec3(hits.origin)};
    fn_7100abbd40(hits.items(), hits.items() + hits.count, compare);
    return found;
}

bool fn_7100ecd1a4(UnkSweepAttack_lecd1a4* self, const Vec3* start, const Vec3* end) {
    Vec3 from = *start;
    Vec3 to = *end;
    Vec3 delta = to - from;
    f32 length = sqrtf(LengthSq(delta));
    UnkSweepSortedHits_lecd1a4 hits(from);
    Vec3 dir = delta * (1.0f / (length + 0.0001f));

    if (!Collect(self, from, to, delta, dir, hits))
        return false;

    bool stop = false;
    for (UnkSweepHit_lecd1a4* it = hits.items(); it != hits.items() + hits.count; ++it) {
        UnkSweepHit_lecd1a4 hit(*it);
        if (it->object) {
            if (UnkSweepActor_lecd1a4* actor = it->object->target) {
                if (actor->kind->vf_0x18() == 0x10)
                    hit.pos.z = start->z;
            }
            UnkSweepActor_lecd1a4* actor;
            UnkSweepComponent_lecd1a4* component;
            if (it->object && (actor = it->object->target) && (component = actor->component)) {
                if (IsA(component, fn_710085fc44)) {
                    UnkSweepComponent_lecd1a4* sub = component->field_0x10;
                    UnkSweepComponent_lecd1a4* typed = IsA(sub, fn_710079a218) ? sub : nullptr;
                    if (!typed->field_0x89 || !(it->field_0x38 >= 0.5f)) {
                        s32 result = fn_7100ecee50(self, component->field_0x30, &from, &delta, &hit, actor);
                        if (result == 0) {
                            self->field_0xdc = self->field_0xd0;
                            self->field_0xd0 = self->field_0x3c;
                        }
                        stop = result == 0;
                    }
                } else if (IsA(component, fn_7100ca0484)) {
                    u8 handled;
                    stop = fn_7100ecebf4(self, component, &from, &delta, &hit, nullptr, &handled);
                    if (!handled)
                        fn_7100ece7fc(self, &hit, 1);
                } else {
                    bool trigger = IsA(component, fn_7100a46444);
                    bool scenario = IsA(component, fn_710119ea84);
                    if (trigger || scenario) {
                        if (!(self->field_0x70 && self->field_0x70->target && self->field_0x70->target->movement &&
                              Dot(dir, hit.normal) > 0.9f)) {
                            stop = !(self->flags & 0x1000) && (self->flags & 0x8001400) != 0x1400;
                            fn_7100ece7fc(self, &hit, hit.Strength());
                        }
                    }
                }
            }
        }
        if (stop)
            break;
    }
    return stop;
}
