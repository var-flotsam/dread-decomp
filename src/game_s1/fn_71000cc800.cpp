// Assignment of a string id from a string object (fn_7100000850 builds the new id on the default
// pool, fn_710001a270 on the file-path pool): unless the string is the id itself, the old id is
// released (reset to the empty string) and the new one taken over. fn_71000cc800, fn_71000ccca8
// and fn_71000ccd84 use the default pool, fn_71000cc890 the file-path pool.

#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::UnkStringData;

void fn_7100000850(UnkStringData** out, const void* str, bool flag);  // default pool
void fn_710001a270(UnkStringData** out, const void* str, bool flag);  // file-path pool

static inline void BuildId(base::UnkStrIdMember*, UnkStringData** out, const void* str) {
    fn_7100000850(out, str, true);
}
static inline void BuildId(base::UnkStrIdMemberOf<g_unk_7101d47200>*, UnkStringData** out, const void* str) {
    fn_710001a270(out, str, true);
}

template <base::global::UnkStringPool*& Pool>
static inline base::UnkStrIdMemberOf<Pool>* AssignFromString(base::UnkStrIdMemberOf<Pool>* self,
                                                             const void* str) {
    if (self != str) {
        UnkStringData* data;
        BuildId(self, &data, str);
        using Id = base::UnkStrIdMemberOf<Pool>;
        self->~Id();
        self->data = data;
    }
    return self;
}

base::UnkStrIdMember* fn_71000cc800(base::UnkStrIdMember* self, const void* str) {
    return AssignFromString(self, str);
}

base::UnkStrIdMemberOf<g_unk_7101d47200>* fn_71000cc890(base::UnkStrIdMemberOf<g_unk_7101d47200>* self,
                                                         const void* str) {
    return AssignFromString(self, str);
}

base::UnkStrIdMember* fn_71000ccca8(base::UnkStrIdMember* self, const void* str) {
    return AssignFromString(self, str);
}

base::UnkStrIdMember* fn_71000ccd84(base::UnkStrIdMember* self, const void* str) {
    return AssignFromString(self, str);
}
