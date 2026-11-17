// build-flags: no-strict-aliasing
// Unit 11 (base::animation::CAnimationResource hint): builds a record of two string ids and a
// vector of string ids filled from a list of names. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkNamedIdList_004578 {
    CStrId field_0x0;
    CStrId field_0x8;
    base::UnkStrIdVec ids;  // 0x10
};

// Appends an id, moving it in: the moved-from id is left holding the empty id.
inline void AppendMoved(base::UnkStrIdVec* ids, CStrId& id) {
    u32 n = ids->count;
    u32 c = ids->capacity;
    if (c <= n && ids->step != 0)
        c = (n / ids->step + 1) * ids->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    ids->Reserve(c);
    CStrId::MoveConstructReloading(&ids->data[ids->count], id);
    ++ids->count;
}

void fn_7100004578(UnkNamedIdList_004578* self, const char* first, const char* second, const char* const* names,
                   u64 count) {
    new (&self->field_0x0) CStrId(first, true);
    new (&self->field_0x8) CStrId(second, true);
    base::UnkStrIdVec* ids = &self->ids;
    ids->data = nullptr;
    ids->count = 0;
    ids->capacity = 0;
    ids->step = 10;
    ids->field_0x14 = true;
    ids->flags = 0xf;
    ids->allocator = fn_7100083990();
    ids->Reserve(count);
    for (const char* const* p = names; p != names + count; ++p) {
        CStrId id(*p, true);
        AppendMoved(ids, id);
    }
}
