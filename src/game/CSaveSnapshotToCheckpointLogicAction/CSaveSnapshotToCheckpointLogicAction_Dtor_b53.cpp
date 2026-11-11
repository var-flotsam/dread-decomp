// Logic-action destructors (tools/tu_map.py: the CSaveSnapshotToCheckpointLogicAction,
// CSaveGameLogicAction and CMarkMinimapLogicAction units): the class vtable, then the members in reverse order (callback lists,
// whose vtables are g_unk_7101a20950 over g_unk_7101a20998, and string ids), then the logic-action base
// destructor (fn_7100a55ab8). Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/CMarkMinimapLogicAction.h"
#include "game/CSaveGameLogicAction.h"
#include "game/CSaveSnapshotToCheckpointLogicAction.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkLogicActionBase;

struct UnkLogicActionCallbacks_b53;
template <>
struct base::UnkCallbackTraits<UnkLogicActionCallbacks_b53> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkLogicActionCallbackList_b53 = base::UnkCallbackListShort<UnkLogicActionCallbacks_b53>;

#define LOGIC_ACTION_DTOR_TRAITS_B53(Tag, Vtable)                                              \
    struct Tag;                                                                                \
    template <>                                                                                \
    struct base::UnkDtorTraits<Tag> {                                                          \
        static char* vtable() { return Vtable; }                                               \
        static void base(char* p) { fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(p)); } \
    };
LOGIC_ACTION_DTOR_TRAITS_B53(UnkDtor_fn_7100a4299c, g_unk_7101a8e6c0)
LOGIC_ACTION_DTOR_TRAITS_B53(UnkDtor_fn_7100a3ed8c, g_unk_7101a8e540)
LOGIC_ACTION_DTOR_TRAITS_B53(UnkDtor_fn_7100a4151c, g_unk_7101a8e640)

template void base::refl_Dtor<UnkDtor_fn_7100a4299c, base::UnkMemberAt<UnkLogicActionCallbackList_b53, 0x20>,
                              base::UnkStrIdAt<0x18>, base::UnkStrIdAt<0x10>>(char*);
template void base::refl_Dtor<UnkDtor_fn_7100a3ed8c, base::UnkMemberAt<UnkLogicActionCallbackList_b53, 0x50>,
                              base::UnkMemberAt<UnkLogicActionCallbackList_b53, 0x10>>(char*);
template void base::refl_Dtor<UnkDtor_fn_7100a4151c, base::UnkMemberAt<UnkLogicActionCallbackList_b53, 0x20>,
                              base::UnkStrIdAt<0x18>>(char*);
