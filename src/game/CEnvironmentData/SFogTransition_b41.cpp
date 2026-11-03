// CEnvironmentData::SFogTransition unit (tools/tu_map.py, the name from reflection): a reflected getter that returns a copy of the
// CFilePathStrId at +0x8 of the object at 8 as a reflected value. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_7100f6f874(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    base::global::CFilePathStrId value = *reinterpret_cast<base::global::CFilePathStrId*>(target + 0x8);
    const base::reflection::CType* type = fn_710000ad9c();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
