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
struct base::reflection::UnkReflClassTraits<UnkRefl_fn_71000f56d4> {
    static constexpr u64 size = 0x400888;
    static constexpr bool abstract = false;
    static constexpr UnkFn_0x58 fn_0x58 = &fn_7100103bd8;
    static constexpr UnkFn_0x60 fn_0x60 = &fn_7100103be4;
    static constexpr UnkFn_0x68 fn_0x68 = &fn_7100103bec;
    static constexpr UnkFn fn_0x28 = &fn_7100103bf8;
    static constexpr UnkFn_0x30 fn_0x30 = nullptr;
    static constexpr UnkFn_0x30 fn_0x38 = nullptr;
    static constexpr UnkFn_0x40 fn_0x40 = &fn_7100103bfc;
    static constexpr UnkFn_0x48 fn_0x48 = nullptr;
    static constexpr UnkFn_0x48 fn_0x50 = nullptr;
};
template base::reflection::CClass::CClass(const base::global::CStrId&, const base::reflection::CClass*,
                                          base::reflection::UnkFn_0x70, base::reflection::UnkFn_0x70,
                                          UnkRefl_fn_71000f56d4*);
