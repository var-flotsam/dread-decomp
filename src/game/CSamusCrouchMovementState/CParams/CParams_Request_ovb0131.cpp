// In the CSamusCrouchMovementState::CParams unit (tools/tu_map.py unit 1457): fn_7100990e14 passes the
// target at +0x8 a default request (fn_710098ae04): the reflected params base holding an unset track
// request (no id, the default time and blend descriptors, the all-flags mask), then the derived part
// (vtable g_unk_7101a22d18) with 3 and zeros, as fn_71009a9434
// (src/game/CSamusMagnetGloveMovementState/CParams/CParams_Fall_ovb0082.cpp). Names and layouts are
// placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;
extern UnkDesc_SpitSend g_unk_7172769740;
extern void* g_unk_7101a22cb8[];
extern void* g_unk_7101a22d18[];
struct UnkAirTarget_B71a6;
struct UnkAirRequest_B71a6;
void fn_710098ae04(UnkAirTarget_B71a6* target, UnkAirRequest_B71a6* req);

struct UnkCrouchTrackTime_ovb0131 {
    u64 key;
    u64 time;
};
struct UnkCrouchTrackBlend_ovb0131 {
    u32 a, b, c;
};

struct UnkCrouchTrackParams_ovb0131 {
    s64 id;
    UnkCrouchTrackTime_ovb0131 time;
    UnkCrouchTrackBlend_ovb0131 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkCrouchTrackParams_ovb0131(UnkCrouchTrackTime_ovb0131 time_,
                                                                UnkCrouchTrackBlend_ovb0131 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkCrouchTrackParams_ovb0131()
        : UnkCrouchTrackParams_ovb0131(reinterpret_cast<const UnkCrouchTrackTime_ovb0131&>(g_unk_710164b958),
                                       reinterpret_cast<const UnkCrouchTrackBlend_ovb0131&>(g_unk_7172769740),
                                       base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkCrouchParamsVtable_ovb0131 {
    void* vtable;
    __attribute__((always_inline)) UnkCrouchParamsVtable_ovb0131() { vtable = g_unk_7101a22cb8 + 2; }
};

struct UnkCrouchRequest_ovb0131 : UnkCrouchParamsVtable_ovb0131 {
    UnkCrouchTrackParams_ovb0131 params;  // 0x8
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;

    __attribute__((always_inline)) UnkCrouchRequest_ovb0131() {
        field_0x38 = 3;
        __builtin_memset(&field_0x3c, 0, 8);
        field_0x44 = 0;
        vtable = g_unk_7101a22d18 + 2;
    }
};

struct UnkCrouchState_ovb0131 {
    u8 unk_0x0[0x8];
    UnkAirTarget_B71a6* target;  // 0x8
};

void fn_7100990e14(UnkCrouchState_ovb0131* self) {
    UnkCrouchRequest_ovb0131 req;
    fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&req));
}

