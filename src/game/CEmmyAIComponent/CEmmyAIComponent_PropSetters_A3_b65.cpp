// CEmmyAIComponent's unit (tools/tu_map.py): bool property setters in the shape of fn_71004224bc
// (src/game/CCharClassChozoCommanderPowerPulseAttack/CCharClassChozoCommanderPowerPulseAttack_Set_b40.cpp):
// the bool picked from the argument goes to the object returned by slot 0xd8 of the component's
// +0x29ee0 member, and is mirrored in the component. Return an empty result of kind 2. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CEnumType.h"
#include "game/UnkActor.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkValueSet_A3_b65 {
    u8 unk_0x0[0x18];
    u64 count;
};

// An argument: the value is inline unless it is kind 1 or its type is 9 bytes or more.
struct UnkValueSource_A3_b65 {
    void* holder;
    UnkValueSet_A3_b65* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_A3_b65 {
    u64 empty[2];
    u64 kind_and_flag;
};

template <class T>
static inline __attribute__((always_inline)) T* PickArg_A3_b65(UnkValueSource_A3_b65* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<T*>(src->holder);
    return reinterpret_cast<T*>(src);
}

struct UnkEmmyPropState_A3_b65 {
    u8 unk_0x0[0x38];
    bool field_0x38;
    u8 unk_0x39[0x124 - 0x39];
    f32 field_0x124;
    bool field_0x128;
};

struct UnkEmmyPropHolder_A3_b65 {
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
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkEmmyPropState_A3_b65* vf_0xd8();
};

struct UnkEmmyPropComponent_A3_b65 {
    void** vtable;
    u8 unk_0x8[0x14778 - 0x8];
    bool field_0x14778;
    u8 unk_0x14779[0x14c1c - 0x14779];
    f32 field_0x14c1c;
    bool field_0x14c20;
    u8 unk_0x14c21[0x29ee0 - 0x14c21];
    UnkEmmyPropHolder_A3_b65* field_0x29ee0;
};

struct UnkEmmyPropOwner_A3_b65 {
    u8 unk_0x0[8];
    UnkEmmyPropComponent_A3_b65* target;
};

UnkResult_0x18_A3_b65 fn_710056524c(void* unused, UnkEmmyPropOwner_A3_b65* owner, UnkValueSource_A3_b65** source) {
    UnkEmmyPropComponent_A3_b65* target = owner->target;
    bool v = *PickArg_A3_b65<bool>(*source);
    target->field_0x29ee0->vf_0xd8()->field_0x38 = v;
    if (v != target->field_0x14778) {
        target->field_0x14778 = v;
        reinterpret_cast<void (*)(UnkEmmyPropComponent_A3_b65*)>(target->vtable[0x828 / 8])(target);
    }
    UnkResult_0x18_A3_b65 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_A3_b65 fn_7100565538(void* unused, UnkEmmyPropOwner_A3_b65* owner, UnkValueSource_A3_b65** source) {
    UnkEmmyPropComponent_A3_b65* target = owner->target;
    bool v = *PickArg_A3_b65<bool>(*source);
    target->field_0x29ee0->vf_0xd8()->field_0x128 = v;
    target->field_0x14c20 = target->field_0x29ee0->vf_0xd8()->field_0x128;
    UnkResult_0x18_A3_b65 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_A3_b65 fn_7100565430(void* unused, UnkEmmyPropOwner_A3_b65* owner, UnkValueSource_A3_b65** source) {
    UnkEmmyPropComponent_A3_b65* target = owner->target;
    f32 v = *PickArg_A3_b65<f32>(*source);
    target->field_0x29ee0->vf_0xd8()->field_0x124 = v < 0.0f ? 0.0f : v;
    target->field_0x14c1c = target->field_0x29ee0->vf_0xd8()->field_0x124;
    UnkResult_0x18_A3_b65 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

// The same unit: a two-argument call (a perception-type name looked up in
// CEmmyAIComponent::ETargetPerceptionType, g_unk_71726dfca0, and a pointer), each argument null when it
// holds no value, passed to fn_7100546a30 with the owner actor (+0x30).
template <class T>
static inline __attribute__((always_inline)) T* PickArgOrNull_A3_b65(UnkValueSource_A3_b65* src) {
    if (!src->set)
        return nullptr;
    return PickArg_A3_b65<T>(src);
}

struct UnkEmmyRelaxAI_b59;
struct UnkEnumType;
extern UnkEnumType g_unk_71726dfca0;  // as src/game/CEmmyOverrideDeathPositionDef/fn_7100536550.cpp declares it
s32 fn_710009976c(const base::reflection::CEnumType* self, const base::global::CStrId* name);

struct UnkEmmyPerceptionComponent_A3_b65 {
    u8 unk_0x0[0x30];
    UnkActor* owner;
};

struct UnkEmmyPerceptionOwner_A3_b65 {
    u8 unk_0x0[8];
    UnkEmmyPerceptionComponent_A3_b65* target;
};

UnkResult_0x18_A3_b65 fn_7100565b44(void* unused, UnkEmmyPerceptionOwner_A3_b65* owner,
                                    UnkValueSource_A3_b65** source) {
    UnkValueSource_A3_b65* args = *source;
    UnkEmmyPerceptionComponent_A3_b65* target = owner->target;
    base::global::CStrId* name = PickArgOrNull_A3_b65<base::global::CStrId>(&args[0]);
    void* arg = PickArgOrNull_A3_b65<void>(&args[1]);
    s32 type = fn_710009976c(reinterpret_cast<base::reflection::CEnumType*>(&g_unk_71726dfca0), name);
    fn_7100546a30(reinterpret_cast<UnkEmmyRelaxAI_b59*>(target), type, fn_7100d7c990(target->owner), arg);
    UnkResult_0x18_A3_b65 result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
