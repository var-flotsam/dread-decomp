// The resource format's constructor (base/UnkResType.h): its class name and extension as string
// ids, and its source extensions as a vector of string ids filled from a list of names.
#include <new>

#include "base/UnkResFormatMembers.h"
#include "base/UnkResType.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

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

static inline void Construct(UnkResFormatMembers* self, const char* class_name, const char* extension,
                             const char* const* names, u64 count) {
    new (&self->class_name) CStrId(class_name, true);
    new (&self->extension) CStrId(extension, true);
    base::UnkStrIdVec* ids = &self->sources;
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

void fn_7100004578(UnkResFormat* self, const char* class_name, const char* extension, const char* const* names,
                   u64 count) {
    Construct(reinterpret_cast<UnkResFormatMembers*>(self), class_name, extension, names, count);
}
