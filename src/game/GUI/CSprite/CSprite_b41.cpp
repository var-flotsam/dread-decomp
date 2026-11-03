// GUI::CSprite unit (tools/tu_map.py, the name from reflection): two reflected setters of an enum
// held as a u32 (+0x1c0 / +0x1c4) from its name, a string-id argument compared with the four names
// of the table g_unk_7101cb6868 (fn_710007f140). A known name sets the value and marks the sprite
// dirty (+0x184, virtual at 0xa0) once. Reading the name bumps its use count. Returns an empty value.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflValue;

// A string id's data as the name read sees it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_b41a3 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x10[0x34 - sizeof(CRntString)];
    union {
        s32 field_0x44;
    };
};

extern const char* const g_unk_7101cb6868[4];

struct UnkSprite_b41a3 {
    void** vtable;
    u8 pad_0x8[0x17c];
    bool dirty;  // 0x184
    u8 pad_0x185[0x3b];
    u32 field_0x1c0;
    u32 field_0x1c4;
};

static inline __attribute__((always_inline)) void SetDirty_b41a3(UnkSprite_b41a3* target) {
    if (!target->dirty) {
        target->dirty = true;
        reinterpret_cast<void (*)(UnkSprite_b41a3*)>(target->vtable[0xa0 / 8])(target);
    }
}

template <u32 UnkSprite_b41a3::*Field>
static inline __attribute__((always_inline)) UnkReflValue SetEnumByName_b41a3(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* target = *reinterpret_cast<UnkSprite_b41a3**>(obj + 8);
    const CStrId* id;
    if (!a->type)
        id = nullptr;
    else if (a->field_0x14 == 1 || *reinterpret_cast<const u64*>(a->type + 0x18) > 8)
        id = static_cast<const CStrId*>(a->field_0x0);
    else
        id = reinterpret_cast<const CStrId*>(a);
    ++reinterpret_cast<UnkStrIdText_b41a3*>(id->data)->field_0x44;
    const CRntString* name = &reinterpret_cast<UnkStrIdText_b41a3*>(id->data)->str;
    u32 value;
    if (fn_710007f140(name, g_unk_7101cb6868[0]))
        value = 0;
    else if (fn_710007f140(name, g_unk_7101cb6868[1]))
        value = 1;
    else if (fn_710007f140(name, g_unk_7101cb6868[2]))
        value = 2;
    else if (fn_710007f140(name, g_unk_7101cb6868[3]))
        value = 3;
    else
        return base::reflection::refl_EmptyValue();
    target->*Field = value;
    SetDirty_b41a3(target);
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_71011faccc(void*, char* obj, UnkReflValue* const* args) {
    return SetEnumByName_b41a3<&UnkSprite_b41a3::field_0x1c0>(obj, args);
}

UnkReflValue fn_71011faedc(void*, char* obj, UnkReflValue* const* args) {
    return SetEnumByName_b41a3<&UnkSprite_b41a3::field_0x1c4>(obj, args);
}

// The matching getters: the name of the enum value at +0x1b8 / +0x1c0 / +0x1c4 (from the tables
// g_unk_7101cb6858, two names, and g_unk_7101cb6868, four), or the empty id when it is out of range,
// as a reflected string id (fn_7100009658).
extern const char* const g_unk_7101cb6858[2];

template <u32 Offset, s32 Last, const char* const* Names>
static inline __attribute__((always_inline)) UnkReflValue GetEnumName_b41a3(char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    s32 value = *reinterpret_cast<s32*>(target + Offset);
    CStrId name = value <= Last ? CStrId(Names[static_cast<u32>(value)], true) : CStrId(g_unk_7101d470e0);
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &name, false);
    return r;
}

UnkReflValue fn_71011faaa0(void*, char* obj) {
    return GetEnumName_b41a3<0x1b8, 1, g_unk_7101cb6858>(obj);
}

UnkReflValue fn_71011fabe8(void*, char* obj) {
    return GetEnumName_b41a3<0x1c0, 3, g_unk_7101cb6868>(obj);
}

UnkReflValue fn_71011fadf8(void*, char* obj) {
    return GetEnumName_b41a3<0x1c4, 3, g_unk_7101cb6868>(obj);
}

// The id of the object at +0x1d8 (its first member), or an id of the empty string when there is
// none, as a reflected string id.
UnkReflValue fn_71011fa900(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    const CStrId* source = *reinterpret_cast<const CStrId**>(target + 0x1d8);
    CStrId name = source ? CStrId(*source) : CStrId("", true);
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &name, false);
    return r;
}
