// Copies of fn_7100d2ac00 (src/base/reflection/fn_7100b04900.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a15260, fn_7100a1d550, fn_7100a419e0, fn_7100a4d830, fn_7100a51650, fn_7100ac1950, fn_7100967ec0, fn_71009bbed0.
// Per-TU static initialisers (unguarded; destructors registered with fn_7100000250, the
// __cxa_atexit stub). Each TU first builds the same header data (a word and the "binaries" and
// "sources" ids), then registers its reflected types: a class or enum type built from its name
// (destroyed at exit) and noted in the TU's registry flags, and for enums a pair of converter
// records (to and from the integer type) registered with fn_71000a16c4.
#include "base/reflection.h"
#include "base/reflection/CEnumType.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassAnalogAimTrack.h"
#include "game/CCharClassCommanderWingsTrack.h"
#include "game/CCharClassSetCameraNearEvent.h"
#include "game/CExplosionGfx.h"
#include "game/CharClassSakaiModifyShieldEvent.h"
#include "game/IPath.h"
#include "game/UnkTU1640.h"
#include "game/UnkTU1726.h"
#include "game/UnkTU2176.h"
#include "game/UnkTU2180.h"
#include "game/UnkTU2201.h"
#include "game/UnkTU2203.h"
#include "game/UnkTU2526.h"
#include "game/UnkTU2528.h"
#include "game/UnkTU2536.h"
#include "game/UnkTU2555.h"
#include "game/UnkTU2557.h"
#include "game/UnkTU2559.h"
#include "game/UnkTUInit.h"

void fn_7100da5c70(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);

void fn_7100b03f60(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);

void fn_7100b87e10(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);

