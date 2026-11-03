// CCharClassWeaponMovement unit (tools/tu_map.py, the name from reflection): a reflected setter of
// the string id at +0xd8 of the object at 8; it bumps the new name's use count and caches the kind
// fn_7100d4bc00 finds for its text at +0xe0, and returns the empty value. Names are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflValue;

u32 fn_7100d4bc00(const CRntString*);

// A string id's data as the name read sees it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_b40a3 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x10[0x34 - sizeof(CRntString)];
    union {
        s32 field_0x44;
    };
};

struct UnkWeaponMovement_b40a3 {
    u8 pad_0x0[0xd8];
    CStrId name;
    u32 kind;
};

UnkReflValue fn_7100ab3bf8(void*, char* obj, UnkReflValue* const* args) {
    auto* target = *reinterpret_cast<UnkWeaponMovement_b40a3**>(obj + 8);
    target->name = *base::reflection::refl_ArgValue<CStrId>(args[0]);
    ++reinterpret_cast<UnkStrIdText_b40a3*>(target->name.data)->field_0x44;
    target->kind = fn_7100d4bc00(&reinterpret_cast<UnkStrIdText_b40a3*>(target->name.data)->str);
    return base::reflection::refl_EmptyValue();
}
