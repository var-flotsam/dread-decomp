// Copies of fn_7100acd5d0 (src/base/reflection/fn_7100adcf10.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71009ba9f0.
// More per-TU static initialisers (helpers in game/UnkTUInit.h; see also fn_7100b04900.cpp):
// some TUs' data has function records after the header, and collection and vector types are
// registered for the reflected containers.
#include "base/UnkStrIdRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

using base::global::CFilePathStrId;
using base::global::CRntString;

UnkReflType* fn_7100adc1d8();  // base::reflection::refl_GetType<gameeditor::TPropertyId>
UnkReflType* fn_7100adc2b4();
UnkReflType* fn_7100adc390();
void fn_7100adc470();
void fn_7100adc4c0();

extern UnkTUHeaderFns g_unk_71727585d8 __attribute__((visibility("hidden")));
extern char g_unk_7172758254;
extern char g_unk_7172758255;
extern char g_unk_7172758256;
extern UnkConverter g_unk_7172758258;
extern UnkConverter g_unk_7172758278;
extern UnkValue g_unk_7172758298;

extern UnkTUHeader g_unk_71727568f8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172755e70;
void fn_7100acd79c();
void fn_7100ace28c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172755fa8;
UnkReflType* fn_7100ace884();
extern char g_unk_7172755fa9;
UnkReflType* fn_7100ace960();
extern char g_unk_7172755faa;
extern UnkReflType g_unk_7172755fb0;
void fn_7100acea3c();
void fn_7100acedc0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727560e8;
UnkReflType* fn_7100acef44();
extern char g_unk_71727560e9;
extern UnkReflType g_unk_71727560f0;
void fn_7100acf020();
void fn_7100acf3a4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172756228;
UnkReflType* fn_7100acf528();
extern char g_unk_7172756229;
extern UnkReflType g_unk_7172756230;
void fn_7100acf604(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71727563a0;
UnkReflType* fn_7100acf7f8();
extern char g_unk_71727563a1;

extern UnkTUHeader g_unk_717275bf40 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717275b580;
void fn_7100ae963c();
void fn_7100ae96e8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275b6b8;
extern UnkConverter g_unk_717275b6c0;
void fn_7100aebf80() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_717275b6e0;
void fn_7100aec000() __attribute__((visibility("hidden")));
extern char g_unk_717275b700;
extern UnkReflType g_unk_717275bf60 __attribute__((visibility("hidden")));
UnkReflType* fn_7100aea26c();
extern char g_unk_717275b701;
UnkReflType* fn_7100aea348();
extern char g_unk_717275b702;
extern UnkReflType g_unk_717275b708;
void fn_7100aea424();
void fn_7100aea7a8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275b840;
UnkReflType* fn_7100aea92c();
extern char g_unk_717275b841;
extern UnkReflType g_unk_717275b848;
void fn_7100aeaa08();
void fn_7100aead8c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275b980;
UnkReflType* fn_7100aeaf10();
extern char g_unk_717275b981;
extern UnkReflType g_unk_717275b988;
void fn_7100aeafec(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_717275baf8;

extern UnkTUHeaderFnsClass g_unk_717275ed68 __attribute__((visibility("hidden")));
extern char g_unk_717275e1f8;
UnkReflType* fn_7100b05394();  // base::reflection::refl_GetType<GUI::CDisplayObjectDef*>
extern char g_unk_717275e1f9;
UnkReflType* fn_7100b05470();
extern char g_unk_717275e1fa;
UnkReflType* fn_7100b0554c();
extern char g_unk_717275e1fb;
UnkReflType* fn_7100b05628();
extern char g_unk_717275e1fc;
extern UnkReflType g_unk_717275e200;
void fn_7100b05704();
void fn_7100b05a88(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275e338;
UnkReflType* fn_7100b05c0c();
extern char g_unk_717275e339;
extern UnkReflType g_unk_717275e340;
void fn_7100b05ce8();
void fn_7100b0606c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275e478;
UnkReflType* fn_7100b061f0();
extern char g_unk_717275e479;
extern UnkReflType g_unk_717275e480;
void fn_7100b062cc(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_717275e5f0;

extern UnkTUHeader g_unk_717275f8c8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717275ef50;
void fn_7100b0a450();
void fn_7100b0a6cc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275f088;
UnkReflType* fn_7100b08378();  // base::reflection::refl_GetType<GUI::CDisplayObjectStateDef*>
extern char g_unk_717275f089;
UnkReflType* fn_7100b0a9a8();
extern char g_unk_717275f08a;
UnkReflType* fn_7100b0aa84();
extern char g_unk_717275f08b;
extern UnkReflType g_unk_717275f090;
void fn_7100b0ab60();
void fn_7100b0aee4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275f1c8;
UnkReflType* fn_7100b0b068();
extern char g_unk_717275f1c9;
extern UnkReflType g_unk_717275f1d0;
void fn_7100b0b144();
void fn_7100b0b4c8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275f308;
UnkReflType* fn_7100b0b64c();
extern char g_unk_717275f309;
extern UnkReflType g_unk_717275f310;
void fn_7100b0b728(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_717275f480;

extern UnkTUHeader g_unk_7172766bb8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172766130;
void fn_7100b75ea8();
void fn_7100b7612c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172766268;
UnkReflType* fn_7100b762b0();  // base::reflection::refl_GetType<GUI::CSkin*>
extern char g_unk_7172766269;
UnkReflType* fn_7100b7638c();
extern char g_unk_717276626a;
UnkReflType* fn_7100b76468();
extern char g_unk_717276626b;
extern UnkReflType g_unk_7172766270;
void fn_7100b76544();
void fn_7100b768c8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727663a8;
UnkReflType* fn_7100b76a4c();
extern char g_unk_71727663a9;
extern UnkReflType g_unk_71727663b0;
void fn_7100b76b28();
void fn_7100b76eac(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727664e8;
UnkReflType* fn_7100b77030();
extern char g_unk_71727664e9;
extern UnkReflType g_unk_71727664f0;
void fn_7100b7710c(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172766660;

extern UnkTUHeader g_unk_7172767b98 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172767000;
void fn_7100b7d05c();
void fn_7100b7d580(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172767138;
UnkReflType* fn_7100b7d880();  // base::reflection::refl_GetType<GUI::CSpriteSheet*>
extern char g_unk_7172767139;
UnkReflType* fn_7100b7d95c();
extern char g_unk_717276713a;
UnkReflType* fn_7100b7da38();
extern char g_unk_717276713b;
extern UnkReflType g_unk_7172767140;
void fn_7100b7db14();
void fn_7100b7de98(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172767278;
UnkReflType* fn_7100b7e01c();
extern char g_unk_7172767279;
extern UnkReflType g_unk_7172767280;
void fn_7100b7e0f8();
void fn_7100b7e47c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727673b8;
UnkReflType* fn_7100b7e600();
extern char g_unk_71727673b9;
extern UnkReflType g_unk_71727673c0;
void fn_7100b7e6dc(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172767530;

extern UnkTUHeader g_unk_7172768668 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172767bb0;
void fn_7100b81620();
void fn_7100b8172c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172767ce8;
extern UnkReflType g_unk_7172767cf0;
void fn_7100b818b0();
void fn_7100b819bc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172767e28;
UnkReflType* fn_7100b80034();  // base::reflection::refl_GetType<GUI::CSpriteSheetItem*>
extern char g_unk_7172767e29;
UnkReflType* fn_7100b81b40();
extern char g_unk_7172767e2a;
UnkReflType* fn_7100b81c1c();
extern char g_unk_7172767e2b;
extern UnkReflType g_unk_7172767e30;
void fn_7100b81cf8();
void fn_7100b8207c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172767f68;
UnkReflType* fn_7100b82200();
extern char g_unk_7172767f69;
extern UnkReflType g_unk_7172767f70;
void fn_7100b822dc();
void fn_7100b82660(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727680a8;
UnkReflType* fn_7100b827e4();
extern char g_unk_71727680a9;
extern UnkReflType g_unk_71727680b0;
void fn_7100b828c0(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172768220;

extern UnkTUHeader g_unk_7172794a98 __attribute__((visibility("hidden")));
extern char g_unk_71727930c8;
extern char g_unk_71727930c9;
UnkReflType* fn_7100c837d4();
extern char g_unk_71727930ca;
UnkReflType* fn_7100c838b0();
extern char g_unk_71727930cb;
extern UnkReflType g_unk_71727930d0;
void fn_7100c8398c();
void fn_7100c83d1c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172793208;
UnkReflType* fn_7100c83ea0();
extern char g_unk_7172793209;
extern UnkReflType g_unk_7172793210;
void fn_7100c83f7c();
void fn_7100c8430c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172793348;
UnkReflType* fn_7100c84490();
extern char g_unk_7172793349;
extern UnkReflType g_unk_7172793350;
void fn_7100c8456c(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71727934c0;
extern char g_unk_71727934c1;
extern UnkReflType g_unk_7172794d40 __attribute__((visibility("hidden")));
UnkReflType* fn_7100c84da4();
extern char g_unk_71727934c2;
UnkReflType* fn_7100c84e80();
extern char g_unk_71727934c3;
extern UnkReflType g_unk_71727934c8;
void fn_7100c84f5c();
void fn_7100c852ec(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172793600;
UnkReflType* fn_7100c85470();
extern char g_unk_7172793601;
extern UnkReflType g_unk_7172793608;
void fn_7100c8554c();
void fn_7100c858dc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172793740;
UnkReflType* fn_7100c85a60();
extern char g_unk_7172793741;
extern UnkReflType g_unk_7172793748;
void fn_7100c85b3c(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71727938b8;
extern char g_unk_71727938b9;
UnkReflType* fn_7100c868ec();
extern char g_unk_71727938ba;
UnkReflType* fn_7100c869c8();
extern char g_unk_71727938bb;
extern UnkReflType g_unk_71727938c0;
void fn_7100c86aa4();
void fn_7100c86e34(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727939f8;
UnkReflType* fn_7100c86fb8();
extern char g_unk_71727939f9;
extern UnkReflType g_unk_7172793a00;
void fn_7100c87094();
void fn_7100c87424(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172793b38;
UnkReflType* fn_7100c875a8();
extern char g_unk_7172793b39;
extern UnkReflType g_unk_7172793b40;
void fn_7100c87684(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172793cb0;
UnkReflType* fn_7100c8787c();
extern char g_unk_7172793cb1;

extern UnkTUHeader g_unk_71727b4f88 __attribute__((visibility("hidden")));
extern char g_unk_71727b3dd8;
UnkReflType* fn_71009155a8();  // base::reflection::refl_GetType<game::logic::collision::CShape*>
extern char g_unk_71727b3dd9;
UnkReflType* fn_7100d6a05c();
extern char g_unk_71727b3dda;
UnkReflType* fn_7100913068();  // base::reflection::refl_GetType<std::unique_ptr<game::logic::collision::CShape>>
extern char g_unk_71727b3ddb;
extern UnkConverter g_unk_71727b3de0;
void fn_7100d6e22c();
UnkReflType* fn_7100d6a138();
extern char g_unk_71727b3e00;
UnkReflType* fn_7100d6a214();
extern char g_unk_71727b3e01;
extern UnkReflType g_unk_71727b3e08;
void fn_7100d6a2f0();
void fn_7100d6a674(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727b3f40;
UnkReflType* fn_7100d6a7f8();
extern char g_unk_71727b3f41;
extern UnkReflType g_unk_71727b3f48;
void fn_7100d6a8d4();
void fn_7100d6ac58(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727b4080;
UnkReflType* fn_7100d6addc();
extern char g_unk_71727b4081;
extern UnkReflType g_unk_71727b4088;
void fn_7100d6aeb8(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71727b41f8;
UnkReflType* fn_7100d6b088();
extern char g_unk_71727b41f9;
UnkReflType* fn_7100d6b164();
extern char g_unk_71727b41fa;
extern UnkReflType g_unk_71727b4200;
void fn_7100d6b240();
void fn_7100d6b5c4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727b4338;
UnkReflType* fn_7100d6b748();
extern char g_unk_71727b4339;
extern UnkReflType g_unk_71727b4340;
void fn_7100d6b824();
void fn_7100d6bba8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727b4478;
UnkReflType* fn_7100d6bd2c();
extern char g_unk_71727b4479;
extern UnkReflType g_unk_71727b4480;
void fn_7100d6be08(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71727b45f0;

extern UnkTUHeader g_unk_71727bb618 __attribute__((visibility("hidden")));
extern char g_unk_71727ba998;
UnkReflType* fn_7100d9d978();
extern char g_unk_71727ba999;
UnkReflType* fn_7100d9da54();
extern char g_unk_71727ba99a;
UnkReflType* fn_7100d9db30();
extern char g_unk_71727ba99b;
UnkReflType* fn_7100d9dc0c();
extern char g_unk_71727ba99c;
extern UnkReflType g_unk_71727ba9a0;
void fn_7100d9dce8();
void fn_7100d9e06c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727baad8;
UnkReflType* fn_7100d9e1f0();
extern char g_unk_71727baad9;
extern UnkReflType g_unk_71727baae0;
void fn_7100d9e2cc();
void fn_7100d9e650(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727bac18;
UnkReflType* fn_7100d9e7d4();
extern char g_unk_71727bac19;
extern UnkReflType g_unk_71727bac20;
void fn_7100d9e8b0(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71727bad90;
UnkReflType* fn_7100d9eaa4();
extern char g_unk_71727bad91;
UnkReflType* fn_7100d9eb80();
extern char g_unk_71727bad92;

extern UnkTUHeader g_unk_71727bc9d0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bc870;
void fn_7100daf1cc();
void fn_7100daf558(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bc988;
void fn_7100db2038() __attribute__((visibility("hidden")));
void fn_7100db2044() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bc990;
extern UnkConverter g_unk_71727bc9b0;

extern UnkTUHeaderFnsClass g_unk_71727c0520 __attribute__((visibility("hidden")));
extern char g_unk_71727bf9b0;
UnkReflType* fn_7100e085b0();
extern char g_unk_71727bf9b1;
UnkReflType* fn_7100e0868c();
extern char g_unk_71727bf9b2;
extern UnkReflType g_unk_71727bf9b8;
void fn_7100e08768();
void fn_7100e08aec(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727bfaf0;
UnkReflType* fn_7100e08c70();
extern char g_unk_71727bfaf1;
extern UnkReflType g_unk_71727bfaf8;
void fn_7100e08d4c();
void fn_7100e090d0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727bfc30;
UnkReflType* fn_7100e09254();
extern char g_unk_71727bfc31;
extern UnkReflType g_unk_71727bfc38;
void fn_7100e09330(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71727bfda8;
extern char g_unk_71727bfda9;
UnkReflType* fn_7100e09500();
extern char g_unk_71727bfdaa;
UnkReflType* fn_7100e095dc();
extern char g_unk_71727bfdab;
UnkReflType* fn_7100e096b8();
extern char g_unk_71727bfdac;

extern UnkTUHeader g_unk_71727c9948 __attribute__((visibility("hidden")));
struct UnkCharMap;
extern UnkCharMap g_unk_71727c8f58;
void fn_7100e768b0(UnkCharMap* self, const char* chars, u32 count, u32 a, void* b, u32 c);
void fn_7100e76a5c(void* self);
UnkReflType* fn_7100e76a98();
extern char g_unk_71727c8f80;
UnkReflType* fn_7100e76b74();
extern char g_unk_71727c8f81;
extern UnkReflType g_unk_71727c8f88;
void fn_7100e76c50();
void fn_7100e76fd4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727c90c0;
UnkReflType* fn_7100e77158();
extern char g_unk_71727c90c1;
extern UnkReflType g_unk_71727c90c8;
void fn_7100e77234();
void fn_7100e775b8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727c9200;
UnkReflType* fn_7100e7773c();
extern char g_unk_71727c9201;
extern UnkReflType g_unk_71727c9208;
void fn_7100e77818(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71727c9378;
extern UnkReflType g_unk_71727c9380;
void fn_7100e77a0c();
void fn_7100e77d68(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727c94b8;
extern UnkPropertyEx<> g_unk_71727c94c0;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101a45f90[];
void fn_71004fa3b8(void* p);

extern UnkTUHeader g_unk_7172755b78 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172755b00;
extern UnkProperty g_unk_7172755b08;
extern void* g_unk_7101a97370[];
void fn_7100acacf8(UnkProperty* p);
void fn_7100ac8db0(void* p);
extern CStrId g_unk_7172755b90 __attribute__((visibility("hidden")));
extern char g_unk_7172755b30;

extern UnkTUHeader g_unk_7172797460 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172796388;
extern UnkReflType g_unk_71727966a0;
void fn_7100ca3c84();
void fn_7100ca3e50(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727967d8;
UnkReflType* fn_7100ca3fb0();
extern char g_unk_71727967d9;
extern UnkReflType g_unk_71727967e0;
void fn_7100ca408c();
void fn_7100ca4090(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172796918;
UnkReflType* fn_7100ca4214();
extern char g_unk_7172796919;
extern char g_unk_717279691a;
extern UnkReflType g_unk_7172797480 __attribute__((visibility("hidden")));
extern UnkTunable g_unk_7172796920;
extern void* g_unk_7101aa5328[];
void fn_7100075104(void* self);
UnkReflType* fn_7100ca4f04();
extern char g_unk_7172796971;
UnkReflType* fn_7100ca4fe0();
extern char g_unk_7172796972;
extern UnkReflType g_unk_7172796978;
void fn_7100ca50bc();
void fn_7100ca5440(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172796ab0;
UnkReflType* fn_7100ca55c4();
extern char g_unk_7172796ab1;
extern UnkReflType g_unk_7172796ab8;
void fn_7100ca56a0();
void fn_7100ca5a24(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172796bf0;
UnkReflType* fn_7100ca5ba8();
extern char g_unk_7172796bf1;
extern UnkReflType g_unk_7172796bf8;
void fn_7100ca5c84(UnkReflType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172796d68;
extern UnkPropertyEx<0x160> g_unk_7172796390;
extern UnkPropertyEx<0x160> g_unk_7172796518;
extern void* g_unk_7101a889c8[];
extern void* g_unk_7101aa52d0[];
void fn_7100ca9504(UnkPropertyEx<0x160>* p);
void fn_7100ca3be4(void* p);
extern CFilePathStrId g_unk_7172796d70[10];
void fn_7100ca5e60(void*) __attribute__((visibility("hidden")));  // destroys g_unk_7172796d70

extern UnkTUHeader g_unk_71727c0a38 __attribute__((visibility("hidden")));
extern CStrId g_unk_71727c0918;
extern UnkProperty g_unk_71727c0920;
extern UnkProperty g_unk_71727c0948;
extern UnkProperty g_unk_71727c0970;
extern UnkProperty g_unk_71727c0998;
extern void* g_unk_7101aa9c38[];
void fn_7100e165e8(UnkProperty* p);
void fn_7100e140c0(void* p);
extern char g_unk_71727c09c0;
extern CRntString g_unk_71727c0a58 __attribute__((visibility("hidden")));
extern UnkTunable g_unk_71727c09c8;
extern void* g_unk_7101aa9c90[];
void fn_7100075104(void* self);

extern UnkTUHeader g_unk_717273d710 __attribute__((visibility("hidden")));
extern CStrId g_unk_717273d728 __attribute__((visibility("hidden")));
extern char g_unk_717273d6f8;

// The id lives in an inline function: its guard and object are reached through the GOT.
inline const CStrId& ChangeStageNavmeshItemId() {
    static CStrId s_id("CHANGE_STAGE_NAVMESH_ITEM", true);
    return s_id;
}

void fn_71009ba9f0() {
    InitHeader(g_unk_717273d710);
    new (&g_unk_717273d728) CStrId(fn_7100de8f78(ChangeStageNavmeshItemId()));
    fn_7100000250(fn_710000051c, &g_unk_717273d728, &g_unk_7101a19000);
    fn_71009b9654();
    fn_710009ec00(&g_unk_717273d6f8, TUType<0x10>(g_unk_717273d728));
}

extern UnkTUHeader g_unk_7172756998 __attribute__((visibility("hidden")));
UnkReflType* fn_710025d654();  // base::reflection::refl_GetType<gameeditor::CGameModelIndex>
extern char g_unk_7172756940;
extern UnkConverter g_unk_7172756948;
void fn_7100ad37c0() __attribute__((visibility("hidden")));
extern UnkValue g_unk_7172756968;
extern UnkValue g_unk_7172756978;
extern UnkValue g_unk_7172756988;
extern const char g_unk_710164b8ab[];
extern const char g_unk_710164b8ad[];
extern const char g_unk_710164b8af[];

extern UnkTUHeader g_unk_71727bc598 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bc290;
void fn_7100da684c();
void fn_7100da6938(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bc3a8;
void fn_7100dab9bc() __attribute__((visibility("hidden")));
void fn_7100dab9c8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bc3b0;
extern UnkConverter g_unk_71727bc3d0;
extern UnkReflType g_unk_71727bc3f0;
void fn_7100da6ab8();
void fn_7100da6cc4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bc508;
void fn_7100dab9d4() __attribute__((visibility("hidden")));
void fn_7100dab9e0() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bc510;
extern UnkConverter g_unk_71727bc530;
extern UnkTUData g_unk_71727bc5b0 __attribute__((visibility("hidden")));
void fn_7100da6e50(void*) __attribute__((visibility("hidden")));  // destroys an array
void fn_7100da6f50(void*) __attribute__((visibility("hidden")));  // destroys an array
extern char g_unk_71727bc550;
extern UnkTunable g_unk_71727bc558;
extern void* g_unk_7101aa9278[];
void fn_7100075104(void* self);

extern UnkTUHeader g_unk_7172791e70 __attribute__((visibility("hidden")));

struct UnkElem188 {
    virtual void vf_0x0();
    virtual ~UnkElem188();
    UnkElem188(UnkElem188&& other);  // fn_7100d36688
    u8 unk_0x8[0x180];
};
extern base::UnkVector<UnkElem188> g_unk_7172791e48;
void fn_7100c66ab0(void* vector);
extern CStrId g_unk_7172791e88 __attribute__((visibility("hidden")));

extern UnkTUHeader g_unk_71727bc0f8 __attribute__((visibility("hidden")));
void fn_7100da4720();
void fn_7100da4a08(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727bbfa0;
UnkReflType* fn_7100da4cf0();
extern char g_unk_71727bbfa1;
extern UnkConverter g_unk_71727bbfa8;
void fn_7100da5184() __attribute__((visibility("hidden")));
extern CStrId g_unk_71727bbfc8;
extern CStrId g_unk_71727bbfd0;
struct UnkPair16 {
    u64 field_0x0;
    u64 field_0x8;
};
extern UnkPair16 g_unk_71727bbfd8;

extern UnkTUHeader g_unk_71729ce040 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71729cdd78;
void fn_7100eb695c();
void fn_7100eb6b48(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71729cdeb0;
extern UnkReflType g_unk_71729cdeb8;
void fn_7100eb6ccc();
void fn_7100eb71d8(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71729cdfd0;
void fn_7100ebecec() __attribute__((visibility("hidden")));
void fn_7100ebecf8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71729cdfd8;
extern UnkConverter g_unk_71729cdff8;
extern UnkTUData g_unk_71729ce058 __attribute__((visibility("hidden")));
void fn_7100eb7590(void* self);
// An array-like global: three zero words, a growth step of 10, flags 0xe and the default
// allocator.
struct UnkArray_71729ce018 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;
};
extern UnkArray_71729ce018 g_unk_71729ce018;
