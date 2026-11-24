// Node routine that calls fn_71002c6c6c, fn_710041a198, fn_710045ee14, fn_71004623f0, fn_71004632ac, fn_710046424c, fn_71008c2da8, builds the string ids "relax", "walk", "back", "walkinit", "backinit", "walkend", "backend", sets its vtable and initialises or clears fields.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CCharClassChozoRobotSoldierAIComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkBossAIComponent.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode_46e130 : UnkBossAIComponent {
    u8 oChozoRobotSoldierCannonShotAttackDef[0x118];
    u8 oChozoRobotSoldierDashSlashAttackDef[0xf0];
    u8 oChozoRobotSoldierDisruptionFieldAttackDef[0xf0];
    u8 oChozoRobotSoldierUppercutAttackDef[0x100];
    void** field_0x920;
    UnkStrId field_0x928;
    UnkStrId field_0x930;
    UnkStrId field_0x938;
    UnkStrId field_0x940;
    UnkStrId field_0x948;
    UnkStrId field_0x950;
    UnkStrId field_0x958;
    u8 oShootingPositionQuerySystemDef[0x58];
    u8 oSelectedShootingPositionQuerySystemDef[0x58];
    u8 oNoPathFoundShootingPositionQuerySystemDef[0x58];
    u8 oDamageSourceFactorElite[0x70];
    u8 oDamageSourceFactorCrazy[0x70];
    u8 oDamageSourceFactorCrazyElite[0x70];
};

extern void* g_unk_7101a38b28[];
void CCharClassChozoRobotSoldierCannonShotAttack_Construct(void*);
void fn_71004623f0(void*);
void fn_71004632ac(void*);
void fn_710046424c(void*);
void fn_71002c6c6c(void*);

void fn_710046e130(UnkNode_46e130* self) {
    fn_710041a198(self);
    self->vtable = &g_unk_7101a38b28[2];
    CCharClassChozoRobotSoldierCannonShotAttack_Construct(self->oChozoRobotSoldierCannonShotAttackDef);
    fn_71004623f0(self->oChozoRobotSoldierDashSlashAttackDef);
    fn_71004632ac(self->oChozoRobotSoldierDisruptionFieldAttackDef);
    fn_710046424c(self->oChozoRobotSoldierUppercutAttackDef);
    self->field_0x920 = &g_unk_7101a38c40[2];
    new (&self->field_0x928) UnkStrId("relax", true);
    new (&self->field_0x930) UnkStrId("walk", true);
    new (&self->field_0x938) UnkStrId("back", true);
    new (&self->field_0x940) UnkStrId("walkinit", true);
    new (&self->field_0x948) UnkStrId("backinit", true);
    new (&self->field_0x950) UnkStrId("walkend", true);
    new (&self->field_0x958) UnkStrId("backend", true);
    fn_71002c6c6c(self->oShootingPositionQuerySystemDef);
    fn_71002c6c6c(self->oSelectedShootingPositionQuerySystemDef);
    fn_71002c6c6c(self->oNoPathFoundShootingPositionQuerySystemDef);
    fn_71008c2da8(self->oDamageSourceFactorElite);
    fn_71008c2da8(self->oDamageSourceFactorCrazy);
    fn_71008c2da8(self->oDamageSourceFactorCrazyElite);
    self->field_0x409 = 0;
    self->field_0x3a1 = 1;
}
