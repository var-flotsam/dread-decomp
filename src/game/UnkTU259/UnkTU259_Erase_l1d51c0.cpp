// Unit 259: erases a record from an array of resource references with a string id per record
// alongside, by moving the last record and id into its place. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A counted reference to a resource.
struct UnkResRef_l1d51c0 {
    UnkCountedResource* ptr;

    UnkResRef_l1d51c0(const UnkResRef_l1d51c0& other) : ptr(nullptr) { *this = other; }
    UnkResRef_l1d51c0& operator=(const UnkResRef_l1d51c0& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkResRef_l1d51c0() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

struct UnkResRecord_l1d51c0 {
    UnkResRef_l1d51c0 resource;
    u32 field_0x8;
    u32 field_0xc;
};

struct UnkResIdArray_l1d51c0 {
    UnkResRecord_l1d51c0* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
};

UnkResRecord_l1d51c0* fn_71001d51c0(char* array, void* it) {
    UnkResIdArray_l1d51c0* self = reinterpret_cast<UnkResIdArray_l1d51c0*>(array);
    u32 index = static_cast<u32>(static_cast<UnkResRecord_l1d51c0*>(it) - self->records);
    self->records[index].~UnkResRecord_l1d51c0();
    self->ids[index].Reset();
    if (self->count - 1 != index) {
        new (&self->records[index]) UnkResRecord_l1d51c0(self->records[self->count - 1]);
        self->records[self->count - 1].~UnkResRecord_l1d51c0();
        CStrId::MoveConstructReloading(&self->ids[index], self->ids[self->count - 1]);
        self->ids[self->count - 1].Reset();
    }
    --self->count;
    return &self->records[index];
}
