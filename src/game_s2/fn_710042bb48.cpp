// Decides whether the owner's current position is close enough: moves the output x to the target's x
// minus the controller's step (signed by which side the target is on), clamped to the last entry of
// the owner's limit table when it is past the first one, copies the owner's y, then compares the
// remaining distance against the two ranges the controller reports. Returns false at once when the
// controller's step at +0xf0 is -1.
#include "types.h"

typedef f32 __attribute__((may_alias)) a_f32;
typedef u32 __attribute__((may_alias)) a_u32;

struct UnkOut {
    f32 x;
    u32 y;
};

struct UnkVec3 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkLimits {
    u8 pad_0x0[0x68];
    UnkVec3* field_0x68;
    u32 field_0x70;
};

struct UnkPoint {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0x1e8];
    UnkLimits* field_0x240;
};

struct UnkController;

struct UnkControllerVtable {
    void* pad_0x0[5];
    f32 (*vf_0x28)(UnkController* self, UnkPoint* point);
    f32 (*vf_0x30)(UnkController* self, UnkPoint* point);
};

struct UnkController {
    UnkControllerVtable* vtable;
    u8 pad_0x8[0xe8];
    f32 field_0xf0;
};

struct UnkSelf {
    u8 pad_0x0[0x8];
    UnkPoint* field_0x8;
    u8 pad_0x10[0x38];
    UnkController* field_0x48;
};

UnkPoint* fn_7100d7c990(UnkPoint* point);

bool fn_710042bb48(UnkSelf* self, UnkOut* out) {
    bool result = false;
    if (self->field_0x48->field_0xf0 != -1.0f) {
        f32 from = self->field_0x8->field_0x44;
        u32 y = self->field_0x8->field_0x54;
        f32 to = fn_7100d7c990(self->field_0x8)->field_0x44;
        *reinterpret_cast<a_u32*>(&out->y) = y;
        f32 diff = to - from;
        s32 sign = (diff > 0.0f) - (diff < 0.0f);
        f32 signf = (f32)sign;
        f32 x = to - self->field_0x48->field_0xf0 * signf;
        *reinterpret_cast<a_f32*>(&out->x) = x;
        UnkLimits* limits = self->field_0x8->field_0x240;
        UnkVec3* table = limits->field_0x68;
        f32 clamped = table[0].x;
        if (!(x <= clamped)) {
            f32 last = table[limits->field_0x70 - 1].x;
            clamped = x >= last ? last : x;
        }
        *reinterpret_cast<a_f32*>(&out->x) = clamped;
        UnkController* c = self->field_0x48;
        UnkPoint* p = self->field_0x8;
        f32 first = c->vtable->vf_0x28(c, p);
        f32 second = self->field_0x48->vtable->vf_0x30(self->field_0x48, self->field_0x8);
        f32 distance = __builtin_fabsf(to - clamped);
        if (first != -1.0f && !(distance >= first)) {
            result = false;
        } else if (second == -1.0f) {
            result = true;
        } else {
            result = distance <= second;
        }
    }
    return result;
}
