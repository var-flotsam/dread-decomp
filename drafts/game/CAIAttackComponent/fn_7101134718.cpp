// CAIAttackComponent unit (tools/tu_map.py unit 2705): after a hit (fn_7101136f28), when the owner's
// AttackFailAction track names an action, plays it and turns the owner: to face sideways along the hit (and,
// for a hit that asks for it, moved onto the target's height), else upright along its own facing. A move is
// swept through the collision world first and stops short of what it would hit. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "../../../src/game_s2/UnkSpitSend.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);
UnkVec3f RotateVec3ByAngles(const UnkVec3f& v, const UnkMatrix44& m);
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// Directions in the owner's frame (turned by its angles) and the origin of the basis built from them.
__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164cfb8;  // forward
__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164cfd0;  // up
__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164cfc4;  // origin

struct UnkHitObj_l134718 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l134718 {
    UnkHitObj_l134718* ptr;

    UnkHitRef_l134718() : ptr(nullptr) {}
    ~UnkHitRef_l134718() {
        if (UnkHitObj_l134718* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitVec_l134718 {
    f32 x, y, z;
    UnkHitVec_l134718(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l134718 {
    UnkHitRef_l134718 object;
    UnkHitRef_l134718 field_0x8;
    s32 id;
    UnkHitVec_l134718 point;
    UnkHitVec_l134718 field_0x20;
    UnkHitVec_l134718 normal;
    f32 fraction;  // how far along the sweep the hit is
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l134718()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), fraction(0),
          flags(0), field_0x40(0) {}
};

// The hit query the sweep fills.
struct UnkSweepQuery_l134718 {
    void** vtable;
    UnkCollisionHit_l134718 hit;
    bool field_0x50;

    UnkSweepQuery_l134718() {
        field_0x50 = false;
        vtable = g_unk_7101a23590 + 2;
    }
    ~UnkSweepQuery_l134718() { vtable = g_unk_7101a20ed8 + 2; }
};

// What fn_7100976588 returns: the collider whose shape is at +0x158.
struct UnkCollider_l134718 {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

// A sweep offset: the x and y of a 3D one.
struct UnkSweepOffset_l134718 {
    base::math::CVector2D v;

    // From one point to another.
    UnkSweepOffset_l134718(const UnkVec3f& from, const UnkVec3f& to) {
        base::math::CVector3D offset = {to.x - from.x, to.y - from.y, to.z - from.z};
        fn_710007b430(&v, &offset);
    }
};

// A 3D vector for the turn's arithmetic.
struct UnkTurnVec_l134718 {
    f32 x, y, z;

    UnkTurnVec_l134718(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    f32 LengthSq() const { return x * x + y * y + z * z; }
    UnkTurnVec_l134718 operator*(f32 s) const { return UnkTurnVec_l134718(x * s, y * s, z * s); }
    UnkTurnVec_l134718 Normalized() const { return *this * (1.0f / __builtin_sqrtf(LengthSq())); }
};

static inline UnkTurnVec_l134718 Cross(const UnkTurnVec_l134718& a, const UnkTurnVec_l134718& b) {
    return UnkTurnVec_l134718(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

static inline bool Near(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

static inline void SetIdentity(UnkMatrix44* m) {
    *m = UnkMatrix44{};
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

// The id table's slot at +0xd38 (the header names its neighbours only): its string's key.
inline const base::global::CStrId& TurnKeyId_l134718() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return (&s_table.field_0xd30)[1];
}

static inline u64 TurnKey() {
    return static_cast<const base::global::UnkHashedString*>(TurnKeyId_l134718().data)->Key();
}

struct UnkTurnAction_l134718 {
    u8 pad_0x0[0x30];
    u64 id;  // ~0: none
};

// The record the AttackFailAction track resolves: the action for a hit and the one without.
struct UnkTurnRecord_l134718 {
    u8 pad_0x0[0x20];
    UnkTurnAction_l134718* on_hit;
    UnkTurnAction_l134718* on_miss;
};

struct UnkTurnHit_l134718 {
    u8 pad_0x0[0x28];
    bool to_target_height;
};

struct UnkTurnProbe_l134718 {
    u8 data[0x40];
};

struct UnkTurnMovement_l134718 {
    virtual void vf_0x0();
    UnkTurnProbe_l134718 vf_0x198(const UnkVec3f* pos, void* a, void* b, f32 range) {
        return reinterpret_cast<UnkTurnProbe_l134718 (*)(UnkTurnMovement_l134718*, const UnkVec3f*, void*, void*,
                                                         f32)>((*reinterpret_cast<void***>(this))[0x198 / 8])(
            this, pos, a, b, range);
    }
};

struct UnkTurnTarget_l134718 {
    u8 pad_0x0[0x54];
    f32 y;
    u8 pad_0x58[0x1f0 - 0x58];
    UnkTurnMovement_l134718* movement;
};

struct UnkTurnOwner_l134718 {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x1f0 - 0x94];
    UnkReflObject* movement;
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkAnimationComponent* anim;
    u8 pad_0x228[0x3b0 - 0x228];
    u8 moved;
    u8 pad_0x3b1[0x3c0 - 0x3b1];
    u8 turned;
};
static_assert(__builtin_offsetof(UnkTurnOwner_l134718, turned) == 0x3c0, "layout");

struct UnkAIAttackComponent_l134718 {
    u8 pad_0x0[0x30];
    UnkTurnOwner_l134718* owner;
    u8 pad_0x38[0x198 - 0x38];
    u32 field_0x198;
    u8 pad_0x19c[0x1ac - 0x19c];
    u32 field_0x1ac;
    u8 pad_0x1b0[0x2cc - 0x1b0];
    u32 fail_count;
};

void fn_7101136f28(UnkAIAttackComponent_l134718* self, UnkTurnTarget_l134718* target, UnkTurnHit_l134718* hit);

void fn_7101134718(UnkAIAttackComponent_l134718* self, UnkTurnTarget_l134718* target, UnkTurnHit_l134718* hit) {
    fn_7101136f28(self, target, hit);
    UnkTrackQuery query = {fn_7100cecf98(), nullptr, -1.0f};
    base::reflection::UnkReflTypeIdArg mode;  // what the action is played with
    base::reflection::UnkReflTypeIdArg mask;
    fn_710117b08c(self->owner->anim, &query);
    if (!query.result)
        return;
    self->fail_count++;
    self->field_0x1ac = self->field_0x198;
    UnkTurnRecord_l134718* record = reinterpret_cast<UnkTurnRecord_l134718*>(query.result);
    u64 id = (hit ? record->on_hit : record->on_miss)->id;
    if (id == ~0ull)
        return;
    // Plays the action (mode 0, under the AttackFail kind hash).
    void* anim = self->owner->anim;
    mode.id = 0;
    mask.id = UnkAll_SpitSend().value;
    if (!CAnimationComponent_SendActionToTrack(anim, id, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask))
        return;

    UnkTurnOwner_l134718* owner = self->owner;
    UnkVec3f pos = owner->xform.GetTranslation();
    UnkVec3f angle = owner->xform.GetAngle();
    UnkVec3f forward = RotateVec3ByAngles(g_unk_710164cfb8, *reinterpret_cast<const UnkMatrix44*>(&angle));
    UnkVec3f moved = pos;
    UnkVec3f turned;
    if (hit) {
        // Sideways along the hit, upright.
        turned = UnkVec3f(0.0f, forward.x < 0.0f ? -1.5707964f : 1.5707964f, 0.0f);
        if (hit->to_target_height) {
            if (target->movement)
                target->movement->vf_0x198(&moved, nullptr, nullptr, 1000.0f);
            UnkMatrix44 m;
            SetIdentity(&m);
            anim = self->owner->anim;
            if (fn_7101179de0(static_cast<UnkAnimActionSetsOwner*>(anim), reinterpret_cast<void*>(id), TurnKey(),
                              0.0f, reinterpret_cast<UnkMtx44_b46a2*>(&m), 2)) {
                m = fn_71000127c8(self->owner->xform.GetMatrix(), m);
                moved.y += target->y - m.m[7];
            }
        }
    } else {
        // Upright along its own facing.
        turned = angle;
        UnkTurnVec_l134718 fwd(forward.x, forward.y, 0.0f);
        if (fwd.LengthSq() > 1.1920929e-07f) {
            UnkVec3f up3 = RotateVec3ByAngles(g_unk_710164cfd0, *reinterpret_cast<const UnkMatrix44*>(&angle));
            UnkTurnVec_l134718 up(up3.x, up3.y, 0.0f);
            if (up.LengthSq() != 0.0f) {
                UnkTurnVec_l134718 f = fwd.Normalized();
                UnkTurnVec_l134718 u = up.Normalized();
                UnkTurnVec_l134718 side = Cross(u, f);
                UnkMatrix44 basis;
                fn_71000769f0(&basis, &side, &u, &f, &g_unk_710164cfc4);
                turned = fn_7100079b18(reinterpret_cast<UnkXform*>(&basis), angle.z);
            }
        }
    }

    if (!Near(pos.x, moved.x) || !Near(pos.y, moved.y) || !Near(pos.z, moved.z)) {
        UnkGameManager* gm = g_unk_7101d47310;
        if (gm->field_0x608) {
            if (void* tracker = gm->field_0x608->field_0x820) {
                UnkReflObject* movement = self->owner->movement;
                UnkFlagged* flagged = movement && fn_710009d50c(movement->vf_0x0(), fn_7100975b3c())
                                          ? reinterpret_cast<UnkFlagged*>(movement)
                                          : nullptr;
                UnkCollider_l134718* collider = reinterpret_cast<UnkCollider_l134718*>(fn_7100976588(flagged));
                UnkShape* shape;
                if (collider && (shape = collider->shape)) {
                    UnkSweepOffset_l134718 sweep(pos, moved);
                    UnkSweepQuery_l134718 sweep_hit;
                    UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(pos);
                    if (fn_7100d5babc(static_cast<UnkTree*>(tracker), 3, shape,
                                      reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&sweep.v),
                                      &sweep_hit, false)) {
                        // Stops short of the hit.
                        UnkTurnVec_l134718 step = UnkTurnVec_l134718(sweep.v.x, sweep.v.y, 0.0f) *
                                                  sweep_hit.hit.fraction * 0.9f;
                        moved = UnkVec3f(pos.x + step.x, pos.y + step.y, pos.z + step.z);
                    }
                }
            }
        }
    }

    self->owner->xform.SetTranslation(moved);
    self->owner->moved = true;
    self->owner->xform.SetAngle(turned);
    self->owner->turned = true;
}
