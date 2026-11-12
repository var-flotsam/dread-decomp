// CEnvironmentData unit (tools/tu_map.py names it after CEnvironmentData::SFogTransition, from
// reflection): reflected setters that store one value of the environment settings and then have the
// environment manager g_unk_7101d47368 (built on first use: 0x4c8 bytes, flags 3, fn_7100d8ced8) apply
// them (fn_7100d8f1c0), as fn_7100f5bd64 and its generated copies do for plain values. The values here are
// a CVector4D (assigned out of line, fn_710007b518), two or three floats, or a file path. Each takes the
// value picked from the argument's holder (none when the argument has no type; inline, or behind a
// pointer for 9 or more entries or kind 1) and returns an empty result of kind 2. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/UnkVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::math::CVector4D;

void fn_7100d8f1c0(char*);

struct UnkValueSet_eas {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_eas {
    void* holder;
    UnkValueSet_eas* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_eas {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkEnvOwner_eas {
    u8 unk_0x0[8];
    char* target;
};

struct UnkFloat2_eas {
    f32 x, y;
    UnkFloat2_eas& operator=(const UnkFloat2_eas& o) {
        x = o.x;
        y = o.y;
        return *this;
    }
};

struct UnkFloat3_eas {
    f32 x, y, z;
    UnkFloat3_eas& operator=(const UnkFloat3_eas& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

template <class T>
static inline __attribute__((always_inline)) const T* PickValue_eas(UnkValueSource_eas* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<const T*>(src->holder);
    return reinterpret_cast<const T*>(src);
}

template <class T>
static inline __attribute__((always_inline)) const T* PickArg_eas(UnkValueSource_eas* src) {
    if (!src->set)
        return nullptr;
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<const T*>(src->holder);
    return reinterpret_cast<const T*>(src);
}

static inline __attribute__((always_inline)) UnkResult_0x18_eas ApplyAndReturn_eas() {
    void* manager = g_unk_7101d47368;
    if (!manager) {
        manager = fn_71000839a0(0x4c8, 3);
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
    }
    fn_7100d8f1c0(static_cast<char*>(manager));
    UnkResult_0x18_eas result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

template <u32 Offset>
static inline __attribute__((always_inline)) UnkResult_0x18_eas SetVector4D_eas(UnkEnvOwner_eas* owner,
                                                                                UnkValueSource_eas** source) {
    char* target = owner->target;
    const CVector4D* value = PickArg_eas<CVector4D>(*source);
    fn_710007b518(reinterpret_cast<CVector4D*>(target + Offset), value);
    return ApplyAndReturn_eas();
}

template <class T, u32 Offset>
static inline __attribute__((always_inline)) UnkResult_0x18_eas SetValue_eas(UnkEnvOwner_eas* owner,
                                                                             UnkValueSource_eas** source) {
    UnkValueSource_eas* src = *source;
    char* target = owner->target;
    *reinterpret_cast<T*>(target + Offset) = *PickValue_eas<T>(src);
    return ApplyAndReturn_eas();
}

UnkResult_0x18_eas fn_7100f5bc78(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetVector4D_eas<4>(owner, source);
}
UnkResult_0x18_eas fn_7100f5e804(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetVector4D_eas<4>(owner, source);
}
UnkResult_0x18_eas fn_7100f63010(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetVector4D_eas<4>(owner, source);
}
UnkResult_0x18_eas fn_7100f6553c(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetVector4D_eas<4>(owner, source);
}
UnkResult_0x18_eas fn_7100f65640(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetVector4D_eas<0x14>(owner, source);
}
UnkResult_0x18_eas fn_7100f65744(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetVector4D_eas<0x24>(owner, source);
}
UnkResult_0x18_eas fn_7100f6c304(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetVector4D_eas<4>(owner, source);
}
UnkResult_0x18_eas fn_7100f740d8(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetVector4D_eas<0x18>(owner, source);
}
UnkResult_0x18_eas fn_7100f5c004(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetValue_eas<UnkFloat2_eas, 0x20>(owner, source);
}
UnkResult_0x18_eas fn_7100f65910(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetValue_eas<UnkFloat2_eas, 0x38>(owner, source);
}
UnkResult_0x18_eas fn_7100f6c690(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetValue_eas<UnkFloat2_eas, 0x20>(owner, source);
}
UnkResult_0x18_eas fn_7100f67ba8(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetValue_eas<UnkFloat3_eas, 4>(owner, source);
}
UnkResult_0x18_eas fn_7100f67d88(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetValue_eas<UnkFloat3_eas, 0x14>(owner, source);
}
UnkResult_0x18_eas fn_7100f69fa4(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    return SetValue_eas<UnkFloat3_eas, 4>(owner, source);
}

// The file path at +8, a counted copy.
UnkResult_0x18_eas fn_7100f6f928(void*, UnkEnvOwner_eas* owner, UnkValueSource_eas** source) {
    UnkValueSource_eas* src = *source;
    char* target = owner->target;
    const CFilePathStrId* value = PickArg_eas<CFilePathStrId>(src);
    CFilePathStrId* path = reinterpret_cast<CFilePathStrId*>(target + 8);
    if (path->data != value->data) {
        UnkStringData* d = value->data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (path->data != &pool->empty) {
            --path->data->ref_count;
            if (path->data->ref_count == 0)
                fn_71000803a4(pool, path->data);
        }
        path->data = d;
    }
    return ApplyAndReturn_eas();
}
