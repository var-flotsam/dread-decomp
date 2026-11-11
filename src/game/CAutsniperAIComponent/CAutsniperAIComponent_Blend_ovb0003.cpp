// CAutsniperAIComponent's unit (tools/tu_map.py unit 579):
// - fn_71003dabd8: the update step: the behaviour tree's step (fn_7100369c44), then the blend at
//   +0x27230 advances over the time its parameters give (+0x520): state 0 blends the layer weight
//   down to 0 (fn_71003db3cc), state 1 up to 1 (fn_71003db2c4); then fn_71003dace0.
// - fn_71003dace0: sets the mover's velocity along the owner's x axis.
// - fn_71003db6e0: on a state change from 2 to 1, raises a flag of the owner's object at +0x240.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector4D.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU2523.h"
#include "types.h"

struct UnkAutsniperParams_ovb0003 {
    u8 pad_0x0[0x520];
    f32 blend_time;  // 0x520
    f32 speed;       // 0x524
};

struct UnkAutsniperBlend_ovb0003 {
    UnkAutsniperParams_ovb0003* params;
    u8 pad_0x8[0x14];
    s32 state;    // 0x1c
    f32 weight;   // 0x20
    f32 elapsed;  // 0x24
};

// As in src/game/CKraidBouncingCreaturesMovementComponent/CKraidBouncingCreaturesMovementComponent_3.cpp.
struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkAutsniperMover_ovb0003 {
    u8 pad_0x0[0x14c];
    base::math::CVector4D velocity;  // 0x14c
    u8 pad_0x15c[0x94];
    struct {
        u8 pad_0x0[0x30];
        base::math::CVector4D* velocity;  // 0x30
    }* mirror;  // 0x1f0
};

struct UnkAutsniperLink48_ovb0003 {
    u8 pad_0x0[0x48];
    UnkAutsniperMover_ovb0003** mover;  // 0x48
};

struct UnkAutsniperLink78_ovb0003 {
    u8 pad_0x0[0x78];
    UnkAutsniperLink48_ovb0003* field_0x78;
};

struct UnkAutsniperLink138_ovb0003 {
    u8 pad_0x0[0x138];
    UnkAutsniperLink78_ovb0003* field_0x138;
};

struct UnkAutsniperTarget_ovb0003 {
    u8 pad_0x0[0x50];
    UnkAutsniperLink138_ovb0003* field_0x50;
};

struct UnkAutsniperFlags_ovb0003 {
    u8 pad_0x0[0x14689];
    bool field_0x14689;
};

struct UnkAutsniperOwner_ovb0003 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1a8];
    void* field_0x220;
    UnkAutsniperTarget_ovb0003* field_0x228;
    u8 pad_0x230[0x10];
    UnkAutsniperFlags_ovb0003* field_0x240;
};

struct UnkAutsniperAI_ovb0003 {
    u8 pad_0x0[0x30];
    UnkAutsniperOwner_ovb0003* owner;  // 0x30
    u8 pad_0x38[0x271f8];
    UnkAutsniperBlend_ovb0003 blend;  // 0x27230
};

// The layer forwarders of CAutsniperAIComponent_LayerLookup_b55.cpp.
struct UnkSelfB55;
void fn_71003db2c4(UnkSelfB55* self, f32 value);
void fn_71003db3cc(UnkSelfB55* self, f32 value);
void fn_71003dace0(UnkAutsniperAI_ovb0003* self);

struct UnkVec3_ovb0003 {
    f32 x, y, z;
};

static inline f32 Dot(const UnkVec3_ovb0003& a, const UnkVec3_ovb0003& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

// The matrix's column `i` as a vector.
static inline UnkVec3_ovb0003 Column(const UnkMatrix44& m, s32 i) { return {m.m[i], m.m[i + 4], m.m[i + 8]}; }

static inline f32 Lerp(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

void fn_71003dabd8(UnkAutsniperAI_ovb0003* self, f32 dt) {
    UnkAutsniperBlend_ovb0003& blend = self->blend;
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    blend.elapsed += dt;
    f32 t = blend.elapsed / blend.params->blend_time;
    switch (blend.state) {
    case 1:
        if (blend.weight < 1.0f) {
            f32 w = Lerp(0.0f, 1.0f, t);
            blend.weight = w > 1.0f ? 1.0f : w;
            fn_71003db2c4(reinterpret_cast<UnkSelfB55*>(self), blend.weight);
        }
        break;
    case 0:
        if (blend.weight > 0.0f) {
            f32 w = Lerp(1.0f, 0.0f, t);
            blend.weight = w < 0.0f ? 0.0f : w;
            fn_71003db3cc(reinterpret_cast<UnkSelfB55*>(self), blend.weight);
        }
        break;
    }
    fn_71003dace0(self);
}

// Sets the mover's velocity: along the owner's x axis at the parameters' speed, away from the
// target position fn_7100d7be08 gives (when it is more than 1 away), flipped by the side the owner
// faces; zero otherwise.
void fn_71003dace0(UnkAutsniperAI_ovb0003* self) {
    UnkAutsniperOwner_ovb0003* owner = self->owner;
    UnkAutsniperTarget_ovb0003* target = owner->field_0x228;
    if (!target || !target->field_0x50)
        return;
    UnkAutsniperMover_ovb0003* mover = *target->field_0x50->field_0x138->field_0x78->mover;
    if (!mover)
        return;
    base::math::CVector4D velocity = {0.0f, 0.0f, 0.0f, 0.0f};
    UnkKraidCreatureVec v = fn_7100d7be08(owner);
    if (v.x * v.x + v.y * v.y + v.z * v.z > 1.0f) {
        const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
        UnkVec3_ovb0003 axis = Column(m, 2);
        f32 d = axis.x * v.x + axis.y * v.y + axis.z * v.z;
        f32 speed = self->blend.params->speed;
        velocity.x = speed;
        if (!(d >= 0.0f))
            velocity.x = -speed;
        const UnkMatrix44& m2 = fn_710002614c(&self->owner->transform);
        if (Dot(Column(m2, 0), {0.0f, 0.0f, 1.0f}) >= 0.0f)
            velocity.x = -velocity.x;
    }
    fn_710007b518(&mover->velocity, &velocity);
    if (mover->mirror)
        *mover->mirror->velocity = velocity;
}

void fn_71003db6e0(UnkAutsniperAI_ovb0003* self, s32 from, s32 to) {
    if (from != to) {
        if (from == 2 && to == 1)
            self->owner->field_0x240->field_0x14689 = true;
    }
}
