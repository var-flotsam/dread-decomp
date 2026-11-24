// CScenario unit (tools/tu_map.py unit 2735): the scenario's destructor. Resets the managers that hold
// scenario state (creating them on first use), clears the layers, hands back the objects it registered and
// releases its path list, then destroys its members and runs the base destructor.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkVtableList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/snd/CSoundSystem.h"
#include "game/CGameObject.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2506.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "game/UnkTU63.h"
#include "types.h"

using base::UnkRefPtrMember;
using base::UnkStrIdMember;
using base::UnkStrIdMemberOf;
using base::global::CRntVector;

extern char g_unk_7101cb5528[];  // CScenario's vtable

struct UnkCameraCtrl_ovb0049;
struct UnkObserverReset_ovb0055;
struct UnkRecord_ld8d45c;
struct UnkStruct_710025ad0c;
void fn_7100ebfa5c(UnkNavMeshManager* self);
void fn_7100e8efac(void* world);
void CBreakableTileManager_ClearTileGroups(UnkObserverReset_ovb0055* self);
void fn_7100d8d45c(UnkRecord_ld8d45c* self);
void fn_7100d8ced8(void* p);
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_71011a19a4(void* scenario);
void fn_71011a1d00(void* scenario);
void fn_7101196f4c(void* self);
void CSubAreaManager_Reset(UnkManager* self);
void fn_71011a4f1c(void* self);
void fn_7100e1480c(char* self);
void fn_7100274b48(void* self);
void fn_710025ad0c(UnkStruct_710025ad0c* self);

// The manager in `slot`, made on first use.
template <class T, class A, void (*Ctor)(A*)>
static inline T* Instance(T*& slot, u32 size) {
    T* m = slot;
    if (!m) {
        m = static_cast<T*>(fn_71000839a0(size, 0x3f));
        Ctor(m);
        slot = m;
    }
    return m;
}

struct UnkTag_l1a12b0;
template <>
struct base::UnkPoolListTraits<UnkTag_l1a12b0> {
    static char* vtable() { return g_unk_7101a20a30; }
    static char* base_vtable() { return g_unk_7101a20a50; }
};
using UnkScenarioPoolList_l1a12b0 = base::UnkPoolList<UnkTag_l1a12b0, 8, 0x10>;

// UnkListedPath as src/base/file/CPackManagerDevice.cpp defines it.
struct UnkListedPath {
    UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath* next;
};

struct UnkScenarioPaths_l1a12b0 : base::UnkVtableList<UnkListedPath> {
    ~UnkScenarioPaths_l1a12b0() {
        vtable = &g_unk_7101a1b030[2];
        ClearDestroying();
    }
};

struct UnkScenario_l1a12b0 {
    char* vtable;
    u8 pad_0x8[0x8];
    UnkStrIdMember field_0x10;
    UnkStrIdMember field_0x18;
    u8 field_0x20[0xc8];
    u8 field_0xe8[0xc8];
    u8 field_0x1b0[0xc8];
    CRntVector<UnkStrIdMember> field_0x278;
    UnkScenarioPoolList_l1a12b0 field_0x2a0;
    UnkScenarioPoolList_l1a12b0 field_0x310;
    UnkScenarioPoolList_l1a12b0 field_0x380;
    u8 pad_0x3f0[0x8];
    u8 field_0x3f8[0x428];
    void* registered;  // 0x820
    CRntVector<UnkRefPtrMember> field_0x828;
    UnkRefPtrMember field_0x850;
    u8 field_0x858[0x30];
    UnkScenarioPaths_l1a12b0 paths;  // 0x888
    u8 field_0x8b8[0x60];
    void* gui_a;  // 0x918
    void* gui_b;  // 0x920
};
static_assert(__builtin_offsetof(UnkScenario_l1a12b0, field_0x278) == 0x278, "layout");
static_assert(__builtin_offsetof(UnkScenario_l1a12b0, field_0x3f8) == 0x3f8, "layout");
static_assert(__builtin_offsetof(UnkScenario_l1a12b0, paths) == 0x888, "layout");
static_assert(__builtin_offsetof(UnkScenario_l1a12b0, gui_b) == 0x920, "layout");

static inline void ReleaseGui(void*& object) {
    if (object) {
        fn_71011b57c4(g_unk_717271e8f8, object);
        object = nullptr;
    }
}

void fn_71011a12b0(UnkScenario_l1a12b0* self) {
    self->vtable = g_unk_7101cb5528 + 0x10;
    fn_7100ebfa5c(Instance<UnkNavMeshManager, UnkNavMeshManager, fn_7100ebf278>(g_unk_71726a4510, 0x62ec8));
    fn_7100e8efac(g_unk_7101d47310->field_0x2228);
    CBreakableTileManager_ClearTileGroups(reinterpret_cast<UnkObserverReset_ovb0055*>(g_unk_7101d47310->field_0x1af8));
    fn_7100d8d45c(static_cast<UnkRecord_ld8d45c*>(Instance<void, void, fn_7100d8ced8>(g_unk_7101d47368, 0x4c8)));
    fn_71011a19a4(self);
    fn_71011a1d00(self);
    if (self->registered) {
        UnkManager_7100863a8c* manager =
            Instance<UnkManager_7100863a8c, UnkManager_7100863a8c, fn_7100d46804>(g_unk_71726a4788, 0x6b0);
        fn_7100d476e4(manager, self->registered);
        self->registered = nullptr;
    }
    fn_7101196f4c(g_unk_7101d47330);
    for (UnkListedPath* node = self->paths.head; node; node = node->next)
        fn_7100108094(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(node));
    self->paths.ClearDestroying();
    CSubAreaManager_Reset(Instance<UnkManager, void, fn_7100ee1f34>(g_unk_7101d47360, 0xae8));
    ReleaseGui(self->gui_a);
    ReleaseGui(self->gui_b);

    fn_71011a4f1c(self->field_0x8b8);
    self->paths.~UnkScenarioPaths_l1a12b0();
    fn_7100e1480c(reinterpret_cast<char*>(self->field_0x858));
    self->field_0x850.~UnkRefPtrMember();
    self->field_0x828.Free();
    fn_7100274b48(self->field_0x3f8);
    self->field_0x380.~UnkScenarioPoolList_l1a12b0();
    self->field_0x310.~UnkScenarioPoolList_l1a12b0();
    self->field_0x2a0.~UnkScenarioPoolList_l1a12b0();
    self->field_0x278.Free();
    fn_710025ad0c(reinterpret_cast<UnkStruct_710025ad0c*>(self->field_0x1b0));
    fn_710025ad0c(reinterpret_cast<UnkStruct_710025ad0c*>(self->field_0xe8));
    fn_710025ad0c(reinterpret_cast<UnkStruct_710025ad0c*>(self->field_0x20));
    self->field_0x18.~UnkStrIdMember();
    self->field_0x10.~UnkStrIdMember();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
