// CSPBTutoLogicAction unit (tools/tu_map.py, the name from reflection): the action's destructor
// (fn_7100a5243c). The class vtable, then a unit-wide u32 (g_unk_71726e1ee8) is cleared, then the
// registration in a component's +0x68 set (+0x198, {set, obj, fn}), the embedded CSPRTutoLogicAction-like
// object (+0xa0, its destructor fn_7100a53508), the two lazily resolved references' callback lists
// (+0x60 and +0x20, the list vtable g_unk_7101a20950 over g_unk_7101a20998), then the logic-action base
// destructor (fn_7100a55ab8). Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkLogicActionBase;
void fn_7100a55ab8(UnkLogicActionBase*);

struct UnkTutoHandle;
void fn_7100565ef0(UnkTutoHandle* handle, UnkTutoHandle** slot);

void fn_7100a53508(char* self);

extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];
extern char g_unk_7101a8f418[];  // CSPBTutoLogicAction
extern u32 g_unk_71726e1ee8;

struct UnkSPBTutoCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkSPBTutoCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkSPBTutoCallbackList_b54 = base::UnkCallbackListShort<UnkSPBTutoCallbacks_b54>;

struct UnkSPBTutoClearFlag_b54 {
    static void Release(char*) { g_unk_71726e1ee8 = 0; }
};

// A registration {set, obj, fn}; leaving it unlinks it.
struct UnkSPBTutoReg_b54 {
    UnkTutoHandle* set;
    void* obj;
    void (*fn)();
    ~UnkSPBTutoReg_b54() {
        if (set) {
            fn_7100565ef0(set, &set);
            set = nullptr;
        }
        obj = nullptr;
        fn = nullptr;
    }
};

struct UnkSPBTutoInner_b54 {
    static void Release(char* self) { fn_7100a53508(self + 0xa0); }
};

struct UnkDtor_fn_7100a5243c;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100a5243c> {
    static char* vtable() { return g_unk_7101a8f418; }
    static void base(char* p) { fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(p)); }
};

template void base::refl_Dtor<UnkDtor_fn_7100a5243c, UnkSPBTutoClearFlag_b54,
                              base::UnkMemberAt<UnkSPBTutoReg_b54, 0x198>, UnkSPBTutoInner_b54,
                              base::UnkMemberAt<UnkSPBTutoCallbackList_b54, 0x60>,
                              base::UnkMemberAt<UnkSPBTutoCallbackList_b54, 0x20>>(char*);
