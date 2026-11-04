// Unit 1441 (no reflection hint): on a message of type 1 whose sender's +0x10 object has the kind cached in the
// hidden global at 0x717273a730, and when the owner (+0x30) has its +0x220 part, restarts the movement at +0x38
// (fn_71010d08d4) with velocity + impulse / mass. Returns true. Names and layouts are placeholders.
#include "types.h"

// Declared as src/game/CMovementComponent/fn_71010d08d4.cpp defines it.
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
void fn_71010d08d4(UnkMovement_71010d08d4* self, const UnkMovementVec_71010d08d4* v, bool start);

extern u64 g_unk_717273a730 __attribute__((visibility("hidden")));

struct UnkImpulseMovement_A5_b68 {
    u8 pad_0x0[0x1d8];
    f32 velocity_x;
    f32 velocity_y;
    f32 velocity_z;
    u8 pad_0x1e4[0x9c];
    f32 mass;
    u8 pad_0x284[0x5c];
    f32 impulse_x;
    f32 impulse_y;
    f32 impulse_z;
};

struct UnkImpulseVec_A5_b68 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkImpulseKind_A5_b68 {
    u8 pad_0x0[0x38];
    u64 kind;
};

struct UnkImpulseSender_A5_b68 {
    u8 pad_0x0[0x10];
    UnkImpulseKind_A5_b68* field_0x10;
};

struct UnkImpulseMessage_A5_b68 {
    s32 type;
    UnkImpulseSender_A5_b68* sender;
};

struct UnkImpulseOwner_A5_b68 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkImpulseUser_A5_b68 {
    u8 pad_0x0[0x30];
    UnkImpulseOwner_A5_b68* owner;
    UnkImpulseMovement_A5_b68* movement;
};

bool fn_7100969e84(UnkImpulseUser_A5_b68* self, const UnkImpulseMessage_A5_b68* msg) {
    bool hit = self->owner->field_0x220 && msg->type == 1 && msg->sender->field_0x10->kind == g_unk_717273a730;
    if (__builtin_expect(hit, 0)) {
        UnkImpulseMovement_A5_b68* m = self->movement;
        f32 inv = 1.0f / m->mass;
        f32 sx = inv * m->impulse_x;
        f32 sy = inv * m->impulse_y;
        f32 sz = inv * m->impulse_z;
        f32 ax = sx + m->velocity_x;
        f32 ay = sy + m->velocity_y;
        f32 az = sz + m->velocity_z;
        UnkImpulseVec_A5_b68 v;
        v.x = ax;
        v.y = ay;
        v.z = az;
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(m), reinterpret_cast<UnkMovementVec_71010d08d4*>(&v),
                      true);
    }
    return true;
}
