// Unit 93 of tools/tu_map.py: writers that print a value as text into a 0x200-byte buffer at +0xc of
// the writer and report success (see src/base/UnkUnit93/UnkValueToText.cpp): the name of an enum value and
// the names of the set bits of a flag set. Names and layouts are placeholders.
#include <stdio.h>
#include <string.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/CEnumType.h"
#include "base/reflection/CFlagsetType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkTextWriter_b0166 {
    u8 unk_0x0[0xc];
    char text[0x200];
};

struct UnkTypedValue_b0166 {
    UnkReflObject* type;
    u32* ptr;
};

CStrId fn_71000999cc(UnkReflObject* enum_type, u32 value);

struct UnkCountedStrData_b0166 {
    u8 pad_0x0[0x10];
    const char* str;
    u8 pad_0x18[0x2c];
    union {
        s32 field_0x44;
    };
};

// Reading the string bumps the data's use count (as in src/game/lua_bind/lua_lib_PushTyped_b49a6.cpp).
static inline const char* CStr(const CStrId& id) {
    auto* data = reinterpret_cast<UnkCountedStrData_b0166*>(id.data);
    ++data->field_0x44;
    return data->str ? data->str : g_unk_710162e2ed;
}

bool fn_71000efb88(UnkTextWriter_b0166* self, UnkTypedValue_b0166* value) {
    char* text = self->text;
    UnkReflObject* type = value->type;
    UnkReflObject* enum_type = type && fn_710009d50c(type->vf_0x0(), fn_710009921c()) ? type : nullptr;
    CStrId name = fn_71000999cc(enum_type, *value->ptr);
    strncpy(text, CStr(name), sizeof(self->text));
    return true;
}

void fn_710007e6c4(void* self);
void fn_710007f564(void* self, const char* str);

struct UnkStringBuf_b0166 {
    const char* data;
    u32 length;
    u8 pad_0xc[0x24];
};

struct UnkEnumEntry_b0166 {
    CStrId name;
    s32 bit;
};

static inline const base::reflection::CFlagsetType* FlagsetOf(UnkTypedValue_b0166* value) {
    UnkReflObject* type = value->type;
    return type && fn_710009d50c(type->vf_0x0(), fn_710009a11c())
               ? static_cast<const base::reflection::CFlagsetType*>(reinterpret_cast<const base::reflection::CType*>(type))
               : nullptr;
}

bool fn_71000ef764(UnkTextWriter_b0166* self, UnkTypedValue_b0166* value) {
    UnkStringBuf_b0166 text;
    fn_710007e6c4(&text);
    const base::reflection::CEnumType* flags = reinterpret_cast<const base::reflection::CEnumType*>(FlagsetOf(value)->field_0xe8);
    u64 size = FlagsetOf(value)->m_underlying->m_size;
    u64 bits;
    switch (size) {
    case 4:
        bits = *value->ptr;
        break;
    case 8:
        bits = *reinterpret_cast<u64*>(value->ptr);
        break;
    default:
        bits = 0;
        break;
    }
    bool any = false;
    const UnkEnumEntry_b0166* entries = reinterpret_cast<const UnkEnumEntry_b0166*>(flags->m_values.data);
    for (const UnkEnumEntry_b0166 *it = entries, *end = entries + flags->m_values.count; it != end; ++it) {
        if (!((1ul << it->bit) & bits))
            continue;
        if (any)
            fn_710007f564(&text, ", ");
        auto* data = reinterpret_cast<UnkCountedStrData_b0166*>(it->name.data);
        ++data->field_0x44;
        auto* again = reinterpret_cast<UnkCountedStrData_b0166*>(it->name.data);
        fn_710007f564(&text, again->str ? again->str : g_unk_710162e2ed);
        any = true;
    }
    if (text.length == 0)
        fn_710007e77c(reinterpret_cast<base::global::CRntString*>(&text), "0");
    strncpy(self->text, text.data ? text.data : g_unk_710162e2ed, sizeof(self->text));
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&text));
    return true;
}

