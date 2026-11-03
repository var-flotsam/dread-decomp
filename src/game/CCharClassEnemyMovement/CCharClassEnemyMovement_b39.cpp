// CCharClassEnemyMovement unit (tools/tu_map.py, the name from reflection): angle setters storing radians
// only, near copies of
// fn_7100c43308 (src/game/CCharClassOverrideAnalogAimInvalidAnglesTrack/
// CCharClassOverrideAnalogAimInvalidAnglesTrack_Set.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkValueSet_cem {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_cem {
    void* holder;
    UnkValueSet_cem* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_cem {
    u64 empty[2];
    u64 kind_and_flag;
};

template <class T>
static inline __attribute__((always_inline)) T* PickArg_cem(UnkValueSource_cem* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<T*>(src->holder);
    return reinterpret_cast<T*>(src);
}

struct UnkOwner_cem {
    u8 unk_0x0[8];
    u8* target;
};

// Only the radians are stored here (no degrees copy).
static inline __attribute__((always_inline)) void SetRadians_cem(f32* out, f32 degrees) {
    *out = static_cast<f32>(static_cast<double>(degrees) * 3.141592653589793 / 180.0);
}

#define ANGLE_SETTER_CEM(addr, offset)                                                                  \
    UnkResult_0x18_cem fn_##addr(void* unused, UnkOwner_cem* owner, UnkValueSource_cem** source) {     \
        UnkValueSource_cem* src = *source;                                                              \
        u8* target = owner->target;                                                                     \
        SetRadians_cem(reinterpret_cast<f32*>(target + (offset)), *PickArg_cem<f32>(src));             \
        UnkResult_0x18_cem result = {};                                                                 \
        result.kind_and_flag = 0x100000002;                                                             \
        return result;                                                                                  \
    }

ANGLE_SETTER_CEM(71009667bc, 0x12c)
ANGLE_SETTER_CEM(71009668a8, 0x130)
ANGLE_SETTER_CEM(7100966ff4, 0x174)
ANGLE_SETTER_CEM(71009670e0, 0x178)
ANGLE_SETTER_CEM(71009671cc, 0x17c)
ANGLE_SETTER_CEM(71009672b8, 0x180)
