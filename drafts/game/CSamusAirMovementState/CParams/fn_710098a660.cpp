// The CSamusAirMovementState::CParams unit: when the movement may enter its state and both the owner's
// facing and the movement's speed agree in sign with the stored direction, requests the state, places
// both transforms of the pose at the stored position (shifted by the collider's offset) turned a quarter
// turn, resets the movement and marks it pending. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CMovementComponent.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkMove_b152;
bool fn_71009e69c4(UnkMove_b152* self);

__attribute__((visibility("hidden"))) extern UnkMovementVec_71010d08d4 g_unk_710164b328;

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_l98a660 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_l98a660 {
    s64 id;
    UnkDesc16_l98a660 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_l98a660(UnkDesc16_l98a660 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_l98a660()
        : UnkMoveTransition_l98a660(*reinterpret_cast<const UnkDesc16_l98a660*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_l98a660) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_l98a660 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8), given the vtable of the state it asks for.
struct UnkEnterParams_l98a660 {
    void* vtable;
    UnkMoveTransition_l98a660 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_l98a660 field_0x3c;
    u32 field_0x44;

    explicit UnkEnterParams_l98a660(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkEnterParams_l98a660) == 0x48, "layout");

struct UnkEnterShape_l98a660 {
    u8 pad_0x0[0x244];
    f32 offset_x;  // 0x244
    f32 offset_y;  // 0x248
    f32 offset_z;  // 0x24c
};

struct UnkEnterCollider_l98a660 {
    u8 pad_0x0[0x2f0];
    UnkEnterShape_l98a660* shape;  // 0x2f0
};

// A CCollisionComponent, as far as this function reads it.
struct UnkEnterCollision_l98a660 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkEnterCollider_l98a660* collider;  // 0x30
};

struct UnkEnterLink_l98a660 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkEnterLinks_l98a660 {
    u8 pad_0x0[0x10];
    UnkEnterLink_l98a660* link;  // 0x10
};

struct UnkEnterMovement_l98a660 {
    void** vtable;
    u8 pad_0x8[0x50];
    f32 field_0x58;  // 0x58
    u8 pad_0x5c[0x208 - 0x5c];
    u64 field_0x208;  // 0x208
    u32 field_0x210;  // 0x210
    u8 pad_0x214[0x3a8 - 0x214];
    u8 pending;  // 0x3a8
    u8 pad_0x3a9[0x51b0 - 0x3a9];
    bool can_enter;      // 0x51b0
    UnkVec3f position;   // 0x51b4
    f32 direction;       // 0x51c0
    u8 pad_0x51c4[0x51d8 - 0x51c4];
    UnkEnterLinks_l98a660* links;  // 0x51d8

    void vf_0x2b8() { reinterpret_cast<void (*)(UnkEnterMovement_l98a660*)>(vtable[0x2b8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEnterMovement_l98a660, links) == 0x51d8, "layout");

struct UnkEnterOwner_l98a660 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkEnterState_l98a660 {
    void* vtable;
    UnkAirTarget_B71a6* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkEnterOwner_l98a660* owner;          // 0x30
    UnkEnterMovement_l98a660* movement;    // 0x38
};

struct UnkEnterQuat_l98a660 {
    f32 x, y, z, w;
    void SetIdentity() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;
    }
};

// The pose: two transforms with the values between them.
struct UnkEnterPose_l98a660 {
    u32 field_0x0;
    UnkXform xform_a;  // 0x4
    UnkVec3f field_0x60;
    UnkEnterQuat_l98a660 field_0x6c;
    f32 field_0x7c;
    f32 field_0x80;
    UnkEnterQuat_l98a660 field_0x84;
    UnkEnterQuat_l98a660 field_0x94;
    f32 field_0xa4;
    UnkEnterQuat_l98a660 field_0xa8;
    UnkXform xform_b;  // 0xb8
};
static_assert(__builtin_offsetof(UnkEnterPose_l98a660, xform_b) == 0xb8, "layout");

bool fn_710098a660(UnkEnterState_l98a660* self, UnkEnterPose_l98a660* pose) {
    if (!self->movement->can_enter)
        return false;
    if (!fn_71009e69c4(reinterpret_cast<UnkMove_b152*>(self->movement)))
        return false;
    f32 speed = self->movement->field_0x58;
    f32 facing = self->owner->xform.GetMatrix().m[2];
    f32 direction = self->movement->direction;
    if (!(facing * direction > 0.0f))
        return false;
    if (!(speed * direction > 0.0f))
        return false;

    UnkEnterParams_l98a660 params(reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10);
    params.transition.id = 0x5184bdea49639a85;
    params.transition.desc = *reinterpret_cast<const UnkDesc16_l98a660*>(&g_unk_710164b968);
    params.transition.field_0x24 = 9;
    fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));

    UnkEnterMovement_l98a660* movement = self->movement;
    UnkVec3f position = movement->position;
    if (movement->links && movement->links->link) {
        UnkReflObject* component = movement->links->link->component;
        if (component && fn_710009d50c(component->vf_0x0(), fn_710085fc44())) {
            UnkEnterCollider_l98a660* collider = static_cast<UnkEnterCollision_l98a660*>(component)->collider;
            if (collider && collider->shape) {
                UnkEnterShape_l98a660* shape = collider->shape;
                position.x = shape->offset_x + position.x;
                position.y = shape->offset_y + position.y;
                position.z = shape->offset_z + position.z;
            }
        }
    }

    UnkVec3f angle(0.0f, self->movement->direction * 1.5707964f, 0.0f);
    pose->xform_a.SetTranslation(position);
    pose->xform_a.SetAngle(angle);
    pose->xform_b.SetTranslation(position);
    pose->xform_b.SetAngle(angle);
    pose->field_0x60 = UnkVec3f(0.0f, 0.0f, 0.0f);
    pose->field_0x6c.SetIdentity();
    pose->field_0x7c = 0.0f;
    pose->field_0x80 = 0.0f;
    pose->field_0x84.SetIdentity();
    pose->field_0x94.SetIdentity();
    pose->field_0xa4 = 0.0f;
    pose->field_0xa8.SetIdentity();

    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(self->movement), &g_unk_710164b328, true);
    self->movement->field_0x208 = 0;
    self->movement->field_0x210 = 0;
    movement = self->movement;
    if (movement->pending != 1) {
        movement->pending = 1;
        movement->vf_0x2b8();
    }
    return true;
}
