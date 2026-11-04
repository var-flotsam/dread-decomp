// SCameraRailNode unit (tools/tu_map.py, unit 1934): fn_7100c04264, a copy of fn_7100612f4c (CPattern_Dtor_b69a4):
// the destructor of a class without a base destructor call (vtable 0x7101aa2830) holding a string id at +8 and,
// at +0x10, a base::UnkVector of 0x50-byte polymorphic elements (destroyed through vtable slot 1). Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101aa2830[];

struct UnkRailElem_A5_b72 {
    virtual void vf_0x0();
    virtual ~UnkRailElem_A5_b72();
    u8 pad_0x8[0x48];
};

struct UnkRailDtor_A5_b72;

template <>
struct base::UnkDtorTraits<UnkRailDtor_A5_b72> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101aa2830); }
};

struct UnkRailVector_A5_b72 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkVector<UnkRailElem_A5_b72>*>(self + 0x10)->Free();
    }
};

__attribute__((flatten)) void fn_7100c04264(char* self) {
    base::refl_DtorNoBase<UnkRailDtor_A5_b72, UnkRailVector_A5_b72, base::UnkStrIdAt<0x8>>(self);
}
