// More per-TU static initialisers (helpers in game/UnkTUInit.h; see also fn_7100b04900.cpp):
// some TUs' data has function records after the header, and collection and vector types are
// registered for the reflected containers.
#include "base/UnkStrIdRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

using base::global::CFilePathStrId;
using base::global::CRntString;

// The reflected types UnkReflTypeGlobals.h declares are the generated sources' CType; the registration
// helpers see a type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(const base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(const_cast<base::reflection::CType*>(t));
}

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

void fn_7100adcf10() {
    InitHeaderFns(g_unk_71727585d8);

    fn_710009ec00(&g_unk_7172758254, fn_7100adc1d8());
    fn_710009ec00(&g_unk_7172758255, fn_7100adc2b4());
    fn_710009ec00(&g_unk_7172758256, fn_7100adc390());

    InitConverter(g_unk_7172758258, fn_7100adc1d8, refl_TypeOf<fn_7100005178>, fn_7100adc470);
    InitConverter(g_unk_7172758278, fn_7100adc1d8, refl_TypeOf<fn_7100009658>, fn_7100adc4c0);

    new (&g_unk_7172758298) UnkValue(static_cast<u32>(-1), refl_TypeOf<fn_7100005178>);
    fn_7100000250(fn_710010c424, &g_unk_7172758298, &g_unk_7101a19000);
}

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

void fn_7100ad28e0() {
    InitHeader(g_unk_71727568f8);
    InitClass(&g_unk_7172755e70, "SWorldGraphNode", fn_7100ace28c, fn_7100acd79c);
    fn_710009ec00(&g_unk_7172755fa8, &g_unk_7172755e70);
    fn_710009ec00(&g_unk_7172755fa9, fn_7100ace884());
    fn_710009ec00(&g_unk_7172755faa, fn_7100ace960());
    InitClass(&g_unk_7172755fb0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<SWorldGraphNode>>::TKeyElementIterator::"
              "value_type",
              fn_7100acedc0, fn_7100acea3c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727560e8, &g_unk_7172755fb0);
    fn_710009ec00(&g_unk_71727560e9, fn_7100acef44());
    InitClass(&g_unk_71727560f0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<SWorldGraphNode>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100acf3a4, fn_7100acf020, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172756228, &g_unk_71727560f0);
    fn_710009ec00(&g_unk_7172756229, fn_7100acf528());
    InitVector(&g_unk_7172756230, "base::global::CRntVector<SWorldGraphNode>", fn_7100acf604);
    fn_710009ec00(&g_unk_71727563a0, &g_unk_7172756230);
    fn_710009ec00(&g_unk_71727563a1, fn_7100acf7f8());
}

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

void fn_7100aef910() {
    InitHeader(g_unk_717275bf40);
    InitClass(&g_unk_717275b580, "CGameBBPropID", fn_7100ae96e8, fn_7100ae963c);
    fn_710009ec00(&g_unk_717275b6b8, &g_unk_717275b580);
    InitConverterTo(g_unk_717275b6c0, &g_unk_717275b580, refl_TypeOf<fn_7100009658>, fn_7100aebf80);
    InitConverterFrom(g_unk_717275b6e0, &g_unk_717275b580, refl_TypeOf<fn_7100009658>, fn_7100aec000);
    fn_7100ae9e7c();
    fn_710009ec00(&g_unk_717275b700, &g_unk_717275bf60);
    fn_710009ec00(&g_unk_717275b701, fn_7100aea26c());
    fn_710009ec00(&g_unk_717275b702, fn_7100aea348());
    InitClass(&g_unk_717275b708,
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary< base::global::CStrId, "
              "CGameBlackboard::TPropDeltaValues >>::TKeyElementIterator::value_type",
              fn_7100aea7a8, fn_7100aea424, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717275b840, &g_unk_717275b708);
    fn_710009ec00(&g_unk_717275b841, fn_7100aea92c());
    InitClass(&g_unk_717275b848,
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary< base::global::CStrId, "
              "CGameBlackboard::TPropDeltaValues >>::TKeyElementConstIterator::value_type",
              fn_7100aead8c, fn_7100aeaa08, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717275b980, &g_unk_717275b848);
    fn_710009ec00(&g_unk_717275b981, fn_7100aeaf10());
    InitVector(&g_unk_717275b988,
               "base::global::CRntSmallDictionary< base::global::CStrId, CGameBlackboard::TPropDeltaValues >",
               fn_7100aeafec);
    fn_710009ec00(&g_unk_717275baf8, &g_unk_717275b988);
}

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

