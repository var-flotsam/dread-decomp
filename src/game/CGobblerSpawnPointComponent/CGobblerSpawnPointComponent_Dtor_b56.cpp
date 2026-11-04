// CGobblerSpawnPointComponent unit (tools/tu_map.py, the name from reflection): the component's destructor
// (fn_7100a1a63c). The class vtable, then the two callback lists (+0x3f8 and +0x3b8, list vtable
// g_unk_7101a33fa0 over g_unk_7101a20998), then the spawn-point base destructor (fn_7101125c90).
// Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "types.h"

void fn_7101125c90(char*);

extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern char g_unk_7101a8c8d8[];  // CGobblerSpawnPointComponent

struct UnkGobblerCallbacks_b56;
template <>
struct base::UnkCallbackTraits<UnkGobblerCallbacks_b56> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkGobblerCallbackList_b56 = base::UnkCallbackListShort<UnkGobblerCallbacks_b56>;

struct UnkDtor_fn_7100a1a63c;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100a1a63c> {
    static char* vtable() { return g_unk_7101a8c8d8; }
    static void base(char* p) { fn_7101125c90(p); }
};

template void base::refl_Dtor<UnkDtor_fn_7100a1a63c, base::UnkMemberAt<UnkGobblerCallbackList_b56, 0x3f8>,
                              base::UnkMemberAt<UnkGobblerCallbackList_b56, 0x3b8>>(char*);
