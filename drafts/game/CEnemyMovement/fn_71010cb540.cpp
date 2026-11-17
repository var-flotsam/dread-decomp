// CEnemyMovement unit: after the character movement's own contact step (fn_7100977974), takes the ledge the
// contact state holds or leaves the ground; then, while still grounded, probes the ground below (fn_7100977e50)
// and, on a hit it may stand on, slides the velocity along the ground and holds the new ground. Without one it
// falls unless the owner's class is in a track that ignores relocation. Names and layouts are placeholders.
#include <float.h>
#include "base/reflection/UnkClassGetters.h"
#include "base/math/CVector.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCharacterMovement;
struct UnkCollisionHit;
struct UnkCharacterMovementHit_ovb0021;
struct UnkMoveHitList_ovb0021;
struct UnkMoveContactState_ovb0021;
struct UnkEnemyMovement_l0cb838;
struct UnkLedgeRef_l0cb838;

bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);
void fn_7100977974(UnkCharacterMovementHit_ovb0021* self, UnkMoveHitList_ovb0021* hits, void*, void*, void*,
                   UnkMoveContactState_ovb0021* state);
void fn_71010cb838(UnkEnemyMovement_l0cb838* self, UnkLedgeRef_l0cb838** slot);

struct UnkGroundRef_l0cb540 {
    UnkRefObject* ptr;

