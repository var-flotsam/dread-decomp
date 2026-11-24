// A constructor in the CCharClassChozoWarriorXAIComponent unit: the CCharClassChozoWarriorAIComponent
// constructor fn_7100497584, the vtable, then six members from +0x808 (0xf0 bytes apart). Names and
// layouts are placeholders.
#include "game/CCharClassChozoWarriorAIComponent.h"
#include "game/CCharClassChozoWarriorXAIComponent.h"
#include "game/CCharClassChozoWarriorXChangeWallAttack.h"
#include "game/CCharClassChozoWarriorXGlaiveSpinAttack.h"
#include "game/CCharClassChozoWarriorXLandAttack.h"
#include "game/CCharClassChozoWarriorXSpitAttack.h"
#include "game/CCharClassChozoWarriorXUltimateGrabAttack.h"
#include "game/CCharClassChozoWarriorXWallClimbAttack.h"
#include "types.h"

struct UnkChozoWarriorCharClass;

struct UnkChozoWarriorXDef {
    void** vtable;
    u8 pad_0x8[0x800];
    u8 oChozoWarriorXGlaiveSpinAttackDef[0xf0];
    u8 oChozoWarriorXWallClimbAttackDef[0xf0];
    u8 oChozoWarriorXLandAttackDef[0xf0];
    u8 oChozoWarriorXChangeWallAttackDef[0xf0];
    u8 oChozoWarriorXSpitAttackDef[0xf0];
    u8 oChozoWarriorXUltimateGrabAttackDef[0xf0];
};

void fn_710048d8b4(UnkChozoWarriorXDef* self) {
    fn_7100497584(reinterpret_cast<UnkChozoWarriorCharClass*>(self));
    self->vtable = g_unk_7101a3b748 + 2;
    fn_710047c6b4(self->oChozoWarriorXGlaiveSpinAttackDef);
    fn_7100480114(self->oChozoWarriorXWallClimbAttackDef);
    fn_710047d830(self->oChozoWarriorXLandAttackDef);
    fn_710047ba88(self->oChozoWarriorXChangeWallAttackDef);
    fn_710047e698(self->oChozoWarriorXSpitAttackDef);
    fn_710047f4d4(self->oChozoWarriorXUltimateGrabAttackDef);
}
