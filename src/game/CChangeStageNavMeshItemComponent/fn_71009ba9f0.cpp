// Copies of fn_7100acd5d0 (src/base/reflection/fn_7100adcf10.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71009ba9f0.
// More per-TU static initialisers (helpers in game/UnkTUInit.h; see also fn_7100b04900.cpp):
// some TUs' data has function records after the header, and collection and vector types are
// registered for the reflected containers.
#include "base/UnkStrIdRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CArenaManager/CTunableArenaManager.h"
#include "game/CBTObserverComponent.h"
#include "game/CBreakableTileManager/CTunableBreakableTileManager.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/CGameBlackboard.h"
#include "game/CLightManager.h"
#include "game/CLightsDef.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CWeightActivablePropComponent.h"
#include "game/GUI/CDisplayObjectDef.h"
#include "game/UnkTU1629.h"
#include "game/UnkTU1632.h"
#include "game/UnkTU1670.h"
#include "game/UnkTU1722.h"
#include "game/UnkTU2177.h"
#include "game/UnkTU2200.h"
#include "game/UnkTU2535.h"
#include "game/UnkTU2541.h"
#include "game/UnkTU2565.h"
#include "game/UnkTU2567.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/logic/collision/CShape.h"

using base::global::CFilePathStrId;
using base::global::CRntString;

UnkReflType* fn_7100adc1d8();  // base::reflection::refl_GetType<gameeditor::TPropertyId>
UnkReflType* fn_7100adc2b4();
UnkReflType* fn_7100adc390();

extern UnkTUHeaderFns g_unk_71727585d8 __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172758258;
extern UnkConverter g_unk_7172758278;
extern UnkValue g_unk_7172758298;

