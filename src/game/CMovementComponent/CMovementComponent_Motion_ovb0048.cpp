// The CMovementComponent unit (unit 2688 of tools/tu_map.py), batch b0048: the character-class
// track checks (ignore gravity, ignore collision, flying), the floor the component stands on
// (fn_71010d14e4, fn_71010d1204) and the downward probe for it (fn_71010d0e88), the facing and
// turn sign from the animation's root motion (fn_71010d2b08, fn_71010d2c5c, fn_71010d2b9c), the
// motion reset (fn_71010d1e88, fn_71010d1f1c), the facing of another entity (fn_71010d32ec), the
// blend toward a target (fn_71010d432c), the reset from a transform (fn_71010d45b8, fn_71010d2f84)
// and a sideways vector (fn_71010d252c). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

extern "C" float sqrtf(float);


struct UnkMovementOwner_ovb0048 {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;  // 0x28
    u8 pad_0x30[0x1f0];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkMovement_ovb0048;

struct UnkMovementVtbl_ovb0048 {
    u8 pad_0x0[0x180];
    bool (*vf_0x180)(UnkMovement_ovb0048* self);
};

struct UnkMovement_ovb0048 {
    UnkMovementVtbl_ovb0048* vtable;
    u8 pad_0x8[0x28];
    UnkMovementOwner_ovb0048* owner;  // 0x30
    u8 pad_0x38[0xcc];
    bool field_0x104;
    u8 pad_0x105[0x3];
    f32 field_0x108;
    u8 pad_0x10c[0xe4];
    bool field_0x1f0;
    u8 pad_0x1f1[0x17];
    f32 field_0x208;
    f32 field_0x20c;
    f32 field_0x210;
};
static_assert(__builtin_offsetof(UnkMovement_ovb0048, field_0x208) == 0x208, "layout");

static inline bool IsCharClass(UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), fn_71011abbd4());
}

bool fn_71010d0d1c(UnkMovement_ovb0048* self) {
    if (!self->field_0x104)
        return false;
    UnkReflObject* cc = self->owner->char_class;
    if (!cc || !IsCharClass(cc))
        return true;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return true;
    return !fn_710117b348(animation, fn_7100d0c7a8());
}

bool fn_71010d0da8(UnkMovement_ovb0048* self) {
    UnkMovementOwner_ovb0048* owner = self->owner;
    if (!owner->animation)
        return false;
    UnkReflObject* cc = owner->char_class;
    if (!cc)
        return false;
    if (!IsCharClass(cc))
        return false;
    if (self->field_0x1f0)
        return true;
    return fn_710117b348(self->owner->animation, fn_7100d07ac8());
}

void fn_71010d17c0(UnkMovement_ovb0048* self) {
    if (!self->vtable->vf_0x180(self))
        return;
    if (self->owner) {
        if (UnkAnimationComponent* animation = self->owner->animation) {
            if (fn_710117b348(animation, fn_7100cf96a8()))
                return;
        }
    }
    f32 f = self->field_0x108;
    f32 t = f * 0.0f;
    f32 x = self->field_0x208 + t;
    f32 y = self->field_0x20c - f;
    f32 z = t + self->field_0x210;
    self->field_0x208 = x;
    self->field_0x20c = y;
    self->field_0x210 = z;
}

struct UnkContactShape_ovb0048 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkContact_ovb0048 {
    UnkContactShape_ovb0048* shape;
    u8 pad_0x8[0x18];
    f32 normal_x, normal_y, normal_z;  // 0x20
};

static inline UnkVec3f Cross(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    UnkVec3f r;
    r.x = ay * bz - az * by;
    r.y = az * bx - ax * bz;
    r.z = ax * by - ay * bx;
    return r;
}

UnkVec3f fn_71010d252c(bool flag, const UnkContact_ovb0048* contact) {
    f32 x = 0.0f, y = 1.0f, z = 0.0f;
    if (contact && !flag && contact->shape && contact->shape->field_0x10) {
        x = -contact->normal_x;
        y = -contact->normal_y;
        z = -contact->normal_z;
    }
    return Cross(x, y, z, 0.0f, 0.0f, 1.0f);
}

struct UnkAssignedSlot;
struct UnkStruct_71007142e8;
void fn_71007142e8(UnkStruct_71007142e8* self);
void fn_7101179d08(UnkAnimationComponent* self, UnkStruct_71007142e8* motion, const void* key);
extern u8 g_unk_717270ec68;

// A reference to a counted object, assigned out of line (fn_71002a5d1c).
struct UnkSlot_ovb0048 {
    base::UnkRefCounted* ptr;

