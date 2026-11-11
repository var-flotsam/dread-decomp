// behaviortree::CEmmyPatrolAction unit (tools/tu_map.py): the destructors of a class without a base
// destructor call (vtable 0x7101a47c40) holding a string id at +8 and, at +0x18, a base::UnkVector of
// 0x10-byte polymorphic elements (each destroyed through its vtable slot 0, then the array freed;
// UnkVector::Free). fn_7100514880 is the plain form, fn_7100514a70 the deleting one. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "types.h"

struct UnkPatrolElem_A3_b68 {
    virtual ~UnkPatrolElem_A3_b68();
    void* field_0x8;
};

struct UnkPatrolDtor_A3_b68;

template <>
struct base::UnkDtorTraits<UnkPatrolDtor_A3_b68> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a47c40); }
};

struct UnkPatrolVector_A3_b68 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkVector<UnkPatrolElem_A3_b68>*>(self + 0x18)->Free();
    }
};

__attribute__((flatten)) void fn_7100514880(char* self) {
    base::refl_DtorNoBase<UnkPatrolDtor_A3_b68, UnkPatrolVector_A3_b68, base::UnkStrIdAt<0x8>>(self);
}

__attribute__((flatten)) void fn_7100514a70(char* self) {
    base::refl_DeletingDtorNoBase<UnkPatrolDtor_A3_b68, UnkPatrolVector_A3_b68, base::UnkStrIdAt<0x8>>(self);
}