    UnkGroundRef_l0cb540() : ptr(nullptr) {}
    ~UnkGroundRef_l0cb540() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkGroundVec_l0cb540 {
    f32 x, y, z;

    UnkGroundVec_l0cb540(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit (0x48 bytes): the object hit, a second reference, an id, the point and two vectors.
struct UnkGroundHit_l0cb540 {
    UnkGroundRef_l0cb540 object;
    UnkGroundRef_l0cb540 field_0x8;
    s32 id;
    UnkGroundVec_l0cb540 point;
    UnkGroundVec_l0cb540 normal;  // 0x20
    UnkGroundVec_l0cb540 field_0x2c;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkGroundHit_l0cb540()
        : id(-1), point(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x2c(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkGroundHit_l0cb540) == 0x48, "UnkGroundHit_l0cb540 layout");

struct UnkGroundRefHolder_l0cb540 {
    u8 pad_0x0[0x10];
    void* owner;  // 0x10
};

// The contact state of one step: the velocity, its part along the ground, the position and the ledge held.
struct UnkGroundState_l0cb540 {
    u8 pad_0x0[0x20];
    CVector3D velocity;  // 0x20
    CVector3D slide;     // 0x2c
    CVector3D pos;       // 0x38
    u8 pad_0x44[0x960 - 0x44];
    UnkGroundRefHolder_l0cb540* ledge;  // 0x960
};
static_assert(__builtin_offsetof(UnkGroundState_l0cb540, ledge) == 0x960, "layout");

struct UnkGroundContext_l0cb540 {
    u8 pad_0x0[0x50];
    UnkCharacterMovement* movement;  // 0x50
};

struct UnkGroundOwner_l0cb540 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkGroundConfig_l0cb540 {
    u8 pad_0x0[0x1c4];
    f32 probe_extra;  // 0x1c4
};

struct UnkGroundMovement_l0cb540 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkGroundOwner_l0cb540* owner;  // 0x30
    u8 pad_0x38[0xc0];
    bool slide_only;  // 0xf8
    bool falling;     // 0xf9
    u8 pad_0xfa[0x3a8 - 0xfa];
    bool ignore_relocate;  // 0x3a8
    u8 pad_0x3a9[0x3d0 - 0x3a9];
    u32 mask;  // 0x3d0
    u8 pad_0x3d4[0x1c10 - 0x3d4];
    bool probe_ground;  // 0x1c10
    u8 pad_0x1c11[7];
    UnkGroundConfig_l0cb540* config;  // 0x1c18

    UnkVec3f vf_0x1a8() { return reinterpret_cast<UnkVec3f (*)(UnkGroundMovement_l0cb540*)>(vtable[0x1a8 / 8])(this); }
    void vf_0x218(const void* hit) {
        reinterpret_cast<void (*)(UnkGroundMovement_l0cb540*, const void*)>(vtable[0x218 / 8])(this, hit);
    }
    void vf_0x220() { reinterpret_cast<void (*)(UnkGroundMovement_l0cb540*)>(vtable[0x220 / 8])(this); }
    void vf_0x260() { reinterpret_cast<void (*)(UnkGroundMovement_l0cb540*)>(vtable[0x260 / 8])(this); }
    void vf_0x2b8() { reinterpret_cast<void (*)(UnkGroundMovement_l0cb540*)>(vtable[0x2b8 / 8])(this); }

    void SetFalling(bool value) {
        if (falling != value) {
            falling = value;
            vf_0x260();
        }
    }
    void SetIgnoreRelocate(bool value) {
        if (ignore_relocate != value) {
            ignore_relocate = value;
            vf_0x2b8();
        }
    }
};
static_assert(__builtin_offsetof(UnkGroundMovement_l0cb540, config) == 0x1c18, "layout");

void fn_71010cb540(UnkGroundMovement_l0cb540* self, UnkMoveHitList_ovb0021* hits, UnkGroundContext_l0cb540* context,
                   void* arg3, void* arg4, UnkGroundState_l0cb540* state) {
    fn_7100977974(reinterpret_cast<UnkCharacterMovementHit_ovb0021*>(self), hits, context, arg3, arg4,
                  reinterpret_cast<UnkMoveContactState_ovb0021*>(state));
    if (fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self)))
        return;

    if (state->ledge && state->ledge->owner) {
        self->vf_0x218(&state->ledge);
        self->SetFalling(false);
    } else {
        self->vf_0x220();
    }
    if (self->falling)
        return;

    UnkGroundHit_l0cb540 hit;
    UnkAnimationComponent* animation = self->owner->animation;
    UnkCharacterMovement* movement = context->movement;
    u32 mask = self->mask;
    const CVector3D* pos = &state->pos;
    UnkVec3f dir = self->vf_0x1a8();
    if (fn_7100977e50(movement, -0.3f, self->config->probe_extra, FLT_MAX, 0.0f, mask, pos, nullptr,
                      reinterpret_cast<UnkCollisionHit*>(&hit), reinterpret_cast<const CVector3D*>(&dir)) &&
        self->probe_ground) {
        CVector3D up = {-hit.normal.x, -hit.normal.y, -hit.normal.z};
        CVector3D& v = state->velocity;
        f32 into = v.x * up.x + v.y * up.y + v.z * up.z;
        if (self->slide_only) {
            f32 push = !(into < 0.0f) ? into : 0.0f;
            state->slide.x = 0.0f;
            state->slide.y = 0.0f;
            state->slide.z = 0.0f;
            v.x = push * up.x;
            v.y = push * up.y;
            v.z = push * up.z;
        } else {
            CVector3D slide = {v.x + into * hit.normal.x, v.y + into * hit.normal.y, v.z + into * hit.normal.z};
            f32 push = !(into < 0.0f) ? into : 0.0f;
            state->slide = slide;
            v.x = slide.x + push * up.x;
            v.y = slide.y + push * up.y;
            v.z = slide.z + push * up.z;
        }
        fn_71010cb838(reinterpret_cast<UnkEnemyMovement_l0cb838*>(self), reinterpret_cast<UnkLedgeRef_l0cb838**>(&hit));
        self->vf_0x218(&hit);
        self->SetIgnoreRelocate(true);
    } else {
        if (animation && fn_710117b348(animation, fn_7100c33638())) {
            self->SetIgnoreRelocate(true);
        } else {
            self->SetIgnoreRelocate(false);
            self->SetFalling(true);
        }
        self->vf_0x220();
    }
}
