// CFreeAimTutoLogicAction unit (tools/tu_map.py, the name from reflection): the action's step. When the
// player's +0x2b0 object holds a state object (+0x50) in state 2 and the game manager's tutorial flag
// (+0x2270) has a composition, leaves the tutorial and is done. Otherwise, on the first pass (state 1 at
// +0x40) takes the handle fn_7100353be8 builds from +0x18 into the slot at +0x48 and moves to state 2,
// then is done when the slot's target has a LIFE component whose flag fn_71008c5970 reads as set.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CTriggerLogicAction.h"
#include "game/GUI/CTutorialComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTU437.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

struct UnkPlayer;

using UnkRef = base::UnkRefPtrMember;

struct UnkTutorialComposition;
struct UnkTutorialRequest;

struct UnkDemolitionBlockLife_b48;
bool fn_71008c5970(UnkDemolitionBlockLife_b48* self);

struct UnkRefl_fn_710031cd8c;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(void*);

struct UnkFreeAimTutoLogicAction_b53 {
    u8 pad_0x0[0x18];
    u8 field_0x18[0x28];
    u32 state;
    u8 pad_0x44[4];
    base::UnkRefHandle target;
};

static inline void* At(void* p, u32 off) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(p) + off);
}

u32 fn_7100a3da50(UnkFreeAimTutoLogicAction_b53* self) {
    if (fn_7100dec788(g_unk_7101d47310) && At(fn_7100dec788(g_unk_7101d47310), 0x2b0)) {
        void* state = At(At(fn_7100dec788(g_unk_7101d47310), 0x2b0), 0x50);
        if (state && *reinterpret_cast<u32*>(reinterpret_cast<char*>(state) + 0x10) == 2) {
            void* flag = g_unk_7101d47310->field_0x2270;
            if (flag && At(flag, 0x10)) {
                fn_7100b8903c(static_cast<UnkTutorialComposition*>(At(flag, 0x10)), nullptr);
                return 1;
            }
        }
    }
    if (self->state == 1) {
        {
            UnkRef handle = fn_7100353be8(self->field_0x18);
            fn_710031d208(&self->target, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        self->state = 2;
    }
    if (base::UnkRefCounted* target = self->target.ptr) {
        if (At(target, 0x10)) {
            asm("" : "+r"(target));
            auto* life = reinterpret_cast<UnkDemolitionBlockLife_b48*>(
                base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(At(target, 0x10)));
            if (life && fn_71008c5970(life))
                return 1;
        }
    }
    return 0;
}

// The destructor (fn_7100a3d7d0) and the set-up (fn_7100a3d920) of the action. The action holds the
// lazily resolved handle's callback list (+0x10, the list's vtables g_unk_7101a33fa0 over
// g_unk_7101a20998), a registration in the player's +0x2a8 set (+0x50) and one in the target's +0x198
// list (+0x68), each with a delegate {self, callback}.

struct UnkLinkList;

struct UnkLogicActionBase;
struct UnkTutoExitAction_b47;

extern char g_unk_7101a8e480[];

struct UnkFreeAimTutoCallbacks_b53;
template <>
struct base::UnkCallbackTraits<UnkFreeAimTutoCallbacks_b53> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

struct UnkFreeAimSetReg_b53 {
    base::UnkDeferredSet<void>* set;
    void* obj;
    void (*fn)();
    // Registers in the set at +0x198 of `owner`.
    void Attach(char* owner) {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        set = reinterpret_cast<base::UnkDeferredSet<void>*>(owner + 0x198);
        fn_71004f3cf0(set, &set);
    }
    ~UnkFreeAimSetReg_b53() {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        fn = nullptr;
        obj = nullptr;
    }
};

struct UnkFreeAimListReg_b53 {
    UnkLinkList* list;
    void* obj;
    void (*fn)();
    // Registers in the list at +0x198 of `owner`.
    void Attach(char* owner) {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        list = reinterpret_cast<UnkLinkList*>(owner + 0x198);
        fn_71002af070(list, &list);
    }
    ~UnkFreeAimListReg_b53() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        fn = nullptr;
        obj = nullptr;
    }
};

struct UnkFreeAimTutoAction_b53 {
    char* vtable;
    u8 pad_0x8[0x8];
    base::UnkCallbackListShort<UnkFreeAimTutoCallbacks_b53> handle;  // 0x10
    UnkFreeAimSetReg_b53 player_reg;                                 // 0x50
    UnkFreeAimListReg_b53 target_reg;                                // 0x68
};

void fn_7100a3d7d0(UnkFreeAimTutoAction_b53* self) {
    self->vtable = g_unk_7101a8e480 + 0x10;
    self->target_reg.~UnkFreeAimListReg_b53();
    self->player_reg.~UnkFreeAimSetReg_b53();
    self->handle.~UnkCallbackListShort();
    fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(self));
}

void fn_7100a3dc44();  // the player-set callback
void fn_7100a3dc4c();  // the target-list callback

struct UnkRefl_fn_71007bb940;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_71007bb940>(void*);

void fn_7100a3d920(UnkFreeAimTutoAction_b53* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    self->player_reg.obj = self;
    self->player_reg.fn = &fn_7100a3dc44;
    if (fn_7100dec788(g_unk_7101d47310) && At(fn_7100dec788(g_unk_7101d47310), 0x2a8)) {
        self->player_reg.Attach(static_cast<char*>(At(fn_7100dec788(g_unk_7101d47310), 0x2a8)));
    }
    auto* action = reinterpret_cast<UnkFreeAimTutoLogicAction_b53*>(self);
    if (action->state == 1) {
        {
            UnkRef handle = fn_7100353be8(action->field_0x18);
            fn_710031d208(&action->target, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        action->state = 2;
    }
    if (base::UnkRefCounted* target = action->target.ptr) {
        if (At(target, 0x10)) {
            asm("" : "+r"(target));
            char* comp = reinterpret_cast<char*>(
                base::reflection::refl_FindTyped<UnkRefl_fn_71007bb940>(At(target, 0x10)));
            if (comp) {
                self->target_reg.obj = self;
                self->target_reg.fn = &fn_7100a3dc4c;
                self->target_reg.Attach(comp);
            }
        }
    }
}