extern UnkTUHeader g_unk_71727568f8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172755e70;
void fn_7100ace28c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100ace884();
UnkReflType* fn_7100ace960();
extern UnkReflType g_unk_7172755fb0;
void fn_7100acedc0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100acef44();
extern UnkReflType g_unk_71727560f0;
void fn_7100acf3a4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100acf528();
extern UnkReflType g_unk_7172756230;
void fn_7100acf604(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_7100acf7f8();

extern UnkTUHeader g_unk_717275bf40 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717275b580;
void fn_7100ae96e8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkConverter g_unk_717275b6c0;
extern UnkConverter g_unk_717275b6e0;
extern UnkReflType g_unk_717275bf60 __attribute__((visibility("hidden")));
UnkReflType* fn_7100aea26c();
UnkReflType* fn_7100aea348();
extern UnkReflType g_unk_717275b708;
void fn_7100aea7a8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100aea92c();
extern UnkReflType g_unk_717275b848;
void fn_7100aead8c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100aeaf10();
extern UnkReflType g_unk_717275b988;
void fn_7100aeafec(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkTUHeaderFnsClass g_unk_717275ed68 __attribute__((visibility("hidden")));
UnkReflType* fn_7100b05394();  // base::reflection::refl_GetType<GUI::CDisplayObjectDef*>
UnkReflType* fn_7100b05470();
UnkReflType* fn_7100b0554c();
UnkReflType* fn_7100b05628();
extern UnkReflType g_unk_717275e200;
void fn_7100b05a88(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b05c0c();
extern UnkReflType g_unk_717275e340;
void fn_7100b0606c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b061f0();
extern UnkReflType g_unk_717275e480;
void fn_7100b062cc(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkTUHeader g_unk_717275f8c8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717275ef50;
void fn_7100b0a6cc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b08378();  // base::reflection::refl_GetType<GUI::CDisplayObjectStateDef*>
UnkReflType* fn_7100b0a9a8();
UnkReflType* fn_7100b0aa84();
extern UnkReflType g_unk_717275f090;
void fn_7100b0aee4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b0b068();
extern UnkReflType g_unk_717275f1d0;
void fn_7100b0b4c8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b0b64c();
extern UnkReflType g_unk_717275f310;
void fn_7100b0b728(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkTUHeader g_unk_7172766bb8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172766130;
void fn_7100b7612c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b762b0();  // base::reflection::refl_GetType<GUI::CSkin*>
UnkReflType* fn_7100b7638c();
UnkReflType* fn_7100b76468();
extern UnkReflType g_unk_7172766270;
void fn_7100b768c8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b76a4c();
extern UnkReflType g_unk_71727663b0;
void fn_7100b76eac(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b77030();
extern UnkReflType g_unk_71727664f0;
void fn_7100b7710c(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkTUHeader g_unk_7172767b98 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172767000;
void fn_7100b7d580(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b7d880();  // base::reflection::refl_GetType<GUI::CSpriteSheet*>
UnkReflType* fn_7100b7d95c();
UnkReflType* fn_7100b7da38();
extern UnkReflType g_unk_7172767140;
void fn_7100b7de98(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b7e01c();
extern UnkReflType g_unk_7172767280;
void fn_7100b7e47c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b7e600();
extern UnkReflType g_unk_71727673c0;
void fn_7100b7e6dc(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkTUHeader g_unk_7172768668 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172767bb0;
void fn_7100b8172c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkReflType g_unk_7172767cf0;
void fn_7100b819bc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b80034();  // base::reflection::refl_GetType<GUI::CSpriteSheetItem*>
UnkReflType* fn_7100b81b40();
UnkReflType* fn_7100b81c1c();
extern UnkReflType g_unk_7172767e30;
void fn_7100b8207c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b82200();
extern UnkReflType g_unk_7172767f70;
void fn_7100b82660(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100b827e4();
extern UnkReflType g_unk_71727680b0;
void fn_7100b828c0(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkTUHeader g_unk_7172794a98 __attribute__((visibility("hidden")));
UnkReflType* fn_7100c837d4();
UnkReflType* fn_7100c838b0();
extern UnkReflType g_unk_71727930d0;
void fn_7100c83d1c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100c83ea0();
extern UnkReflType g_unk_7172793210;
void fn_7100c8430c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100c84490();
extern UnkReflType g_unk_7172793350;
void fn_7100c8456c(UnkReflType* self, const CStrId& name, void* a, void* b);
extern UnkReflType g_unk_7172794d40 __attribute__((visibility("hidden")));
UnkReflType* fn_7100c84da4();
UnkReflType* fn_7100c84e80();
extern UnkReflType g_unk_71727934c8;
void fn_7100c852ec(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100c85470();
extern UnkReflType g_unk_7172793608;
void fn_7100c858dc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100c85a60();
extern UnkReflType g_unk_7172793748;
void fn_7100c85b3c(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_7100c868ec();
UnkReflType* fn_7100c869c8();
extern UnkReflType g_unk_71727938c0;
void fn_7100c86e34(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100c86fb8();
extern UnkReflType g_unk_7172793a00;
void fn_7100c87424(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100c875a8();
extern UnkReflType g_unk_7172793b40;
void fn_7100c87684(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_7100c8787c();

extern UnkTUHeader g_unk_71727b4f88 __attribute__((visibility("hidden")));
UnkReflType* fn_71009155a8();  // base::reflection::refl_GetType<game::logic::collision::CShape*>
UnkReflType* fn_7100d6a05c();
UnkReflType* fn_7100913068();  // base::reflection::refl_GetType<std::unique_ptr<game::logic::collision::CShape>>
extern UnkConverter g_unk_71727b3de0;
UnkReflType* fn_7100d6a138();
UnkReflType* fn_7100d6a214();
extern UnkReflType g_unk_71727b3e08;
void fn_7100d6a674(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100d6a7f8();
extern UnkReflType g_unk_71727b3f48;
void fn_7100d6ac58(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100d6addc();
extern UnkReflType g_unk_71727b4088;
void fn_7100d6aeb8(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_7100d6b088();
UnkReflType* fn_7100d6b164();
extern UnkReflType g_unk_71727b4200;
void fn_7100d6b5c4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100d6b748();
extern UnkReflType g_unk_71727b4340;
void fn_7100d6bba8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100d6bd2c();
extern UnkReflType g_unk_71727b4480;
void fn_7100d6be08(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkTUHeader g_unk_71727bb618 __attribute__((visibility("hidden")));
UnkReflType* fn_7100d9d978();
UnkReflType* fn_7100d9da54();
UnkReflType* fn_7100d9db30();
UnkReflType* fn_7100d9dc0c();
extern UnkReflType g_unk_71727ba9a0;
void fn_7100d9e06c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100d9e1f0();
extern UnkReflType g_unk_71727baae0;
void fn_7100d9e650(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100d9e7d4();
extern UnkReflType g_unk_71727bac20;
void fn_7100d9e8b0(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_7100d9eaa4();
UnkReflType* fn_7100d9eb80();

extern UnkTUHeader g_unk_71727bc9d0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bc870;
void fn_7100daf558(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727bc990;
extern UnkConverter g_unk_71727bc9b0;

extern UnkTUHeaderFnsClass g_unk_71727c0520 __attribute__((visibility("hidden")));
UnkReflType* fn_7100e085b0();
UnkReflType* fn_7100e0868c();
extern UnkReflType g_unk_71727bf9b8;
void fn_7100e08aec(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100e08c70();
extern UnkReflType g_unk_71727bfaf8;
void fn_7100e090d0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100e09254();
extern UnkReflType g_unk_71727bfc38;
void fn_7100e09330(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_7100e09500();
UnkReflType* fn_7100e095dc();
UnkReflType* fn_7100e096b8();

extern UnkTUHeader g_unk_71727c9948 __attribute__((visibility("hidden")));
struct UnkCharMap;
UnkReflType* fn_7100e76a98();
UnkReflType* fn_7100e76b74();
extern UnkReflType g_unk_71727c8f88;
void fn_7100e76fd4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100e77158();
extern UnkReflType g_unk_71727c90c8;
void fn_7100e775b8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100e7773c();
extern UnkReflType g_unk_71727c9208;
void fn_7100e77818(UnkReflType* self, const CStrId& name, void* a, void* b);
extern UnkReflType g_unk_71727c9380;
void fn_7100e77d68(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkPropertyEx<> g_unk_71727c94c0;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101a45f90[];

extern UnkTUHeader g_unk_7172755b78 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172755b00;
extern UnkProperty g_unk_7172755b08;
void fn_7100acacf8(UnkProperty* p);
extern CStrId g_unk_7172755b90 __attribute__((visibility("hidden")));

extern UnkTUHeader g_unk_7172797460 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172796388;
extern UnkReflType g_unk_71727966a0;
void fn_7100ca3e50(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100ca3fb0();
extern UnkReflType g_unk_71727967e0;
void fn_7100ca4090(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100ca4214();
extern UnkReflType g_unk_7172797480 __attribute__((visibility("hidden")));
extern UnkTunable g_unk_7172796920;
UnkReflType* fn_7100ca4f04();
UnkReflType* fn_7100ca4fe0();
extern UnkReflType g_unk_7172796978;
void fn_7100ca5440(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100ca55c4();
extern UnkReflType g_unk_7172796ab8;
void fn_7100ca5a24(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100ca5ba8();
extern UnkReflType g_unk_7172796bf8;
void fn_7100ca5c84(UnkReflType* self, const CStrId& name, void* a, void* b);
extern UnkPropertyEx<0x160> g_unk_7172796390;
extern UnkPropertyEx<0x160> g_unk_7172796518;
void fn_7100ca9504(UnkPropertyEx<0x160>* p);
extern CFilePathStrId g_unk_7172796d70[10];

extern UnkTUHeader g_unk_71727c0a38 __attribute__((visibility("hidden")));
extern CStrId g_unk_71727c0918;
extern UnkProperty g_unk_71727c0920;
extern UnkProperty g_unk_71727c0948;
extern UnkProperty g_unk_71727c0970;
extern UnkProperty g_unk_71727c0998;
void fn_7100e165e8(UnkProperty* p);
extern CRntString g_unk_71727c0a58 __attribute__((visibility("hidden")));
extern UnkTunable g_unk_71727c09c8;

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
extern UnkConverter g_unk_7172756948;
extern UnkValue g_unk_7172756968;
extern UnkValue g_unk_7172756978;
extern UnkValue g_unk_7172756988;
extern const char g_unk_710164b8ab[];
extern const char g_unk_710164b8ad[];
extern const char g_unk_710164b8af[];

extern UnkTUHeader g_unk_71727bc598 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bc290;
void fn_7100da6938(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727bc3b0;
extern UnkConverter g_unk_71727bc3d0;
extern UnkReflType g_unk_71727bc3f0;
void fn_7100da6cc4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727bc510;
extern UnkConverter g_unk_71727bc530;
extern UnkTUData g_unk_71727bc5b0 __attribute__((visibility("hidden")));
extern UnkTunable g_unk_71727bc558;
extern void* g_unk_7101aa9278[];

extern UnkTUHeader g_unk_7172791e70 __attribute__((visibility("hidden")));

struct UnkElem188 {
    virtual void vf_0x0();
    virtual ~UnkElem188();
    UnkElem188(UnkElem188&& other);  // fn_7100d36688
    u8 unk_0x8[0x180];
};
extern base::UnkVector<UnkElem188> g_unk_7172791e48;
extern CStrId g_unk_7172791e88 __attribute__((visibility("hidden")));

extern UnkTUHeader g_unk_71727bc0f8 __attribute__((visibility("hidden")));
void fn_7100da4a08(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100da4cf0();
extern UnkConverter g_unk_71727bbfa8;
extern CStrId g_unk_71727bbfc8;
extern CStrId g_unk_71727bbfd0;
struct UnkPair16 {
    u64 field_0x0;
    u64 field_0x8;
};

extern UnkTUHeader g_unk_71729ce040 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71729cdd78;
void fn_7100eb6b48(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkReflType g_unk_71729cdeb8;
void fn_7100eb71d8(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71729cdfd8;
extern UnkConverter g_unk_71729cdff8;
extern UnkTUData g_unk_71729ce058 __attribute__((visibility("hidden")));
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
