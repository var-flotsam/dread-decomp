// CCharClassOverrideAnalogAimInvalidAnglesTrack unit (tools/tu_map.py, the name from reflection): two
// angle setters. The float picked from the argument's holder (inline, or behind a pointer for 9 or more
// entries or kind 1) is stored in degrees, followed by its value in radians (computed in double), at
// +0x18 (fn_7100c43248) or +0x20 (fn_7100c43308) of the object at 8. Returns an empty result of kind 2.
// Names and layouts are placeholders.
#include "types.h"

struct UnkValueSet_aia {
    u8 unk_0x0[0x18];
    u64 count;
};

// An argument: the value is inline unless it is kind 1 or its type is 9 bytes or more.
struct UnkValueSource_aia {
    void* holder;
    UnkValueSet_aia* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_aia {
    u64 empty[2];
    u64 kind_and_flag;
};

template <class T>
static inline __attribute__((always_inline)) T* PickArg_aia(UnkValueSource_aia* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<T*>(src->holder);
    return reinterpret_cast<T*>(src);
}

struct UnkAimAngle_aia {
    f32 degrees;
    f32 radians;
};

struct UnkAimInvalidAnglesTrack_aia {
    u8 unk_0x0[0x18];
    UnkAimAngle_aia field_0x18;
    UnkAimAngle_aia field_0x20;
};

struct UnkAimInvalidAnglesTrackOwner_aia {
    u8 unk_0x0[8];
    UnkAimInvalidAnglesTrack_aia* target;
};

static inline __attribute__((always_inline)) void SetAngle_aia(UnkAimAngle_aia* angle, f32 degrees) {
    angle->degrees = degrees;
    angle->radians = static_cast<f32>(static_cast<double>(degrees) * 3.141592653589793 / 180.0);
}

UnkResult_0x18_aia fn_7100c43248(void* unused, UnkAimInvalidAnglesTrackOwner_aia* owner,
                                 UnkValueSource_aia** source) {
    UnkValueSource_aia* src = *source;
    UnkAimInvalidAnglesTrack_aia* target = owner->target;
    SetAngle_aia(&target->field_0x18, *PickArg_aia<f32>(src));
    UnkResult_0x18_aia result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_aia fn_7100c43308(void* unused, UnkAimInvalidAnglesTrackOwner_aia* owner,
                                 UnkValueSource_aia** source) {
    UnkValueSource_aia* src = *source;
    UnkAimInvalidAnglesTrack_aia* target = owner->target;
    SetAngle_aia(&target->field_0x20, *PickArg_aia<f32>(src));
    UnkResult_0x18_aia result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
