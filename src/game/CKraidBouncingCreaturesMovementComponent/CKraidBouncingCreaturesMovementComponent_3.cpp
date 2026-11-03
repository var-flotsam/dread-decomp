// In the CKraidBouncingCreaturesMovementComponent unit: fn_71006052a4 runs the base update
// (fn_7100ac3880), registers the object with the game manager under the id g_unk_71726f41d0
// (fn_7100de9540) and, when the actor held at +0x230 (+0x10) exists and the definition at +0x5d0 has
// its flag at +0x11a, adds that actor's velocity (fn_7100d7be08) scaled by the definition's +0x11c to
// the vector at +0x4b8. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

// Returned in memory (x8): the copy constructor is user-provided.
struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkSelf;
void fn_7100ac3880(UnkSelf* self);
UnkKraidCreatureVec fn_7100d7be08(void* actor);
extern u64 g_unk_71726f41d0 __attribute__((visibility("hidden")));

struct UnkKraidCreatureHeld {
    u8 pad_0x0[0x10];
    void* actor;
};

struct UnkKraidCreatureDef {
    u8 pad_0x0[0x11a];
    bool follow;
    u8 pad_0x11b[0x1];
    f32 scale;
};

struct UnkKraidCreatureMovement {
    u8 pad_0x0[0x230];
    UnkKraidCreatureHeld* held;
    u8 pad_0x238[0x4b8 - 0x238];
    f32 velocity_x;
    f32 velocity_y;
    f32 velocity_z;
    u8 pad_0x4c4[0x5d0 - 0x4c4];
    UnkKraidCreatureDef* def;
};

void fn_71006052a4(UnkKraidCreatureMovement* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    fn_7100de9540(g_unk_7101d47310, &g_unk_71726f41d0, reinterpret_cast<UnkWeakTarget*>(self));
    UnkKraidCreatureHeld* held = self->held;
    if (held && held->actor && self->def->follow) {
        UnkKraidCreatureVec v = fn_7100d7be08(held->actor);
        f32 scale = self->def->scale;
        f32 x = scale * v.x;
        f32 y = scale * v.y;
        f32 z = scale * v.z;
        self->velocity_x += x;
        self->velocity_y += y;
        self->velocity_z += z;
    }
}
