// Static initialiser of a reflected unit: the header ids, then the unit's own statics and
// registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100d96670 {
    HeaderIds ids;
};
extern UnkStatics_7100d96670 g_unk_71727ba2c8 __attribute__((visibility("hidden")));
extern char g_unk_71727b9030[];
extern char g_unk_71727b90b0[];
extern char g_unk_71727b9130[];
extern char g_unk_71727b91b0[];
extern char g_unk_71727b9230[];
extern char g_unk_71727b92b0[];
extern char g_unk_71727b9330[];
extern char g_unk_71727b93b0[];
extern char g_unk_71727b9430[];
extern char g_unk_71727b94b0[];
extern char g_unk_71727b9530[];
extern char g_unk_71727b95b0[];
extern char g_unk_71727b9630[];
extern char g_unk_71727b96b0[];
extern char g_unk_71727b9730[];
extern char g_unk_71727b97b0[];
extern char g_unk_71727b9830[];
extern char g_unk_71727b98b0[];
extern char g_unk_71727b9930[];
extern char g_unk_71727b99b0[];
extern char g_unk_71727b9a30[];
extern char g_unk_71727b9ab0[];
extern char g_unk_71727b9b30[];
extern char g_unk_71727b9bb0[];
extern char g_unk_71727b9c30[];
extern char g_unk_71727b9cb0[];
extern char g_unk_71727b9d30[];
extern char g_unk_71727b9db0[];
extern char g_unk_71727b9e30[];
extern char g_unk_71727b9eb0[];
extern char g_unk_71727b9f30[];
extern char g_unk_71727b9fb0[];
extern char g_unk_71727ba030[];
extern char g_unk_71727ba0b0[];
extern char g_unk_71727ba130[];
extern char g_unk_71727ba1b0[];
extern char g_unk_71727ba230[];

