// GUI::CText unit (tools/tu_map.py, the name from reflection): a reflected setter of the text's font
// (a string id argument, null when it has no type): when fn_7100b879a0 knows the name, its text is
// copied to +0x1c0 and the index stored at +0x1b8, and the object is marked dirty (+0x184, virtual at
// 0xa0) once. Each read of the name bumps its use count. Returns the empty value. Names and layouts
// are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflValue;

s32 fn_7100b879a0(const CRntString* name);

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

struct UnkText_b40a3 {
    void** vtable;
    u8 pad_0x8[0x17c];
    bool dirty;  // 0x184
    u8 pad_0x185[0x33];
    s32 font;  // 0x1b8
    u8 pad_0x1bc[4];
    CRntString font_name;  // 0x1c0
};

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

static inline UnkStrIdText_b40a3* Text_b40a3(const CStrId* id) {
    ++reinterpret_cast<UnkStrIdText_b40a3*>(id->data)->field_0x44;
    return reinterpret_cast<UnkStrIdText_b40a3*>(id->data);
}

UnkReflValue fn_7100b873b0(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* target = *reinterpret_cast<UnkText_b40a3**>(obj + 8);
    const CStrId* id = static_cast<const CStrId*>(ArgPtr_b40a3(a));
    asm("" : "+r"(id));  // keeps the null-argument path the original has
    s32 font = fn_7100b879a0(&Text_b40a3(id)->str);
    if (font != 0x7fffffff) {
        fn_710007eaa0(&target->font_name, &Text_b40a3(id)->str);
        target->font = font;
        if (!target->dirty) {
            target->dirty = true;
            reinterpret_cast<void (*)(UnkText_b40a3*)>(target->vtable[0xa0 / 8])(target);
        }
    }
    return base::reflection::refl_EmptyValue();
}