void fn_7100b0a100() {
    InitHeaderFns(g_unk_717275ed68);
    fn_7100b04a5c();
    fn_710009ec00(&g_unk_717275e1f8, TUType<0xb0>(g_unk_717275ed68));
    fn_710009ec00(&g_unk_717275e1f9, fn_7100b05394());
    fn_710009ec00(&g_unk_717275e1fa, fn_7100b05470());
    fn_710009ec00(&g_unk_717275e1fb, fn_7100b0554c());
    fn_710009ec00(&g_unk_717275e1fc, fn_7100b05628());
    InitClass(&g_unk_717275e200,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObjectDef*>>::"
              "TKeyElementIterator::value_type",
              fn_7100b05a88, fn_7100b05704, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717275e338, &g_unk_717275e200);
    fn_710009ec00(&g_unk_717275e339, fn_7100b05c0c());
    InitClass(&g_unk_717275e340,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObjectDef*>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100b0606c, fn_7100b05ce8, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717275e478, &g_unk_717275e340);
    fn_710009ec00(&g_unk_717275e479, fn_7100b061f0());
    InitVector(&g_unk_717275e480, "base::global::CRntVector<GUI::CDisplayObjectDef*>", fn_7100b062cc);
    fn_710009ec00(&g_unk_717275e5f0, &g_unk_717275e480);
}

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

void fn_7100b0d6c0() {
    InitHeader(g_unk_717275f8c8);
    InitClass(&g_unk_717275ef50, "GUI::CDisplayObjectStateDef", fn_7100b0a6cc, fn_7100b0a450);
    fn_710009ec00(&g_unk_717275f088, &g_unk_717275ef50);
    fn_710009ec00(&g_unk_717275f089, fn_7100b08378());
    fn_710009ec00(&g_unk_717275f08a, fn_7100b0a9a8());
    fn_710009ec00(&g_unk_717275f08b, fn_7100b0aa84());
    InitClass(&g_unk_717275f090,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObjectStateDef*>>::"
              "TKeyElementIterator::value_type",
              fn_7100b0aee4, fn_7100b0ab60, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717275f1c8, &g_unk_717275f090);
    fn_710009ec00(&g_unk_717275f1c9, fn_7100b0b068());
    InitClass(&g_unk_717275f1d0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObjectStateDef*>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100b0b4c8, fn_7100b0b144, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717275f308, &g_unk_717275f1d0);
    fn_710009ec00(&g_unk_717275f309, fn_7100b0b64c());
    InitVector(&g_unk_717275f310, "base::global::CRntVector<GUI::CDisplayObjectStateDef*>", fn_7100b0b728);
    fn_710009ec00(&g_unk_717275f480, &g_unk_717275f310);
}

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

void fn_7100b797e0() {
    InitHeader(g_unk_7172766bb8);
    InitClass(&g_unk_7172766130, "GUI::CSkin", fn_7100b7612c, fn_7100b75ea8);
    fn_710009ec00(&g_unk_7172766268, &g_unk_7172766130);
    fn_710009ec00(&g_unk_7172766269, fn_7100b762b0());
    fn_710009ec00(&g_unk_717276626a, fn_7100b7638c());
    fn_710009ec00(&g_unk_717276626b, fn_7100b76468());
    InitClass(&g_unk_7172766270,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CSkin*>>::TKeyElementIterator::"
              "value_type",
              fn_7100b768c8, fn_7100b76544, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727663a8, &g_unk_7172766270);
    fn_710009ec00(&g_unk_71727663a9, fn_7100b76a4c());
    InitClass(&g_unk_71727663b0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CSkin*>>::TKeyElementConstIterator:"
              ":value_type",
              fn_7100b76eac, fn_7100b76b28, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727664e8, &g_unk_71727663b0);
    fn_710009ec00(&g_unk_71727664e9, fn_7100b77030());
    InitVector(&g_unk_71727664f0, "base::global::CRntVector<GUI::CSkin*>", fn_7100b7710c);
    fn_710009ec00(&g_unk_7172766660, &g_unk_71727664f0);
}

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

void fn_7100b81290() {
    InitHeader(g_unk_7172767b98);
    InitClass(&g_unk_7172767000, "GUI::CSpriteSheet", fn_7100b7d580, fn_7100b7d05c);
    fn_710009ec00(&g_unk_7172767138, &g_unk_7172767000);
    fn_710009ec00(&g_unk_7172767139, fn_7100b7d880());
    fn_710009ec00(&g_unk_717276713a, fn_7100b7d95c());
    fn_710009ec00(&g_unk_717276713b, fn_7100b7da38());
    InitClass(&g_unk_7172767140,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "GUI::CSpriteSheet*>>::TKeyElementIterator::value_type",
              fn_7100b7de98, fn_7100b7db14, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172767278, &g_unk_7172767140);
    fn_710009ec00(&g_unk_7172767279, fn_7100b7e01c());
    InitClass(&g_unk_7172767280,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "GUI::CSpriteSheet*>>::TKeyElementConstIterator::value_type",
              fn_7100b7e47c, fn_7100b7e0f8, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727673b8, &g_unk_7172767280);
    fn_710009ec00(&g_unk_71727673b9, fn_7100b7e600());
    InitVector(&g_unk_71727673c0, "base::global::CRntDictionary<base::global::CStrId, GUI::CSpriteSheet*>",
               fn_7100b7e6dc);
    fn_710009ec00(&g_unk_7172767530, &g_unk_71727673c0);
}

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

