// Unit 83 (base::reflection): collection slot 0x30, the copy constructor of a reflected string-id
// vector (base::UnkStrIdVec, fn_71000cde78) and of a file-path id vector (UnkResPathVec,
// fn_71000d0634, written out as base::UnkStrIdVec's on the file-path pool and its Reserve).

#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkResPathVec.h"
#include "base/global/CStrId.h"
#include "types.h"

void fn_71000cde78(base::UnkStrIdVec* self, const base::UnkStrIdVec* other) {
    new (self) base::UnkStrIdVec(*other);
}

void fn_71000d0634(UnkResPathVec* self, const UnkResPathVec* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100018450(self, other->capacity);
    for (const base::global::CFilePathStrId *p = other->data, *e = p + other->count; p != e; ++p) {
        u32 n = self->count;
        u32 c = self->capacity;
        if (c <= n && self->step != 0)
            c = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)c)
            for (;;) {
            }
        fn_7100018450(self, c);
        base::global::CFilePathStrId* slot = &self->data[self->count];
        base::global::UnkStringData* d = p->data;
        __builtin_memcpy(slot, &d, 8);
        if (&g_unk_7101d47200->empty != d)
            ++d->ref_count;
        ++self->count;
    }
}
