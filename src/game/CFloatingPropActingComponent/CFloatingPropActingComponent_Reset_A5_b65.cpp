// CFloatingPropActingComponent's unit: writes 0 under the unit's id into the property bag of the
// object the weak handle at +0x40 names (as fn_71007e664c in CFloatingPropActingComponent_b50.cpp).
// Names are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "../../game_s7/UnkPropBagS7.h"

extern char g_unk_717271f800[] __attribute__((visibility("hidden")));

struct UnkFloatingProp_A5_b65 {
    u8 pad_0x0[0x40];
    UnkWeakHandle* handle;
};

void fn_71007e662c(UnkFloatingProp_A5_b65* self) {
    char* target = reinterpret_cast<char*>(self->handle ? self->handle->target : nullptr);
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(target + 0x80),
                                *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_717271f800), 0.0f);
}
