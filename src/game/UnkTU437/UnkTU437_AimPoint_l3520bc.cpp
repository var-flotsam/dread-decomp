// Unit 437 (aim component): the aim point of an actor's collision object named by its character
// class (a CCharClassAIComponent), taken from the animation component's attach transform; the
// given default when there is none. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkNoArg.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CCollisionComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

struct UnkAimClass_l3520bc {
    void** vtable;
    u8 pad_0x8[0x148];
    CStrId aim_name;  // 0x150
    bool field_0x158;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkAimClass_l3520bc*)>(vtable[0])(this); }
};

struct UnkAimAI_l3520bc {
    u8 pad_0x0[0x10];
    UnkAimClass_l3520bc* char_class;
};

struct UnkAimActor_l3520bc {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkAimAI_l3520bc* ai;  // 0x240
    u8 pad_0x248[0x58];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkNameText_l3520bc {
    u8 pad_0x0[0x30];
    const char* text;  // 0x30
};

template <class Id>
static inline const char* Text(const Id& id) {
    return reinterpret_cast<const UnkNameText_l3520bc*>(id.data)->text;
}

void fn_7101179ed0(UnkAnimationComponent* self, const char* a, const char* b, UnkMatrix44* matrix, u32 flag);

UnkVec3f fn_71003520bc(UnkNoArg, UnkAimActor_l3520bc* actor, const UnkVec3f* fallback) {
    UnkVec3f result = *fallback;
    UnkCollisionContainer* collision = actor->collision;
    if (!collision || !actor->ai || !actor->animation)
        return result;
    UnkAnimationComponent* animation = actor->animation;
    UnkAimClass_l3520bc* char_class = actor->ai->char_class;
    UnkAimClass_l3520bc* cast = nullptr;
    if (char_class && fn_710009d50c(char_class->vf_0x0(), fn_7100408b2c()))
        cast = char_class;
    if (!cast->field_0x158 || cast->aim_name.data == &g_unk_7101d47108->empty)
        return result;
    UnkCollisionObject* object = fn_71008640a4(collision, &cast->aim_name);
    if (!object)
        return result;
    UnkCollisionRecord* record = fn_710086411c(collision, object);
    if (!record)
        return result;
    UnkMatrix44 matrix;
    UnkCopyMatrix(&matrix, g_unk_7101a1a680);
    const CStrId* attach = fn_7101177ae8(animation, 0xf4061ff25a2163d6);
    fn_7101179ed0(animation, Text(*attach), Text(record->field_0x10), &matrix, 0);
    result = UnkVec3f(matrix.m[3], matrix.m[7], matrix.m[11]);
    return result;
}
