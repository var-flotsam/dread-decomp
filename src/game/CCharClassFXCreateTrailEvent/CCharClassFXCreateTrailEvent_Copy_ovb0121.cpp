// CCharClassFXCreateTrailEvent's unit (tools/tu_map.py): the destructor (fn_7100bb1740: the vtable, the string
// ids at +0x28, +0x18 (a file path) and +0x10 released, then the base destructor fn_7100067ed4) and the copy
// assignment (fn_7100bb1abc). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

template <UnkStringPool** Pool>
struct UnkReleasedId_ovb0121 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedId_ovb0121() {
        if (UnkStringPool* pool = *Pool) {
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
using UnkReleasedStrId_ovb0121 = UnkReleasedId_ovb0121<&g_unk_7101d47108>;
using UnkReleasedPathId_ovb0121 = UnkReleasedId_ovb0121<&g_unk_7101d47200>;

void fn_7100067ed4(void* self);  // the base's destructor

extern void* g_unk_7101aa0f28[];

struct UnkTrailEvent_ovb0121 {
    void** vtable;
    u8 unk_0x8[0x8];
    UnkReleasedStrId_ovb0121 a;      // 0x10
    UnkReleasedPathId_ovb0121 path;  // 0x18
    u8 unk_0x20[0x8];
    UnkReleasedStrId_ovb0121 b;      // 0x28

    __attribute__((always_inline)) ~UnkTrailEvent_ovb0121() { vtable = g_unk_7101aa0f28 + 2; }
};

void fn_7100bb1740(UnkTrailEvent_ovb0121* self) {
    self->~UnkTrailEvent_ovb0121();
    fn_7100067ed4(self);
}


template <UnkStringPool** Pool>
struct UnkCopiedId_ovb0121 {
    UnkStringData* data;

    __attribute__((always_inline)) UnkCopiedId_ovb0121& operator=(const UnkCopiedId_ovb0121& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = *Pool;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkTrailBlock_ovb0121 {
    u64 words[8];
};

struct UnkTrailEventData_ovb0121 {
    void* vtable;
    u32 field_0x8;
    UnkCopiedId_ovb0121<&g_unk_7101d47108> a;     // 0x10
    UnkCopiedId_ovb0121<&g_unk_7101d47200> path;  // 0x18
    u32 field_0x20;
    UnkCopiedId_ovb0121<&g_unk_7101d47108> b;     // 0x28
    UnkTrailBlock_ovb0121 block;                  // 0x30
};

UnkTrailEventData_ovb0121* fn_7100bb1abc(UnkTrailEventData_ovb0121* self, const UnkTrailEventData_ovb0121* other) {
    self->field_0x8 = other->field_0x8;
    self->a = other->a;
    self->path = other->path;
    self->field_0x20 = other->field_0x20;
    self->b = other->b;
    self->block = other->block;
    return self;
}
