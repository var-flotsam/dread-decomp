// CCharClassBillboardComponent unit (tools/tu_map.py unit 1154). fn_7100730f44 and fn_710073106c: two reflected
// Vec3 setters, the same code as src/game/SLogicPathNode/SLogicPathNode_SetPos.cpp: each copies the Vec3 picked
// from the argument's holder (inline, or behind a pointer for kind 1 or a type of 9 bytes or more) to +0x80 or
// +0x90 of the component at 8, and returns an empty result of kind 2. fn_71007302e4: when the list at +0x108 is
// not empty, appends an item to the list at +0xe0, switches off the item's toggle (+0x18; fn_71001f3224 and
// fn_71001f32b0) and removes it from the list at +0x108. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "engine/scene/UnkToggle.h"
#include "types.h"

void fn_71001f3224(void* object, u32 value);
void fn_71001f32b0(UnkToggle* self, bool on);

struct UnkValueSet_ovb0020 {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_ovb0020 {
    void* holder;
    UnkValueSet_ovb0020* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_ovb0020 {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkVec3_ovb0020 {
    f32 x, y, z;
};

static inline __attribute__((always_inline)) UnkVec3_ovb0020* PickVec3_ovb0020(UnkValueSource_ovb0020* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<UnkVec3_ovb0020*>(src->holder);
    return reinterpret_cast<UnkVec3_ovb0020*>(src);
}

struct UnkCharClassBillboard_ovb0020 {
    u8 unk_0x0[0x80];
    UnkVec3_ovb0020 field_0x80;
    u8 unk_0x8c[4];
    UnkVec3_ovb0020 field_0x90;
};

struct UnkBillboardOwner_ovb0020 {
    u8 unk_0x0[8];
    UnkCharClassBillboard_ovb0020* target;
};

static inline __attribute__((always_inline)) UnkResult_0x18_ovb0020 SetVec3_ovb0020(UnkVec3_ovb0020* dst,
                                                                                    UnkValueSource_ovb0020** source) {
    UnkVec3_ovb0020* v = PickVec3_ovb0020(*source);
    dst->x = v->x;
    dst->y = v->y;
    dst->z = v->z;
    UnkResult_0x18_ovb0020 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_ovb0020 fn_7100730f44(void* unused, UnkBillboardOwner_ovb0020* owner, UnkValueSource_ovb0020** source) {
    UnkCharClassBillboard_ovb0020* target = owner->target;
    return SetVec3_ovb0020(&target->field_0x80, source);
}

UnkResult_0x18_ovb0020 fn_710073106c(void* unused, UnkBillboardOwner_ovb0020* owner, UnkValueSource_ovb0020** source) {
    UnkCharClassBillboard_ovb0020* target = owner->target;
    return SetVec3_ovb0020(&target->field_0x90, source);
}

struct UnkCharClassBillboardLists_ovb0020 {
    u8 unk_0x0[0xe0];
    base::UnkVector<char*> shown;   // 0xe0
    base::UnkVector<char*> hidden;  // 0x108
};

void fn_71007302e4(UnkCharClassBillboardLists_ovb0020* self, char* item) {
    if (self->hidden.count == 0)
        return;
    self->shown.PushBack(item);
    UnkToggle* toggle = reinterpret_cast<UnkToggle*>(item + 0x18);
    fn_71001f3224(toggle, 0);
    fn_71001f32b0(toggle, false);
    self->hidden.Remove(item);
}
