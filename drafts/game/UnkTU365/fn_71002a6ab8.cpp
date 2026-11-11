#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"
#include <new>
#include "base/UnkCountedResource.h"

using base::global::CStrId;

struct UnkResRef_b0228 {
    UnkCountedResource* ptr;

    UnkResRef_b0228() : ptr(nullptr) {}
    UnkResRef_b0228& operator=(const UnkResRef_b0228& other) {
        UnkCountedResource* p = other.ptr;
        if (p)
            ++p->ref_count;
        UnkCountedResource* old = ptr;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        ptr = p;
        return *this;
    }
};

struct UnkRefTable_b0228 {
    UnkResRef_b0228* refs;  // 0x0
    CStrId* ids;            // 0x8
    u32 count;              // 0x10
    u32 capacity;           // 0x14
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;
};

void fn_71002a6d38(UnkRefTable_b0228* self, u32 capacity);

void fn_71002a6ab8(UnkRefTable_b0228* self, const UnkRefTable_b0228* other) {
    self->refs = nullptr;
    self->ids = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->field_0x18 = other->field_0x18;
    self->field_0x1c = other->field_0x1c;
    self->field_0x20 = other->field_0x20;
    fn_71002a6d38(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; i++) {
        const UnkResRef_b0228* src = &other->refs[i];
        new (&self->refs[i]) UnkResRef_b0228();
        self->refs[i] = *src;
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; i++) {
        base::global::UnkStringData* d = other->ids[i].data;
        // Untyped store: the count is read again after it.
        __builtin_memcpy(&self->ids[i].data, &d, sizeof(d));
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
    }
}
