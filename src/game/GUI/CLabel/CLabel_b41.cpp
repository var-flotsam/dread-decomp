// GUI::CLabel unit (tools/tu_map.py, the name from reflection): two reflected getters that return
// the names of the label's two layout values (+0x2b4 and +0x328, game/UnkGuiLabel.h) as reflected
// string ids, named by fn_7100b87ac0 and fn_7100b87b10. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

CStrId fn_7100b87ac0(u32 value);
CStrId fn_7100b87b10(u32 value);

template <u32 Offset, CStrId (*Name)(u32)>
static inline __attribute__((always_inline)) UnkReflValue GetValueName_b41a3(char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    CStrId name = Name(*reinterpret_cast<u32*>(target + Offset));
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &name, false);
    return r;
}

UnkReflValue fn_71011efe68(void*, char* obj) {
    return GetValueName_b41a3<0x2b4, fn_7100b87ac0>(obj);
}

UnkReflValue fn_71011effbc(void*, char* obj) {
    return GetValueName_b41a3<0x328, fn_7100b87b10>(obj);
}
