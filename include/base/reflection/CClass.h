#pragma once

// Mercury Engine reflection: class descriptors and the per-type getter pattern.
//
// Names: `base::reflection::CClass` is the engine's own name (the descriptor constructor
// passes that string). UnkReflName is the temporary name string the getter builds; it is a
// reference-counted pooled string (likely one of base::global's string types, unconfirmed).

#include "types.h"

namespace base::reflection {

struct UnkStringData {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

struct UnkStringPool {
    u8 unk_0x0[0x10];
    UnkStringData empty;
};

}  // namespace base::reflection

// The global string pool. Accessed through the GOT in the original.
extern base::reflection::UnkStringPool* g_unk_7101d47108;
void fn_71000803a4(base::reflection::UnkStringPool* pool, base::reflection::UnkStringData* data);

namespace base::reflection {

struct UnkReflName {
    UnkStringData* data;

    UnkReflName(const char* str, bool flag);
    ~UnkReflName() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

class CClass {
public:
    // One instantiation per reflected type (data/functions: T::refl_InitClass).
    template <class Create, class Destroy>
    CClass(const UnkReflName& name, const CClass* parent, Create create, Destroy destroy, void* extra);
    ~CClass();

private:
    u8 unk_0x0[0x138];
};

static_assert(sizeof(CClass) == 0x138, "descriptor stride observed in .bss");

}  // namespace base::reflection

// Defines the class getter for one reflected type (data/functions: T::refl_GetClass):
// a guarded static descriptor built from the type name, the parent's descriptor and two
// type-specific functions. ADDR/PARENT/CREATE/DESTROY are bare hex addresses (no 0x).
#define DREAD_REFL_CLASS_GETTER(ADDR, NAME, PARENT, CREATE, DESTROY)                              \
    struct UnkReflType_##ADDR;                                                                    \
    UnkReflType_##ADDR* fn_##CREATE();                                                            \
    void fn_##DESTROY(UnkReflType_##ADDR*);                                                       \
    const base::reflection::CClass* fn_##PARENT();                                                \
    const base::reflection::CClass* fn_##ADDR() {                                                 \
        static base::reflection::CClass s_class(base::reflection::UnkReflName(NAME, true),       \
                                                fn_##PARENT(), &fn_##CREATE, &fn_##DESTROY,      \
                                                nullptr);                                         \
        return &s_class;                                                                          \
    }
