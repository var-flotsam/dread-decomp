// Units 2601, 2604 and 2609 of tools/tu_map.py (no reflected class; 0x7100f7deac-0x7100fa1254): reflected
// setters of a string id (CStrId, or a CFilePathStrId at +8 in fn_7100f91198). Each takes the id picked
// from the argument's holder (inline, or behind a pointer for 9 or more entries or kind 1) by value (a
// counted copy), assigns it to the object's field and returns an empty result of kind 2. Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkValueSet_ids {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_ids {
    void* holder;
    UnkValueSet_ids* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_ids {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkIdOwner_ids {
    u8 unk_0x0[8];
    char* target;
};

template <class T>
static inline __attribute__((always_inline)) const T* PickArg_ids(UnkValueSource_ids* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return static_cast<const T*>(src->holder);
    return reinterpret_cast<const T*>(src);
}

// CFilePathStrId's counted assignment, as CStrId's.
static inline __attribute__((always_inline)) void AssignPath_ids(CFilePathStrId* self, const CFilePathStrId& other) {
    if (self->data != other.data) {
        UnkStringData* d = other.data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (self->data != &pool->empty) {
            --self->data->ref_count;
            if (self->data->ref_count == 0)
                fn_71000803a4(pool, self->data);
        }
        self->data = d;
    }
}

static inline __attribute__((always_inline)) UnkResult_0x18_ids Done_ids() {
    UnkResult_0x18_ids result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

static inline __attribute__((always_inline)) void SetId_ids(char* target, CStrId id) {
    *reinterpret_cast<CStrId*>(target) = id;
}

static inline __attribute__((always_inline)) void SetPath_ids(char* target, CFilePathStrId path) {
    AssignPath_ids(reinterpret_cast<CFilePathStrId*>(target + 8), path);
}

static inline __attribute__((always_inline)) UnkResult_0x18_ids SetIdArg_ids(UnkIdOwner_ids* owner,
                                                                             UnkValueSource_ids** source) {
    UnkValueSource_ids* src = *source;
    char* target = owner->target;
    SetId_ids(target, *PickArg_ids<CStrId>(src));
    return Done_ids();
}

UnkResult_0x18_ids fn_7100f86550(void*, UnkIdOwner_ids* owner, UnkValueSource_ids** source) {
    return SetIdArg_ids(owner, source);
}
UnkResult_0x18_ids fn_7100f91054(void*, UnkIdOwner_ids* owner, UnkValueSource_ids** source) {
    return SetIdArg_ids(owner, source);
}
UnkResult_0x18_ids fn_7100fa00ac(void*, UnkIdOwner_ids* owner, UnkValueSource_ids** source) {
    return SetIdArg_ids(owner, source);
}
UnkResult_0x18_ids fn_7100f91198(void*, UnkIdOwner_ids* owner, UnkValueSource_ids** source) {
    UnkValueSource_ids* src = *source;
    char* target = owner->target;
    SetPath_ids(target, *PickArg_ids<CFilePathStrId>(src));
    return Done_ids();
}
