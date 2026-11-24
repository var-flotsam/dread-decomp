// CCharClassScorpiusAIComponent unit (tools/tu_map.py unit 1026): the constructor (fn_7100690498). The
// boss AI component's base constructor, the vtable, two members at +0x528 and +0x558 (fn_710007e6c4), an
// empty string id at +0x598, the members from +0x5b8 (each by its own constructor), then the flags at
// +0x4fc (both set, one halfword store) and +0x409 (cleared). fn_71006905c0 is the destructor: the
// members in reverse, the string id released, the two members at +0x558 and +0x528, then the base
// destructor (fn_710041a230). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCharClassBossAIComponent.h"
#include "game/CCharClassScorpiusPoisonousGasAttack.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkTU57.h"
#include "types.h"

extern void* g_unk_7101a60e40[];
void fn_7100685f90(void* member);
void fn_7100684150(void* member);
void fn_7100682fd0(void* member);
void fn_710067ecc0(void* member);
void fn_710068504c(void* member);
void fn_71006807f8(void* member);
void fn_710067f9a8(void* member);

struct UnkCharClassScorpiusAIComponent : UnkBossAIComponent {
    u8 sHeadMovementFunction[0x30];
    u8 field_0x558[0x40];
    base::global::UnkStringData* field_0x598;
    u8 pad_0x5a0[0x18];
    u8 oGrabDamageSourceFactor[0x70];
    u8 oWhiplashAttackDef[0xf8];
    u8 oSpikeBallPrickAttackDef[0x100];
    u8 oPoisonousSpitAttackDef[0xf8];
    u8 oDefensiveSpikeBallPrickAttackDef[0xf8];
    u8 oTailSmashAttackDef[0xf8];
    u8 oPoisonousGasAttackDef[0x108];
    u8 oMovingPoisonousGasAttackDef[0x108];
    u8 field_0xd18[0x8];
};

void fn_7100690498(UnkCharClassScorpiusAIComponent* self) {
    fn_710041a198(self);
    self->vtable = g_unk_7101a60e40 + 2;
    fn_710007e6c4(self->sHeadMovementFunction);
    fn_710007e6c4(self->field_0x558);
    self->field_0x598 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x598 = &g_unk_7101d47108->empty;
    fn_71008c2da8(self->oGrabDamageSourceFactor);
    fn_7100685f90(self->oWhiplashAttackDef);
    fn_7100684150(self->oSpikeBallPrickAttackDef);
    fn_7100682fd0(self->oPoisonousSpitAttackDef);
    fn_710067ecc0(self->oDefensiveSpikeBallPrickAttackDef);
    fn_710068504c(self->oTailSmashAttackDef);
    fn_7100681a6c(self->oPoisonousGasAttackDef);
    fn_71006807f8(self->oMovingPoisonousGasAttackDef);
    fn_710067f9a8(self->field_0xd18);
    self->field_0x4fc = 0x101;
    self->field_0x409 = 0;
}

struct UnkGoliathXBase;
void fn_7100685fc0(void* member);
void fn_7100684214(void* member);
void fn_7100683000(void* member);
void fn_710067ecf0(void* member);
void fn_710068507c(void* member);
void fn_7100681c30(void* member);
void fn_71006808a8(void* member);
void fn_710067f9d8(void* member);

void fn_71006905c0(UnkCharClassScorpiusAIComponent* self) {
    self->vtable = g_unk_7101a60e40 + 2;
    fn_710067f9d8(self->field_0xd18);
    fn_71006808a8(self->oMovingPoisonousGasAttackDef);
    fn_7100681c30(self->oPoisonousGasAttackDef);
    fn_710068507c(self->oTailSmashAttackDef);
    fn_710067ecf0(self->oDefensiveSpikeBallPrickAttackDef);
    fn_7100683000(self->oPoisonousSpitAttackDef);
    fn_7100684214(self->oSpikeBallPrickAttackDef);
    fn_7100685fc0(self->oWhiplashAttackDef);
    fn_71008c2de0(self->oGrabDamageSourceFactor);
    reinterpret_cast<base::global::CStrId*>(&self->field_0x598)->Reset();
    CRntString_Destroy(reinterpret_cast<base::global::CRntString*>(self->field_0x558));
    CRntString_Destroy(reinterpret_cast<base::global::CRntString*>(self->sHeadMovementFunction));
    fn_710041a230(reinterpret_cast<UnkGoliathXBase*>(self));
}
