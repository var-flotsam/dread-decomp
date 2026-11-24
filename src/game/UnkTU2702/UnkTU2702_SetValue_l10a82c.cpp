// CAIComponent unit (tools/tu_map.py unit 2702): stores a reflected value (a float, bool, int, vector or,
// converted, a string) under `name` in the default section of the AI's blackboard at +0x1260, unless the
// blackboard is locked. Names and layouts are placeholders.
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkTU2702.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkEnumType;
UnkEnumType* fn_710009ffd4();  // base::global::TRntString256's type

struct UnkAIBlackboardOwner_l10a82c {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

// Where a reflected value's data is: none without a type, else inline when it fits in 8 bytes.
template <class T>
static inline T* ValueData(UnkReflValue* value) {
    if (!value->type)
        return nullptr;
    if (value->field_0x14 == 1 || *reinterpret_cast<const u64*>(value->type + 0x18) > 8)
        return static_cast<T*>(value->field_0x0);
    return reinterpret_cast<T*>(value);
}

// Stores a copy of the C string `str` as a string with inline storage.
inline void SetString(CBlackboard* blackboard, UnkPropertyName* name, const char** str) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            UnkTunableLine copy(*str);
            v.type = reinterpret_cast<const CType*>(fn_710009ffd4());
            v.value = &copy;
            fn_7100332c80(s, name, &v);
        }
    }
}

template <const CType* (*Type)()>
inline void SetValue(CBlackboard* blackboard, UnkPropertyName* name, void* data) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v = {Type(), data};
            fn_7100332c80(s, name, &v);
        }
    }
}

bool fn_710110a82c(char* self, void* a, void* b) {
    UnkAIBlackboardOwner_l10a82c* owner = reinterpret_cast<UnkAIBlackboardOwner_l10a82c*>(self);
    UnkPropertyName* name = static_cast<UnkPropertyName*>(a);
    UnkReflValue* value = static_cast<UnkReflValue*>(b);
    const CType* type = reinterpret_cast<const CType*>(value->type);
    if (type == fn_710000abbc()) {
        SetValue<fn_710000abbc>(&owner->blackboard, name, ValueData<void>(value));
    } else if (reinterpret_cast<const CType*>(value->type) == fn_710000d228()) {
        SetValue<fn_710000d228>(&owner->blackboard, name, ValueData<void>(value));
    } else if (reinterpret_cast<const CType*>(value->type) == fn_7100014768()) {
        SetValue<fn_7100014768>(&owner->blackboard, name, ValueData<void>(value));
    } else if (reinterpret_cast<const CType*>(value->type) == fn_71000246e4()) {
        SetValue<fn_71000246e4>(&owner->blackboard, name, ValueData<void>(value));
    } else if (fn_710009d52c(reinterpret_cast<const CType*>(value->type), fn_71000081e8())) {
        CBlackboard* blackboard = &owner->blackboard;
        UnkReflValue text = ConvertReflValueToType(value, fn_71000081e8());
        SetString(blackboard, name, ValueData<const char*>(&text));
        fn_71000a1c60(&text);
    }
    return false;
}
