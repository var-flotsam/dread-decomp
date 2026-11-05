// Assignment of a string id from a refcounted object that holds a string (the counterpart of
// fn_71000cc800/fn_71000cc890, src/game_s1/fn_71000cc800.cpp, through a handle): the handle's
// object is retained (count at +0x44), then the id is built from its string at +0x10 and replaces
// the old one. fn_71000cce60 builds it on the file-path pool, fn_71000ccf08 on the default pool.

#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;


typedef u32 UnkAliasU32A1 __attribute__((may_alias));

struct UnkStringHolderA1 {
    u8 pad_0x0[0x10];
    CRntString str;  // +0x10
    u8 pad_after[0x44 - 0x10 - sizeof(CRntString)];
    UnkAliasU32A1 ref_count;  // +0x44 (may alias: the original reloads the handle after the increment)
};


static inline void BuildIdA1(base::UnkStrIdMember*, UnkStringData** out, const CRntString* str) {
    fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(out), *str, true);
}
static inline void BuildIdA1(base::UnkStrIdMemberOf<g_unk_7101d47200>*, UnkStringData** out, const CRntString* str) {
    fn_710001a270(reinterpret_cast<base::global::UnkFilePathStrIdBase*>(out), *str, true);
}

template <base::global::UnkStringPool*& Pool>
static inline base::UnkStrIdMemberOf<Pool>* AssignFromHolder(base::UnkStrIdMemberOf<Pool>* self,
                                                             UnkStringHolderA1** holder) {
    if (reinterpret_cast<void*>(holder) != reinterpret_cast<void*>(self)) {
        ++*reinterpret_cast<UnkAliasU32A1*>(&(*holder)->ref_count);
        UnkStringData* data;
        BuildIdA1(self, &data, &(*holder)->str);
        using Id = base::UnkStrIdMemberOf<Pool>;
        self->~Id();
        self->data = data;
    }
    return self;
}

base::UnkStrIdMemberOf<g_unk_7101d47200>* fn_71000cce60(base::UnkStrIdMemberOf<g_unk_7101d47200>* self,
                                                         UnkStringHolderA1** holder) {
    return AssignFromHolder(self, holder);
}

base::UnkStrIdMember* fn_71000ccf08(base::UnkStrIdMember* self, UnkStringHolderA1** holder) {
    return AssignFromHolder(self, holder);
}
