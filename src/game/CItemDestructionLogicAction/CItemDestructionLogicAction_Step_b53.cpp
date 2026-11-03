// CItemDestructionLogicAction unit (tools/tu_map.py, the name from reflection): the action's set-up. After
// the base step (fn_7100a55ac8) it resolves its first reference (source +0x18 through fn_7100353be8, state
// +0x40, handle +0x48) and registers {self, fn_7100c95134} in the +0x198 list of the target's
// fn_7100a683a4 component (slot +0x90); then resolves the second (source +0x58 through fn_710023b580, state
// +0x80, handle +0x88) and hands itself to the target's fn_7100a58d88 component (fn_7100cab228). The shape
// of src/game/CFreeAimTutoLogicAction/CFreeAimTutoLogicAction_b53.cpp. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflStatics.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTutoExitAction_b47;
void fn_7100a55ac8(UnkTutoExitAction_b47* self);

using UnkRef = base::UnkRefPtrMember;
UnkRef fn_7100353be8(void* source);

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);
void fn_71002af070(UnkLinkList* list, UnkLinkList** slot);

struct UnkRefl_fn_7100a683a4;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100a683a4>(void*);
struct UnkRefl_fn_7100a58d88;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100a58d88>(void*);

void fn_7100c95134();  // the list callback
void fn_7100cab228(void* component, void* action);

static inline void* Actor(base::UnkRefCounted* target) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10);
}

struct UnkItemDestructionListReg_b53 {
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
};

struct UnkItemDestructionAction_b53 {
    void* vtable;
    u8 pad_0x8[0x10];
    u8 source1[0x28];  // 0x18
    u32 state1;        // 0x40
    u8 pad_0x44[4];
    base::UnkRefHandle handle1;  // 0x48
    u8 pad_0x50[0x8];
    u8 source2[0x28];  // 0x58
    u32 state2;        // 0x80
    u8 pad_0x84[4];
    base::UnkRefHandle handle2;          // 0x88
    UnkItemDestructionListReg_b53 reg;  // 0x90
};

void fn_7100c94e8c(UnkItemDestructionAction_b53* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    if (self->state1 == 1) {
        {
            UnkRef handle = fn_7100353be8(self->source1);
            fn_710031d208(&self->handle1, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        self->state1 = 2;
    }
    if (base::UnkRefCounted* target = self->handle1.ptr) {
        if (Actor(target)) {
            asm("" : "+r"(target));
            char* comp = reinterpret_cast<char*>(base::reflection::refl_FindTyped<UnkRefl_fn_7100a683a4>(Actor(target)));
            if (comp) {
                self->reg.obj = self;
                self->reg.fn = &fn_7100c95134;
                self->reg.Attach(comp);
            }
        }
    }
    if (self->state2 == 1) {
        {
            UnkRefPtr fresh = fn_710023b580(self->source2);
            fn_7100234f78(&self->handle2, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state2 = 2;
    }
    if (base::UnkRefCounted* target = self->handle2.ptr) {
        if (Actor(target)) {
            asm("" : "+r"(target));
            if (void* comp = base::reflection::refl_FindTyped<UnkRefl_fn_7100a58d88>(Actor(target)))
                fn_7100cab228(comp, self);
        }
    }
}
