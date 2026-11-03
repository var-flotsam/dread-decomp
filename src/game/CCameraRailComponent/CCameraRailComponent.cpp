// Per-TU static initialisers of components whose id lives in an inline function (shape of
// fn_7100acd5d0 in src/base/reflection/fn_7100adcf10.cpp, with the class registered first):
// CCameraRailComponent, CLandmarkComponent, CLogicCameraComponent, CLogicPathComponent and
// CSonarTargetComponent.
#include "base/UnkStrIdRegistry.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

// The header, then the component's id (header +0x20) and its type (header +0x28). The id's
// guard and object are reached through the GOT.
#define COMPONENT_TU_INIT(FN, HDR, ID, LIST, IDFN, NAME, GETCLASS)          \
    extern UnkTUHeader g_unk_##HDR __attribute__((visibility("hidden")));   \
    extern CStrId g_unk_##ID __attribute__((visibility("hidden")));         \
    extern char g_unk_##LIST;                                               \
    inline const CStrId& IDFN() {                                           \
        static CStrId s_id(NAME, true);                                     \
        return s_id;                                                        \
    }                                                                       \
    void FN() {                                                             \
        InitHeader(g_unk_##HDR);                                            \
        GETCLASS();                                                         \
        fn_710009ec00(&g_unk_##LIST, TUType<0x28>(g_unk_##HDR));            \
        new (&g_unk_##ID) CStrId(fn_7100de8f78(IDFN()));                    \
        fn_7100000250(fn_710000051c, &g_unk_##ID, &g_unk_7101a19000);       \
    }

COMPONENT_TU_INIT(fn_7100769fc0, 7172713c70, 7172713c90, 7172713c58, CameraRailId, "CAMERARAIL", fn_7100769854)
COMPONENT_TU_INIT(fn_71008a3540, 717272dea0, 717272dec0, 717272de88, LandmarkId, "LANDMARK", fn_71008a2d70)
COMPONENT_TU_INIT(fn_7100911630, 7172735888, 71727358a8, 7172735660, LogicCameraId, "LOGICCAMERA", fn_7100910320)
COMPONENT_TU_INIT(fn_7100912900, 7172735b70, 7172735b90, 7172735b68, LogicPathId, "LOGICPATH", fn_71009120e4)
COMPONENT_TU_INIT(fn_7100a10980, 7172743a50, 7172743a70, 7172743a48, SonarTargetId, "SONAR_TARGET", fn_7100a10580)
