// The descriptor constructor (refl_InitClass) of a reflected class whose getter is inlined into
// fn_7100103ef0 (no name source yet): the shared CClass constructor instantiated with this class's
// traits (size 0x400888, helpers fn_7100103bd8 to fn_7100103bfc).
#include "base/reflection/CClass.h"

DREAD_REFL_TAG(71000f56d4);
bool fn_7100103bd8(const void*, const void*);
s32 fn_7100103be4();
const base::reflection::CType& fn_7100103bec(const void* obj);
void fn_7100103bf8();
void fn_7100103bfc(void* obj);
template <>
struct base::reflection::ClassTraits<UnkRefl_fn_71000f56d4> {
    static constexpr u64 size = 0x400888;
    static constexpr bool abstract = false;
    static constexpr EqualsFn equals = &fn_7100103bd8;
    static constexpr HashFn hash = &fn_7100103be4;
    static constexpr TypeOfFn type_of = &fn_7100103bec;
    static constexpr SlotFn construct = &fn_7100103bf8;
    static constexpr CopyConstructFn copy_construct = nullptr;
    static constexpr CopyConstructFn move_construct = nullptr;
    static constexpr DestroyFn destroy = &fn_7100103bfc;
    static constexpr AssignFn copy_assign = nullptr;
    static constexpr AssignFn move_assign = nullptr;
};
template base::reflection::CClass::CClass(const base::global::CStrId&, const base::reflection::CClass*,
                                          base::reflection::ClassSetupFn, base::reflection::ClassSetupFn,
                                          UnkRefl_fn_71000f56d4*);
