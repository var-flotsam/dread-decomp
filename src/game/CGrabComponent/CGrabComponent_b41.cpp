// CGrabComponent unit (tools/tu_map.py, the name from reflection): a reflected getter of whether
// either of the component's two links (+0xc8, +0xd0) holds a target (its +0x10), as a reflected
// bool. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkGrabLink_b41a3 {
    u8 pad_0x0[0x10];
    void* target;
};

UnkReflValue fn_710113f59c(void*, char* obj) {
    char* self = *reinterpret_cast<char**>(obj + 8);
    UnkGrabLink_b41a3* a = *reinterpret_cast<UnkGrabLink_b41a3**>(self + 0xc8);
    bool held;
    if (a && a->target) {
        held = true;
    } else {
        UnkGrabLink_b41a3* b = *reinterpret_cast<UnkGrabLink_b41a3**>(self + 0xd0);
        held = b && b->target;
    }
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &held, false);
    return r;
}
