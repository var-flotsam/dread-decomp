// CFloatingPropActingComponent unit. fn_71007e664c: after the base update (fn_71010c424c), while
// the curve at +0x48 is running (+0x54 not 0) it is advanced by `dt` (fn_71000763c0); the value is
// kept at +0x98 and written into the property bag (+0x80) of the object the weak handle at +0x40
// names, under the unit's id (g_unk_717271f800). Names and layouts are placeholders.
#include "base/UnkTimedCurve.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "../../game_s7/UnkPropBagS7.h"

struct UnkParent;
void fn_71010c424c(UnkParent* self);

extern char g_unk_717271f800[] __attribute__((visibility("hidden")));

struct UnkFloatingProp_b50 {
    u8 pad_0x0[0x40];
    UnkWeakHandle* handle;  // 0x40
    UnkTimedCurve curve;    // 0x48
};

void fn_71007e664c(UnkFloatingProp_b50* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    char* raw = reinterpret_cast<char*>(self);
    if (*reinterpret_cast<f32*>(raw + 0x54) != 0.0f) {
        f32 value = fn_71000763c0(dt, &self->curve);
        *reinterpret_cast<f32*>(raw + 0x98) = value;
        char* target = reinterpret_cast<char*>(self->handle ? self->handle->target : nullptr);
        unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(target + 0x80),
                                    *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_717271f800), value);
    }
}
