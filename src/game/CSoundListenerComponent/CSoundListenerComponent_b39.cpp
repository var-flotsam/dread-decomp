// CSoundListenerComponent unit (tools/tu_map.py, the name from reflection): a Vec3 property setter, the
// same code as fn_710105ce04 (src/game/SLogicPathNode/SLogicPathNode_SetPos.cpp). Names and layouts are
// placeholders.
#include "types.h"

struct UnkValueSet_slc {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_slc {
    void* holder;
    UnkValueSet_slc* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_slc {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkVec3_slc {
    f32 x, y, z;
};

struct UnkTarget_slc {
    u8 unk_0x0[0x40];
    UnkVec3_slc pos;
};

struct UnkOwner_slc {
    u8 unk_0x0[8];
    UnkTarget_slc* target;
};

UnkResult_0x18_slc fn_710090eeb0(void* unused, UnkOwner_slc* owner, UnkValueSource_slc** source) {
    UnkTarget_slc* target = owner->target;
    UnkValueSource_slc* src = *source;
    UnkVec3_slc* v;
    if (src->kind == 1 || src->set->count >= 9)
        v = static_cast<UnkVec3_slc*>(src->holder);
    else
        v = reinterpret_cast<UnkVec3_slc*>(src);
    target->pos.x = v->x;
    target->pos.y = v->y;
    target->pos.z = v->z;
    UnkResult_0x18_slc result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
