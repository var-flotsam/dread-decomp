// CPlayerInfo unit (tools/tu_map.py): a copy of fn_7100514880 (CEmmyPatrolAction_Dtor_A3_b68.cpp) for another
// class without a base destructor call (vtable 0x7101caa4b0): string id at +8 and, at +0x10, a base::UnkVector of
// 0x10-byte polymorphic elements. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101caa4b0[];

struct UnkPlayerInfoElem_A6_b72 {
    virtual ~UnkPlayerInfoElem_A6_b72();
    void* field_0x8;
};

struct UnkPlayerInfoDtor_A6_b72;

template <>
struct base::UnkDtorTraits<UnkPlayerInfoDtor_A6_b72> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101caa4b0); }
};

struct UnkPlayerInfoVector_A6_b72 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkVector<UnkPlayerInfoElem_A6_b72>*>(self + 0x10)->Free();
    }
};

__attribute__((flatten)) void fn_7100f0f698(char* self) {
    base::refl_DtorNoBase<UnkPlayerInfoDtor_A6_b72, UnkPlayerInfoVector_A6_b72, base::UnkStrIdAt<0x8>>(self);
}
