// CSamusMovementState::CParams's unit (tools/tu_map.py): caching the parameters from a reflected value,
// applying the movement offset (by itself, or added to the target's position) and the E.M.M.I. contact
// check while standing still. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

extern char g_unk_717273cc20[] __attribute__((visibility("hidden")));  // CSamusMovementState::CParams's class

static inline const CType* ParamsClass() {
    fn_71009b0304();
    return reinterpret_cast<const CType*>(g_unk_717273cc20);
}

// The value's object: in place when it fits in 8 bytes, else through field_0x0.
static inline void* ValueObject(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    return v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8 ? v->field_0x0 : v;
}

struct UnkSamusParamsHolder_b0105 {
    u8 pad_0x0[0x18];
    UnkReflValue value;  // 0x18
    u8 pad_0x30[0x120];
    void* params;  // 0x150
};

// Stores `obj` with its dynamic type in the value at +0x18 and caches it at +0x150 when it is a
// CSamusMovementState::CParams.
void fn_71009b0adc(UnkSamusParamsHolder_b0105* self, UnkReflObject* obj) {
    UnkReflValue* v = &self->value;
    const CType* t = obj->vf_0x0();
    const CType* cur = reinterpret_cast<const CType*>(v->type);
    if (cur && v->field_0x14 == 0 &&
        (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                  : fn_710009d52c(t, cur))) {
        fn_71000a1cc8(v, t, obj);
    } else {
        fn_71000a1c60(v);
        fn_71000a18d4(v, t, obj, false);
    }
    const CType* type = reinterpret_cast<const CType*>(v->type);
    self->params = type && fn_710009d50c(type, ParamsClass()) ? ValueObject(v) : nullptr;
}

struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
void fn_71010d08d4(UnkMovement_71010d08d4* self, const UnkMovementVec_71010d08d4* v, bool start);

struct UnkSamusVec_b0105 {
    f32 x, y, z;
};

struct UnkSamusMovementTarget_b0105 {
    u8 pad_0x0[0x1d8];
    UnkSamusVec_b0105 position;  // 0x1d8
};

struct UnkSamusImpulseParams_b0105 {
    u8 pad_0x0[0x38];
    s32 mode;  // 0x38
    UnkSamusVec_b0105 offset;  // 0x3c
};

static inline bool NearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

// Moves the target by the offset (mode 1) or to its position plus the offset (mode 2), unless the
// offset is zero.
bool fn_71009b05c0(UnkSamusImpulseParams_b0105* self, UnkSamusMovementTarget_b0105* target) {
    if (!target)
        return false;
    if (NearZero(self->offset.x) && NearZero(self->offset.y) && NearZero(self->offset.z))
        return false;
    switch (self->mode) {
    case 1:
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(target),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&self->offset), true);
        break;
    case 2: {
        f32 x = target->position.x + self->offset.x;
        f32 y = target->position.y + self->offset.y;
        f32 z = target->position.z + self->offset.z;
        UnkSamusVec_b0105 p;
        p.x = x;
        p.y = y;
        p.z = z;
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(target),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&p), true);
        break;
    }
    }
    return false;
}

struct UnkEmmyAIStagger_b57a3;
void fn_7100552f10(UnkEmmyAIStagger_b57a3* self);

struct UnkSamusEmmyOwner_b0105 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkSamusCollision_b0105 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkSamusEmmyOwner_b0105* owner;  // 0x30
};

struct UnkSamusContactNode_b0105 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkSamusContactLink_b0105 {
    u8 pad_0x0[0x10];
    UnkSamusContactNode_b0105* node;  // 0x10
};

struct UnkSamusContact_b0105 {
    UnkSamusContactLink_b0105* link;
};

struct UnkSamusMovementData_b0105 {
    u8 pad_0x0[0x118];
    f32 field_0x118;
    u8 pad_0x11c[0xc0];
    f32 field_0x1dc;
};

struct UnkSamusMovementCheck_b0105 {
    u8 pad_0x0[0x38];
    UnkSamusMovementData_b0105* data;  // 0x38
};

// When standing still (+0x118 near zero) while +0x1dc is negative, a contact with an E.M.M.I.'s
// collision component makes the E.M.M.I. react (fn_7100552f10).
void fn_71009b2d38(UnkSamusMovementCheck_b0105* self, UnkSamusContact_b0105* contact) {
    UnkSamusMovementData_b0105* data = self->data;
    f32 v = data->field_0x118;
    if (!(v + 0.001f >= 0.0f && v - 0.001f <= 0.0f))
        return;
    if (!(data->field_0x1dc < 0.0f))
        return;
    if (!contact->link || !contact->link->node)
        return;
    UnkReflObject* object = contact->link->node->object;
    if (!object || !fn_710009d50c(object->vf_0x0(), fn_710085fc44()))
        return;
    UnkSamusEmmyOwner_b0105* owner = static_cast<UnkSamusCollision_b0105*>(object)->owner;
    if (!owner)
        return;
    UnkReflObject* ai = owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        return;
    fn_7100552f10(reinterpret_cast<UnkEmmyAIStagger_b57a3*>(ai));
}
