// CGameBlackboard's unit (1647): shrinks a dictionary of keyed string-id tables, keyed by string id, to
// its count: the tables are moved and destroyed, the ids copied and released. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

using UnkKeyedStrIdsDict_laeed64 = base::global::CRntSmallDictionary<CStrId, UnkKeyedStrIds>;

void fn_7100aeed64(UnkKeyedStrIdsDict_laeed64* self) {
    if (self->count == self->capacity)
        return;
    UnkKeyedStrIds* tables = static_cast<UnkKeyedStrIds*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkKeyedStrIds), 8, self->flags));
    CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (UnkKeyedStrIds* old = self->values) {
        UnkKeyedStrIds* dst = tables;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
            new (dst) UnkKeyedStrIds(static_cast<UnkKeyedStrIds&&>(*old));
            fn_71000df388(old);
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        CStrId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) CStrId(*old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = tables;
    self->keys = ids;
    self->capacity = self->count;
}
