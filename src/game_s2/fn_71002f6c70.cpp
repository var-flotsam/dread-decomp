// Creates an object through the global allocator from a type description: the description is
// chosen by whether the source's first word equals the global's, checked with the is-kind-of
// test (fn_710009d50c), and, when it has a constructor hook (CType::m_fn_0x28), its storage is
// allocated (CType::m_size, alignment 8) and constructed through the hook. Returns the object or
// null.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkSource {
    u64 field_0x0;
};

extern u64 g_unk_71727b9db0;

// NON_MATCHING(minor): the two description calls are laid out the other way round (the branch on the
// comparison is b.ne where the original has b.eq); behaviour is identical
void* fn_71002f6c70(void* self, UnkSource* source, u32 flags) {
    const CClass* desc = source->field_0x0 == g_unk_71727b9db0 ? fn_71002effc4() : fn_71002e970c();
    if (fn_710009d50c(desc, fn_7100016894()) && desc->m_fn_0x28) {
        void* obj = fn_7100083990()->vf_0x40(desc->m_size, 8, flags);
        if (obj)
            reinterpret_cast<void (*)(void*)>(desc->m_fn_0x28)(obj);
        return obj;
    }
    return nullptr;
}
