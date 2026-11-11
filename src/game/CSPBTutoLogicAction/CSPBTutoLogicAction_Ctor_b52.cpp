// CSPBTutoLogicAction's unit (tools/tu_map.py): its constructor, on the logic-action base
// (fn_7100a55aa4): the vtable, a block of flags and values at +0x10, two callback sets at +0x20 and
// +0x60 (each built through its base: a vtable, an empty array with allocator flags 0xf, a cleared
// word; then its own vtable and a cleared owner), an embedded CSPRTutoLogicAction-like object at +0xa0
// (fn_7100a5340c, CSPRTutoLogicAction_Ctor_b52.cpp), a cleared flag, then a delegate at +0x198 bound to
// the object and fn_7100a52ebc. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CSPBTutoLogicAction.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU341.h"
#include "types.h"

// The base constructor, as src/game/CChangeSetupLogicAction/CChangeSetupLogicAction_b35.cpp declares it.
struct UnkChangeSetupLogicActionDef;
struct UnkSPRTutoLogicAction_b52;
void fn_7100a5340c(UnkSPRTutoLogicAction_b52* self);
void fn_7100a52ebc();

struct UnkSPBTutoCallable_b52;

struct UnkSPBTutoCallbackSet_b52 {
    void* vtable;
    base::UnkVector<UnkSPBTutoCallable_b52*> items;
    u32 field_0x30;
    void* owner;

    __attribute__((always_inline)) void Init() {
        vtable = g_unk_7101a20998 + 2;
        new (&items) base::UnkVector<UnkSPBTutoCallable_b52*>(0xf);
        field_0x30 = 0;
        vtable = g_unk_7101a20950 + 2;
        owner = nullptr;
    }
};

struct UnkSPBTutoLogicAction_b52 {
    void* vtable;
    u8 unk_0x8[0x8];
    bool field_0x10;
    bool field_0x11;
    bool field_0x12;
    s32 field_0x14;
    u8 field_0x18;
    u8 field_0x19;
    u8 field_0x1a;
    u8 field_0x1b;
    UnkSPBTutoCallbackSet_b52 first;   // 0x20
    UnkSPBTutoCallbackSet_b52 second;  // 0x60
    u8 spr[0xf0];                      // 0xa0
    bool field_0x190;
    void* field_0x198;
    void* target;      // 0x1a0
    void (*method)();  // 0x1a8
    u64 adjust;        // 0x1b0
};
static_assert(__builtin_offsetof(UnkSPBTutoLogicAction_b52, adjust) == 0x1b0, "layout");

void fn_7100a52dcc(UnkSPBTutoLogicAction_b52* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8f418 + 0x10;
    self->field_0x10 = false;
    self->field_0x11 = true;
    self->field_0x12 = false;
    self->field_0x14 = -1;
    self->field_0x18 = 0;
    self->field_0x19 = 8;
    self->field_0x1a = 0;
    self->field_0x1b = 0;
    self->first.Init();
    self->second.Init();
    fn_7100a5340c(reinterpret_cast<UnkSPRTutoLogicAction_b52*>(self->spr));
    self->field_0x190 = false;
    self->field_0x198 = nullptr;
    self->target = self;
    self->method = &fn_7100a52ebc;
    self->adjust = 0;
}
