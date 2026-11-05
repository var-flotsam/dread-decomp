// CItemDestructionLogicAction unit (tools/tu_map.py, the name from reflection): the action's destructor
// (fn_7100c94c88). The class vtable, then the registration in a target's +0x198 list (+0x90, {list, obj,
// fn}), the two lazily resolved references' callback lists (+0x50 with the list vtable g_unk_7101a20950,
// +0x10 with g_unk_7101a33fa0, both over g_unk_7101a20998), then the logic-action base destructor
// (fn_7100a55ab8). Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkLogicActionBase;
void fn_7100a55ab8(UnkLogicActionBase*);

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);

extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern char g_unk_7101aa50a0[];  // CItemDestructionLogicAction

struct UnkItemDestructionCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkItemDestructionCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
struct UnkItemDestructionHandleCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkItemDestructionHandleCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

// A registration {list, obj, fn} in a list; leaving it unlinks it.
struct UnkItemDestructionListReg_b54 {
    UnkLinkList* list;
    void* obj;
    void (*fn)();
    ~UnkItemDestructionListReg_b54() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        obj = nullptr;
        fn = nullptr;
    }
};

struct UnkDtor_fn_7100c94c88;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100c94c88> {
    static char* vtable() { return g_unk_7101aa50a0; }
    static void base(char* p) { fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(p)); }
};

template void base::refl_Dtor<
    UnkDtor_fn_7100c94c88, base::UnkMemberAt<UnkItemDestructionListReg_b54, 0x90>,
    base::UnkMemberAt<base::UnkCallbackListShort<UnkItemDestructionCallbacks_b54>, 0x50>,
    base::UnkMemberAt<base::UnkCallbackListShort<UnkItemDestructionHandleCallbacks_b54>, 0x10>>(char*);
