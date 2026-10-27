// Static initialiser of the unit of S9PatchConfig, SMapIconDef, SMapLabelDef and others: the
// header ids, then the unit's own statics and registrations (helpers in UnkStaticInitS7.h).
// Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkMinimapIconKinds.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100e72680 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    UnkFilePathStrIdBase field_0x28;
    u8 pad_0x30[0x58];
    u64 field_0x88;
    u64 field_0x90;
    u64 field_0x98;
    void (*field_0xa0)();
    u64 field_0xa8;
    u64 field_0xb0;
    u64 field_0xb8;
    u64 field_0xc0;
    u64 field_0xc8;
    void (*field_0xd0)();
    u64 field_0xd8;
    u64 field_0xe0;
    u64 field_0xe8;
    u64 field_0xf0;
    u64 field_0xf8;
    void (*field_0x100)();
    u64 field_0x108;
    u64 field_0x110;
    u8 field_0x118[8];
};
extern UnkStatics_7100e72680 g_unk_71727c8d08 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71727c7438;
void fn_7100e65554(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e65204();
extern char g_unk_71727c7570[];
extern UnkEnumType g_unk_71727c7578;
void fn_7100e65d70(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e656d8();
extern char g_unk_71727c76b0[];
extern UnkEnumType g_unk_71727c76b8;
void fn_7100e66074(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e65ef4();
extern char g_unk_71727c77f0[];
extern char g_unk_71727c77f1[];
extern char g_unk_71727c77f2[];
UnkEnumType* fn_7100e66954();
extern char g_unk_71727c77f3[];
UnkEnumType* fn_7100e66a30();
extern UnkEnumType g_unk_71727c77f8;
void fn_7100e66e90(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100e66b0c();
extern char g_unk_71727c7930[];
extern char g_unk_71727c7931[];
UnkEnumType* fn_7100e67014();
extern UnkEnumType g_unk_71727c7938;
void fn_7100e67474(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100e670f0();
extern char g_unk_71727c7a70[];
extern char g_unk_71727c7a71[];
UnkEnumType* fn_7100e675f8();
extern UnkEnumType g_unk_71727c7a78;
void fn_7100e676d4(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c7be8[];
extern char g_unk_71727c7be9[];
UnkEnumType* fn_7100e678cc();
extern char g_unk_71727c7bea[];
UnkEnumType* fn_7100e679a8();
extern UnkEnumType g_unk_71727c7bf0;
void fn_7100e67e08(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100e67a84();
extern char g_unk_71727c7d28[];
extern char g_unk_71727c7d29[];
UnkEnumType* fn_7100e67f8c();
extern UnkEnumType g_unk_71727c7d30;
void fn_7100e683ec(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100e68068();
extern char g_unk_71727c7e68[];
extern char g_unk_71727c7e69[];
UnkEnumType* fn_7100e68570();
extern UnkEnumType g_unk_71727c7e70;
void fn_7100e6864c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c7fe0[];
extern UnkStrIdBase g_unk_71727c7fe8;
extern UnkStrIdBase g_unk_71727c7ff0;
extern UnkStrIdBase g_unk_71727c7ff8;
extern UnkStrIdBase g_unk_71727c8000;
extern UnkStrIdBase g_unk_71727c8008;
extern UnkStrIdBase g_unk_71727c8010;
extern UnkStrIdBase g_unk_71727c8018;
extern UnkStrIdBase g_unk_71727c8020;
extern UnkStrIdBase g_unk_71727c8028;
// The minimap's icon kinds, 0x71727c8030 to 0x71727c83d0 (game/UnkMinimapIconKinds.h declares the
// ones other files read): fn_7100e68cac copies a kind's name into it and adds it to the list of all
// of them.
extern CStrId g_unk_71727c8030;
void fn_7100e68cac(CStrId* kind, const CStrId& name);
void fn_7100e68dd0(void* kind);  // the kind's destructor, as registered
extern CStrId g_unk_71727c8038;
extern CStrId g_unk_71727c8040;
extern CStrId g_unk_71727c8048;
extern CStrId g_unk_71727c8050;
extern CStrId g_unk_71727c8058;
extern CStrId g_unk_71727c8060;
extern CStrId g_unk_71727c8068;
extern CStrId g_unk_71727c8070;
extern CStrId g_unk_71727c8078;
extern CStrId g_unk_71727c8080;
extern CStrId g_unk_71727c8088;
extern CStrId g_unk_71727c8090;
extern CStrId g_unk_71727c8098;
extern CStrId g_unk_71727c80a0;
extern CStrId g_unk_71727c80a8;
// 0x71727c80b0 to 0x71727c80e8: game/UnkMinimapIconKinds.h.
extern CStrId g_unk_71727c80f0;
extern CStrId g_unk_71727c80f8;
extern CStrId g_unk_71727c8100;
extern CStrId g_unk_71727c8108;
extern CStrId g_unk_71727c8110;
extern CStrId g_unk_71727c8118;
extern CStrId g_unk_71727c8120;
extern CStrId g_unk_71727c8128;
extern CStrId g_unk_71727c8130;
extern CStrId g_unk_71727c8138;
extern CStrId g_unk_71727c8140;
extern CStrId g_unk_71727c8148;
extern CStrId g_unk_71727c8150;
extern CStrId g_unk_71727c8158;
extern CStrId g_unk_71727c8160;
extern CStrId g_unk_71727c8168;
extern CStrId g_unk_71727c8170;
extern CStrId g_unk_71727c8178;
extern CStrId g_unk_71727c8180;
extern CStrId g_unk_71727c8188;
extern CStrId g_unk_71727c8190;
extern CStrId g_unk_71727c8198;
extern CStrId g_unk_71727c81a0;
extern CStrId g_unk_71727c81a8;
extern CStrId g_unk_71727c81b0;
extern CStrId g_unk_71727c81b8;
extern CStrId g_unk_71727c81c0;
extern CStrId g_unk_71727c81c8;
extern CStrId g_unk_71727c81d0;
extern CStrId g_unk_71727c81d8;
extern CStrId g_unk_71727c81e0;
extern CStrId g_unk_71727c81e8;
extern CStrId g_unk_71727c81f0;
extern CStrId g_unk_71727c81f8;
extern CStrId g_unk_71727c8200;
extern CStrId g_unk_71727c8208;
extern CStrId g_unk_71727c8210;
extern CStrId g_unk_71727c8218;
extern CStrId g_unk_71727c8220;
extern CStrId g_unk_71727c8228;
extern CStrId g_unk_71727c8230;
extern CStrId g_unk_71727c8238;
extern CStrId g_unk_71727c8240;
extern CStrId g_unk_71727c8248;
extern CStrId g_unk_71727c8250;
extern CStrId g_unk_71727c8258;
extern CStrId g_unk_71727c8260;
extern CStrId g_unk_71727c8268;
extern CStrId g_unk_71727c8270;
extern CStrId g_unk_71727c8278;
extern CStrId g_unk_71727c8280;
extern CStrId g_unk_71727c8288;
extern CStrId g_unk_71727c8290;
extern CStrId g_unk_71727c8298;
extern CStrId g_unk_71727c82a0;
extern CStrId g_unk_71727c82a8;
extern CStrId g_unk_71727c82b0;
extern CStrId g_unk_71727c82b8;
extern CStrId g_unk_71727c82c0;
extern CStrId g_unk_71727c82c8;
extern CStrId g_unk_71727c82d0;
extern CStrId g_unk_71727c82d8;
extern CStrId g_unk_71727c82e0;
extern CStrId g_unk_71727c82e8;
extern CStrId g_unk_71727c82f0;
extern CStrId g_unk_71727c82f8;
extern CStrId g_unk_71727c8300;
extern CStrId g_unk_71727c8308;
extern CStrId g_unk_71727c8310;
extern CStrId g_unk_71727c8318;
extern CStrId g_unk_71727c8320;
extern CStrId g_unk_71727c8328;
extern CStrId g_unk_71727c8330;
extern CStrId g_unk_71727c8338;
extern CStrId g_unk_71727c8340;
extern CStrId g_unk_71727c8348;
extern CStrId g_unk_71727c8350;
extern CStrId g_unk_71727c8358;
extern CStrId g_unk_71727c8360;
extern CStrId g_unk_71727c8368;
extern CStrId g_unk_71727c8370;
extern CStrId g_unk_71727c8378;
extern CStrId g_unk_71727c8380;
extern CStrId g_unk_71727c8388;
extern CStrId g_unk_71727c8390;
extern CStrId g_unk_71727c8398;
extern CStrId g_unk_71727c83a0;
extern CStrId g_unk_71727c83a8;
extern CStrId g_unk_71727c83b0;
extern CStrId g_unk_71727c83b8;
extern CStrId g_unk_71727c83c0;
extern CStrId g_unk_71727c83c8;
extern CStrId g_unk_71727c83d0;
extern char g_unk_71727c83d8[];

void fn_7100e72680() {
    InitHeaderIds(&g_unk_71727c8d08.ids);
    g_unk_71727c8d08.field_0x88 = 0;
    g_unk_71727c8d08.field_0x90 = 0;
    g_unk_71727c8d08.field_0x98 = 0;
    g_unk_71727c8d08.field_0xa0 = fn_710000cf00;
    g_unk_71727c8d08.field_0xa8 = 0;
    g_unk_71727c8d08.field_0xb0 = 0;
    g_unk_71727c8d08.field_0xb8 = 0;
    g_unk_71727c8d08.field_0xc0 = 0;
    g_unk_71727c8d08.field_0xc8 = 0;
    g_unk_71727c8d08.field_0xd0 = fn_710000d470;
    g_unk_71727c8d08.field_0xd8 = 0;
    g_unk_71727c8d08.field_0xe0 = 0;
    g_unk_71727c8d08.field_0xe8 = 0;
    g_unk_71727c8d08.field_0xf0 = 0;
    g_unk_71727c8d08.field_0xf8 = 0;
    g_unk_71727c8d08.field_0x100 = fn_710000d4f4;
    g_unk_71727c8d08.field_0x108 = 0;
    g_unk_71727c8d08.field_0x110 = 0;
    {
        CStrId n("S9PatchConfig", true);
        fn_7100e65554(&g_unk_71727c7438, n, nullptr, nullptr, fn_7100e65204, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c7438, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c7570, &g_unk_71727c7438);
    {
        CStrId n("SMapIconDef", true);
        fn_7100e65d70(&g_unk_71727c7578, n, nullptr, nullptr, fn_7100e656d8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c7578, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c76b0, &g_unk_71727c7578);
    {
        CStrId n("SMapLabelDef", true);
        fn_7100e66074(&g_unk_71727c76b8, n, nullptr, nullptr, fn_7100e65ef4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c76b8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c77f0, &g_unk_71727c76b8);
    fn_7100e66250();
    fn_710009ec00(g_unk_71727c77f1, g_unk_71727c8d08.field_0x118);
    fn_710009ec00(g_unk_71727c77f2, fn_7100e66954());
    fn_710009ec00(g_unk_71727c77f3, fn_7100e66a30());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CMinimapDef::TMapIconDefs>::TKeyElementIterator::value_type", true);
        fn_7100e66e90(&g_unk_71727c77f8, n, &g_unk_7101d5e2e0, nullptr, fn_7100e66b0c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c77f8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c7930, &g_unk_71727c77f8);
    fn_710009ec00(g_unk_71727c7931, fn_7100e67014());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CMinimapDef::TMapIconDefs>::TKeyElementConstIterator::value_type", true);
        fn_7100e67474(&g_unk_71727c7938, n, &g_unk_7101d5e2e0, nullptr, fn_7100e670f0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c7938, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c7a70, &g_unk_71727c7938);
    fn_710009ec00(g_unk_71727c7a71, fn_7100e675f8());
    {
        CStrId n("CMinimapDef::TMapIconDefs", true);
        fn_7100e676d4(&g_unk_71727c7a78, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c7a78, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c7be8, &g_unk_71727c7a78);
    fn_710009ec00(g_unk_71727c7be9, fn_7100e678cc());
    fn_710009ec00(g_unk_71727c7bea, fn_7100e679a8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CMinimapDef::TMapLabelDefs>::TKeyElementIterator::value_type", true);
        fn_7100e67e08(&g_unk_71727c7bf0, n, &g_unk_7101d5e2e0, nullptr, fn_7100e67a84, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c7bf0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c7d28, &g_unk_71727c7bf0);
    fn_710009ec00(g_unk_71727c7d29, fn_7100e67f8c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CMinimapDef::TMapLabelDefs>::TKeyElementConstIterator::value_type", true);
        fn_7100e683ec(&g_unk_71727c7d30, n, &g_unk_7101d5e2e0, nullptr, fn_7100e68068, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c7d30, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c7e68, &g_unk_71727c7d30);
    fn_710009ec00(g_unk_71727c7e69, fn_7100e68570());
    {
        CStrId n("CMinimapDef::TMapLabelDefs", true);
        fn_7100e6864c(&g_unk_71727c7e70, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c7e70, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c7fe0, &g_unk_71727c7e70);
    InitId(&g_unk_71727c7fe8, "s010_cave");
    InitId(&g_unk_71727c7ff0, "s020_magma");
    InitId(&g_unk_71727c7ff8, "s030_baselab");
    InitId(&g_unk_71727c8000, "s040_aqua");
    InitId(&g_unk_71727c8008, "s050_forest");
    InitId(&g_unk_71727c8010, "s060_quarantine");
    InitId(&g_unk_71727c8018, "s070_basesanc");
    InitId(&g_unk_71727c8020, "s080_shipyard");
    InitId(&g_unk_71727c8028, "s090_skybase");
    {
        CStrId n("Samus", true);
        fn_7100e68cac(&g_unk_71727c8030, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8030, g_unk_7101a19000);
    {
        CStrId n("SamusGlow", true);
        fn_7100e68cac(&g_unk_71727c8038, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8038, g_unk_7101a19000);
    {
        CStrId n("Emmy", true);
        fn_7100e68cac(&g_unk_71727c8040, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8040, g_unk_7101a19000);
    {
        CStrId n("Boss", true);
        fn_7100e68cac(&g_unk_71727c8048, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8048, g_unk_7101a19000);
    {
        CStrId n("BossDefeated", true);
        fn_7100e68cac(&g_unk_71727c8050, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8050, g_unk_7101a19000);
    {
        CStrId n("CentralUnit", true);
        fn_7100e68cac(&g_unk_71727c8058, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8058, g_unk_7101a19000);
    {
        CStrId n("Gunship", true);
        fn_7100e68cac(&g_unk_71727c8060, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8060, g_unk_7101a19000);
    {
        CStrId n("Glow", true);
        fn_7100e68cac(&g_unk_71727c8068, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8068, g_unk_7101a19000);
    {
        CStrId n("HyperBeamGlow", true);
        fn_7100e68cac(&g_unk_71727c8070, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8070, g_unk_7101a19000);
    {
        CStrId n("CustomMarker", true);
        fn_7100e68cac(&g_unk_71727c8078, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8078, g_unk_7101a19000);
    {
        CStrId n("CustomMarkerBack", true);
        fn_7100e68cac(&g_unk_71727c8080, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8080, g_unk_7101a19000);
    {
        CStrId n("MarkerBorder", true);
        fn_7100e68cac(&g_unk_71727c8088, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8088, g_unk_7101a19000);
    {
        CStrId n("MarkerBorderFrame", true);
        fn_7100e68cac(&g_unk_71727c8090, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8090, g_unk_7101a19000);
    {
        CStrId n("HighlightFrame", true);
        fn_7100e68cac(&g_unk_71727c8098, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8098, g_unk_7101a19000);
    {
        CStrId n("HighlightFrameFull", true);
        fn_7100e68cac(&g_unk_71727c80a0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80a0, g_unk_7101a19000);
    {
        CStrId n("MagnetSurface", true);
        fn_7100e68cac(&g_unk_71727c80a8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80a8, g_unk_7101a19000);
    {
        CStrId n("TilePowerbeam", true);
        fn_7100e68cac(&g_unk_71727c80b0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80b0, g_unk_7101a19000);
    {
        CStrId n("TileBomb", true);
        fn_7100e68cac(&g_unk_71727c80b8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80b8, g_unk_7101a19000);
    {
        CStrId n("TileMissile", true);
        fn_7100e68cac(&g_unk_71727c80c0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80c0, g_unk_7101a19000);
    {
        CStrId n("TileSupermissile", true);
        fn_7100e68cac(&g_unk_71727c80c8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80c8, g_unk_7101a19000);
    {
        CStrId n("TilePowerbomb", true);
        fn_7100e68cac(&g_unk_71727c80d0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80d0, g_unk_7101a19000);
    {
        CStrId n("TileScrewattack", true);
        fn_7100e68cac(&g_unk_71727c80d8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80d8, g_unk_7101a19000);
    {
        CStrId n("TileWeight", true);
        fn_7100e68cac(&g_unk_71727c80e0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80e0, g_unk_7101a19000);
    {
        CStrId n("TileSpeedboost", true);
        fn_7100e68cac(&g_unk_71727c80e8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80e8, g_unk_7101a19000);
    {
        CStrId n("DoorFrame", true);
        fn_7100e68cac(&g_unk_71727c80f0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80f0, g_unk_7101a19000);
    {
        CStrId n("DoorFrame4x2", true);
        fn_7100e68cac(&g_unk_71727c80f8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c80f8, g_unk_7101a19000);
    {
        CStrId n("DoorFrameTrap", true);
        fn_7100e68cac(&g_unk_71727c8100, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8100, g_unk_7101a19000);
    {
        CStrId n("DoorFrameL", true);
        fn_7100e68cac(&g_unk_71727c8108, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8108, g_unk_7101a19000);
    {
        CStrId n("DoorFrameR", true);
        fn_7100e68cac(&g_unk_71727c8110, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8110, g_unk_7101a19000);
    {
        CStrId n("DoorClosedL", true);
        fn_7100e68cac(&g_unk_71727c8118, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8118, g_unk_7101a19000);
    {
        CStrId n("DoorClosedR", true);
        fn_7100e68cac(&g_unk_71727c8120, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8120, g_unk_7101a19000);
    {
        CStrId n("DoorOpenedL", true);
        fn_7100e68cac(&g_unk_71727c8128, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8128, g_unk_7101a19000);
    {
        CStrId n("DoorOpenedR", true);
        fn_7100e68cac(&g_unk_71727c8130, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8130, g_unk_7101a19000);
    {
        CStrId n("DoorPowerL", true);
        fn_7100e68cac(&g_unk_71727c8138, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8138, g_unk_7101a19000);
    {
        CStrId n("DoorPowerR", true);
        fn_7100e68cac(&g_unk_71727c8140, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8140, g_unk_7101a19000);
    {
        CStrId n("DoorPowerLockedL", true);
        fn_7100e68cac(&g_unk_71727c8148, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8148, g_unk_7101a19000);
    {
        CStrId n("DoorPowerLockedR", true);
        fn_7100e68cac(&g_unk_71727c8150, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8150, g_unk_7101a19000);
    {
        CStrId n("DoorChargeL", true);
        fn_7100e68cac(&g_unk_71727c8158, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8158, g_unk_7101a19000);
    {
        CStrId n("DoorChargeR", true);
        fn_7100e68cac(&g_unk_71727c8160, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8160, g_unk_7101a19000);
    {
        CStrId n("DoorChargeLockedL", true);
        fn_7100e68cac(&g_unk_71727c8168, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8168, g_unk_7101a19000);
    {
        CStrId n("DoorChargeLockedR", true);
        fn_7100e68cac(&g_unk_71727c8170, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8170, g_unk_7101a19000);
    {
        CStrId n("DoorGrappleL", true);
        fn_7100e68cac(&g_unk_71727c8178, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8178, g_unk_7101a19000);
    {
        CStrId n("DoorGrappleR", true);
        fn_7100e68cac(&g_unk_71727c8180, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8180, g_unk_7101a19000);
    {
        CStrId n("DoorWideL", true);
        fn_7100e68cac(&g_unk_71727c8188, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8188, g_unk_7101a19000);
    {
        CStrId n("DoorWideR", true);
        fn_7100e68cac(&g_unk_71727c8190, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8190, g_unk_7101a19000);
    {
        CStrId n("DoorPresenceL", true);
        fn_7100e68cac(&g_unk_71727c8198, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8198, g_unk_7101a19000);
    {
        CStrId n("DoorPresenceR", true);
        fn_7100e68cac(&g_unk_71727c81a0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81a0, g_unk_7101a19000);
    {
        CStrId n("DoorShutter", true);
        fn_7100e68cac(&g_unk_71727c81a8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81a8, g_unk_7101a19000);
    {
        CStrId n("DoorEmmyOpen", true);
        fn_7100e68cac(&g_unk_71727c81b0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81b0, g_unk_7101a19000);
    {
        CStrId n("DoorEmmyClosed", true);
        fn_7100e68cac(&g_unk_71727c81b8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81b8, g_unk_7101a19000);
    {
        CStrId n("DoorThermalOpen", true);
        fn_7100e68cac(&g_unk_71727c81c0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81c0, g_unk_7101a19000);
    {
        CStrId n("DoorThermalClosed", true);
        fn_7100e68cac(&g_unk_71727c81c8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81c8, g_unk_7101a19000);
    {
        CStrId n("DoorThermalTrapOpen", true);
        fn_7100e68cac(&g_unk_71727c81d0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81d0, g_unk_7101a19000);
    {
        CStrId n("DoorThermalTrapClosed", true);
        fn_7100e68cac(&g_unk_71727c81d8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81d8, g_unk_7101a19000);
    {
        CStrId n("DoorCentralUnit", true);
        fn_7100e68cac(&g_unk_71727c81e0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81e0, g_unk_7101a19000);
    {
        CStrId n("BlockageMissileL", true);
        fn_7100e68cac(&g_unk_71727c81e8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81e8, g_unk_7101a19000);
    {
        CStrId n("BlockageMissileR", true);
        fn_7100e68cac(&g_unk_71727c81f0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81f0, g_unk_7101a19000);
    {
        CStrId n("BlockageSuperMissileL", true);
        fn_7100e68cac(&g_unk_71727c81f8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c81f8, g_unk_7101a19000);
    {
        CStrId n("BlockageSuperMissileR", true);
        fn_7100e68cac(&g_unk_71727c8200, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8200, g_unk_7101a19000);
    {
        CStrId n("BlockagePlasmaL", true);
        fn_7100e68cac(&g_unk_71727c8208, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8208, g_unk_7101a19000);
    {
        CStrId n("BlockagePlasmaR", true);
        fn_7100e68cac(&g_unk_71727c8210, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8210, g_unk_7101a19000);
    {
        CStrId n("BlockageWaveL", true);
        fn_7100e68cac(&g_unk_71727c8218, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8218, g_unk_7101a19000);
    {
        CStrId n("BlockageWaveR", true);
        fn_7100e68cac(&g_unk_71727c8220, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8220, g_unk_7101a19000);
    {
        CStrId n("BlockageDisabledL", true);
        fn_7100e68cac(&g_unk_71727c8228, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8228, g_unk_7101a19000);
    {
        CStrId n("BlockageDisabledR", true);
        fn_7100e68cac(&g_unk_71727c8230, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8230, g_unk_7101a19000);
    {
        CStrId n("UsableWaterValve", true);
        fn_7100e68cac(&g_unk_71727c8238, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8238, g_unk_7101a19000);
    {
        CStrId n("UsableThermalDevice", true);
        fn_7100e68cac(&g_unk_71727c8240, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8240, g_unk_7101a19000);
    {
        CStrId n("UsablePowerGenerator", true);
        fn_7100e68cac(&g_unk_71727c8248, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8248, g_unk_7101a19000);
    {
        CStrId n("UsableTrain", true);
        fn_7100e68cac(&g_unk_71727c8250, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8250, g_unk_7101a19000);
    {
        CStrId n("UsableTransport", true);
        fn_7100e68cac(&g_unk_71727c8258, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8258, g_unk_7101a19000);
    {
        CStrId n("UsableElevator", true);
        fn_7100e68cac(&g_unk_71727c8260, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8260, g_unk_7101a19000);
    {
        CStrId n("DisabledElevator", true);
        fn_7100e68cac(&g_unk_71727c8268, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8268, g_unk_7101a19000);
    {
        CStrId n("UsableStationEnergy", true);
        fn_7100e68cac(&g_unk_71727c8270, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8270, g_unk_7101a19000);
    {
        CStrId n("DisabledStationEnergy", true);
        fn_7100e68cac(&g_unk_71727c8278, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8278, g_unk_7101a19000);
    {
        CStrId n("UsableStationAmmo", true);
        fn_7100e68cac(&g_unk_71727c8280, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8280, g_unk_7101a19000);
    {
        CStrId n("DisabledStationAmmo", true);
        fn_7100e68cac(&g_unk_71727c8288, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8288, g_unk_7101a19000);
    {
        CStrId n("UsableStationTotal", true);
        fn_7100e68cac(&g_unk_71727c8290, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8290, g_unk_7101a19000);
    {
        CStrId n("DisabledStationTotal", true);
        fn_7100e68cac(&g_unk_71727c8298, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8298, g_unk_7101a19000);
    {
        CStrId n("UsableStationSave", true);
        fn_7100e68cac(&g_unk_71727c82a0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82a0, g_unk_7101a19000);
    {
        CStrId n("DisabledStationSave", true);
        fn_7100e68cac(&g_unk_71727c82a8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82a8, g_unk_7101a19000);
    {
        CStrId n("UsableStationMap", true);
        fn_7100e68cac(&g_unk_71727c82b0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82b0, g_unk_7101a19000);
    {
        CStrId n("DisabledStationMap", true);
        fn_7100e68cac(&g_unk_71727c82b8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82b8, g_unk_7101a19000);
    {
        CStrId n("UsableAccessPoint", true);
        fn_7100e68cac(&g_unk_71727c82c0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82c0, g_unk_7101a19000);
    {
        CStrId n("DisabledAccessPoint", true);
        fn_7100e68cac(&g_unk_71727c82c8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82c8, g_unk_7101a19000);
    {
        CStrId n("DisabledTeleport", true);
        fn_7100e68cac(&g_unk_71727c82d0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82d0, g_unk_7101a19000);
    {
        CStrId n("UsableTeleportA", true);
        fn_7100e68cac(&g_unk_71727c82d8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82d8, g_unk_7101a19000);
    {
        CStrId n("UsableTeleportE", true);
        fn_7100e68cac(&g_unk_71727c82e0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82e0, g_unk_7101a19000);
    {
        CStrId n("UsableTeleportI", true);
        fn_7100e68cac(&g_unk_71727c82e8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82e8, g_unk_7101a19000);
    {
        CStrId n("UsableTeleportO", true);
        fn_7100e68cac(&g_unk_71727c82f0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82f0, g_unk_7101a19000);
    {
        CStrId n("UsableTeleportU", true);
        fn_7100e68cac(&g_unk_71727c82f8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c82f8, g_unk_7101a19000);
    {
        CStrId n("UsableTeleportX", true);
        fn_7100e68cac(&g_unk_71727c8300, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8300, g_unk_7101a19000);
    {
        CStrId n("UsableTeleportY", true);
        fn_7100e68cac(&g_unk_71727c8308, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8308, g_unk_7101a19000);
    {
        CStrId n("UsableTeleportZ", true);
        fn_7100e68cac(&g_unk_71727c8310, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8310, g_unk_7101a19000);
    {
        CStrId n("ItemSphere", true);
        fn_7100e68cac(&g_unk_71727c8318, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8318, g_unk_7101a19000);
    {
        CStrId n("ItemCube", true);
        fn_7100e68cac(&g_unk_71727c8320, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8320, g_unk_7101a19000);
    {
        CStrId n("ItemEnergyTank", true);
        fn_7100e68cac(&g_unk_71727c8328, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8328, g_unk_7101a19000);
    {
        CStrId n("ItemEnergyFragment", true);
        fn_7100e68cac(&g_unk_71727c8330, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8330, g_unk_7101a19000);
    {
        CStrId n("ItemMissileTank", true);
        fn_7100e68cac(&g_unk_71727c8338, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8338, g_unk_7101a19000);
    {
        CStrId n("ItemMissileTankPlus", true);
        fn_7100e68cac(&g_unk_71727c8340, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8340, g_unk_7101a19000);
    {
        CStrId n("ItemPowerbombExpansion", true);
        fn_7100e68cac(&g_unk_71727c8348, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8348, g_unk_7101a19000);
    {
        CStrId n("ItemAdquired", true);
        fn_7100e68cac(&g_unk_71727c8350, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8350, g_unk_7101a19000);
    {
        CStrId n("PropGrappleBoxL", true);
        fn_7100e68cac(&g_unk_71727c8358, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8358, g_unk_7101a19000);
    {
        CStrId n("PropGrappleBoxR", true);
        fn_7100e68cac(&g_unk_71727c8360, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8360, g_unk_7101a19000);
    {
        CStrId n("PropGrappleBlockL", true);
        fn_7100e68cac(&g_unk_71727c8368, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8368, g_unk_7101a19000);
    {
        CStrId n("PropGrappleBlockR", true);
        fn_7100e68cac(&g_unk_71727c8370, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8370, g_unk_7101a19000);
    {
        CStrId n("PropMorphTunnelEntry", true);
        fn_7100e68cac(&g_unk_71727c8378, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8378, g_unk_7101a19000);
    {
        CStrId n("PropMorphTunnelExit", true);
        fn_7100e68cac(&g_unk_71727c8380, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8380, g_unk_7101a19000);
    {
        CStrId n("PropEmmyValve", true);
        fn_7100e68cac(&g_unk_71727c8388, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8388, g_unk_7101a19000);
    {
        CStrId n("PropHyperBeam", true);
        fn_7100e68cac(&g_unk_71727c8390, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8390, g_unk_7101a19000);
    {
        CStrId n("PropWideBeamBoxR", true);
        fn_7100e68cac(&g_unk_71727c8398, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c8398, g_unk_7101a19000);
    {
        CStrId n("PropWideBeamBoxL", true);
        fn_7100e68cac(&g_unk_71727c83a0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c83a0, g_unk_7101a19000);
    {
        CStrId n("PropMultiLockon", true);
        fn_7100e68cac(&g_unk_71727c83a8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c83a8, g_unk_7101a19000);
    {
        CStrId n("PropFootstepPlatform", true);
        fn_7100e68cac(&g_unk_71727c83b0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c83b0, g_unk_7101a19000);
    {
        CStrId n("PropZipline", true);
        fn_7100e68cac(&g_unk_71727c83b8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c83b8, g_unk_7101a19000);
    {
        CStrId n("PropSwingPoint", true);
        fn_7100e68cac(&g_unk_71727c83c0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c83c0, g_unk_7101a19000);
    {
        CStrId n("PropTurbineActivator", true);
        fn_7100e68cac(&g_unk_71727c83c8, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c83c8, g_unk_7101a19000);
    {
        CStrId n("PropWarlotus", true);
        fn_7100e68cac(&g_unk_71727c83d0, n);
    }
    __cxa_atexit(fn_7100e68dd0, &g_unk_71727c83d0, g_unk_7101a19000);
    InitPathId(&g_unk_71727c8d08.field_0x28, "system/minimap/icons/icons.bsmat");
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mmdef";
        fn_7100004578(&types[0], "CMinimapDef", ".bmmdef", sources0, 1);
        fn_7100004698(g_unk_71727c83d8, "mmapdef", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727c83d8, g_unk_7101a19000);
}
