// CBatalloonAIComponent's unit (tools/tu_map.py unit 583): the lazily resolved weak link of a spawned member
// (fn_71003df2d0, as CBossAIComponent's door links but looked up through fn_71003df480) and the turn
// towards the movement's up vector (fn_71003dda84). Names and layouts are placeholders.
#include <math.h>

#include "game/CBatalloonAIComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"

// A counted reference to a weak handle; assigning another one refers to the handle of its target.
struct UnkRef_c3 {
    UnkWeakHandle* ptr;

    __attribute__((always_inline)) UnkRef_c3& operator=(const UnkRef_c3& other) {
        if (this != &other)
            CopyWeakRef(&ptr, other.ptr);
        return *this;
    }
    ~UnkRef_c3() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkBatalloonLink_ovb0132 {
    u8 pad_0x0[0x8];
    u8 key[0x28];    // +0x8
    u32 state;       // +0x30
    u8 pad_0x34[4];
    UnkRef_c3 handle;  // +0x38
};

struct UnkSwifterMember_ovb0114;
struct UnkSwifterActor_ovb0114;

// The member's weak handle, resolved from its key on first use (state 1 -> 2).
UnkSwifterActor_ovb0114** fn_71003df2d0(UnkSwifterMember_ovb0114* member) {
    UnkBatalloonLink_ovb0132* self = reinterpret_cast<UnkBatalloonLink_ovb0132*>(member);
    switch (self->state) {
    case 0:
        return reinterpret_cast<UnkSwifterActor_ovb0114**>(&self->handle);
    case 1:
        self->handle = fn_71003df480(self->key);
        self->state = 2;
        return reinterpret_cast<UnkSwifterActor_ovb0114**>(&self->handle);
    case 2:
        return reinterpret_cast<UnkSwifterActor_ovb0114**>(&self->handle);
    default:
        return reinterpret_cast<UnkSwifterActor_ovb0114**>(&self->handle);
    }
    return reinterpret_cast<UnkSwifterActor_ovb0114**>(&self->handle);
}

struct UnkEnemyMovement_A5;
UnkVec3f fn_71010c9ad0(UnkEnemyMovement_A5* self);
UnkVec3f fn_710007a220(const UnkVec3f* v);

struct UnkBatalloonOwner_ovb0132 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // +0x38

    void vf_0xd8(const UnkVec3f* angles, bool flag) {
        reinterpret_cast<void (*)(UnkBatalloonOwner_ovb0132*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, angles,
                                                                                                      flag);
    }
};

struct UnkBatalloonAI3_ovb0132 {
    u8 pad_0x0[0x30];
    UnkBatalloonOwner_ovb0132* owner;  // +0x30
};

static inline UnkActor* Actor(UnkBatalloonOwner_ovb0132* owner) {
    return reinterpret_cast<UnkActor*>(owner);
}

static inline UnkVec3f Facing(const UnkVec3f& angles) {
    return fn_710007a220(&angles);
}

// Turns the owner to face along its movement's up vector, pitched towards the side the actor it
// acts against is on.
void fn_71003dda84(UnkBatalloonAI3_ovb0132* self) {
    UnkActor* actor = Actor(self->owner);
    UnkEnemyMovement_A5* movement = static_cast<UnkEnemyMovement_A5*>(actor->field_0x218);
    if (!movement)
        return;
    void* sink = actor->field_0x298;
    UnkParams params;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x30 = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    params.field_0x18 = 0;
    params.field_0x1c = 1;
    fn_71006f4b0c(sink, &params, 1);
    UnkVec3f up = fn_71010c9ad0(movement);
    f32 angle = -(atan2f(-up.y, -up.x) + 1.5707964f);
    UnkVec3f result = Facing(UnkVec3f(angle, self->owner->xform.GetAngle().y, 0.0f));
    UnkActor* own = Actor(self->owner);
    f32 x = own->field_0x44;
    f32 y = own->field_0x54;
    f32 z = own->field_0x64;
    UnkActor* target = fn_7100d7c990(own);
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 tz = target->field_0x64;
    f32 dx = x - tx;
    f32 dy = y - ty;
    f32 dz = z - tz;
    f32 nx = dx * (1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f));
    result.y = nx < 0.0f ? 1.5707964f : -1.5707964f;
    self->owner->vf_0xd8(&result, true);
}
