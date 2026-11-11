// The deleting destructor of the class fn_7100086d7c destroys (vtable unk_7101a1aeb0): the same
// body, then the object is freed (fn_7100083c00).

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/file/CFileCacheDevice.h"
#include "types.h"

extern char unk_7101a1aeb0[];

struct UnkObject_7100086e1c {
    char* vtable;
    base::UnkStrIdMember id;
    u64 field_0x10;
    u8 member_0x18[0x98];
    u8 member_0xb0[0x8];
};

void fn_7100086e1c(UnkObject_7100086e1c* self) {
    self->vtable = unk_7101a1aeb0 + 0x10;
    fn_7100085d8c(self->member_0xb0);
    fn_7100085d68(self->member_0xb0);
    fn_71000853b8(self->member_0x18);
    // The id is released without being reset: the object is freed next.
    if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
        base::global::UnkStringData* data = self->id.data;
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (self->id.data->ref_count == 0)
                fn_71000803a4(pool, self->id.data);
        }
    }
    fn_7100083c00(self);
}
