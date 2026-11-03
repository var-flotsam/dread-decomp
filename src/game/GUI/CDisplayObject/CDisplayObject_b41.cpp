// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): a reflected getter that returns a copy of the
// CStrId at +0x18 of the object at 8 as a reflected value. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_71011d62fc(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    base::global::CStrId value = *reinterpret_cast<base::global::CStrId*>(target + 0x18);
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

// The name (the first member) of the GUI frame the object's frame key (+0xb0) names
// (fn_71011b7a4c on the GUI root), or the empty id when there is none, as a reflected string id.

UnkReflValue fn_71011d3cec(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkGuiFrame* frame = fn_71011b7a4c(g_unk_717271e8f8, *reinterpret_cast<s32*>(target + 0xb0));
    const base::global::CStrId* source =
        frame ? reinterpret_cast<const base::global::CStrId*>(frame) : &g_unk_7101d470e0;
    base::global::CStrId value = *source;
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

// The string id fn_71011dcc54 makes from what fn_71011dccfc gives for the object at +0x128, or the
// empty id when there is none, as a reflected string id.
void* fn_71011dccfc(void* self);
base::global::CStrId fn_71011dcc54(void* self);

UnkReflValue fn_71011d3e5c(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* parent = *reinterpret_cast<void**>(target + 0x128);
    base::global::CStrId value =
        parent ? fn_71011dcc54(fn_71011dccfc(parent)) : base::global::CStrId(g_unk_7101d470e0);
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
