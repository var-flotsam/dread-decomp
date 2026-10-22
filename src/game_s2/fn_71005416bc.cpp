// Steps a hand-over: with a position object present, takes its position (or, when it has an
// object at +0x1f8, the position that object's fn_7100979a60 answers; zero without a position
// object on the second look), narrows it to two components, runs fn_71005528fc on it, then
// fn_7100552a34 with the result and the flag. Returns whether a pair of state bytes at +0x14c34
// allows the transition (running fn_7100544fb8 when the first byte is clear).
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

union UnkBits {
    u32 u;
    f32 f;
};

// Not trivially copyable, so it is returned in memory.
struct UnkVec3Bits {
    UnkBits x;
    UnkBits y;
    UnkBits z;

    UnkVec3Bits(u32 x, u32 y, u32 z) : x{x}, y{y}, z{z} {}
    UnkVec3Bits(const UnkVec3Bits& other) : x(other.x), y(other.y), z(other.z) {}
};

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
    void* field_0x30;
    u8 pad_0x38[0x14bfc];
    u8 field_0x14c34;
    u8 field_0x14c35;
};

UnkBody* fn_7100d7c990(void* object);
UnkVec3Bits fn_7100979a60(UnkPlace* place);
void fn_710007b430(CVector2D* out, const CVector3D* in);
bool fn_71005528fc(UnkStepper* self, const CVector2D* point);
void fn_7100552a34(UnkStepper* self, bool a, bool b);
void fn_7100544fb8(UnkStepper* self, u32 arg);

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
        UnkBody* body = fn_7100d7c990(self->field_0x30);
        UnkPos pos;
        if (body) {
            pos = {body->field_0x44, body->field_0x54, body->field_0x64};
            if (body->field_0x1f8) {
                UnkVec3Bits other = fn_7100979a60(body->field_0x1f8);
                pos = {other.x.u, other.y.u, other.z.u};
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
    fn_7100544fb8(self, 0);
    return self->field_0x14c35 != 0;
}
