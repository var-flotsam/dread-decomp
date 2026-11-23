// Unit 92 (tools/tu_map.py): reads one value into a typed reference from the listing's current entry,
// by kind (0 and 2 through their own readers, 1 as the name of the entry's type). Names and layouts
// are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkNoArg.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkListEntry_b0236.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;
#include "base/reflection/TypeObject.h"
template <> base::reflection::CType base::reflection::TypeObject<void>::object asm("g_unk_7101d5af48");

struct UnkReadArg_b0026;

// One entry of the listing (0x68 bytes). The list keeps them after a 4-byte count, so they are only 4-aligned.
#pragma pack(push, 4)

struct UnkEntryList_b0236 {
    u32 count;
    UnkListEntry_b0236 entries[100];
};

struct UnkListOwner_b0236 {
    char* type;  // 0x0
    void* ptr;   // 0x8
    u32 field_0x10;
    UnkEntryList_b0236 list;  // 0x14
    s32 field_0x28b8;
    u8 field_0x28bc;
};
#pragma pack(pop)

bool fn_71000ee4e8(void* reader, u32 kind, UnkReadArg_b0026* arg, const base::global::CStrId* name);
// Neither reader is handed a first argument: the original leaves x0 as the call above returned it (the
// definition of fn_71000eed30 in UnkTU92_Reflist_ovb0236.cpp names it `u32 flag` and never reads it).
bool fn_71000eeae4(UnkNoArg, UnkListEntry_b0236* entry, UnkTypedRef* out);
bool fn_71000eed30(UnkNoArg, UnkListEntry_b0236* entry, UnkTypedRef* out);

// Copies the string id `name` into the value `dst` refers to.
inline bool ReadStrId(UnkTypedRef* dst, const base::global::CStrId* name) {
    const CType* type = dst->type;
    UnkTypedRef src = {fn_7100009658(), const_cast<base::global::CStrId*>(name)};
    return fn_710009d7a8(type, dst, &src);
}

bool fn_71000ee9fc(void* reader, u32 kind, UnkReadArg_b0026* arg, const base::global::CStrId* name) {
    if (!fn_71000ee4e8(reader, kind, arg, name))
        return false;
    UnkListOwner_b0236* self = static_cast<UnkListOwner_b0236*>(reader);
    UnkListEntry_b0236* entry = &self->list.entries[self->list.count - 1];
    UnkTypedRef* out = reinterpret_cast<UnkTypedRef*>(arg);
    switch (kind) {
    case 0:
        fn_71000eeae4(UnkNoArg(), entry, out);
        break;
    case 1: {
        const CType* type = reinterpret_cast<const CType*>(entry->typed.type);
        if (type && entry->typed.ptr)
            ReadStrId(out, &type->m_name);
        else
            ReadStrId(out, &base::reflection::TypeObject<void>::object.m_name);
        break;
    }
    case 2:
        fn_71000eed30(UnkNoArg(), entry, out);
        break;
    default:
        return false;
    }
    return true;
}
