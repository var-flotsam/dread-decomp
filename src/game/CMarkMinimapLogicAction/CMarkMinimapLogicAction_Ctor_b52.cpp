// CMarkMinimapLogicAction's unit (tools/tu_map.py): its two identical constructors, on the logic-action base
// (fn_7100a55aa4): the vtable, then two callback sets at +0x10 and +0x50, each built through its base
// (a vtable, an empty array with allocator flags 0xf, a cleared word) before its own vtable and a
// cleared owner. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CMarkMinimapLogicAction.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU341.h"
#include "types.h"

// The base constructor, as src/game/CChangeSetupLogicAction/CChangeSetupLogicAction_b35.cpp declares it.
struct UnkChangeSetupLogicActionDef;

struct UnkMarkMinimapCallable_b52;

struct UnkMarkMinimapCallbackSet_b52 {
    void* vtable;
    base::UnkVector<UnkMarkMinimapCallable_b52*> items;
    u32 field_0x30;
    void* owner;

    __attribute__((always_inline)) void Init() {
        vtable = g_unk_7101a20998 + 2;
        new (&items) base::UnkVector<UnkMarkMinimapCallable_b52*>(0xf);
        field_0x30 = 0;
        vtable = g_unk_7101a20950 + 2;
        owner = nullptr;
    }
};

struct UnkMarkMinimapLogicAction_b52 {
    void* vtable;
    u8 unk_0x8[0x8];
    UnkMarkMinimapCallbackSet_b52 first;   // 0x10
    UnkMarkMinimapCallbackSet_b52 second;  // 0x50
};

void fn_7100a3ecdc(UnkMarkMinimapLogicAction_b52* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8e540 + 0x10;
    self->first.Init();
    self->second.Init();
}

void fn_7100a3f0e8(UnkMarkMinimapLogicAction_b52* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8e540 + 0x10;
    self->first.Init();
    self->second.Init();
}