void fn_7100b84970() {
    InitHeader(g_unk_7172768668);
    InitClass(&g_unk_7172767bb0, "GUI::CSpriteSheetItem::STexUV", fn_7100b8172c, fn_7100b81620);
    fn_710009ec00(&g_unk_7172767ce8, &g_unk_7172767bb0);
    InitClass(&g_unk_7172767cf0, "GUI::CSpriteSheetItem", fn_7100b819bc, fn_7100b818b0);
    fn_710009ec00(&g_unk_7172767e28, &g_unk_7172767cf0);
    fn_710009ec00(&g_unk_7172767e29, fn_7100b80034());
    fn_710009ec00(&g_unk_7172767e2a, fn_7100b81b40());
    fn_710009ec00(&g_unk_7172767e2b, fn_7100b81c1c());
    InitClass(&g_unk_7172767e30,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CSpriteSheetItem*>>::"
              "TKeyElementIterator::value_type",
              fn_7100b8207c, fn_7100b81cf8, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172767f68, &g_unk_7172767e30);
    fn_710009ec00(&g_unk_7172767f69, fn_7100b82200());
    InitClass(&g_unk_7172767f70,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CSpriteSheetItem*>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100b82660, fn_7100b822dc, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727680a8, &g_unk_7172767f70);
    fn_710009ec00(&g_unk_71727680a9, fn_7100b827e4());
    InitVector(&g_unk_71727680b0, "base::global::CRntVector<GUI::CSpriteSheetItem*>", fn_7100b828c0);
    fn_710009ec00(&g_unk_7172768220, &g_unk_71727680b0);
}

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

void fn_7100c92b10() {
    InitHeader(g_unk_7172794a98);
    fn_7100c81418();
    fn_710009ec00(&g_unk_71727930c8, TUType<0x38>(g_unk_7172794a98));
    fn_7100c828a4();
    fn_710009ec00(&g_unk_71727930c9, TUType<0x170>(g_unk_7172794a98));
    fn_710009ec00(&g_unk_71727930ca, fn_7100c837d4());
    fn_710009ec00(&g_unk_71727930cb, fn_7100c838b0());
    InitClass(&g_unk_71727930d0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutSceneDef::CActorDef>>::"
              "TKeyElementIterator::value_type",
              fn_7100c83d1c, fn_7100c8398c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172793208, &g_unk_71727930d0);
    fn_710009ec00(&g_unk_7172793209, fn_7100c83ea0());
    InitClass(&g_unk_7172793210,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutSceneDef::CActorDef>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100c8430c, fn_7100c83f7c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172793348, &g_unk_7172793210);
    fn_710009ec00(&g_unk_7172793349, fn_7100c84490());
    InitVector(&g_unk_7172793350, "base::global::CRntVector<CCutSceneDef::CActorDef>", fn_7100c8456c);
    fn_710009ec00(&g_unk_71727934c0, &g_unk_7172793350);
    fn_7100c84768();
    fn_710009ec00(&g_unk_71727934c1, &g_unk_7172794d40);
    fn_710009ec00(&g_unk_71727934c2, fn_7100c84da4());
    fn_710009ec00(&g_unk_71727934c3, fn_7100c84e80());
    InitClass(&g_unk_71727934c8,
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, "
              "CCutSceneDef::CAnimationLayerDef>>::TKeyElementIterator::value_type",
              fn_7100c852ec, fn_7100c84f5c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172793600, &g_unk_71727934c8);
    fn_710009ec00(&g_unk_7172793601, fn_7100c85470());
    InitClass(&g_unk_7172793608,
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, "
              "CCutSceneDef::CAnimationLayerDef>>::TKeyElementConstIterator::value_type",
              fn_7100c858dc, fn_7100c8554c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172793740, &g_unk_7172793608);
    fn_710009ec00(&g_unk_7172793741, fn_7100c85a60());
    InitVector(&g_unk_7172793748,
               "base::global::CRntSmallDictionary<base::global::CStrId, CCutSceneDef::CAnimationLayerDef>",
               fn_7100c85b3c);
    fn_710009ec00(&g_unk_71727938b8, &g_unk_7172793748);
    fn_7100c85d3c();
    fn_710009ec00(&g_unk_71727938b9, AsUnk(&g_unk_7172794e78));
    fn_710009ec00(&g_unk_71727938ba, fn_7100c868ec());
    fn_710009ec00(&g_unk_71727938bb, fn_7100c869c8());
    InitClass(&g_unk_71727938c0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutSceneDef::CTakeDef>>::"
              "TKeyElementIterator::value_type",
              fn_7100c86e34, fn_7100c86aa4, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727939f8, &g_unk_71727938c0);
    fn_710009ec00(&g_unk_71727939f9, fn_7100c86fb8());
    InitClass(&g_unk_7172793a00,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutSceneDef::CTakeDef>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100c87424, fn_7100c87094, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172793b38, &g_unk_7172793a00);
    fn_710009ec00(&g_unk_7172793b39, fn_7100c875a8());
    InitVector(&g_unk_7172793b40, "base::global::CRntVector<CCutSceneDef::CTakeDef>", fn_7100c87684);
    fn_710009ec00(&g_unk_7172793cb0, &g_unk_7172793b40);
    fn_710009ec00(&g_unk_7172793cb1, fn_7100c8787c());
}

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

