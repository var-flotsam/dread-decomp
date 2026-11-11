// CForbiddenEdgesLogicAction's unit (tools/tu_map.py): its constructor, on the logic-action base
// (fn_7100a55aa4): the vtable, then two callback sets at +0x10 and +0x50 (each a base vtable, an empty
// array with allocator flags 0xf, a cleared word, then its own vtable and a cleared owner), and a word
// set to 1. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CTriggerLogicAction.h"
#include "types.h"

// The base constructor, as src/game/CChangeSetupLogicAction/CChangeSetupLogicAction_b35.cpp declares it.
struct UnkChangeSetupLogicActionDef;
extern char g_unk_7101a8f2d8[];
extern void* g_unk_7101a20998[];  // the callback set's base vtable
extern void* g_unk_7101a33fa0[];  // the first set's vtable
extern void* g_unk_7101a20950[];  // the second set's vtable

struct UnkForbiddenEdgesCallable_ovb0085;

struct UnkForbiddenEdgesCallbackSet_ovb0085 {
    void* vtable;
    base::UnkVector<UnkForbiddenEdgesCallable_ovb0085*> items;
    u32 field_0x30;
    void* owner;

    __attribute__((always_inline)) void Init(void** own) {
        vtable = g_unk_7101a20998 + 2;
        new (&items) base::UnkVector<UnkForbiddenEdgesCallable_ovb0085*>(0xf);
        field_0x30 = 0;
        vtable = own + 2;
        owner = nullptr;
    }
};

struct UnkForbiddenEdgesLogicAction_ovb0085 {
    void* vtable;
    u8 unk_0x8[0x8];
    UnkForbiddenEdgesCallbackSet_ovb0085 first;   // 0x10
    UnkForbiddenEdgesCallbackSet_ovb0085 second;  // 0x50
    u32 field_0x90;
};

void fn_7100a4f604(UnkForbiddenEdgesLogicAction_ovb0085* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8f2d8 + 0x10;
    self->first.Init(g_unk_7101a33fa0);
    self->second.Init(g_unk_7101a20950);
    self->field_0x90 = 1;
}
