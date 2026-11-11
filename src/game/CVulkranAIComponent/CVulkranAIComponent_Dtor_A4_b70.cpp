// CVulkranAIComponent's unit (tools/tu_map.py unit 1084): the component's destructor (vtable 0x7101a688d0):
// two arrays of refcounted pointers at +0x14760 and +0x14738, a string id at +0x14710, a plain array at
// +0x146e8, then the base's destructor fn_7100369380. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "types.h"

extern void* g_unk_7101a688d0[];

struct UnkVulkranDtor_A4_b70;

template <>
struct base::UnkDtorTraits<UnkVulkranDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a688d0); }
    static void base(char* self) { fn_7100369380(self); }
};

template <class T, u32 Offset>
struct UnkVulkranVec_A4_b70 {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<T>*>(self + Offset)->Free(); }
};

// The arrays of refcounted pointers.
template <u32 Offset>
struct UnkVulkranRefs_A4_b70 {
    static void Release(char* self) {
        auto* v = reinterpret_cast<base::UnkVector<base::UnkRefPtrMember>*>(self + Offset);
        if (v->data) {
            for (u64 i = 0; i < v->count; ++i) {
                // The count is a plain int access, so the decrement may alias the array's count.
                if (char* p = reinterpret_cast<char*>(v->data[i].ptr)) {
                    s32* rc = reinterpret_cast<s32*>(p + 8);
                    if (--*rc == 0)
                        delete v->data[i].ptr;
                }
            }
            v->count = 0;
            v->allocator->vf_0x48(v->data);
            v->capacity = 0;
        }
    }
};

__attribute__((flatten)) void fn_71006db650(char* self) {
    base::refl_Dtor<UnkVulkranDtor_A4_b70, UnkVulkranRefs_A4_b70<0x14760>,
                    UnkVulkranRefs_A4_b70<0x14738>, base::UnkStrIdAt<0x14710>,
                    UnkVulkranVec_A4_b70<u32, 0x146e8>>(self);
}
