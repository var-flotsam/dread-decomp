// GUI::CSpriteGrid unit (tools/tu_map.py, the name from reflection): a reflected getter that returns
// the string id fn_71011fd7a0 makes for the grid at 8. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

CStrId fn_71011fd7a0(char* self);

UnkReflValue fn_71011fdf34(void*, char* obj) {
    CStrId value = fn_71011fd7a0(*reinterpret_cast<char**>(obj + 8));
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
