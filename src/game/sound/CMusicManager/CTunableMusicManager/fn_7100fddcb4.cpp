// Grows an array of 0x60-byte entries to at least the given capacity, copying each entry into
// the new storage and destroying the old one (each entry owns a file-path id list and a string id).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

// A string id over the given pool whose destructor decrements and then re-tests the count.
template <base::global::UnkStringPool** Pool>
struct UnkPooledId {
    base::global::UnkStringData* data;

    UnkPooledId(const UnkPooledId& other) : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
    ~UnkPooledId() {
        if (base::global::UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkPathEntry_0x18 {
    UnkPooledId<&g_unk_7101d47200> field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkPathList : base::UnkVector<UnkPathEntry_0x18> {
    UnkPathList(const UnkPathList& other) asm("fn_7100fddf18");  // fn_7100fddf18
    ~UnkPathList() { Free(); }
};

struct UnkEntry_0x60 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u8 field_0x24;
    u8 field_0x25;
    u8 field_0x26;
    UnkPathList field_0x28;
    u64 field_0x50;
    UnkPooledId<&g_unk_7101d47108> field_0x58;

    UnkEntry_0x60(const UnkEntry_0x60& o)
        : field_0x0(o.field_0x0), field_0x4(o.field_0x4), field_0x8(o.field_0x8), field_0xc(o.field_0xc),
          field_0x10(o.field_0x10), field_0x14(o.field_0x14), field_0x18(o.field_0x18),
          field_0x1c(o.field_0x1c), field_0x20(o.field_0x20), field_0x24(o.field_0x24),
          field_0x25(o.field_0x25), field_0x26(o.field_0x26), field_0x28(o.field_0x28),
          field_0x50(o.field_0x50), field_0x58(o.field_0x58) {}
};

void fn_7100fddcb4(base::UnkVector<UnkEntry_0x60>* v, u32 n) {
    if (v->capacity < n) {
        UnkEntry_0x60* p =
            static_cast<UnkEntry_0x60*>(v->allocator->vf_0x40((u64)n * sizeof(UnkEntry_0x60), 8, v->flags));
        if (v->data) {
            UnkEntry_0x60* src = v->data;
            UnkEntry_0x60* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
                new (dst) UnkEntry_0x60(*src);
                src->~UnkEntry_0x60();
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = p;
        v->capacity = n;
    }
}