void fn_7100d6ff80() {
    InitHeader(g_unk_71727b4f88);
    fn_7100d69690();
    fn_710009ec00(&g_unk_71727b3dd8, TUType<0x20>(g_unk_71727b4f88));
    fn_710009ec00(&g_unk_71727b3dd9, fn_71009155a8());
    fn_710009ec00(&g_unk_71727b3dda, fn_7100d6a05c());
    fn_710009ec00(&g_unk_71727b3ddb, fn_7100913068());
    InitConverter(g_unk_71727b3de0, fn_71009155a8, fn_7100913068, fn_7100d6e22c);
    fn_710009ec00(&g_unk_71727b3e00, fn_7100d6a138());
    fn_710009ec00(&g_unk_71727b3e01, fn_7100d6a214());
    InitClass(&g_unk_71727b3e08,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "game::logic::collision::CShape*>>::TKeyElementIterator::value_type",
              fn_7100d6a674, fn_7100d6a2f0, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727b3f40, &g_unk_71727b3e08);
    fn_710009ec00(&g_unk_71727b3f41, fn_7100d6a7f8());
    InitClass(&g_unk_71727b3f48,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "game::logic::collision::CShape*>>::TKeyElementConstIterator::value_type",
              fn_7100d6ac58, fn_7100d6a8d4, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727b4080, &g_unk_71727b3f48);
    fn_710009ec00(&g_unk_71727b4081, fn_7100d6addc());
    InitVector(&g_unk_71727b4088, "base::global::CRntDictionary<base::global::CStrId, game::logic::collision::CShape*>",
               fn_7100d6aeb8);
    fn_710009ec00(&g_unk_71727b41f8, &g_unk_71727b4088);
    fn_710009ec00(&g_unk_71727b41f9, fn_7100d6b088());
    fn_710009ec00(&g_unk_71727b41fa, fn_7100d6b164());
    InitClass(&g_unk_71727b4200,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "game::logic::collision::CShape const*>>::TKeyElementIterator::value_type",
              fn_7100d6b5c4, fn_7100d6b240, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727b4338, &g_unk_71727b4200);
    fn_710009ec00(&g_unk_71727b4339, fn_7100d6b748());
    InitClass(&g_unk_71727b4340,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "game::logic::collision::CShape const*>>::TKeyElementConstIterator::value_type",
              fn_7100d6bba8, fn_7100d6b824, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727b4478, &g_unk_71727b4340);
    fn_710009ec00(&g_unk_71727b4479, fn_7100d6bd2c());
    InitVector(&g_unk_71727b4480,
               "base::global::CRntDictionary<base::global::CStrId, game::logic::collision::CShape const*>",
               fn_7100d6be08);
    fn_710009ec00(&g_unk_71727b45f0, &g_unk_71727b4480);
}

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

void fn_7100da1ce0() {
    InitHeader(g_unk_71727bb618);
    fn_7100d9cf20();
    fn_710009ec00(&g_unk_71727ba998, TUType<0x20>(g_unk_71727bb618));
    fn_710009ec00(&g_unk_71727ba999, fn_7100d9d978());
    fn_710009ec00(&g_unk_71727ba99a, fn_7100d9da54());
    fn_710009ec00(&g_unk_71727ba99b, fn_7100d9db30());
    fn_710009ec00(&g_unk_71727ba99c, fn_7100d9dc0c());
    InitClass(
        &g_unk_71727ba9a0,
        "base::reflection::CollectionTypeMapper<base::global::CRntVector<CLightInfo>>::TKeyElementIterator::value_type",
        fn_7100d9e06c, fn_7100d9dce8, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727baad8, &g_unk_71727ba9a0);
    fn_710009ec00(&g_unk_71727baad9, fn_7100d9e1f0());
    InitClass(&g_unk_71727baae0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<CLightInfo>>::TKeyElementConstIterator::"
              "value_type",
              fn_7100d9e650, fn_7100d9e2cc, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727bac18, &g_unk_71727baae0);
    fn_710009ec00(&g_unk_71727bac19, fn_7100d9e7d4());
    InitVector(&g_unk_71727bac20, "base::global::CRntVector<CLightInfo>", fn_7100d9e8b0);
    fn_710009ec00(&g_unk_71727bad90, &g_unk_71727bac20);
    fn_710009ec00(&g_unk_71727bad91, fn_7100d9eaa4());
    fn_710009ec00(&g_unk_71727bad92, fn_7100d9eb80());
}

