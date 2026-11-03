// A destructor in the CCharClassEmmyValveComponent unit (tools/tu_map.py unit 1212): a record of a
// string id (+0x8) and a string-id table (+0x10, base/UnkStrIdTable.h). The table is reset
// (fn_71004f11f8) and its two arrays freed, then the id is released, as the same members are in
// src/game/CUsableComponent/fn_7100a9e6bc.cpp. Names and layout are placeholders.
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkValveRecord_b44a2 {
    u64 field_0x0;
    UnkStringData* field_0x8;  // CStrId
    UnkStrIdTable field_0x10;
};

void fn_71007b4578(UnkValveRecord_b44a2* self) {
    fn_71004f11f8(&self->field_0x10);
    if (self->field_0x10.keys)
        self->field_0x10.allocator->vf_0x48(self->field_0x10.keys);
    if (self->field_0x10.values)
        self->field_0x10.allocator->vf_0x48(self->field_0x10.values);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->field_0x8;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
}
