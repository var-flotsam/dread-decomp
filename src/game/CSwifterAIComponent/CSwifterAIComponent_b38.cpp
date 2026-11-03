// CSwifterAIComponent: the per-frame update. After the behaviour tree's update, takes the movement
// component's velocity, replaces it with the speed from the object at +0x146c8 (vf_0xb0) along the
// third column of the owner's matrix and starts the movement with it (fn_71010d08d4).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "types.h"

// Declared as src/game/CMovementComponent/fn_71010d08d4.cpp defines it.
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
void fn_71010d08d4(UnkMovement_71010d08d4* self, const UnkMovementVec_71010d08d4* v, bool start);

// The behaviour tree's update (declared elsewhere without the time step; called with it).
struct UnkGobblerComponent;
void fn_7100369c44(UnkGobblerComponent* self);

struct UnkSwifterMovement {
    u8 pad_0x0[0x1d8];
    u32 velocity_x;
    u32 velocity_y;
    u32 velocity_z;
};

struct UnkSwifterSpeed {
    virtual void vf_0x0();
    f32 Speed(UnkActor* owner) {
        return (*reinterpret_cast<f32 (***)(UnkSwifterSpeed*, UnkActor*)>(this))[0xb0 / 8](this, owner);
    }
};

struct UnkSwifterDrive {
    UnkSwifterSpeed* speed;
    f32 x;
    f32 y;
    f32 z;
};

struct UnkSwifterAI {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

void fn_71006cd8bc(UnkSwifterAI* self, f32 dt) {
    UnkSwifterDrive* drive = reinterpret_cast<UnkSwifterDrive*>(reinterpret_cast<char*>(self) + 0x146c8);
    reinterpret_cast<void (*)(UnkSwifterAI*, f32)>(fn_7100369c44)(self, dt);
    UnkActor* owner = self->owner;
    UnkSwifterMovement* movement = static_cast<UnkSwifterMovement*>(owner->field_0x218);
    u32* v = reinterpret_cast<u32*>(&drive->x);
    v[0] = movement->velocity_x;
    v[1] = movement->velocity_y;
    v[2] = movement->velocity_z;
    f32 speed = drive->speed->Speed(owner);
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 x = speed * m.m[2];
    f32 y = speed * m.m[6];
    f32 z = speed * m.m[10];
    drive->x = x;
    drive->y = y;
    drive->z = z;
    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(movement),
                  reinterpret_cast<const UnkMovementVec_71010d08d4*>(&drive->x), true);
}
