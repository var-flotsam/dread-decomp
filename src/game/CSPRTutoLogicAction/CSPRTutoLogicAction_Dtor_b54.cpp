// CSPRTutoLogicAction unit (tools/tu_map.py, the name from reflection): the action's destructor
// (fn_7100a53508). The class vtable, then the registration in a target's +0x198 list (+0xc8, {list, obj,
// fn}), an array of owned objects (+0x90), the two lazily resolved references' callback lists (+0x50
// and +0x10, the list vtable g_unk_7101a20950 over g_unk_7101a20998), then the logic-action base
// destructor (fn_7100a55ab8). Names are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkLogicActionBase;
void fn_7100a55ab8(UnkLogicActionBase*);

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);

extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];
extern char g_unk_7101a8f458[];  // CSPRTutoLogicAction

struct UnkSPRTutoCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkSPRTutoCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkSPRTutoCallbackList_b54 = base::UnkCallbackListShort<UnkSPRTutoCallbacks_b54>;

// A registration {list, obj, fn} in a list; leaving it unlinks it.
struct UnkSPRTutoListReg_b54 {
    UnkLinkList* list;
    void* obj;
    void (*fn)();
    ~UnkSPRTutoListReg_b54() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        obj = nullptr;
        fn = nullptr;
    }
};

struct UnkSPRTutoOwned_b54;

struct UnkDtor_fn_7100a53508;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100a53508> {
    static char* vtable() { return g_unk_7101a8f458; }
    static void base(char* p) { fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(p)); }
};

template void base::refl_Dtor<UnkDtor_fn_7100a53508, base::UnkMemberAt<UnkSPRTutoListReg_b54, 0xc8>,
                              base::UnkMemberAt<UnkCloneOwnerVec<UnkSPRTutoOwned_b54>, 0x90>,
                              base::UnkMemberAt<UnkSPRTutoCallbackList_b54, 0x50>,
                              base::UnkMemberAt<UnkSPRTutoCallbackList_b54, 0x10>>(char*);
