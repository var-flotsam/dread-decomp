// SCameraRailNode unit (tools/tu_map.py, the name from reflection): the position setter. The Vec3
// picked from the argument's holder (inline, or behind a pointer for 9 or more entries or kind 1) is
// copied to +0x8 of the node at 8. fn_7100c07668 stores a CVector3D argument (none when the argument
// has no type) projected to 2D (fn_710007b430) at +0x0 of its object. Returns an empty result of kind 2. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkValueSet_crn {
    u8 unk_0x0[0x18];
    u64 count;
};

// An argument: the value is inline unless it is kind 1 or its type is 9 bytes or more.
struct UnkValueSource_crn {
    void* holder;
    UnkValueSet_crn* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_crn {
    u64 empty[2];
    u64 kind_and_flag;
};

template <class T>
static inline __attribute__((always_inline)) T* PickArg_crn(UnkValueSource_crn* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<T*>(src->holder);
    return reinterpret_cast<T*>(src);
}

struct UnkVec3_crn {
    f32 x, y, z;
};

struct UnkCameraRailNode_crn {
    u8 unk_0x0[8];
    UnkVec3_crn pos;
};

struct UnkCameraRailNodeOwner_crn {
    u8 unk_0x0[8];
    UnkCameraRailNode_crn* target;
};

UnkResult_0x18_crn fn_7100c07a1c(void* unused, UnkCameraRailNodeOwner_crn* owner, UnkValueSource_crn** source) {
    UnkCameraRailNode_crn* target = owner->target;
    UnkValueSource_crn* src = *source;
    UnkVec3_crn* v = PickArg_crn<UnkVec3_crn>(src);
    target->pos.x = v->x;
    target->pos.y = v->y;
    target->pos.z = v->z;
    UnkResult_0x18_crn result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

struct UnkCameraRailOwner2D_crn {
    u8 unk_0x0[8];
    base::math::CVector2D* target;
};

UnkResult_0x18_crn fn_7100c07668(void* unused, UnkCameraRailOwner2D_crn* owner, UnkValueSource_crn** source) {
    base::math::CVector2D* target = owner->target;
    UnkValueSource_crn* src = *source;
    const base::math::CVector3D* v;
    if (!src->set)
        v = nullptr;
    else
        v = PickArg_crn<base::math::CVector3D>(src);
    base::math::CVector2D out;
    fn_710007b430(&out, v);
    f32 x = out.x;
    f32 y = out.y;
    target->x = x;
    target->y = y;
    UnkResult_0x18_crn result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
