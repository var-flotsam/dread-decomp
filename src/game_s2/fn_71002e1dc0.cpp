// Assignment operator of a record of two string ids and 16 bytes of other data (after a first word
// that isn't copied): the first id is copied, the second is moved (the source keeps the empty id).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord {
    u64 field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    u8 field_0x18[16];

};

UnkRecord* fn_71002e1dc0(UnkRecord* self, UnkRecord* other) {
    if (self != other) {
        self->field_0x8 = other->field_0x8;
        // The second id is released as a destructor would (in two steps, as CStrId's move does) and
        // then takes the other's.
        base::global::UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            base::global::UnkStringData* data = self->field_0x10.data;
            if (data && data != &pool->empty) {
                --self->field_0x10.data->ref_count;
                if (self->field_0x10.data->ref_count == 0) {
                    fn_71000803a4(pool, self->field_0x10.data);
                    pool = g_unk_7101d47108;
                }
            }
            self->field_0x10.data = &pool->empty;
        }
        self->field_0x10.data = other->field_0x10.data;
        other->field_0x10.data = &g_unk_7101d47108->empty;
        __builtin_memcpy(self->field_0x18, other->field_0x18, sizeof(self->field_0x18));
    }
    return self;
}