    UnkSlot_ovb0048() : ptr(nullptr) {}
    ~UnkSlot_ovb0048() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }

    UnkSlot_ovb0048& operator=(const UnkSlot_ovb0048& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, then the point, a
// second vector and the normal (+0x2c), and three words, all copied as words.
struct UnkFloorRef_ovb0048 {
    UnkSlot_ovb0048 slot_a;
    UnkSlot_ovb0048 slot_b;
    u32 field_0x10[13];

    UnkFloorRef_ovb0048() {
        field_0x10[0] = 0xffffffff;
        for (u32 i = 1; i < 13; i++)
            field_0x10[i] = 0;
    }
    const UnkVec3f& Point() const { return *reinterpret_cast<const UnkVec3f*>(&field_0x10[1]); }

    UnkFloorRef_ovb0048& operator=(const UnkFloorRef_ovb0048& other) {
        if (this != &other) {
            slot_a = other.slot_a;
            slot_b = other.slot_b;
            for (u32 i = 0; i < 13; i++)
                field_0x10[i] = other.field_0x10[i];
        }
        return *this;
    }
};

// The motion fn_71007142e8 sets up, read as a transform.
struct alignas(8) UnkMotion_ovb0048 {
    u8 data[0x70];
    UnkMotion_ovb0048() { fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(this)); }
    const UnkMatrix44& Matrix() const { return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(this)); }
};

struct UnkMoveOwner_ovb0048 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkAnimationComponent* animation;  // 0x220

    const UnkMatrix44& Matrix() const { return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&xform)); }
};

struct UnkMovement2_ovb0048 {
    u8 pad_0x0[0x30];
    UnkMoveOwner_ovb0048* owner;  // 0x30
    u8 pad_0x38[0x178 - 0x38];
    UnkFloorRef_ovb0048 floor;  // 0x178
    bool field_0x1c0;
};
static_assert(__builtin_offsetof(UnkMovement2_ovb0048, field_0x1c0) == 0x1c0, "layout");

void fn_71010d14e4(UnkMovement2_ovb0048* self, const UnkFloorRef_ovb0048* floor, bool flag) {
    self->floor = *floor;
    self->field_0x1c0 = flag;
}

UnkVec3f fn_71010d2b08(UnkMovement2_ovb0048* self) {
    const UnkMatrix44& m = self->owner->Matrix();
    UnkVec3f v(m.m[2], m.m[6], m.m[10]);
    if (UnkAnimationComponent* animation = self->owner->animation) {
        UnkMotion_ovb0048 motion;
        fn_7101179d08(animation, reinterpret_cast<UnkStruct_71007142e8*>(&motion), &g_unk_717270ec68);
        v = fn_7100077c04(motion.Matrix(), v);
    }
    return v;
}

void fn_71010d2c5c(UnkMovement2_ovb0048* self, UnkVec3f* out) {
    UnkAnimationComponent* animation = self->owner->animation;
    UnkMotion_ovb0048 motion;
    fn_7101179d08(animation, reinterpret_cast<UnkStruct_71007142e8*>(&motion), &g_unk_717270ec68);
    const UnkMatrix44& a = self->owner->Matrix();
    UnkMatrix44 r = fn_71000127c8(a, motion.Matrix());
    *out = UnkVec3f(r.m[2] >= 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
}

struct UnkAnimMaskHolder_b47a3;
bool fn_7101179cbc(UnkAnimMaskHolder_b47a3* self, const u32* bits);
extern u8 g_unk_717270ebf0;

bool fn_71010d2b9c(UnkMovement2_ovb0048* self, UnkVec3f* out) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    alignas(8) u32 bits = 4;
    if (!fn_7101179cbc(reinterpret_cast<UnkAnimMaskHolder_b47a3*>(animation), &bits))
        return false;
    UnkMotion_ovb0048 motion;
    fn_7101179d08(animation, reinterpret_cast<UnkStruct_71007142e8*>(&motion), &g_unk_717270ebf0);
    const UnkMatrix44& a = self->owner->Matrix();
    UnkMatrix44 r = fn_71000127c8(a, motion.Matrix());
    *out = UnkVec3f(r.m[2] >= 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
    return true;
}

extern "C" float acosf(float);

struct UnkFloorCollection_ovb0048 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 GetType();
};

struct UnkFloorShape_ovb0048 {
    u8 pad_0x0[0x158];
    UnkFloorCollection_ovb0048* collection;
};

struct UnkFloorOwner_ovb0048 {
    u8 pad_0x0[0x10];
    UnkFloorShape_ovb0048* shape;
};

struct UnkFloorMovement_ovb0048;

