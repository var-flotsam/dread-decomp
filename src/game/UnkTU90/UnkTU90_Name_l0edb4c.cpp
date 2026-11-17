// Unit 90 of tools/tu_map.py (no class hint): the version writer's start of a reflected value: an
// enum value (kind 2) is written first; a pointer value then writes its type's name as a CName.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CName.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU80.h"
#include "types.h"

using base::global::CName;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValueRef;
using base::reflection::UnkTypedRef;

struct UnkWriter_b0202;

void fn_71000ed480(UnkWriter_b0202* self, const UnkReflValueRef* ref);
bool fn_710009d760(const UnkReflObject* obj, const CType* type);

struct UnkWriteNode_l0edb4c {
    u8 pad_0x0[0xe0];
    s32 kind;  // 0xe0
    u8 pad_0xe4[0xc];
    UnkReflObject* object;  // 0xf0
};

s32 fn_71000edb4c(UnkWriter_b0202* self, UnkWriteNode_l0edb4c* node, const void* ref, const CType* type) {
    if (node->kind == 2) {
        UnkTypedRef value;
        fn_71000a174c(&value, ref);
        fn_71000ed480(self, reinterpret_cast<const UnkReflValueRef*>(&value));
    }
    UnkReflObject* object = node->object;
    CStrId name = g_unk_7101d470e0;
    if (fn_710009d760(object, reinterpret_cast<const CType*>(fn_710009bedc()))) {
        if (!type)
            type = fn_710009c640(object);
        name = type->m_name;
    }
    if (name.data != &g_unk_7101d47108->empty) {
        CName hashed(base::global::UnkNameHash(), name.data->Key());
        UnkReflValueRef value = {fn_710000fd7c(), &hashed};
        fn_71000ed480(self, &value);
    }
    return 1;
}
