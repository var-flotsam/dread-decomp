// CCharClassChozoCommanderPowerPulseAttack unit (tools/tu_map.py unit 635): an angle setter, the same
// code as fn_7100c43308 (src/game/CCharClassOverrideAnalogAimInvalidAnglesTrack/
// CCharClassOverrideAnalogAimInvalidAnglesTrack_Set.cpp): the float picked from the argument's holder is
// stored in degrees, then in radians. Returns an empty result of kind 2. Names and layouts are
// placeholders.
#include "types.h"

struct UnkValueSet_ppa {
    u8 unk_0x0[0x18];
    u64 count;
};

// An argument: the value is inline unless it is kind 1 or its type is 9 bytes or more.
struct UnkValueSource_ppa {
    void* holder;
    UnkValueSet_ppa* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_ppa {
    u64 empty[2];
    u64 kind_and_flag;
};

template <class T>
static inline __attribute__((always_inline)) T* PickArg_ppa(UnkValueSource_ppa* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<T*>(src->holder);
    return reinterpret_cast<T*>(src);
}

struct UnkAimAngle_ppa {
    f32 degrees;
    f32 radians;
};

struct UnkPowerPulseTrack_ppa {
    u8 unk_0x0[0xfc];
    UnkAimAngle_ppa field_0xfc;
};

struct UnkPowerPulseTrackOwner_ppa {
    u8 unk_0x0[8];
    UnkPowerPulseTrack_ppa* target;
};

static inline __attribute__((always_inline)) void SetAngle_ppa(UnkAimAngle_ppa* angle, f32 degrees) {
    angle->degrees = degrees;
    angle->radians = static_cast<f32>(static_cast<double>(degrees) * 3.141592653589793 / 180.0);
}

UnkResult_0x18_ppa fn_71004224bc(void* unused, UnkPowerPulseTrackOwner_ppa* owner,
                                 UnkValueSource_ppa** source) {
    UnkValueSource_ppa* src = *source;
    UnkPowerPulseTrack_ppa* target = owner->target;
    SetAngle_ppa(&target->field_0xfc, *PickArg_ppa<f32>(src));
    UnkResult_0x18_ppa result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