void fn_7100d96670() {
    InitHeaderIds(&g_unk_71727ba2c8.ids);
    [] {
        UnkResType types[2];
        const char* sources1[2];
        const char* sources0[1];
        sources0[0] = ".rfld";
        fn_7100004578(&types[0], "CScenario", ".brfld", sources0, 1);
        sources1[0] = ".msld";
        sources1[1] = ".mssg";
        fn_7100004578(&types[1], "CScenarioLogicImporter", ".bmsld", sources1, 2);
        fn_7100004698(g_unk_71727b9030, "leveldef", types, 2);
        fn_7100004a44(&types[1]);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9030, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[2];
        sources0[0] = ".mdei";
        sources0[1] = ".rlei";
        fn_7100004578(&types[0], "gameeditor::CEditorGameData", nullptr, sources0, 2);
        fn_7100004698(g_unk_71727b90b0, "mapeditor", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b90b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[2];
        sources0[0] = ".mssd";
        sources0[1] = ".mssl";
        fn_7100004578(&types[0], "engine::scene::CSceneSlot", ".bmssd", sources0, 2);
        fn_7100004698(g_unk_71727b9130, "mapscene", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9130, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".msbk";
        fn_7100004578(&types[0], "CBreakableTileLoader", ".bmsbk", sources0, 1);
        fn_7100004698(g_unk_71727b91b0, "mapbreak", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b91b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".rev";
        fn_7100004578(&types[0], "CEnvironmentVisualPresets", ".brev", sources0, 1);
        fn_7100004698(g_unk_71727b9230, "mapenv", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9230, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".res";
        fn_7100004578(&types[0], "CEnvironmentSoundPresets", ".bres", sources0, 1);
        fn_7100004698(g_unk_71727b92b0, "mapenvsnd", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b92b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".rem";
        fn_7100004578(&types[0], "CEnvironmentMusicPresets", ".brem", sources0, 1);
        fn_7100004698(g_unk_71727b9330, "mapenvmus", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9330, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mscc";
        fn_7100004578(&types[0], "game::logic::collision::CCollisionImporter", ".bmscc", sources0, 1);
        fn_7100004698(g_unk_71727b93b0, "mapcamcoll", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b93b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".rsa";
        fn_7100004578(&types[0], "CSubAreaManager", ".brsa", sources0, 1);
        fn_7100004698(g_unk_71727b9430, "mapsubarea", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9430, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".msnav";
        fn_7100004578(&types[0], "navmesh::CNavMesh", ".bmsnav", sources0, 1);
        fn_7100004698(g_unk_71727b94b0, "navmesh", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b94b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[2];
        sources0[0] = ".msad";
        sources0[1] = ".msaf";
        fn_7100004578(&types[0], "CActorDef", ".bmsad", sources0, 2);
        fn_7100004698(g_unk_71727b9530, "actordef", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9530, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".msact";
        fn_7100004578(&types[0], "CAction", ".bmsact", sources0, 1);
        fn_7100004698(g_unk_71727b95b0, "action", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b95b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[2];
        sources0[0] = ".msas";
        sources0[1] = ".msact";
        fn_7100004578(&types[0], "CActionSet", ".bmsas", sources0, 2);
        fn_7100004698(g_unk_71727b9630, "actionset", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9630, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mbtre";
        fn_7100004578(&types[0], "behaviortree::CBehaviorTree", ".bmtre", sources0, 1);
        fn_7100004698(g_unk_71727b96b0, "behtree", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b96b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mscu";
        fn_7100004578(&types[0], "CCutSceneDef", ".bmscu", sources0, 1);
        fn_7100004698(g_unk_71727b9730, "cutscene", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9730, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".lua";
        fn_7100004578(&types[0], "CLuaLibraryManager", ".lc", sources0, 1);
        fn_7100004698(g_unk_71727b97b0, "script", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b97b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mscd";
        fn_7100004578(&types[0], "game::logic::collision::CCollisionImporter", ".bmscd", sources0, 1);
        fn_7100004698(g_unk_71727b9830, "colldata", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9830, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mslink";
        fn_7100004578(&types[0], "navmesh::CDynamicSmartLinkRules", ".bmslink", sources0, 1);
        fn_7100004698(g_unk_71727b98b0, "slinkrules", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b98b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mslgroup";
        fn_7100004578(&types[0], "navmesh::CDynamicSmartLinkGroup", ".bmslgroup", sources0, 1);
        fn_7100004698(g_unk_71727b9930, "slinkgroup", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9930, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".txt";
        fn_7100004578(&types[0], "CLanguageManager", ".txt", sources0, 1);
        fn_7100004698(g_unk_71727b99b0, "txt", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b99b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mscp";
        fn_7100004578(&types[0], "GUI::CDisplayObjectContainer", ".bmscp", sources0, 1);
        fn_7100004698(g_unk_71727b9a30, "composition", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9a30, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mssk";
        fn_7100004578(&types[0], "GUI::CGUIManager::SkinContainer", ".bmssk", sources0, 1);
        fn_7100004698(g_unk_71727b9ab0, "skin", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9ab0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mssh";
        fn_7100004578(&types[0], "GUI::CGUIManager::ShapeContainer", ".bmssh", sources0, 1);
        fn_7100004698(g_unk_71727b9b30, "shape", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9b30, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".msss";
        fn_7100004578(&types[0], "GUI::CGUIManager::SpriteSheetContainer", ".bmsss", sources0, 1);
        fn_7100004698(g_unk_71727b9bb0, "sprite_sheet", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9bb0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".wav";
        fn_7100004578(&types[0], "base::snd::CSoundSystemATK", nullptr, sources0, 1);
        fn_7100004698(g_unk_71727b9c30, "sound", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9c30, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".rsa";
        fn_7100004578(&types[0], "CSubAreaManager", ".brsa", sources0, 1);
        fn_7100004698(g_unk_71727b9cb0, "subarearef", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9cb0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".apd";
        fn_7100004578(&types[0], "sound::CAudioPresets", ".bapd", sources0, 1);
        fn_7100004698(g_unk_71727b9d30, "audiopreset", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9d30, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".ptdef";
        fn_7100004578(&types[0], "CPlaythroughDef", ".bptdef", sources0, 1);
        fn_7100004698(g_unk_71727b9db0, "playdef", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9db0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".ptdat";
        fn_7100004578(&types[0], "CPlaythrough", ".bptdat", sources0, 1);
        fn_7100004698(g_unk_71727b9e30, "playdata", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9e30, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".chkdat";
        fn_7100004578(&types[0], "CBlackboard", ".bchkdat", sources0, 1);
        fn_7100004698(g_unk_71727b9eb0, "checkpoint", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9eb0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".ldef";
        fn_7100004578(&types[0], "CLightManager", ".bldef", sources0, 1);
        fn_7100004698(g_unk_71727b9f30, "lightsdef", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9f30, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".pfg";
        fn_7100004578(&types[0], "base::file::CPack", ".pkg", sources0, 1);
        fn_7100004698(g_unk_71727b9fb0, "pack", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727b9fb0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".psi";
        fn_7100004578(&types[0], "base::file::CPackSetManager", ".bpsi", sources0, 1);
        fn_7100004698(g_unk_71727ba030, "packsets", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727ba030, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".lsnd";
        fn_7100004578(&types[0], "base::snd::CSoundGroupManager", ".blsnd", sources0, 1);
        fn_7100004698(g_unk_71727ba0b0, "sndlimits", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727ba0b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mdefs";
        fn_7100004578(&types[0], "sound::CMusicManager", ".bmdefs", sources0, 1);
        fn_7100004698(g_unk_71727ba130, "musicdefs", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727ba130, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".rspd";
        fn_7100004578(&types[0], "CShotAudioWeaponPresets", ".brspd", sources0, 1);
        fn_7100004698(g_unk_71727ba1b0, "shotsnds", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727ba1b0, g_unk_7101a19000);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".gsnds";
        fn_7100004578(&types[0], "sound::CSoundEventsDef", ".bgsnds", sources0, 1);
        fn_7100004698(g_unk_71727ba230, "gamesnds", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727ba230, g_unk_7101a19000);
}
