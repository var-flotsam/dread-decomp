// CSabotoruSpawnPointComponent unit (tools/tu_map.py, the name from reflection): the component's destructor
// (fn_7100a1c1e0). The class vtable, then two callback lists (+0x3f8 with the list vtable g_unk_7101a20950,
// +0x3b8 with g_unk_7101a33fa0, both over g_unk_7101a20998), then the spawn-point base destructor
// (fn_7101125c90). Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/UnkTU2704.h"
#include "types.h"

extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a8cc00[];  // CSabotoruSpawnPointComponent

struct UnkSabotoruCallbacks_b56;
template <>
struct base::UnkCallbackTraits<UnkSabotoruCallbacks_b56> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
struct UnkSabotoruHandleCallbacks_b56;
template <>
struct base::UnkCallbackTraits<UnkSabotoruHandleCallbacks_b56> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

struct UnkDtor_fn_7100a1c1e0;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100a1c1e0> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a8cc00); }
    static void base(char* p) { fn_7101125c90(p); }
};

template void base::refl_Dtor<
    UnkDtor_fn_7100a1c1e0,
    base::UnkMemberAt<base::UnkCallbackListShort<UnkSabotoruCallbacks_b56>, 0x3f8>,
    base::UnkMemberAt<base::UnkCallbackListShort<UnkSabotoruHandleCallbacks_b56>, 0x3b8>>(char*);
