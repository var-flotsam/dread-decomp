// Copies of fn_7100d2ac00 (src/game_aca57c/fn_7100b04900.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a15260, fn_7100a1d550, fn_7100a419e0, fn_7100a4d830, fn_7100a51650, fn_7100ac1950, fn_7100967ec0, fn_71009bbed0.
// Per-TU static initialisers (unguarded; destructors registered with fn_7100000250, the
// __cxa_atexit stub). Each TU first builds the same header data (a word and the "binaries" and
// "sources" ids), then registers its reflected types: a class or enum type built from its name
// (destroyed at exit) and noted in the TU's registry flags, and for enums a pair of converter
// records (to and from the integer type) registered with fn_71000a16c4.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_71727bc230 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bc110;
void fn_7100da56a4();
void fn_7100da5c70(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bc228;

extern UnkTUHeader g_unk_717275e1e0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717275de80;
void fn_7100b03ecc();
void fn_7100b03f60(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275dfb8;
UnkReflType* fn_7100b042c4();  // base::reflection::refl_GetType<std::unique_ptr<GUI::CDisplayObjectAnimationDef>>
extern char g_unk_717275dfb9;

extern UnkTUHeader g_unk_7172768ad0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172768990;
void fn_7100b87b5c();
void fn_7100b87e10(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172768ac8;
UnkReflType* fn_7100b040c0();  // base::reflection::refl_GetType<std::unique_ptr<GUI::CTrackSet>>
extern char g_unk_7172768ac9;

extern UnkTUHeader g_unk_7172792168 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172792008;
void fn_7100c79130();
void fn_7100c797bc(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172792120;
void fn_7100c7a8c0() __attribute__((visibility("hidden")));
void fn_7100c7a8cc() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172792128;
extern UnkConverter g_unk_7172792148;

extern UnkTUHeader g_unk_7172795408 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727952a8;
void fn_7100c94244();
void fn_7100c94510(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727953c0;
void fn_7100c94690() __attribute__((visibility("hidden")));
void fn_7100c9469c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727953c8;
extern UnkConverter g_unk_71727953e8;

extern UnkTUHeader g_unk_717277e0f8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717277df90;
void fn_7100c10ce0();
void fn_7100c10e2c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_717277e0a8;
void fn_7100c1140c() __attribute__((visibility("hidden")));
void fn_7100c11418() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_717277e0b0;
extern UnkConverter g_unk_717277e0d0;
extern char g_unk_717277e0f0;
extern UnkReflType g_unk_717277e118 __attribute__((visibility("hidden")));

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

extern UnkTUHeader g_unk_71727bba50 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bb8e8;
void fn_7100da3810();
void fn_7100da3a1c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bba00;
void fn_7100da3e00() __attribute__((visibility("hidden")));
void fn_7100da3e0c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bba08;
extern UnkConverter g_unk_71727bba28;
extern char g_unk_71727bba48;
extern UnkReflType g_unk_71727bba70 __attribute__((visibility("hidden")));

extern UnkTUHeader g_unk_71727b9018 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727b8dd8;
void fn_7100d911d8();
void fn_7100d91564(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727b8ef0;
extern UnkReflType g_unk_71727b8ef8;
void fn_7100d916e4();
void fn_7100d91d10(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727b9010;

extern UnkTUHeader g_unk_71727be710 __attribute__((visibility("hidden")));
UnkReflType* fn_7100dded9c();  // fn_7100dded9c
extern char g_unk_71727be380;
UnkReflType* fn_7100ddee78();  // fn_7100ddee78
extern char g_unk_71727be381;
extern UnkReflType g_unk_71727be388;
void fn_7100ddef54();
void fn_7100ddf1c0(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727be4a0;
void fn_7100de1b0c() __attribute__((visibility("hidden")));
void fn_7100de1b18() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727be4a8;
extern UnkConverter g_unk_71727be4c8;
extern char g_unk_71727be4e8;
extern UnkReflType g_unk_71727be730 __attribute__((visibility("hidden")));

extern UnkTUHeader g_unk_71727bf998 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bf6f8;
void fn_7100e06ce0();
void fn_7100e06e8c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bf810;
void fn_7100e073d4() __attribute__((visibility("hidden")));
void fn_7100e073e0() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bf818;
extern UnkConverter g_unk_71727bf838;
extern UnkReflType g_unk_71727bf858;
void fn_7100e0700c();
void fn_7100e07250(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727bf990;

extern UnkTUHeader g_unk_7172795270 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172794fb0;
void fn_7100c932c4();
void fn_7100c93470(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727950c8;
void fn_7100c93c48() __attribute__((visibility("hidden")));
void fn_7100c93c54() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727950d0;
extern UnkConverter g_unk_71727950f0;
extern UnkReflType g_unk_7172795110;
void fn_7100c935f0();
void fn_7100c9385c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172795228;
void fn_7100c93c60() __attribute__((visibility("hidden")));
void fn_7100c93c6c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172795230;
extern UnkConverter g_unk_7172795250;

extern UnkTUHeader g_unk_7172772d18 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172772bb0;
void fn_7100bcdd30();
void fn_7100bcde7c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172772cc8;
void fn_7100bce500() __attribute__((visibility("hidden")));
void fn_7100bce50c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172772cd0;
extern UnkConverter g_unk_7172772cf0;
extern char g_unk_7172772d10;
extern UnkReflType g_unk_7172772d38 __attribute__((visibility("hidden")));

extern UnkTUHeader g_unk_7172791e30 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172791c90;
void fn_7100c65ffc();
void fn_7100c66688(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172791da8;
void fn_7100c66808() __attribute__((visibility("hidden")));
void fn_7100c66814() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172791db0;
extern UnkConverter g_unk_7172791dd0;
void fn_7100c66820() __attribute__((visibility("hidden")));
void fn_7100c6682c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172791df0;
extern UnkConverter g_unk_7172791e10;

extern UnkTUHeader g_unk_7172758ad0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172758838;
void fn_7100adf0bc();
void fn_7100adf388(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172758950;
void fn_7100adf658() __attribute__((visibility("hidden")));
void fn_7100adf664() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172758958;
extern UnkConverter g_unk_7172758978;
extern UnkReflType g_unk_7172758998;
void fn_7100adf508(UnkReflType* self, const CStrId& name, void* extra);
extern char g_unk_7172758a88;
void fn_7100adf670() __attribute__((visibility("hidden")));
void fn_7100adf67c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172758a90;
extern UnkConverter g_unk_7172758ab0;

extern UnkTUHeader g_unk_71727c0fb8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727c0bc0;
void fn_7100e16e60();
void fn_7100e1766c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727c0cd8;
void fn_7100e1b5c8() __attribute__((visibility("hidden")));
void fn_7100e1b5d4() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727c0ce0;
extern UnkConverter g_unk_71727c0d00;
extern UnkReflType g_unk_71727c0d20;
void fn_7100e177ec(UnkReflType* self, const CStrId& name, void* extra);
extern char g_unk_71727c0e10;
void fn_7100e1b5e0() __attribute__((visibility("hidden")));
void fn_7100e1b5ec() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727c0e18;
extern UnkConverter g_unk_71727c0e38;
extern UnkReflType g_unk_71727c0e58;
void fn_7100e1793c();
void fn_7100e17b48(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727c0f70;
void fn_7100e1b5f8() __attribute__((visibility("hidden")));
void fn_7100e1b604() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727c0f78;
extern UnkConverter g_unk_71727c0f98;

extern UnkTUHeader g_unk_71727ba980 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727ba2e0;
void fn_7100d975d8();
void fn_7100d977e4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727ba3f8;
void fn_7100d9b7a4() __attribute__((visibility("hidden")));
void fn_7100d9b7b0() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ba400;
extern UnkConverter g_unk_71727ba420;
void fn_7100d9b7bc() __attribute__((visibility("hidden")));
void fn_7100d9b7c8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ba440;
extern UnkConverter g_unk_71727ba460;
extern UnkReflType g_unk_71727ba480;
void fn_7100d97964();
void fn_7100d97c30(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727ba598;
void fn_7100d9b7d4() __attribute__((visibility("hidden")));
void fn_7100d9b7e0() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ba5a0;
extern UnkConverter g_unk_71727ba5c0;
void fn_7100d9b7ec() __attribute__((visibility("hidden")));
void fn_7100d9b7f8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ba5e0;
extern UnkConverter g_unk_71727ba600;
extern UnkReflType g_unk_71727ba620;
void fn_7100d97db0();
void fn_7100d99934(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727ba758;
UnkReflType* fn_7100d9af70();  // fn_7100d9af70
extern char g_unk_71727ba759;
UnkReflType* fn_7100d9b04c();  // fn_7100d9b04c
extern char g_unk_71727ba75a;

// The TU's data: the header, then a pair of floats (-1, -1) and a flag.
struct UnkLoadingScreenData {
    UnkTUHeader header;
    u8 unk_0x18[0x80];
    f32 field_0x98;
    f32 field_0x9c;
    bool field_0xa0;
};
extern UnkLoadingScreenData g_unk_71727c0870 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727c0710;
void fn_7100e0f028();
void fn_7100e0f4d4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727c0828;
void fn_7100e1389c() __attribute__((visibility("hidden")));
void fn_7100e138a8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727c0830;
extern UnkConverter g_unk_71727c0850;

extern UnkTUHeaderAndClass g_unk_717279cc60 __attribute__((visibility("hidden")));
extern char g_unk_717279caf8;
extern UnkReflType g_unk_717279cb00;
void fn_7100cd17a8();
void fn_7100cd1954(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_717279cc18;
void fn_7100cd1c90() __attribute__((visibility("hidden")));
void fn_7100cd1c9c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_717279cc20;
extern UnkConverter g_unk_717279cc40;

extern UnkTUHeaderAndClass g_unk_71727a0898 __attribute__((visibility("hidden")));
extern char g_unk_71727a0730;
extern UnkReflType g_unk_71727a0738;
void fn_7100ceb15c();
void fn_7100ceb308(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727a0850;
void fn_7100ceb4cc() __attribute__((visibility("hidden")));
void fn_7100ceb4d8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727a0858;
extern UnkConverter g_unk_71727a0878;

