// Unit 958's (tools/tu_map.py, no hint) destructor of a class without a base destructor (vtable
// 0x7101a58cd8): refcounted pointers at +0xa8 and +0xa0, the node owner at +0x30 (fn_710059c7b8), then an
// array of 0x30-byte polymorphic elements at +8. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a58cd8[];
void fn_710059c7b8(void*);  // as src/game/CVulkranAttackManager/CVulkranAttackManager_Dtor_b49a3.cpp declares it

struct UnkUnit958Dtor_A4_b70;

template <>
struct base::UnkDtorTraits<UnkUnit958Dtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a58cd8); }
};

struct UnkUnit958Elem_A4_b70 {
    virtual ~UnkUnit958Elem_A4_b70();
    u8 field_0x8[0x28];
};
static_assert(sizeof(UnkUnit958Elem_A4_b70) == 0x30, "UnkUnit958Elem_A4_b70 layout");

struct UnkUnit958Nodes_A4_b70 {
    static void Release(char* self) { fn_710059c7b8(self + 0x30); }
};

struct UnkUnit958Elems_A4_b70 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkVector<UnkUnit958Elem_A4_b70>*>(self + 8)->Free();
    }
};

__attribute__((flatten)) void fn_7100639d84(char* self) {
    base::refl_DtorNoBase<UnkUnit958Dtor_A4_b70, base::UnkRefPtrAt<0xa8>, base::UnkRefPtrAt<0xa0>,
                          UnkUnit958Nodes_A4_b70, UnkUnit958Elems_A4_b70>(self);
}