extern UnkTUHeader g_unk_71727bc9d0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bc870;
void fn_7100daf1cc();
void fn_7100daf558(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bc988;
void fn_7100db2038() __attribute__((visibility("hidden")));
void fn_7100db2044() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bc990;
extern UnkConverter g_unk_71727bc9b0;

void fn_7100db21d0() {
    InitHeader(g_unk_71727bc9d0);
    new (TUAt<CStrId, 0x18>(g_unk_71727bc9d0)) CStrId("ITEM_WEAPON_MISSILE_MAX", true);
    fn_7100000250(fn_710000051c, TUAt<CStrId, 0x18>(g_unk_71727bc9d0), &g_unk_7101a19000);
    new (TUAt<CStrId, 0x20>(g_unk_71727bc9d0)) CStrId("ITEM_WEAPON_MISSILE_CURRENT", true);
    fn_7100000250(fn_710000051c, TUAt<CStrId, 0x20>(g_unk_71727bc9d0), &g_unk_7101a19000);
    InitEnum(&g_unk_71727bc870, "EAmiibo", fn_7100daf558, fn_7100daf1cc);
    fn_710009ec00(&g_unk_71727bc988, &g_unk_71727bc870);
    InitEnumConverters(&g_unk_71727bc870, g_unk_71727bc990, fn_7100db2038, g_unk_71727bc9b0, fn_7100db2044);
}

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

void fn_7100e0ecb0() {
    InitHeaderFns(g_unk_71727c0520);
    fn_7100e07bb8();
    fn_710009ec00(&g_unk_71727bf9b0, TUType<0xb0>(g_unk_71727c0520));
    fn_710009ec00(&g_unk_71727bf9b1, fn_7100e085b0());
    fn_710009ec00(&g_unk_71727bf9b2, fn_7100e0868c());
    InitClass(&g_unk_71727bf9b8,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CFilePathStrId, "
              "CLightsDef>>::TKeyElementIterator::value_type",
              fn_7100e08aec, fn_7100e08768, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727bfaf0, &g_unk_71727bf9b8);
    fn_710009ec00(&g_unk_71727bfaf1, fn_7100e08c70());
    InitClass(&g_unk_71727bfaf8,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CFilePathStrId, "
              "CLightsDef>>::TKeyElementConstIterator::value_type",
              fn_7100e090d0, fn_7100e08d4c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727bfc30, &g_unk_71727bfaf8);
    fn_710009ec00(&g_unk_71727bfc31, fn_7100e09254());
    InitVector(&g_unk_71727bfc38, "base::global::CRntDictionary<base::global::CFilePathStrId, CLightsDef>",
               fn_7100e09330);
    fn_710009ec00(&g_unk_71727bfda8, &g_unk_71727bfc38);
    fn_710009ec00(&g_unk_71727bfda9, AsUnk(fn_7100ad6568()));
    fn_710009ec00(&g_unk_71727bfdaa, fn_7100e09500());
    fn_710009ec00(&g_unk_71727bfdab, fn_7100e095dc());
    fn_710009ec00(&g_unk_71727bfdac, fn_7100e096b8());
}

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

void fn_7100e7c980() {
    InitHeader(g_unk_71727c9948);
    {
        char chars[7];
        chars[0] = ' ';
        chars[1] = '-';
        chars[2] = '=';
        chars[3] = '+';
        chars[4] = 'o';
        chars[5] = 'O';
        chars[6] = '@';
        fn_7100e768b0(&g_unk_71727c8f58, chars, sizeof(chars), 10, nullptr, 15);
    }
    fn_7100000250(fn_7100e76a5c, &g_unk_71727c8f58, &g_unk_7101a19000);
    fn_710009ec00(&g_unk_71727c8f80, fn_7100e76a98());
    fn_710009ec00(&g_unk_71727c8f81, fn_7100e76b74());
    InitClass(&g_unk_71727c8f88, "base::reflection::CollectionTypeMapper<minimapGrid::TMinimapVisMap>::TKeyElementIterator::value_type", fn_7100e76fd4, fn_7100e76c50, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727c90c0, &g_unk_71727c8f88);
    fn_710009ec00(&g_unk_71727c90c1, fn_7100e77158());
    InitClass(&g_unk_71727c90c8, "base::reflection::CollectionTypeMapper<minimapGrid::TMinimapVisMap>::TKeyElementConstIterator::value_type", fn_7100e775b8, fn_7100e77234, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71727c9200, &g_unk_71727c90c8);
    fn_710009ec00(&g_unk_71727c9201, fn_7100e7773c());
    InitVector(&g_unk_71727c9208, "minimapGrid::TMinimapVisMap", fn_7100e77818);
    fn_710009ec00(&g_unk_71727c9378, &g_unk_71727c9208);
    InitClass(&g_unk_71727c9380, "CMinimapGrid::SGridDef", fn_7100e77d68, fn_7100e77a0c);
    fn_710009ec00(&g_unk_71727c94b8, &g_unk_71727c9380);
    InitPropertyExVtableFirst(&g_unk_71727c94c0, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, fn_71002ef5bc,
                   "MINIMAP_VISIBILITY", &g_unk_71727c9208,
                   reinterpret_cast<void (*)(UnkPropertyEx<>*)>(fn_71004fd2cc), fn_71004fa3b8);
}



extern UnkTUHeader g_unk_7172755b78 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172755b00;
extern UnkProperty g_unk_7172755b08;
extern void* g_unk_7101a97370[];
void fn_7100acacf8(UnkProperty* p);
void fn_7100ac8db0(void* p);
extern CStrId g_unk_7172755b90 __attribute__((visibility("hidden")));
extern char g_unk_7172755b30;

void fn_7100acb290() {
    InitHeader(g_unk_7172755b78);
    new (&g_unk_7172755b00) CStrId("ACTIVABLE", true);
    fn_7100000250(fn_710000051c, &g_unk_7172755b00, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_7172755b08, g_unk_7101a97370, g_unk_71726db788, "Opened", refl_TypeOf<fn_710000d228>(), fn_7100acacf8,
                 fn_7100ac8db0);
    new (TUAt<CStrId, 0x0>(g_unk_7172755b90)) CStrId("collision_close", true);
    fn_7100000250(fn_710000051c, TUAt<CStrId, 0x0>(g_unk_7172755b90), &g_unk_7101a19000);
    new (TUAt<CStrId, 0x8>(g_unk_7172755b90)) CStrId("player_detector", true);
    fn_7100000250(fn_710000051c, TUAt<CStrId, 0x8>(g_unk_7172755b90), &g_unk_7101a19000);
    new (TUAt<CStrId, 0x10>(g_unk_7172755b90)) CStrId("closed", true);
    fn_7100000250(fn_710000051c, TUAt<CStrId, 0x10>(g_unk_7172755b90), &g_unk_7101a19000);
    new (TUAt<CStrId, 0x18>(g_unk_7172755b90)) CStrId("opened", true);
    fn_7100000250(fn_710000051c, TUAt<CStrId, 0x18>(g_unk_7172755b90), &g_unk_7101a19000);
    fn_7100ac8e2c();
    fn_710009ec00(&g_unk_7172755b30, TUType<0x28>(g_unk_7172755b90));
}

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

void fn_7100ca6150() {
    InitHeader(g_unk_7172797460);
    new (&g_unk_7172796388) CStrId("BLEAKABLE", true);
    fn_7100000250(fn_710000051c, &g_unk_7172796388, &g_unk_7101a19000);
    InitPropertyExVtableFirst(&g_unk_7172796390, g_unk_7101a889c8, g_unk_71726db788, g_unk_7101aa52d0, fn_7100ae9cc8,
                   "PB_AND_SM", refl_TypeOf<fn_7100005178>(), fn_7100ca9504, fn_7100ca3be4);
    InitPropertyExVtableFirst(&g_unk_7172796518, g_unk_7101a889c8, g_unk_71726db788, g_unk_7101aa52d0, fn_7100ae9cc8,
                   "VIGNETTE", refl_TypeOf<fn_7100005178>(), fn_7100ca9504, fn_7100ca3be4);
    InitClass(&g_unk_71727966a0, "CBreakableTileManager", fn_7100ca3e50, fn_7100ca3c84);
    fn_710009ec00(&g_unk_71727967d8, &g_unk_71727966a0);
    fn_710009ec00(&g_unk_71727967d9, fn_7100ca3fb0());
    InitClass(&g_unk_71727967e0, "CBreakableTileManager::STileGroupGroup", fn_7100ca4090, fn_7100ca408c);
    fn_710009ec00(&g_unk_7172796918, &g_unk_71727967e0);
    fn_710009ec00(&g_unk_7172796919, fn_7100ca4214());
    fn_7100ca42f4();
    fn_710009ec00(&g_unk_717279691a, &g_unk_7172797480);
    InitTunable(&g_unk_7172796920, fn_7100136ecc, g_unk_7101aa5328, fn_7100075104);
    fn_710009ec00(&g_unk_7172796971, fn_7100ca4f04());
    fn_710009ec00(&g_unk_7172796972, fn_7100ca4fe0());
    InitClass(&g_unk_7172796978, "base::reflection::CollectionTypeMapper<base::global::CRntPooledDictionary<base::global::CStrId, CBreakableTileManager::STileGroupGroup>>::TKeyElementIterator::value_type", fn_7100ca5440, fn_7100ca50bc, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172796ab0, &g_unk_7172796978);
    fn_710009ec00(&g_unk_7172796ab1, fn_7100ca55c4());
    InitClass(&g_unk_7172796ab8, "base::reflection::CollectionTypeMapper<base::global::CRntPooledDictionary<base::global::CStrId, CBreakableTileManager::STileGroupGroup>>::TKeyElementConstIterator::value_type", fn_7100ca5a24, fn_7100ca56a0, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172796bf0, &g_unk_7172796ab8);
    fn_710009ec00(&g_unk_7172796bf1, fn_7100ca5ba8());
    InitVector(&g_unk_7172796bf8, "base::global::CRntPooledDictionary<base::global::CStrId, CBreakableTileManager::STileGroupGroup>", fn_7100ca5c84);
    fn_710009ec00(&g_unk_7172796d68, &g_unk_7172796bf8);
    new (&g_unk_7172796d70[0]) CFilePathStrId("maps/blocks/breakable_block_undefined/breakable_block_undefined.bcmdl", true);
    new (&g_unk_7172796d70[1]) CFilePathStrId("maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam.bcmdl", true);
    new (&g_unk_7172796d70[2]) CFilePathStrId("maps/blocks/breakable_block_bomb/breakable_block_bomb.bcmdl", true);
    new (&g_unk_7172796d70[3]) CFilePathStrId("maps/blocks/breakable_block_missile/breakable_block_missile.bcmdl", true);
    new (&g_unk_7172796d70[4]) CFilePathStrId("maps/blocks/breakable_block_supermissile/breakable_block_supermissile.bcmdl", true);
    new (&g_unk_7172796d70[5]) CFilePathStrId("maps/blocks/breakable_block_powerbomb/breakable_block_powerbomb.bcmdl", true);
    new (&g_unk_7172796d70[6]) CFilePathStrId("maps/blocks/breakable_block_screwattack/breakable_block_screwattack.bcmdl", true);
    new (&g_unk_7172796d70[7]) CFilePathStrId("maps/blocks/breakable_block_weight/breakable_block_weight.bcmdl", true);
    new (&g_unk_7172796d70[8]) CFilePathStrId("maps/blocks/breakable_block_baby/breakable_block_baby.bcmdl", true);
    new (&g_unk_7172796d70[9]) CFilePathStrId("maps/blocks/breakable_block_speedbuster/breakable_block_speedbuster.bcmdl", true);
    fn_7100000250(fn_7100ca5e60, nullptr, &g_unk_7101a19000);
}

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

void fn_7100e16af0() {
    InitHeader(g_unk_71727c0a38);
    new (&g_unk_71727c0918) CStrId("ARENA", true);
    fn_7100000250(fn_710000051c, &g_unk_71727c0918, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_71727c0920, g_unk_7101aa9c38, g_unk_71726db788, "PrevSG", refl_TypeOf<fn_7100009658>(), fn_7100e165e8,
                 fn_7100e140c0);
    InitPropertyVtableFirst(&g_unk_71727c0948, g_unk_7101aa9c38, g_unk_71726db788, "SG", refl_TypeOf<fn_7100009658>(), fn_7100e165e8,
                 fn_7100e140c0);
    InitPropertyVtableFirst(&g_unk_71727c0970, g_unk_7101aa9c38, g_unk_71726db788, "AllDead", refl_TypeOf<fn_710000d228>(), fn_7100e165e8,
                 fn_7100e140c0);
    InitPropertyVtableFirst(&g_unk_71727c0998, g_unk_7101aa9c38, g_unk_71726db788, "UseIntro", refl_TypeOf<fn_710000d228>(), fn_7100e165e8,
                 fn_7100e140c0);
    fn_7100e1413c();
    fn_710009ec00(&g_unk_71727c09c0, TUType<0x30>(g_unk_71727c0a58));
    InitTunable(&g_unk_71727c09c8, fn_7100136ecc, g_unk_7101aa9c90, fn_7100075104);
    new (&g_unk_71727c0a58) CRntString("Intro");
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_710007ecd4), &g_unk_71727c0a58, &g_unk_7101a19000);
}