void fn_7100c797bc(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_7172792128;
extern UnkConverter g_unk_7172792148;

void fn_7100c94510(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727953c8;
extern UnkConverter g_unk_71727953e8;

void fn_7100c10e2c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_717277e0b0;
extern UnkConverter g_unk_717277e0d0;

extern UnkTUHeader g_unk_71727553d0 __attribute__((visibility("hidden")));
extern UnkTUHeader g_unk_717274c100 __attribute__((visibility("hidden")));
extern UnkTUHeader g_unk_717274b5e0 __attribute__((visibility("hidden")));
extern UnkTUHeader g_unk_717274a1f8 __attribute__((visibility("hidden")));
extern UnkTUHeader g_unk_71727454b0 __attribute__((visibility("hidden")));
extern UnkTUHeader g_unk_71727446e0 __attribute__((visibility("hidden")));
extern UnkTUHeader g_unk_717273db38 __attribute__((visibility("hidden")));
extern UnkTUHeader g_unk_717273a448 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172755268;
extern UnkReflType g_unk_717274bf98;
extern UnkReflType g_unk_717274b478;
extern UnkReflType g_unk_717274a090;
extern UnkReflType g_unk_7172745348;
extern UnkReflType g_unk_7172744578;
extern UnkReflType g_unk_717273d9d0;
extern UnkReflType g_unk_717273a2e0;
void fn_7100ac0400();
void fn_7100a508e0();
void fn_7100a4cec8();
void fn_7100a40bd0();
void fn_7100a1cb90();
void fn_7100a14aac();
void fn_71009bb360();
void fn_7100955f6c();
void fn_7100d2a00c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172755380;
extern char g_unk_717274c0b0;
extern char g_unk_717274b590;
extern char g_unk_717274a1a8;
extern char g_unk_7172745460;
extern char g_unk_7172744690;
extern char g_unk_717273dae8;
extern char g_unk_717273a3f8;
void fn_7100ac1844() __attribute__((visibility("hidden")));
void fn_7100a514f8() __attribute__((visibility("hidden")));
void fn_7100a4d72c() __attribute__((visibility("hidden")));
void fn_7100a41910() __attribute__((visibility("hidden")));
void fn_7100a1d440() __attribute__((visibility("hidden")));
void fn_7100a15168() __attribute__((visibility("hidden")));
void fn_71009bbdc0() __attribute__((visibility("hidden")));
void fn_710096501c() __attribute__((visibility("hidden")));
void fn_7100ac1850() __attribute__((visibility("hidden")));
void fn_7100a51504() __attribute__((visibility("hidden")));
void fn_7100a4d738() __attribute__((visibility("hidden")));
void fn_7100a4191c() __attribute__((visibility("hidden")));
void fn_7100a1d44c() __attribute__((visibility("hidden")));
void fn_7100a15174() __attribute__((visibility("hidden")));
void fn_71009bbdcc() __attribute__((visibility("hidden")));
void fn_7100965028() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172755388;
extern UnkConverter g_unk_717274c0b8;
extern UnkConverter g_unk_717274b598;
extern UnkConverter g_unk_717274a1b0;
extern UnkConverter g_unk_7172745468;
extern UnkConverter g_unk_7172744698;
extern UnkConverter g_unk_717273daf0;
extern UnkConverter g_unk_717273a400;
extern UnkConverter g_unk_71727553a8;
extern UnkConverter g_unk_717274c0d8;
extern UnkConverter g_unk_717274b5b8;
extern UnkConverter g_unk_717274a1d0;
extern UnkConverter g_unk_7172745488;
extern UnkConverter g_unk_71727446b8;
extern UnkConverter g_unk_717273db10;
extern UnkConverter g_unk_717273a420;
extern char g_unk_71727553c8;
extern char g_unk_717274c0f8;
extern char g_unk_717274b5d8;
extern char g_unk_717274a1f0;
extern char g_unk_71727454a8;
extern char g_unk_71727446d8;
extern char g_unk_717273db30;
extern char g_unk_717273a440;
extern UnkReflType g_unk_71727553f0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717274c120 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717274b600 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717274a218 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727454d0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172744700 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717273db58 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717273a468 __attribute__((visibility("hidden")));

void fn_7100a14bf8(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100a1cd3c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100a40d1c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100a4d014(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100a50a2c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100ac05ac(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100956118(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71009bb50c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100a15260() {
    InitHeader(g_unk_71727446e0);
    InitEnum(&g_unk_7172744578, "EAutsniperSpawnPointDir", fn_7100a14bf8, fn_7100a14aac);
    fn_710009ec00(&g_unk_7172744690, &g_unk_7172744578);
    InitEnumConverters(&g_unk_7172744578, g_unk_7172744698, fn_7100a15168, g_unk_71727446b8, fn_7100a15174);
    fn_7100a14d7c();
    fn_710009ec00(&g_unk_71727446d8, &g_unk_7172744700);
}

void fn_7100a1d550() {
    InitHeader(g_unk_71727454b0);
    InitEnum(&g_unk_7172745348, "ESlidleOutSpawnPointDir", fn_7100a1cd3c, fn_7100a1cb90);
    fn_710009ec00(&g_unk_7172745460, &g_unk_7172745348);
    InitEnumConverters(&g_unk_7172745348, g_unk_7172745468, fn_7100a1d440, g_unk_7172745488, fn_7100a1d44c);
    fn_7100a1cec0();
    fn_710009ec00(&g_unk_71727454a8, &g_unk_71727454d0);
}

void fn_7100a419e0() {
    InitHeader(g_unk_717274a1f8);
    InitEnum(&g_unk_717274a090, "CSaveGameLogicAction::EDestination", fn_7100a40d1c, fn_7100a40bd0);
    fn_710009ec00(&g_unk_717274a1a8, &g_unk_717274a090);
    InitEnumConverters(&g_unk_717274a090, g_unk_717274a1b0, fn_7100a41910, g_unk_717274a1d0, fn_7100a4191c);
    fn_7100a40ea0();
    fn_710009ec00(&g_unk_717274a1f0, &g_unk_717274a218);
}

void fn_7100a4d830() {
    InitHeader(g_unk_717274b5e0);
    InitEnum(&g_unk_717274b478, "CEmmyStateOverrideLogicAction::EMode", fn_7100a4d014, fn_7100a4cec8);
    fn_710009ec00(&g_unk_717274b590, &g_unk_717274b478);
    InitEnumConverters(&g_unk_717274b478, g_unk_717274b598, fn_7100a4d72c, g_unk_717274b5b8, fn_7100a4d738);
    fn_7100a4d198();
    fn_710009ec00(&g_unk_717274b5d8, &g_unk_717274b600);
}

void fn_7100a51650() {
    InitHeader(g_unk_717274c100);
    InitEnum(&g_unk_717274bf98, "CPerceptionModifierLogicAction::EMode", fn_7100a50a2c, fn_7100a508e0);
    fn_710009ec00(&g_unk_717274c0b0, &g_unk_717274bf98);
    InitEnumConverters(&g_unk_717274bf98, g_unk_717274c0b8, fn_7100a514f8, g_unk_717274c0d8, fn_7100a51504);
    fn_7100a50bb0();
    fn_710009ec00(&g_unk_717274c0f8, &g_unk_717274c120);
}

void fn_7100ac1950() {
    InitHeader(g_unk_71727553d0);
    InitEnum(&g_unk_7172755268, "CCharClassProjectileMovement::EProjectileMeleeHitReaction", fn_7100ac05ac, fn_7100ac0400);
    fn_710009ec00(&g_unk_7172755380, &g_unk_7172755268);
    InitEnumConverters(&g_unk_7172755268, g_unk_7172755388, fn_7100ac1844, g_unk_71727553a8, fn_7100ac1850);
    fn_7100ac0730();
    fn_710009ec00(&g_unk_71727553c8, &g_unk_71727553f0);
}

void fn_7100967ec0() {
    InitHeader(g_unk_717273a448);
    InitEnum(&g_unk_717273a2e0, "CCharClassEnemyMovement::ETurn180Mode", fn_7100956118, fn_7100955f6c);
    fn_710009ec00(&g_unk_717273a3f8, &g_unk_717273a2e0);
    InitEnumConverters(&g_unk_717273a2e0, g_unk_717273a400, fn_710096501c, g_unk_717273a420, fn_7100965028);
    fn_710095629c();
    fn_710009ec00(&g_unk_717273a440, &g_unk_717273a468);
}

void fn_71009bbed0() {
    InitHeader(g_unk_717273db38);
    InitEnum(&g_unk_717273d9d0, "ENavMeshItemType", fn_71009bb50c, fn_71009bb360);
    fn_710009ec00(&g_unk_717273dae8, &g_unk_717273d9d0);
    InitEnumConverters(&g_unk_717273d9d0, g_unk_717273daf0, fn_71009bbdc0, g_unk_717273db10, fn_71009bbdcc);
    fn_71009bb690();
    fn_710009ec00(&g_unk_717273db30, &g_unk_717273db58);
}

void fn_7100da3a1c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727bba08;
extern UnkConverter g_unk_71727bba28;

void fn_7100d91564(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100d91d10(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);

extern UnkReflType g_unk_71727be388;
void fn_7100ddf1c0(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727be4a8;
extern UnkConverter g_unk_71727be4c8;

void fn_7100e06e8c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727bf818;
extern UnkConverter g_unk_71727bf838;
void fn_7100e07250(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);

void fn_7100c93470(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727950d0;
extern UnkConverter g_unk_71727950f0;
void fn_7100c9385c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_7172795230;
extern UnkConverter g_unk_7172795250;

void fn_7100bcde7c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_7172772cd0;
extern UnkConverter g_unk_7172772cf0;

void fn_7100c66688(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_7172791db0;
extern UnkConverter g_unk_7172791dd0;
extern UnkConverter g_unk_7172791df0;
extern UnkConverter g_unk_7172791e10;

void fn_7100adf388(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_7172758958;
extern UnkConverter g_unk_7172758978;
void fn_7100adf508(UnkReflType* self, const CStrId& name, void* extra);
extern UnkConverter g_unk_7172758a90;
extern UnkConverter g_unk_7172758ab0;

void fn_7100e1766c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727c0ce0;
extern UnkConverter g_unk_71727c0d00;
void fn_7100e177ec(UnkReflType* self, const CStrId& name, void* extra);
extern UnkConverter g_unk_71727c0e18;
extern UnkConverter g_unk_71727c0e38;
void fn_7100e17b48(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727c0f78;
extern UnkConverter g_unk_71727c0f98;

extern UnkReflType g_unk_71727ba2e0;
void fn_7100d977e4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727ba400;
extern UnkConverter g_unk_71727ba420;
extern UnkConverter g_unk_71727ba440;
extern UnkConverter g_unk_71727ba460;
extern UnkReflType g_unk_71727ba480;
void fn_7100d97c30(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727ba5a0;
extern UnkConverter g_unk_71727ba5c0;
extern UnkConverter g_unk_71727ba5e0;
extern UnkConverter g_unk_71727ba600;
void fn_7100d99934(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);

// The TU's data: the header, then a pair of floats (-1, -1) and a flag.
struct UnkLoadingScreenData {
    UnkTUHeader header;
    u8 unk_0x18[0x80];
    f32 field_0x98;
    f32 field_0x9c;
    bool field_0xa0;
};
void fn_7100e0f4d4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727c0830;
extern UnkConverter g_unk_71727c0850;

void fn_7100cd1954(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_717279cc20;
extern UnkConverter g_unk_717279cc40;

void fn_7100ceb308(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727a0858;
extern UnkConverter g_unk_71727a0878;
