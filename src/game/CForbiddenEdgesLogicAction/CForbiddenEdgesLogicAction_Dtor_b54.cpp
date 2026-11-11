// CForbiddenEdgesLogicAction unit (tools/tu_map.py, the name from reflection): the action's destructor
// (fn_7100a4f008). The class vtable, then two callback lists (+0x50 with the list vtable g_unk_7101a20950,
// +0x10 with g_unk_7101a33fa0, both over g_unk_7101a20998), then the logic-action base destructor
// (fn_7100a55ab8). Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CForbiddenEdgesLogicAction.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkLogicActionBase;

struct UnkForbiddenEdgesCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkForbiddenEdgesCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
struct UnkForbiddenEdgesHandleCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkForbiddenEdgesHandleCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

struct UnkDtor_fn_7100a4f008;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100a4f008> {
    static char* vtable() { return g_unk_7101a8f2d8; }
    static void base(char* p) { fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(p)); }
};

template void base::refl_Dtor<
    UnkDtor_fn_7100a4f008,
    base::UnkMemberAt<base::UnkCallbackListShort<UnkForbiddenEdgesCallbacks_b54>, 0x50>,
    base::UnkMemberAt<base::UnkCallbackListShort<UnkForbiddenEdgesHandleCallbacks_b54>, 0x10>>(char*);
