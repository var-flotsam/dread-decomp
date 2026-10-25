// Frees an object's array of string ids while holding its lock, then frees its other
// array (whose elements need no destruction).
#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// The 0x38-byte object at +0x28 is the hash stream (base/UnkHashStream.h); fn_710007c0e4 destroys it.
void fn_710007c144(UnkHashStream_0x38* s);

// A string id whose destructor decrements and tests the count in two steps.
struct UnkStrId_710050e8e4 {
    base::global::UnkStringData* data;
    ~UnkStrId_710050e8e4() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkStruct_710050e8e4 {
    base::UnkVector<u64> field_0x0;
    UnkHashStream_0x38 field_0x28;
    base::UnkVector<UnkStrId_710050e8e4> field_0x60;
};

void fn_710050e8e4(UnkStruct_710050e8e4* self) {
    fn_710007c144(&self->field_0x28);
    self->field_0x60.Free();
    fn_710007c0e4(&self->field_0x28);
    self->field_0x0.Free();
}
