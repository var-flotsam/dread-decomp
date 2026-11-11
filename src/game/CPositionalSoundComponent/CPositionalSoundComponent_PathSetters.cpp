// CPositionalSoundComponent unit (tools/tu_map.py, the name from reflection): reflected setters of the
// four sound paths at +0x50..+0x68. Each takes the path the argument points to (the pointer picked from
// the argument's holder: inline, or behind a pointer for 9 or more entries or kind 1), hands the old
// path back to the sound manager when it is not empty, takes the new one (a counted copy) and hands it
// to the manager when it is not empty. Each returns an empty result of kind 2. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/snd/CSoundSystem.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkSoundMgr;
struct UnkPathNode;
extern UnkSoundMgr* g_unk_7101d4d820;

struct UnkValueSet_psc {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_psc {
    void* holder;
    UnkValueSet_psc* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_psc {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkPositionalSoundComponent_psc {
    u8 unk_0x0[0x50];
    CFilePathStrId paths[4];
};

struct UnkPositionalSoundOwner_psc {
    u8 unk_0x0[8];
    UnkPositionalSoundComponent_psc* target;
};

static inline __attribute__((always_inline)) const CFilePathStrId* PickArg_psc(UnkValueSource_psc* src) {
    if (src->kind == 1 || src->set->count >= 9)
        return *static_cast<const CFilePathStrId**>(src->holder);
    return *reinterpret_cast<const CFilePathStrId**>(src);
}

template <u32 Index>
static inline __attribute__((always_inline)) UnkResult_0x18_psc SetPath_psc(UnkPositionalSoundOwner_psc* owner,
                                                                            UnkValueSource_psc** source) {
    UnkValueSource_psc* src = *source;
    UnkPositionalSoundComponent_psc* target = owner->target;
    const CFilePathStrId* value = PickArg_psc(src);
    CFilePathStrId* path = &target->paths[Index];
    if (path->data != &g_unk_7101d47200->empty)
        fn_7100108094(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(path));
    if (path->data != value->data) {
        UnkStringData* d = value->data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (path->data != &pool->empty) {
            --path->data->ref_count;
            if (path->data->ref_count == 0)
                fn_71000803a4(pool, path->data);
        }
        path->data = d;
    }
    if (path->data != &g_unk_7101d47200->empty)
        fn_710010808c(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(path));
    UnkResult_0x18_psc result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}

UnkResult_0x18_psc fn_710114cf54(void*, UnkPositionalSoundOwner_psc* owner, UnkValueSource_psc** source) {
    return SetPath_psc<0>(owner, source);
}
UnkResult_0x18_psc fn_710114d0c4(void*, UnkPositionalSoundOwner_psc* owner, UnkValueSource_psc** source) {
    return SetPath_psc<1>(owner, source);
}
UnkResult_0x18_psc fn_710114d234(void*, UnkPositionalSoundOwner_psc* owner, UnkValueSource_psc** source) {
    return SetPath_psc<2>(owner, source);
}
UnkResult_0x18_psc fn_710114d3a4(void*, UnkPositionalSoundOwner_psc* owner, UnkValueSource_psc** source) {
    return SetPath_psc<3>(owner, source);
}
