// behaviortree::CBehavior's unit (unit 500 of tools/tu_map.py): the readers of the property a behaviour names. They
// look the value up in the blackboard section g_unk_71726b7cb8 of the AI component (+0x240) of the behaviour's
// actor (+0x18) and, failing that, ask the AI component's class data (+0x10) and the component itself through
// reflection (the field whose name hash matches). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/math/CVector.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU500.h"
#include "game/UnkTU85.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;

// A reflected field (0xb0 bytes).
struct UnkBehaviorField_ovb0265 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkBehaviorClass_ovb0265 {
    u8 pad_0x0[0xe8];
    UnkBehaviorField_ovb0265* fields;  // 0xe8
    u64* keys;                         // 0xf0
    u32 count;                         // 0xf8

    UnkBehaviorField_ovb0265* End() { return fields + count; }
    UnkBehaviorField_ovb0265* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkBehaviorField_ovb0265* FindField(u64 key) {
        UnkBehaviorField_ovb0265* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkBehaviorObject_ovb0265 {
    virtual UnkBehaviorClass_ovb0265* GetClass();
};

struct UnkBehaviorValue_ovb0265 {
    const CType* type;
    void* value;
};

// A string's data: its key (CRC-64) at +0x30.
struct UnkBehaviorStrData_ovb0265 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkBehaviorName_ovb0265 {
    UnkBehaviorStrData_ovb0265* data;
};

struct UnkBehaviorAI_ovb0265 : UnkBehaviorObject_ovb0265 {
    u8 pad_0x8[8];
    UnkBehaviorObject_ovb0265* classData;  // 0x10
    u8 pad_0x18[0x1260 - 0x18];
    CBlackboard blackboard;  // 0x1260
};

struct UnkBehaviorActor_ovb0265 {
    u8 pad_0x0[0x240];
    UnkBehaviorAI_ovb0265* ai;
};

struct UnkBehavior_ovb0265 {
    u8 pad_0x0[0x18];
    UnkBehaviorActor_ovb0265* actor;
};

template <auto Type>
static inline bool Get(UnkBehaviorObject_ovb0265* obj, u64 key, void* out) {
    UnkBehaviorClass_ovb0265* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkBehaviorValue_ovb0265 value = {reinterpret_cast<const CType*>(Type()), out};
    if (UnkBehaviorField_ovb0265* field = cls->FindField(key))
        return fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                             reinterpret_cast<UnkReflValueArg*>(&value));
    return false;
}


// Signature as src/game/CBossRushManager.cpp declares it.

struct UnkSelf;

bool fn_7100394e2c(UnkSelf* behavior, UnkBehaviorName_ovb0265* name, f32* out) {
    UnkBehavior_ovb0265* self = reinterpret_cast<UnkBehavior_ovb0265*>(behavior);
    UnkBehaviorActor_ovb0265* actor = self->actor;
    UnkBehaviorAI_ovb0265* ai = actor ? actor->ai : nullptr;
    if (ai) {
        if (CBlackboard::CSection* section = fn_7101047270(&ai->blackboard, g_unk_71726b7cb8)) {
            if (fn_7100395b34(section, name, out))
                return true;
        }
        UnkBehaviorObject_ovb0265* data = ai->classData;
        if (data && fn_710009d50c(reinterpret_cast<const CType*>(data->GetClass()), fn_7100408b2c())) {
            if (Get<fn_710000abbc>(data, name->data->key, out))
                return true;
        }
    }
    return Get<fn_710000abbc>(ai, name->data->key, out);
}

// The same for a bool, from the section `section` names (the default section while it is empty) of the game
// manager's blackboard when `global` is set.
bool fn_7100394bdc(UnkSelf* behavior, UnkBehaviorName_ovb0265* name, bool* out, const base::global::CStrId* section,
                   bool global) {
    UnkBehavior_ovb0265* self = reinterpret_cast<UnkBehavior_ovb0265*>(behavior);
    UnkBehaviorActor_ovb0265* actor = self->actor;
    UnkBehaviorAI_ovb0265* ai = actor ? actor->ai : nullptr;
    CBlackboard* board;
    if (global) {
        board = &g_unk_7101d47310->field_0x968;
    } else {
        if (!ai)
            goto fallback;
        board = &ai->blackboard;
    }
    {
        const base::global::CStrId& key = section->data == &g_unk_7101d47108->empty ? g_unk_71726b7cb8 : *section;
        if (CBlackboard::CSection* s = fn_7101047270(board, key)) {
            if (fn_710006d11c(s, reinterpret_cast<const base::global::CStrId*>(name), out))
                return true;
        }
        if (ai) {
            UnkBehaviorObject_ovb0265* data = ai->classData;
            if (data && fn_710009d50c(reinterpret_cast<const CType*>(data->GetClass()), fn_7100408b2c())) {
                if (Get<fn_710000d228>(data, name->data->key, out))
                    return true;
            }
        }
    }
fallback:
    return Get<fn_710000d228>(ai, name->data->key, out);
}

// The same for a string of up to 256 characters.
struct UnkEnumType;
UnkEnumType* fn_710009ffd4();
bool fn_7100395c38(CBlackboard::CSection* section, const void* name, CRntString* out);

bool fn_7100395044(UnkSelf* behavior, UnkBehaviorName_ovb0265* name, CRntString* out) {
    UnkBehavior_ovb0265* self = reinterpret_cast<UnkBehavior_ovb0265*>(behavior);
    UnkBehaviorActor_ovb0265* actor = self->actor;
    UnkBehaviorAI_ovb0265* ai = actor ? actor->ai : nullptr;
    if (ai) {
        if (CBlackboard::CSection* section = fn_7101047270(&ai->blackboard, g_unk_71726b7cb8)) {
            if (fn_7100395c38(section, name, out))
                return true;
        }
        UnkBehaviorObject_ovb0265* data = ai->classData;
        if (data && fn_710009d50c(reinterpret_cast<const CType*>(data->GetClass()), fn_7100408b2c())) {
            if (Get<fn_710009ffd4>(data, name->data->key, out))
                return true;
        }
    }
    return Get<fn_710009ffd4>(ai, name->data->key, out);
}

// The same for a vector.
bool fn_7100395d3c(CBlackboard::CSection* section, const CStrId* name, base::math::CVector3D* out);

bool fn_710039525c(UnkSelf* behavior, UnkBehaviorName_ovb0265* name, base::math::CVector3D* out) {
    UnkBehavior_ovb0265* self = reinterpret_cast<UnkBehavior_ovb0265*>(behavior);
    UnkBehaviorActor_ovb0265* actor = self->actor;
    UnkBehaviorAI_ovb0265* ai = actor ? actor->ai : nullptr;
    if (ai) {
        if (CBlackboard::CSection* section = fn_7101047270(&ai->blackboard, g_unk_71726b7cb8)) {
            if (fn_7100395d3c(section, reinterpret_cast<const CStrId*>(name), out))
                return true;
        }
        UnkBehaviorObject_ovb0265* data = ai->classData;
        if (data && fn_710009d50c(reinterpret_cast<const CType*>(data->GetClass()), fn_7100408b2c())) {
            if (Get<fn_71000246e4>(data, name->data->key, out))
                return true;
        }
    }
    return Get<fn_71000246e4>(ai, name->data->key, out);
}
