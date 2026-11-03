// Two component destructors of the logic-action shape (fn_7100a3ed8c,
// src/game/CSaveSnapshotToCheckpointLogicAction/CSaveSnapshotToCheckpointLogicAction_Dtor_b53.cpp):
// the class vtable, two adjacent callback lists (+0x5c0 and +0x580, or +0x80 and +0x40; vtables
// g_unk_7101a33fa0 over g_unk_7101a20998) in reverse order, then the base destructor. fn_7100897040 is in the
// CCharClassShootActivatorHidrogigaComponent unit (base fn_7100890010); fn_7100917758 in the
// CMagmaKraidScenarioControllerComponent unit (the component base, fn_71010c4194). Names are
// placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/UnkComponent.h"
#include "types.h"

struct UnkShootActivatorBase_b54;
void fn_7100890010(UnkShootActivatorBase_b54*);

extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a7c850[];  // fn_7100897040's class
extern void* g_unk_7101a81aa0[];  // fn_7100917758's class

struct UnkComponentCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkComponentCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkComponentCallbackList_b54 = base::UnkCallbackListShort<UnkComponentCallbacks_b54>;

struct UnkDtor_fn_7100897040;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100897040> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a7c850); }
    static void base(char* p) { fn_7100890010(reinterpret_cast<UnkShootActivatorBase_b54*>(p)); }
};
struct UnkDtor_fn_7100917758;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100917758> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a81aa0); }
    static void base(char* p) { fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(p)); }
};

template void base::refl_Dtor<UnkDtor_fn_7100897040, base::UnkMemberAt<UnkComponentCallbackList_b54, 0x5c0>,
                              base::UnkMemberAt<UnkComponentCallbackList_b54, 0x580>>(char*);
template void base::refl_Dtor<UnkDtor_fn_7100917758, base::UnkMemberAt<UnkComponentCallbackList_b54, 0x80>,
                              base::UnkMemberAt<UnkComponentCallbackList_b54, 0x40>>(char*);
