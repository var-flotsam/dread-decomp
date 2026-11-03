// CTutoEnterLogicAction unit (tools/tu_map.py, the name from reflection): the action's destructor
// (fn_7100a58760). The class vtable, then a string id (+0x80), a registration in a deferred set (+0x68,
// {set, obj, fn}; removed through fn_71004567dc), a string id (+0x60), a callback list (+0x20, the list
// vtable g_unk_7101a20950 over g_unk_7101a20998) and a string id (+0x18), then the logic-action base
// destructor (fn_7100a55ab8). Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkLogicActionBase;
void fn_7100a55ab8(UnkLogicActionBase*);

struct UnkTutoEnterSet_b54;
void fn_71004567dc(void* set, void** item);

extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];
extern char g_unk_7101a8f518[];  // CTutoEnterLogicAction

struct UnkTutoEnterCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkTutoEnterCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

// A registration {set, obj, fn}; leaving it unlinks it.
struct UnkTutoEnterReg_b54 {
    UnkTutoEnterSet_b54* set;
    void* obj;
    void (*fn)();
    ~UnkTutoEnterReg_b54() {
        if (set) {
            fn_71004567dc(set, reinterpret_cast<void**>(&set));
            set = nullptr;
        }
        obj = nullptr;
        fn = nullptr;
    }
};

struct UnkDtor_fn_7100a58760;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100a58760> {
    static char* vtable() { return g_unk_7101a8f518; }
    static void base(char* p) { fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(p)); }
};

template void base::refl_Dtor<UnkDtor_fn_7100a58760, base::UnkStrIdAt<0x80>,
                              base::UnkMemberAt<UnkTutoEnterReg_b54, 0x68>, base::UnkStrIdAt<0x60>,
                              base::UnkMemberAt<base::UnkCallbackListShort<UnkTutoEnterCallbacks_b54>, 0x20>,
                              base::UnkStrIdAt<0x18>>(char*);