struct UnkFloorMovementVtbl_ovb0048 {
    u8 pad_0x0[0x228];
    void (*vf_0x228)(UnkFloorMovement_ovb0048* self, const UnkFloorRef_ovb0048* floor, bool flag);
    u8 pad_0x230[0x268 - 0x230];
    void (*vf_0x268)(UnkFloorMovement_ovb0048* self);
    void (*vf_0x270)(UnkFloorMovement_ovb0048* self, bool on);
    void (*vf_0x278)(UnkFloorMovement_ovb0048* self, bool on);
};

struct UnkFloorMovement_ovb0048 {
    UnkFloorMovementVtbl_ovb0048* vtable;
    u8 pad_0x8[0x120];
    UnkFloorRef_ovb0048 floor;  // 0x128
    bool field_0x170;
    u8 pad_0x171[0x3];
    f32 slope;  // 0x174
    u8 pad_0x178[0x1c4 - 0x178];
    f32 last_slope;  // 0x1c4
};
static_assert(__builtin_offsetof(UnkFloorMovement_ovb0048, last_slope) == 0x1c4, "layout");

// Takes the floor the component stands on and the slope of its normal (against straight up).
void fn_71010d1204(UnkFloorMovement_ovb0048* self, const UnkFloorRef_ovb0048* floor) {
    self->floor = *floor;
    UnkFloorOwner_ovb0048* owner = reinterpret_cast<UnkFloorOwner_ovb0048*>(floor->slot_a.ptr);
    if (!owner || !owner->shape) {
        self->vtable->vf_0x270(self, false);
    } else {
        UnkFloorCollection_ovb0048* collection = owner->shape->collection;
        if (!collection || collection->GetType() == 2) {
            f32 slope = self->slope;
            self->slope = 0.0f;
            self->last_slope = slope;
            self->vtable->vf_0x270(self, false);
            self->vtable->vf_0x278(self, false);
        } else {
            const f32* n = reinterpret_cast<const f32*>(&floor->field_0x10[7]);
            f32 c = n[0] * 0.0f + n[1] + n[2] * 0.0f;
            c = c <= -1.0f ? -1.0f : (c > 1.0f ? 1.0f : c);
            self->last_slope = self->slope;
            f32 a = acosf(c);
            self->slope = a;
            bool at_limit = a - 0.001f <= 0.7853982f && a + 0.001f >= 0.7853982f;
            self->vtable->vf_0x270(self, at_limit);
            bool sloped = self->slope > 0.19634955f && self->slope < 0.7853982f;
            self->vtable->vf_0x278(self, sloped);
        }
        self->vtable->vf_0x228(self, floor, self->field_0x170);
    }
    self->vtable->vf_0x268(self);
}

struct UnkProbeShape_ovb0048 {
    u8 pad_0x0[0x60];
    f32 height;
};

struct UnkProbeOwner_ovb0048 {
    u8 pad_0x0[0x228];
    UnkProbeShape_ovb0048* shape;  // 0x228
    u8 pad_0x230[0x378 - 0x230];
    u32 collision_mask;  // 0x378
};

struct UnkProbeMovement_ovb0048 {
    u8 pad_0x0[0x30];
    UnkProbeOwner_ovb0048* owner;  // 0x30
};

// The distance from `pos` down to the floor (looked for at most `depth` below it).
f32 fn_71010d0e88(UnkProbeMovement_ovb0048* self, const UnkVec3f* pos, f32 depth) {
    UnkProbeOwner_ovb0048* owner = self->owner;
    if (!owner->shape)
        return 1.7446744e19f;
    void* tree = g_unk_7101d47310->field_0x608->field_0x820;
    UnkFloorRef_ovb0048 hit;
    u32 mask = owner->collision_mask;
    UnkVec3f end(pos->x, pos->y - depth, pos->z);
    f32 distance;
    if (fn_7100d5b714(tree, mask, pos, &end, &hit, true, false, 0.1f))
        distance = __builtin_fabsf(pos->y - hit.Point().y);
    else
        distance = 1.7446744e19f;
    return distance;
}

struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

