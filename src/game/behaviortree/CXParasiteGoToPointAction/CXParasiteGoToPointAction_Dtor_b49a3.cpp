// behaviortree::CXParasiteGoToPointAction unit (tools/tu_map.py unit 1095). fn_71006e3504: a destructor:
// the vtable, the plain array at +0x78 freed, the string ids at +0x38 and +0x30 released, then the
// base's destructor fn_7100393ea0 (as behaviortree::CGoToPointAction's, CGoToPointAction_Dtor_b45.cpp).
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a69e10[];
void fn_7100393ea0(void*);  // the base's destructor

// An array of plain values: freeing it destroys nothing.
struct UnkPlainArrayMember_b49a3 {
    base::UnkVector<u64> array;
    ~UnkPlainArrayMember_b49a3() { array.Free(); }
};

struct CXParasiteGoToPointActionDtorTag_b49a3;
template <>
struct base::UnkDtorTraits<CXParasiteGoToPointActionDtorTag_b49a3> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a69e10); }
    static void base(char* self) { fn_7100393ea0(self); }
};

__attribute__((flatten)) void fn_71006e3504(char* self) {
    base::refl_Dtor<CXParasiteGoToPointActionDtorTag_b49a3, base::UnkMemberAt<UnkPlainArrayMember_b49a3, 0x78>,
                    base::UnkStrIdAt<0x38>, base::UnkStrIdAt<0x30>>(self);
}
