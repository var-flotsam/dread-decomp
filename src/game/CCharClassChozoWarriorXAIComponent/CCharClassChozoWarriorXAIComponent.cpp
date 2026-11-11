// A constructor in the CCharClassChozoWarriorXAIComponent unit: the CCharClassChozoWarriorAIComponent
// constructor fn_7100497584, the vtable, then six members from +0x808 (0xf0 bytes apart). Names and
// layouts are placeholders.
#include "game/CCharClassChozoWarriorAIComponent.h"
#include "game/CCharClassChozoWarriorXChangeWallAttack.h"
#include "game/CCharClassChozoWarriorXGlaiveSpinAttack.h"
#include "game/CCharClassChozoWarriorXLandAttack.h"
#include "game/CCharClassChozoWarriorXSpitAttack.h"
#include "game/CCharClassChozoWarriorXUltimateGrabAttack.h"
#include "game/CCharClassChozoWarriorXWallClimbAttack.h"
#include "types.h"

struct UnkNode;
extern void* g_unk_7101a3b748[];

struct UnkChozoWarriorXDef {
    void** vtable;
    u8 pad_0x8[0x800];
    u8 field_0x808[0xf0];
    u8 field_0x8f8[0xf0];
    u8 field_0x9e8[0xf0];
    u8 field_0xad8[0xf0];
    u8 field_0xbc8[0xf0];
    u8 field_0xcb8[0xf0];
};

void fn_710048d8b4(UnkChozoWarriorXDef* self) {
    fn_7100497584(reinterpret_cast<UnkNode*>(self));
    self->vtable = g_unk_7101a3b748 + 2;
    fn_710047c6b4(self->field_0x808);
    fn_7100480114(self->field_0x8f8);
    fn_710047d830(self->field_0x9e8);
    fn_710047ba88(self->field_0xad8);
    fn_710047e698(self->field_0xbc8);
    fn_710047f4d4(self->field_0xcb8);
}
