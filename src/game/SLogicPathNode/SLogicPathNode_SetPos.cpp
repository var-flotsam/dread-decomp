// SLogicPathNode unit (tools/tu_map.py, the name from reflection), the same code as
// src/game/SCameraRailNode/SCameraRailNode_SetPos.cpp: the position setter copies the Vec3 picked from
// the argument's holder (inline, or behind a pointer for 9 or more entries or kind 1) to +0x8 of the node
// at 8; fn_710105cc10 stores a CVector3D argument (none when the argument has no type) projected to 2D
// (fn_710007b430) at +0x0 of its object. Each returns an empty result of kind 2. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkValueSet_lpn {
    u8 unk_0x0[0x18];
    u64 count;
};

// An argument: the value is inline unless it is kind 1 or its type is 9 bytes or more.
struct UnkValueSource_lpn {
    void* holder;
    UnkValueSet_lpn* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_lpn {
    u64 empty[2];
    u64 kind_and_flag;
};

template <class T>
static inline __attribute__((always_inline)) T* PickArg_lpn(UnkValueSource_lpn* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<T*>(src->holder);
    return reinterpret_cast<T*>(src);
}

struct UnkVec3_lpn {
    f32 x, y, z;
};

struct UnkLogicPathNode_lpn {
    u8 unk_0x0[8];
    UnkVec3_lpn pos;
};

struct UnkLogicPathNodeOwner_lpn {
    u8 unk_0x0[8];
    UnkLogicPathNode_lpn* target;
};

UnkResult_0x18_lpn fn_710105ce04(void* unused, UnkLogicPathNodeOwner_lpn* owner, UnkValueSource_lpn** source) {
    UnkLogicPathNode_lpn* target = owner->target;
    UnkValueSource_lpn* src = *source;
    UnkVec3_lpn* v = PickArg_lpn<UnkVec3_lpn>(src);
    target->pos.x = v->x;
    target->pos.y = v->y;
    target->pos.z = v->z;
    UnkResult_0x18_lpn result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

struct UnkLogicPathOwner2D_lpn {
    u8 unk_0x0[8];
    base::math::CVector2D* target;
};

UnkResult_0x18_lpn fn_710105cc10(void* unused, UnkLogicPathOwner2D_lpn* owner, UnkValueSource_lpn** source) {
    base::math::CVector2D* target = owner->target;
    UnkValueSource_lpn* src = *source;
    const base::math::CVector3D* v;
    if (!src->set)
        v = nullptr;
    else
        v = PickArg_lpn<base::math::CVector3D>(src);
    base::math::CVector2D out;
    fn_710007b430(&out, v);
    f32 x = out.x;
    f32 y = out.y;
    target->x = x;
    target->y = y;
    UnkResult_0x18_lpn result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
