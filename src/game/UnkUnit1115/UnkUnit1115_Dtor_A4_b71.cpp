// Unit 1115 (tools/tu_map.py): a destructor without a base call (vtable 0x7101a6b7f8): an array of
// string ids at +0x40, a plain array at +0x18 and a string id at +0x10. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a6b7f8[];

struct UnkUnit1115Dtor_A4_b71;

template <>
struct base::UnkDtorTraits<UnkUnit1115Dtor_A4_b71> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a6b7f8); }
};

template <class T, u32 Offset>
struct UnkUnit1115Vec_A4_b71 {
    static void Release(char* self) {
        auto* v = reinterpret_cast<base::UnkVector<T>*>(self + Offset);
        if (v->data) {
            for (u64 i = 0; i < v->count; ++i)
                v->data[i].~T();
            v->count = 0;
            v->allocator->vf_0x48(v->data);
            v->capacity = 0;
        }
    }
};

__attribute__((flatten)) void fn_71006fb290(char* self) {
    base::refl_DtorNoBase<UnkUnit1115Dtor_A4_b71, UnkUnit1115Vec_A4_b71<base::UnkStrIdMember, 0x40>,
                          UnkUnit1115Vec_A4_b71<u32, 0x18>, base::UnkStrIdAt<0x10>>(self);
}