extern UnkTUHeader g_unk_7172755cf8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172755d10 __attribute__((visibility("hidden")));
extern char g_unk_7172755cf0;

// The id lives in an inline function: its guard and object are reached through the GOT.
inline const CStrId& WorldGraphId() {
    static CStrId s_id("WORLDGRAPH", true);
    return s_id;
}

void fn_7100acd5d0() {
    InitHeader(g_unk_7172755cf8);
    new (&g_unk_7172755d10) CStrId(fn_7100de8f78(WorldGraphId()));
    fn_7100000250(fn_710000051c, &g_unk_7172755d10, &g_unk_7101a19000);
    fn_7100acb4ac();
    fn_710009ec00(&g_unk_7172755cf0, TUType<0x10>(g_unk_7172755d10));
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

void fn_7100ad39e0() {
    InitHeader(g_unk_7172756998);
    fn_710009ec00(&g_unk_7172756940, fn_710025d654());
    InitConverter(g_unk_7172756948, fn_710025d654, refl_TypeOf<fn_7100009658>, fn_7100ad37c0);
    new (&g_unk_7172756968) UnkValue(CStrId(g_unk_710164b8ab, true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172756968, &g_unk_7101a19000);
    new (&g_unk_7172756978) UnkValue(CStrId(g_unk_710164b8ad, true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172756978, &g_unk_7101a19000);
    new (&g_unk_7172756988) UnkValue(CStrId(g_unk_710164b8af, true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172756988, &g_unk_7101a19000);
}

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

void fn_7100da7050() {
    InitHeader(g_unk_71727bc598);
    InitEnum(&g_unk_71727bc290, "CAIManager::EAIGroup", fn_7100da6938, fn_7100da684c);
    fn_710009ec00(&g_unk_71727bc3a8, &g_unk_71727bc290);
    InitEnumConverters(&g_unk_71727bc290, g_unk_71727bc3b0, fn_7100dab9bc, g_unk_71727bc3d0, fn_7100dab9c8);
    InitEnum(&g_unk_71727bc3f0, "CAIManager::EBumpMode", fn_7100da6cc4, fn_7100da6ab8);
    fn_710009ec00(&g_unk_71727bc508, &g_unk_71727bc3f0);
    InitEnumConverters(&g_unk_71727bc3f0, g_unk_71727bc510, fn_7100dab9d4, g_unk_71727bc530, fn_7100dab9e0);
    new (TUAt<CFilePathStrId, 0x30>(g_unk_71727bc5b0)) CFilePathStrId("actors/props/barelyice/models/barelyice00_weak.bcmdl", true);
    new (TUAt<CFilePathStrId, 0x38>(g_unk_71727bc5b0)) CFilePathStrId("actors/props/barelyice/models/barelyice01_weak.bcmdl", true);
    new (TUAt<CFilePathStrId, 0x40>(g_unk_71727bc5b0)) CFilePathStrId("actors/props/barelyice/models/barelyice02_weak.bcmdl", true);
    fn_7100000250(fn_7100da6e50, nullptr, &g_unk_7101a19000);
    new (TUAt<CFilePathStrId, 0x48>(g_unk_71727bc5b0)) CFilePathStrId("actors/props/barelyice/models/barelyice00_strong.bcmdl", true);
    new (TUAt<CFilePathStrId, 0x50>(g_unk_71727bc5b0)) CFilePathStrId("actors/props/barelyice/models/barelyice01_strong.bcmdl", true);
    new (TUAt<CFilePathStrId, 0x58>(g_unk_71727bc5b0)) CFilePathStrId("actors/props/barelyice/models/barelyice02_strong.bcmdl", true);
    fn_7100000250(fn_7100da6f50, nullptr, &g_unk_7101a19000);
    new (TUAt<CStrId, 0x0>(g_unk_71727bc5b0)) CStrId("ITEM_OPTIC_CAMOUFLAGE", true);
    fn_7100000250(fn_710000051c, TUAt<CStrId, 0x0>(g_unk_71727bc5b0), &g_unk_7101a19000);
    fn_7100da7408();
    fn_710009ec00(&g_unk_71727bc550, TUType<0xb8>(g_unk_71727bc5b0));
    InitTunable(&g_unk_71727bc558, fn_7100136ecc, g_unk_7101aa9278, fn_7100075104);
}


extern UnkTUHeader g_unk_7172791e70 __attribute__((visibility("hidden")));

struct UnkElem188 {
    virtual void vf_0x0();
    virtual ~UnkElem188();
    UnkElem188(UnkElem188&& other) asm("fn_7100d36688");  // fn_7100d36688
    u8 unk_0x8[0x180];
};
extern base::UnkVector<UnkElem188> g_unk_7172791e48;
void fn_7100c66ab0(void* vector);
extern CStrId g_unk_7172791e88 __attribute__((visibility("hidden")));

void fn_7100c77cd0() {
    InitHeader(g_unk_7172791e70);
    new (&g_unk_7172791e48) base::UnkVector<UnkElem188>(0xf);
    g_unk_7172791e48.Reserve(50);
    fn_7100000250(fn_7100c66ab0, &g_unk_7172791e48, &g_unk_7101a19000);
    new (&g_unk_7172791e88) CStrId("ConeCollisionScene", true);
    fn_7100000250(fn_710000051c, &g_unk_7172791e88, &g_unk_7101a19000);
}

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

void fn_7100da54b0() {
    InitHeader(g_unk_71727bc0f8);
    InitClass(AsUnk(&g_unk_71727bbe68), "CBaseGameLink", fn_7100da4a08, fn_7100da4720);
    fn_710009ec00(&g_unk_71727bbfa0, AsUnk(&g_unk_71727bbe68));
    fn_710009ec00(&g_unk_71727bbfa1, fn_7100da4cf0());
    InitConverterTo(g_unk_71727bbfa8, AsUnk(&g_unk_71727bbe68), refl_TypeOf<fn_7100009658>, fn_7100da5184);
    new (&g_unk_71727bbfc8) CStrId("{EMPTY}", true);
    fn_7100000250(fn_710000051c, &g_unk_71727bbfc8, &g_unk_7101a19000);
    new (&g_unk_71727bbfd0) CStrId("{BROKEN}", true);
    fn_7100000250(fn_710000051c, &g_unk_71727bbfd0, &g_unk_7101a19000);
    g_unk_71727bbfd8.field_0x0 = 0;
    g_unk_71727bbfd8.field_0x8 = 0;
}


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

void fn_7100ebefd0() {
    InitHeader(g_unk_71729ce040);
    InitClass(&g_unk_71729cdd78, "minimap::CAnimatedValue::SAnimConfig", fn_7100eb6b48, fn_7100eb695c);
    fn_710009ec00(&g_unk_71729cdeb0, &g_unk_71729cdd78);
    InitEnum(&g_unk_71729cdeb8, "minimap::EIconAnimID", fn_7100eb71d8, fn_7100eb6ccc);
    fn_710009ec00(&g_unk_71729cdfd0, &g_unk_71729cdeb8);
    InitEnumConverters(&g_unk_71729cdeb8, g_unk_71729cdfd8, fn_7100ebecec, g_unk_71729cdff8, fn_7100ebecf8);
    new (TUAt<CStrId, 0x0>(g_unk_71729ce058)) CStrId("digital_medium", true);
    fn_7100000250(fn_710000051c, TUAt<CStrId, 0x0>(g_unk_71729ce058), &g_unk_7101a19000);
    g_unk_71729ce018.field_0x0 = 0;
    g_unk_71729ce018.field_0x8 = 0;
    g_unk_71729ce018.field_0x10 = 0;
    g_unk_71729ce018.field_0x18 = 10;
    g_unk_71729ce018.field_0x1c = 0xe;
    g_unk_71729ce018.allocator = fn_7100083990();
    fn_7100000250(fn_7100eb7590, &g_unk_71729ce018, &g_unk_7101a19000);
}
