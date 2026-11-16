// Unit 2618 (tools/tu_map.py, "lua_lib"): destroys a record of string ids (two, then an array of five),
// releasing them last to first, each only while the pool exists. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdRecord_lfbd434 {
    CStrId field_0x0;
    CStrId field_0x8;
    CStrId ids[5];
};

void fn_7100fbd434(UnkIdRecord_lfbd434* self) {
    for (int i = 5; i-- > 0;)
        self->ids[i].~CStrId();
    self->field_0x8.~CStrId();
    self->field_0x0.~CStrId();
}
