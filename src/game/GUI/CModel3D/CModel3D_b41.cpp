// GUI::CModel3D unit (tools/tu_map.py, the name from reflection): a reflected getter that returns a copy of the
// CStrId at +0x1a0 of the object at 8 as a reflected value. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_71011f5120(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    base::global::CStrId value = *reinterpret_cast<base::global::CStrId*>(target + 0x1a0);
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