inline const UnkFlags_71007142e8& UnkFlagsNone_71007142e8() {
    static UnkFlags_71007142e8 s_none;
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_71007142e8() {
    static UnkFlags_71007142e8 s_all(~UnkFlagsNone_71007142e8().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8;

// Three motions (0x70 bytes each) reset together.
struct UnkMotions_ovb0048 {
    u8 motion[3][0x70];
};

void fn_71010d1f1c(UnkMotions_ovb0048* self) {
    fn_71007143c8(reinterpret_cast<UnkStruct_71007142e8*>(self->motion[0]), UnkFlagsAll_71007142e8());
    fn_71007143c8(reinterpret_cast<UnkStruct_71007142e8*>(self->motion[1]), UnkFlagsAll_71007142e8());
    fn_71007143c8(reinterpret_cast<UnkStruct_71007142e8*>(self->motion[2]), UnkFlagsAll_71007142e8());
}

struct UnkMoveAnim_ovb0048 {
    u8 pad_0x0[0x108];
    UnkMotions_ovb0048 motions;  // 0x108
};

struct UnkMoveOwner3_ovb0048 {
    u8 pad_0x0[0x220];
    UnkMoveAnim_ovb0048* animation;  // 0x220
};

struct UnkMovement3_ovb0048 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238(UnkMotions_ovb0048* motions, void* a, void* b, void* c);
    virtual void vf_0x240(UnkMotions_ovb0048* motions, void* a, f32 t, void* d, void* e);
    u8 pad_0x8[0x28];
    UnkMoveOwner3_ovb0048* owner;  // 0x30
};

void fn_71010d1e88(UnkMovement3_ovb0048* self, void* a, void* b, f32 t, void* c, void* d, void* e) {
    UnkMotions_ovb0048* motions = &self->owner->animation->motions;
    self->vf_0x238(motions, a, b, c);
    self->vf_0x240(motions, a, t, d, e);
    fn_71010d1f1c(motions);
}


struct UnkWeakTarget;

// A string id built from a string out of line (fn_7100000850), released like CStrId.
struct UnkBuiltStrId_ovb0048 {
    union {
        CStrId id;
    };

    explicit UnkBuiltStrId_ovb0048(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltStrId_ovb0048() { id.~CStrId(); }
};

struct UnkNameData_ovb0048 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkFoundSide_ovb0048 {
    u8 pad_0x0[0x58];
    f32 side;
};

struct UnkFoundEntity_ovb0048 {
    u8 pad_0x0[0x1f0];
    UnkFoundSide_ovb0048* field_0x1f0;
};

struct UnkObj_71010d3508;

struct UnkMovementSide_ovb0048 {
    u8 pad_0x0[0x58];
    f32 side;
};

// Whether the entity named `name` faces the other way from this component (signs of the floats
// at +0x58 of both differ).
bool fn_71010d32ec(UnkObj_71010d3508* obj, const CRntString& name) {
    UnkMovementSide_ovb0048* self = reinterpret_cast<UnkMovementSide_ovb0048*>(obj);
    UnkGameManager* manager = g_unk_7101d47310;
    UnkFoundEntity_ovb0048* entity;
    u64 key;
    alignas(8) u32 kind;
    {
        UnkBuiltStrId_ovb0048 built(name);
        u32 kind_value = g_unk_71727bebb8;
        key = reinterpret_cast<UnkNameData_ovb0048*>(built.id.data)->hash;
        kind = kind_value;
        entity = reinterpret_cast<UnkFoundEntity_ovb0048*>(fn_7100debbc8(manager, &key, &kind));
    }
    if (!entity || !entity->field_0x1f0)
        return false;
    f32 side = entity->field_0x1f0->side;
    if (side > 0.0f && self->side < 0.0f)
        return true;
    if (side < 0.0f)
        return self->side > 0.0f;
    return false;
}


struct UnkResetMovement_ovb0048;

struct UnkResetMovementVtbl_ovb0048 {
    u8 pad_0x0[0x208];
    void (*vf_0x208)(UnkResetMovement_ovb0048* self, const UnkVec3f* dir);
};

struct UnkResetMovement_ovb0048 {
    UnkResetMovementVtbl_ovb0048* vtable;
    u8 pad_0x8[0x7c];
    UnkVec3f position;  // 0x84
    UnkVec3f angle;     // 0x90
    u8 pad_0x9c[0x10c - 0x9c];
    UnkVec3f velocity;  // 0x10c
    f32 speed;          // 0x118
};
static_assert(__builtin_offsetof(UnkResetMovement_ovb0048, speed) == 0x118, "layout");

// Takes the position (bit 0: velocity cleared) and the angles and facing (bit 1) of a transform.
void fn_71010d45b8(UnkResetMovement_ovb0048* self, UnkXform* xform, const u32* what) {
    if (*what & 1) {
        self->position = xform->GetTranslation();
        self->velocity = *reinterpret_cast<const UnkVec3f*>(&g_unk_7101647790);
        self->speed = sqrtf(self->velocity.x * self->velocity.x + self->velocity.y * self->velocity.y +
                            self->velocity.z * self->velocity.z);
    }
    if (*what & 2) {
        self->angle = xform->GetAngle();
        const UnkMatrix44& m = xform->GetMatrix();
        UnkVec3f dir(m.m[2], m.m[6], m.m[10]);
        self->vtable->vf_0x208(self, &dir);
    }
}

// A vector whose x and y form a two-component vector of their own.
struct UnkVec2Part_ovb0048 {
    f32 x, y;
};

struct UnkSplitVec_ovb0048 {
    UnkVec2Part_ovb0048 xy;
    f32 z;

    void Clear() {
        __builtin_memset(this, 0, sizeof(*this));
    }
};

struct UnkBlendStep_ovb0048 {
    UnkVec3f velocity;
    UnkVec3f scale;
    f32 time;

    UnkBlendStep_ovb0048() {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        velocity.z = 0.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        time = 0.0f;
    }
};

// A transform assigned word by word.
static inline void AssignXform(const UnkXform& src, UnkXform* dst) {
    if (&src != dst) {
        UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(dst->m), *reinterpret_cast<const UnkMatrix44*>(src.m));
        dst->angle = src.angle;
        dst->scale = src.scale;
        dst->flags = src.flags;
    }
}

struct UnkResetParams_ovb0048 {
    u8 pad_0x0[0x60];
    u32 field_0x60;
};

struct UnkResetOwner_ovb0048 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkResetMovement2_ovb0048 {
    u8 pad_0x0[0x30];
    UnkResetOwner_ovb0048* owner;  // 0x30
    u8 pad_0x38[0x9c - 0x38];
    UnkXform last_xform;  // 0x9c
    u8 pad_0xf8[0x0];
    bool field_0xf8;
    bool flying;  // 0xf9
    u8 pad_0xfa[0x108 - 0xfa];
    u32 field_0x108;
    UnkSplitVec_ovb0048 velocity;  // 0x10c
    f32 speed;                     // 0x118
    UnkSplitVec_ovb0048 last_velocity;  // 0x11c
    u8 pad_0x128[0x1d8 - 0x128];
    UnkVec3f field_0x1d8;
    UnkVec3f field_0x1e4;
    u8 pad_0x1f0[0x8];
    UnkResetParams_ovb0048* params;  // 0x1f8
    u8 pad_0x200[0x8];
    UnkVec3f field_0x208;
    UnkBlendStep_ovb0048 step;  // 0x214
};
static_assert(__builtin_offsetof(UnkResetMovement2_ovb0048, step) == 0x214, "layout");

void fn_71010d2f84(UnkResetMovement2_ovb0048* self, bool) {
    self->field_0x1d8 = UnkVec3f(0.0f, 0.0f, 0.0f);
    self->field_0x1e4 = UnkVec3f(0.0f, 0.0f, 0.0f);
    self->field_0x208 = UnkVec3f(0.0f, 0.0f, 0.0f);
    self->field_0xf8 = true;
    UnkAnimationComponent* animation;
    if (self->owner && (animation = self->owner->animation) && fn_710117b348(animation, fn_7100cf96a8()))
        self->flying = true;
    else
        self->flying = false;
    self->velocity.Clear();
    self->last_velocity.Clear();
    AssignXform(self->owner->xform, &self->last_xform);
    self->field_0x108 = self->params->field_0x60;
    self->step = UnkBlendStep_ovb0048();
}

UnkVec3f fn_7100714d78(const UnkXform* self, const UnkMatrix44* m);

struct UnkWeightedXform_ovb0048 {
    u8 pad_0x0[0x6c];
    f32 time;
};

struct UnkBlender_ovb0048;

// The speed that takes `from` (moved by the motion `target` gives through `m`) to `to` over the
// motion's time (at most `max_time`).
UnkBlendStep_ovb0048 fn_71010d432c(UnkBlender_ovb0048* self, f32 max_time, const UnkWeightedXform_ovb0048* target,
                                   const UnkVec3f* from, const UnkMatrix44* m, const UnkVec3f* to) {
    UnkBlendStep_ovb0048 r;
    f32 t = target->time;
    if (t > 1.1920929e-7f) {
        if (t > max_time)
            t = max_time;
        r.time = t;
        UnkVec3f d = fn_7100714d78(reinterpret_cast<const UnkXform*>(target), m);
        f32 ax = from->x + d.x;
        f32 ay = from->y + d.y;
        f32 az = from->z + d.z;
        f32 bx = to->x - ax;
        f32 by = to->y - ay;
        f32 bz = to->z - az;
        f32 inv = 1.0f / t;
        r.velocity.x = inv * bx;
        r.velocity.y = inv * by;
        r.velocity.z = inv * bz;
    }
    return r;
}
