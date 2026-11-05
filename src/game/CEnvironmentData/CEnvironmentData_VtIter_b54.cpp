// Begin and End over reflected arrays of 0x20-byte polymorphic elements holding a string id, copies of
// COffset's (src/game/COffset/COffset_VtIter_b52.cpp: fn_710061d54c, fn_710061d634): the
// CEnvironmentData unit's (tools/tu_map.py unit 2599, sync fn_7100f49724) and unit 2601's (sync
// fn_7100f81c98), both with the element base vtable g_unk_7101cab1f8 (declared in
// base/UnkCloneHelpers.h). One file for the family. Names are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"

void fn_7100f49724(void* it);
void fn_7100f81c98(void* it);

#define VT_ITER_B54(Tag, Sync, BeginFn, EndFn)                                                       \
    struct Tag;                                                                                        \
    template <>                                                                                        \
    struct base::UnkVtIterTraits<Tag> {                                                                \
        static char* base_vtable() { return g_unk_7101cab1f8; }                                        \
        static void sync(void* it) { Sync(it); }                                                       \
    };                                                                                                 \
    void BeginFn(const base::UnkVector<base::UnkStrIdVtElem<Tag, 0x20>>* v,                            \
                 base::UnkCollectionIter<base::UnkStrIdVtElem<Tag, 0x20>>* out) {                      \
        base::refl_VtBegin(v, out);                                                                    \
    }                                                                                                  \
    void EndFn(const base::UnkVector<base::UnkStrIdVtElem<Tag, 0x20>>* v,                              \
               base::UnkCollectionIter<base::UnkStrIdVtElem<Tag, 0x20>>* out) {                        \
        base::refl_VtEnd(v, out);                                                                      \
    }

VT_ITER_B54(UnkEnvVtElemTag_b54, fn_7100f49724, fn_7100f5e4d0, fn_7100f5e5b8)
VT_ITER_B54(UnkUnit2601VtElemTag_b54, fn_7100f81c98, fn_7100f856d0, fn_7100f857b8)
