// Unit 259: removes the resource record a string id names from an owner's id-keyed record array,
// dropping its resource reference first. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkParallelFind.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkResRecord_l1d50f8 {
    UnkCountedResource* resource;
    u64 field_0x8;

    void Clear() {
        if (UnkCountedResource* p = resource) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
            resource = nullptr;
        }
        field_0x8 = 0;
    }
};

struct UnkResIdArray_l1d50f8 {
    UnkResRecord_l1d50f8* values;
    base::global::CStrId* keys;
    u32 count;
};

struct UnkResOwner_l1d50f8 {
    u8 pad_0x0[0x3183628];
    UnkResIdArray_l1d50f8 records;
};

struct UnkResRecord_l1d51c0;
UnkResRecord_l1d51c0* fn_71001d51c0(char* array, void* it);

void fn_71001d50f8(UnkResOwner_l1d50f8* self, const base::global::CStrId* name) {
    UnkResIdArray_l1d50f8* records = &self->records;
    UnkResRecord_l1d50f8* slot = UnkParallelFindSlot(records, *name);
    if (slot == &records->values[records->count])
        return;
    slot->Clear();
    fn_71001d51c0(reinterpret_cast<char*>(records), slot);
}
