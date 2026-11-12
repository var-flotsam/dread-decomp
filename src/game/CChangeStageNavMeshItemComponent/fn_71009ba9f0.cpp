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
#include "game/CLogicShapeComponent.h"
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

extern UnkConverter g_unk_7172758258;
extern UnkConverter g_unk_7172758278;

void fn_7100ace28c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100acedc0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100acf3a4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100acf604(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100ae96e8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkConverter g_unk_717275b6c0;
extern UnkConverter g_unk_717275b6e0;
void fn_7100aea7a8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100aead8c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100aeafec(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100b05a88(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b0606c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b062cc(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkReflType g_unk_717275ef50;
void fn_7100b0a6cc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b0aee4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b0b4c8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b0b728(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100b7612c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b768c8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b76eac(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b7710c(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100b7d580(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b7de98(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b7e47c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b7e6dc(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100b8172c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b819bc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b8207c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b82660(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100b828c0(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100c83d1c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c8430c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c8456c(UnkReflType* self, const CStrId& name, void* a, void* b);
struct CType;
extern const CType g_unk_7172794d40 __attribute__((visibility("hidden")));
void fn_7100c852ec(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c858dc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c85b3c(UnkReflType* self, const CStrId& name, void* a, void* b);
void fn_7100c86e34(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c87424(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100c87684(UnkReflType* self, const CStrId& name, void* a, void* b);

extern UnkConverter g_unk_71727b3de0;
void fn_7100d6a674(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100d6ac58(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100d6aeb8(UnkReflType* self, const CStrId& name, void* a, void* b);
void fn_7100d6b5c4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100d6bba8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100d6be08(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100d9e06c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100d9e650(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100d9e8b0(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100daf558(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727bc990;
extern UnkConverter g_unk_71727bc9b0;

void fn_7100e08aec(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100e090d0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100e09330(UnkReflType* self, const CStrId& name, void* a, void* b);

struct UnkCharMap;
void fn_7100e76fd4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100e775b8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100e77818(UnkReflType* self, const CStrId& name, void* a, void* b);
void fn_7100e77d68(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkPropertyEx<> g_unk_71727c94c0;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101a45f90[];

extern CStrId g_unk_7172755b00;
extern CStrId g_unk_7172755b90 __attribute__((visibility("hidden")));

extern CStrId g_unk_7172796388;
void fn_7100ca3e50(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100ca4090(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkReflType g_unk_7172797480 __attribute__((visibility("hidden")));
extern UnkTunable g_unk_7172796920;
void fn_7100ca5440(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100ca5a24(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100ca5c84(UnkReflType* self, const CStrId& name, void* a, void* b);
extern UnkPropertyEx<0x160> g_unk_7172796390;
extern UnkPropertyEx<0x160> g_unk_7172796518;
void fn_7100ca9504(UnkPropertyEx<0x160>* p);
extern CFilePathStrId g_unk_7172796d70[10];

extern CStrId g_unk_71727c0918;
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

extern UnkConverter g_unk_7172756948;
extern const char g_unk_710164b8ab[];
extern const char g_unk_710164b8ad[];
extern const char g_unk_710164b8af[];

void fn_7100da6938(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727bc3b0;
extern UnkConverter g_unk_71727bc3d0;
void fn_7100da6cc4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71727bc510;
extern UnkConverter g_unk_71727bc530;
extern void* g_unk_7101aa9278[];

struct UnkElem188 {
    virtual void vf_0x0();
    virtual ~UnkElem188();
    UnkElem188(UnkElem188&& other);  // fn_7100d36688
    u8 unk_0x8[0x180];
};
extern base::UnkVector<UnkElem188> g_unk_7172791e48;
extern CStrId g_unk_7172791e88 __attribute__((visibility("hidden")));

void fn_7100da4a08(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern UnkConverter g_unk_71727bbfa8;
extern CStrId g_unk_71727bbfc8;
extern CStrId g_unk_71727bbfd0;
struct UnkPair16 {
    u64 field_0x0;
    u64 field_0x8;
};

void fn_7100eb6b48(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100eb71d8(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern UnkConverter g_unk_71729cdfd8;
extern UnkConverter g_unk_71729cdff8;
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
