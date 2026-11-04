// CPattern unit (tools/tu_map.py, unit 936): fn_7100612f4c, a copy of fn_7100514880 (CEmmyPatrolAction_Dtor_A3_b68):
// the destructor of a class without a base destructor call (vtable 0x7101a561a0) holding a string id at +8 and,
// at +0x10, a base::UnkVector of 0x20-byte polymorphic elements (destroyed through vtable slot 1). Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a561a0[];

struct UnkPatternElem_b69a4 {
    virtual void vf_0x0();
    virtual ~UnkPatternElem_b69a4();
    u8 pad_0x8[0x18];
};

struct UnkPatternDtor_b69a4;

template <>
struct base::UnkDtorTraits<UnkPatternDtor_b69a4> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a561a0); }
};

struct UnkPatternVector_b69a4 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkVector<UnkPatternElem_b69a4>*>(self + 0x10)->Free();
    }
};

__attribute__((flatten)) void fn_7100612f4c(char* self) {
    base::refl_DtorNoBase<UnkPatternDtor_b69a4, UnkPatternVector_b69a4, base::UnkStrIdAt<0x8>>(self);
}
