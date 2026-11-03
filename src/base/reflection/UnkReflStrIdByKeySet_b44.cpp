// Keyed arrays of string ids (8-byte keys, base::UnkKeyedArray's layout): the "set by key" slots
// (CCollectionType +0x140 copy, +0x148 move). The key is searched for; when found, the id is
// copy-assigned, or released in two steps and given the source's (the source left empty).
// One over string ids, one over file-path ids. Names are placeholders.
#include "base/UnkCloneCalls.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "types.h"

using base::global::UnkStringPool;
template <UnkStringPool*& Pool>
using UnkIdsByKey = base::UnkKeyedArray<struct UnkIdsByKeyTag, u64, base::UnkStrIdMemberOf<Pool>>;

template <UnkStringPool*& Pool>
__attribute__((always_inline)) static inline void CopySet(UnkIdsByKey<Pool>* self, const u64* key,
                                                          const base::UnkStrIdMemberOf<Pool>* value) {
    base::UnkStrIdMemberOf<Pool>* p = self->Find(*key);
    if (p != self->End())
        *p = *value;
}

template <UnkStringPool*& Pool>
__attribute__((always_inline)) static inline void MoveSet(UnkIdsByKey<Pool>* self, const u64* key,
                                                          base::UnkStrIdMemberOf<Pool>* value) {
    base::UnkStrIdMemberOf<Pool>* p = self->Find(*key);
    if (p != self->End()) {
        if (UnkStringPool* pool = Pool) {
            if (p->data && p->data != &pool->empty) {
                --p->data->ref_count;
                if (p->data->ref_count == 0) {
                    fn_71000803a4(pool, p->data);
                    pool = Pool;
                }
            }
            p->data = &pool->empty;
        }
        p->data = value->data;
        value->data = &Pool->empty;
    }
}

void fn_71000d9310(UnkIdsByKey<g_unk_7101d47108>* self, const u64* key, const base::UnkStrIdMember* value) {
    CopySet(self, key, value);
}
void fn_71000d93f4(UnkIdsByKey<g_unk_7101d47108>* self, const u64* key, base::UnkStrIdMember* value) {
    MoveSet(self, key, value);
}

// File-path ids (CFilePathStrId's pool).
using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;

void fn_71000dad44(UnkIdsByKey<g_unk_7101d47200>* self, const u64* key, const UnkPathIdMember* value) {
    CopySet(self, key, value);
}
void fn_71000dae28(UnkIdsByKey<g_unk_7101d47200>* self, const u64* key, UnkPathIdMember* value) {
    MoveSet(self, key, value);
}

// Begin (+0x160) and End (+0x168) of a keyed array of file-path ids by string id: the iterator value
// is pointed at the first key and value, or one past the last (base::UnkIterValue::SetRef).
struct UnkPathIdsByStrId {
    UnkPathIdMember* values;
    base::UnkStrIdMember* keys;
    u32 count;
};
using UnkPathIdsByStrIdIter = base::UnkIterValue<UnkPathIdMember, base::UnkStrIdMember>;

// base::UnkIterValue::SetRef, inlined.
__attribute__((always_inline)) static inline void SetRef(UnkPathIdsByStrIdIter* out, const base::UnkStrIdMember* index,
                                                         UnkPathIdMember* element) {
    if (out->state == 2) {
        out->state = 0;
    } else if (out->state == 1) {
        out->value.~Value();
        out->state = 0;
    }
    out->state = 2;
    out->ref.index = index;
    out->ref.element = element;
}

void fn_71000dba2c(const UnkPathIdsByStrId* self, UnkPathIdsByStrIdIter* out) { SetRef(out, self->keys, self->values); }
void fn_71000dbb10(const UnkPathIdsByStrId* self, UnkPathIdsByStrIdIter* out) {
    SetRef(out, self->keys + self->count, self->values + self->count);
}
