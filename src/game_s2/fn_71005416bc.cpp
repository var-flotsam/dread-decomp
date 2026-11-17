// Steps a hand-over: with a position object present, takes its position (or, when it has an
// object at +0x1f8, the position that object's fn_7100979a60 answers; zero without a position
// object on the second look), narrows it to two components, runs fn_71005528fc on it, then
// fn_7100552a34 with the result and the flag. Returns whether a pair of state bytes at +0x14c34
// allows the transition (running fn_7100544fb8 when the first byte is clear).
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkVec3Bits.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPlace;

struct UnkBody {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
    u8 pad_0x68[0x190];
    UnkPlace* field_0x1f8;
};

struct UnkStepper {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0x14bfc];
    u8 field_0x14c34;
    u8 field_0x14c35;
};

bool fn_71005528fc(UnkStepper* self, const CVector2D* point);
void fn_7100552a34(UnkStepper* self, bool a, bool b);
// Updates whether the owner can reach its target and returns it (src/game/UnkTU811/UnkTU811_UpdateReach_l544fb8.cpp).
struct UnkEmmyAI_l544fb8;
bool fn_7100544fb8(UnkEmmyAI_l544fb8* self, bool arg);

struct UnkPos {
    u32 x;
    u32 y;
    u32 z;
};

// NON_MATCHING(minor): the two stack slots (the position, and the shared slot of the returned vector
// and the narrowed point) are laid out in the other order
bool fn_71005416bc(UnkStepper* self, bool flag) {
    bool found = false;
    if (fn_7100d7c990(self->field_0x30)) {
        UnkBody* body = reinterpret_cast<UnkBody*>(fn_7100d7c990(self->field_0x30));
        UnkPos pos;
        if (body) {
            pos = {body->field_0x44, body->field_0x54, body->field_0x64};
            if (body->field_0x1f8) {
                UnkVec3Bits other = fn_7100979a60(body->field_0x1f8);
                __builtin_memcpy(&pos, &other, sizeof(pos));
            }
        } else {
            pos = {0, 0, 0};
        }
        CVector2D point;
        fn_710007b430(&point, reinterpret_cast<const CVector3D*>(&pos));
        found = fn_71005528fc(self, &point);
    }
    fn_7100552a34(self, found, flag);
    if (!self->field_0x14c35)
        return false;
    if (self->field_0x14c34)
        return true;
    fn_7100544fb8(reinterpret_cast<UnkEmmyAI_l544fb8*>(self), false);
    return self->field_0x14c35 != 0;
}
