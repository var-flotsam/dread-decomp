// Mercury reflection class getter (the pattern behind every T::refl_GetClass): a guarded
// static descriptor built from the type name, the parent's descriptor, and two
// type-specific function pointers.
#include "types.h"

struct UnkStringData {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

struct UnkStringPool {
    u8 unk_0x0[0x10];
    UnkStringData empty;
};

extern UnkStringPool* g_unk_7101d47108;
void fn_71000803a4(UnkStringPool* pool, UnkStringData* data);

struct UnkName {
    UnkStringData* data;

    UnkName(const char* str, bool flag);
    ~UnkName() {
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

struct UnkClass {
    template <class Create, class Destroy>
    UnkClass(const UnkName& name, const UnkClass* parent, Create create, Destroy destroy, void* extra);
    ~UnkClass();
    u8 unk_0x0[0x1b8];
};

struct UnkActor;
UnkActor* fn_710022739c();
void fn_710022753c(UnkActor*);
const UnkClass* fn_7101043688();

// NON_MATCHING(minor): the descriptor sits 0x18 after its guard in the original but 0x8 here.
// That offset comes from the other statics in the real translation unit; Phase 4 must
// reproduce that layout so every refl_GetClass reaches O.
const UnkClass* fn_7100227448() {
    static UnkClass s_class(UnkName("CActor", true), fn_7101043688(), &fn_710022739c, &fn_710022753c,
                            nullptr);
    return &s_class;
}
