// CCameraComponent unit (tools/tu_map.py, the name from reflection): two Vec3 property setters. The
// Vec3 picked from the argument's holder (none when the argument has no type; inline, or behind a pointer
// for 9 or more entries or kind 1) goes to the camera the component's +0x60 slot holds, when there is
// one: to +0x38, then its vf_0xa0 with the value (fn_71010e717c), or to +0x44 and vf_0xa8 (fn_71010e7278). Each
// returns an empty result of kind 2. Names and layouts are placeholders.
#include "types.h"

struct UnkValueSet_ccc {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_ccc {
    void* holder;
    UnkValueSet_ccc* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_ccc {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkVec3_ccc {
    f32 x, y, z;
};

struct UnkCamera_ccc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0(const struct UnkVec3_ccc* value);  // told of the new +0x38
    virtual void vf_0xa8(const struct UnkVec3_ccc* value);  // told of the new +0x44

    u8 unk_0x8[0x30];
    UnkVec3_ccc field_0x38;
    UnkVec3_ccc field_0x44;
};

struct UnkCameraSlot_ccc {
    UnkCamera_ccc* camera;
};

struct UnkCameraComponent_ccc {
    u8 unk_0x0[0x60];
    UnkCameraSlot_ccc* slot;
};

struct UnkCameraComponentOwner_ccc {
    u8 unk_0x0[8];
    UnkCameraComponent_ccc* target;
};

static inline __attribute__((always_inline)) const UnkVec3_ccc* PickArg_ccc(UnkValueSource_ccc* src) {
    if (!src->set)
        return nullptr;
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<const UnkVec3_ccc*>(src->holder);
    return reinterpret_cast<const UnkVec3_ccc*>(src);
}

static inline __attribute__((always_inline)) UnkResult_0x18_ccc Done_ccc() {
    UnkResult_0x18_ccc result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_ccc fn_71010e717c(void*, UnkCameraComponentOwner_ccc* owner, UnkValueSource_ccc** source) {
    UnkCameraComponent_ccc* target = owner->target;
    const UnkVec3_ccc* v = PickArg_ccc(*source);
    if (target->slot) {
        if (UnkCamera_ccc* camera = target->slot->camera) {
            camera->field_0x38.x = v->x;
            camera->field_0x38.y = v->y;
            camera->field_0x38.z = v->z;
            camera->vf_0xa0(v);
        }
    }
    return Done_ccc();
}

UnkResult_0x18_ccc fn_71010e7278(void*, UnkCameraComponentOwner_ccc* owner, UnkValueSource_ccc** source) {
    UnkCameraComponent_ccc* target = owner->target;
    const UnkVec3_ccc* v = PickArg_ccc(*source);
    if (target->slot) {
        if (UnkCamera_ccc* camera = target->slot->camera) {
            camera->field_0x44.x = v->x;
            camera->field_0x44.y = v->y;
            camera->field_0x44.z = v->z;
            camera->vf_0xa8(v);
        }
    }
    return Done_ccc();
}
