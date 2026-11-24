// Static initialiser of the unit of CEmmyGrabSamusAttack: the header ids, then the unit's own
// statics (path ids, ids, strings, small-buffer strings, floats) and the class registration.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/CEmmyGrabSamusAttackStatics.h"
#include "game/UnkInlineStr50.h"

using namespace unk_static_init;

void fn_7100504fdc(void* self);  // the inline string's destructor, as registered

// Builds a static inline string and registers its destructor.
__attribute__((always_inline)) inline void InitInlineStr50(UnkInlineStr50* s, const char* str) {
    s->Init();
    CRntString_AssignCString(s->Str(), str);
    __cxa_atexit(fn_7100504fdc, s, g_unk_7101a19000);
}

__attribute__((always_inline)) inline void InitRntString(base::UnkNamedEntry* s, const char* str) {
    CRntString_ConstructFromCString(s, str);
    __cxa_atexit(reinterpret_cast<void (*)(void*)>(CRntString_Destroy), s, g_unk_7101a19000);
}

extern char g_unk_71726de550[];
void fn_710050512c(void* self);  // its destructor, as registered

void CEmmyGrabSamusAttack_InitStatics() {
    UnkEmmyGrabStatics& g = g_unk_71726de558;
    InitHeaderIds(&g.ids);
    g.field_0xf8[0] = -1.0f;
    g.field_0xf8[1] = -1.0f;
    g.field_0x100 = true;
    fn_7100504d60();
    fn_710009ec00(g_unk_71726de550, g.field_0x3f8);
    InitPathId(&g.blockable_sound, "gui/blockable_emmy.wav");
    InitPathId(&g.kick_sound, "actors/samus/samus_kickattack_05.wav");
    InitPathId(&g.melee_success_sound, "actors/emmy/melee_success_01.wav");
    InitPathId(&g.blocking_end_effect, "actors/characters/shared/fx/blockingattackend.bcptl");
    InitId(&g.finger_node, "R01_Finger1_SKN");
    InitId(&g.head_node, "C01_Head_SKN");
    InitRntString(&g.far2, "far2");
    InitRntString(&g.far3, "far3");
    InitRntString(&g.far4, "far4");
    InitRntString(&g.headon, "headon");
    InitId(&g.attack_id, "EmmyGrabSamusAttack");
    InitInlineStr50(&g.samus_suffix, "_samus");
    InitInlineStr50(&g.emmy_suffix, "_emmy");
    g.bands[0] = 400.0f;
    g.bands[1] = 300.0f;
    g.bands[2] = 200.0f;
    g.bands[3] = 100.0f;
    {
        UnkInlineStr50 strs[4] = {"4m", "3m", "2m", "1m"};
        CEmmyGrabSamusAttack_InitInlineStrArray4FromList(g.band_names, strs, 4);
    }
    __cxa_atexit(fn_710050512c, g.band_names, g_unk_7101a19000);
    InitInlineStr50(&g.tunnel_suffix, "_tunnel");
    InitInlineStr50(&g.morphball_suffix, "_mb");
    InitId(&g.tunnel_character, "Character_Tunnel");
    InitId(&g.morphball_grab, "morphball_grab");
    InitId(&g.tunnel_near, "TunnelNear");
    InitId(&g.ignore_impact, "IgnoreImpactEmmyGrab");
    InitId(&g.attack_id2, "EmmyGrabSamusAttack");
    InitPathId(&g.tutorial_camera, "actors/characters/emmy/cameras/grabprotoemmytuto.bccam");
}
