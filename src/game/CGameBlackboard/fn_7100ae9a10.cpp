// Builds a qualified id for an object: joins a prefix id, the object's group id (vf_0x20) and
// its own id (vf_0x28) with ':' (skipping empty parts), and stores the result and the object.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkInlineString.h"
#include "game/UnkStringUse.h"

using base::global::CStrId;
using base::global::UnkStringData;

// The id's string (a CRntString at +0x10 of the string data), with its use count raised.
static inline const base::global::CRntString* UseStr(const CStrId& id) {
    ++reinterpret_cast<UnkStringUse*>(id.data)->use_count;
    return reinterpret_cast<const base::global::CRntString*>(reinterpret_cast<char*>(id.data) + 0x10);
}

// Compared as integers: a pointer comparison puts the operands the other way round.
static inline bool HasValue(const CStrId& id) {
    return (u64)id.data != (u64)g_unk_7101d47108 + 0x10;
}

void fn_7100ae9a10(UnkQualifiedId* self, UnkIdSource* source, const CStrId& prefix) {
    UnkInlineString s;
    auto* str = reinterpret_cast<base::global::CRntString*>(&s);
    CRntString_InitWithAllocator(str, reinterpret_cast<base::IAllocator*>(&s.vtable));
    s.vtable = &g_unk_7101a1b750[2];
    s.field_0xb8 = 0;
    s.field_0xc0 = 0;
    self->source = source;
    if (HasValue(prefix)) {
        CRntString_Append(str, UseStr(prefix));
        CRntString_AppendChar(str, ':');
    }
    if (HasValue(source->vf_0x20())) {
        CRntString_Append(str, UseStr(source->vf_0x20()));
        CRntString_AppendChar(str, ':');
    }
    CRntString_Append(str, UseStr(source->vf_0x28()));
    {
        UnkStringData* tmp;
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(&tmp), *str, true);
        // Move-assign: release the old id, then take over the new one.
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (self->id.data && self->id.data != &pool->empty) {
                --self->id.data->ref_count;
                if (self->id.data->ref_count == 0) {
                    StringPool_Release(pool, self->id.data);
                    pool = g_unk_7101d47108;
                }
            }
            self->id.data = &pool->empty;
    }
        self->id.data = tmp;
    }
    CRntString_Clear(str);
    CRntString_Destroy(str);
}
