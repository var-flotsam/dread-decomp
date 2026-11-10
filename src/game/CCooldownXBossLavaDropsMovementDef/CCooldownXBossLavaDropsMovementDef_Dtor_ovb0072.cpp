// CCooldownXBossLavaDropsMovementDef's unit (tools/tu_map.py): the destructor of the
// lava drops attack definition whose fields fn_71004ade14 reflects (fTimeInit at +0x100 to
// fAimMaxSpeedMaxDistance at +0x124, tInitialState at +0x128, tPatterns at +0x168). The base is
// CCharClassCooldownXBossAttack's (its destructor fn_71004a0a10). tInitialState holds a vtable
// (unk_7101a3e090) and an array of 0x18-byte polymorphic entries at its +0x18; tPatterns is an array
// of owned definitions deleted through their vtables. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

struct UnkCooldownXBossAttackDef;
void fn_71004a0a10(UnkCooldownXBossAttackDef* self);
extern char g_unk_7101a3e0b8[];
extern void* g_unk_7101a3e090[];

struct UnkLavaDropsPolyEntry_ovb0072 {
    virtual void vf_0x0();
    virtual ~UnkLavaDropsPolyEntry_ovb0072();
    u8 pad_0x8[0x10];
};

struct UnkLavaDropsState_ovb0072 {
    char* vtable;
    u8 pad_0x8[0x10];
    base::UnkVector<UnkLavaDropsPolyEntry_ovb0072> entries;  // 0x18
};

struct UnkLavaDropsAttackDef_ovb0072 {
    char* vtable;
    u8 pad_0x8[0xf8];
    f32 fTimeInit;                // 0x100
    f32 fTimeBetweenShots;        // 0x104
    f32 fTimeLocked;              // 0x108
    f32 fShotPreparationTime;     // 0x10c
    f32 fShotTime;                // 0x110
    f32 fDamageRadius;            // 0x114
    f32 fAimSpeed;                // 0x118
    f32 fAimMinSpeed;             // 0x11c
    f32 fAimMaxSpeed;             // 0x120
    f32 fAimMaxSpeedMaxDistance;  // 0x124
    UnkLavaDropsState_ovb0072 tInitialState;                    // 0x128
    base::UnkVector<UnkOwnedPtr> tPatterns;                     // 0x168
};
static_assert(__builtin_offsetof(UnkLavaDropsAttackDef_ovb0072, tPatterns) == 0x168, "layout");

void fn_71004ae7c0(UnkLavaDropsAttackDef_ovb0072* self) {
    self->vtable = g_unk_7101a3e0b8 + 0x10;
    self->tPatterns.Free();
    self->tInitialState.vtable = reinterpret_cast<char*>(g_unk_7101a3e090 + 2);
    self->tInitialState.entries.Free();
    fn_71004a0a10(reinterpret_cast<UnkCooldownXBossAttackDef*>(self));
}
