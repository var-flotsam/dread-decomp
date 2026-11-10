// msapi::res::CRenderRes's unit (tools/tu_map.py): a render resource's destructor (its data object
// by kind, then the base, fn_71000e9670), and a list of file-path ids built from an array of them.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkResPathVec.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkRenderResData_ovb0076;
void fn_71001accdc(UnkRenderResData_ovb0076* data);
void fn_71001afdfc(UnkRenderResData_ovb0076* data);
void fn_71001b3754(UnkRenderResData_ovb0076* data);
void fn_71001aa3d0(UnkRenderResData_ovb0076* data);
void fn_71001aae7c(UnkRenderResData_ovb0076* data);
void fn_71001aed30(UnkRenderResData_ovb0076* data);
void fn_71001ac1f8(UnkRenderResData_ovb0076* data);
void fn_71001ab964(UnkRenderResData_ovb0076* data);

struct UnkRenderRes_ovb0076;
void fn_71000e9670(UnkRenderRes_ovb0076* self);  // the base destructor

struct UnkRenderRes_ovb0076 {
    u8 pad_0x0[0x70];
    UnkRenderResData_ovb0076* data;
    u32 kind;  // 0x78
};

void fn_71001b2d04(UnkRenderRes_ovb0076* self) {
    if (UnkRenderResData_ovb0076* data = self->data) {
        // An unknown kind leaves the data object alone.
        switch (self->kind) {
        case 0:
            fn_71001accdc(data);
            break;
        case 1:
            fn_71001afdfc(data);
            break;
        case 2:
            fn_71001b3754(data);
            break;
        case 3:
            fn_71001aa3d0(data);
            break;
        case 4:
            fn_71001aae7c(data);
            break;
        case 5:
            fn_71001aed30(data);
            break;
        case 6:
            fn_71001ac1f8(data);
            break;
        case 7:
            fn_71001ab964(data);
            break;
        default:
            goto clear;
        }
        fn_7100083c00(data);
    clear:
        self->data = nullptr;
    }
    fn_71000e9670(self);
}

// A list of file-path ids built from an array of them (each one copied in turn), growing by
// `step`, its allocator the default one when none is given.
void fn_71001b1478(UnkResPathVec* self, const CFilePathStrId* ids, u64 count, u32 step,
                   base::UnkAllocator* allocator, u32 flags) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = step;
    self->field_0x14 = step != 0;
    self->flags = flags;
    if (!allocator)
        allocator = fn_7100083990();
    self->allocator = allocator;
    fn_7100018450(self, count);
    for (const CFilePathStrId* p = ids, *end = ids + count; p != end; ++p) {
        CFilePathStrId id(*p);
        u32 n = self->count;
        u32 c = self->capacity;
        if (c <= n && self->step != 0)
            c = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)c)
            for (;;) {
            }
        fn_7100018450(self, c);
        // Copied bitwise, then the reference taken: count is reloaded after the store.
        CFilePathStrId* slot = &self->data[self->count];
        base::global::UnkStringData* value = id.data;
        __builtin_memcpy(slot, &value, sizeof(value));
        if (&g_unk_7101d47200->empty != value)
            ++value->ref_count;
        ++self->count;
    }
}
