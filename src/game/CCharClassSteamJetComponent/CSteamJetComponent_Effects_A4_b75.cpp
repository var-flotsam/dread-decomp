// CCharClassSteamJetComponent unit (tools/tu_map.py, unit 1350): (re)creates one of the steam jet's two effects on
// its effect object (+0xc50), as the fan's pair (src/game/CCharClassFanComponent/CFanComponent_Effects_A4_b75.cpp):
// drops the old one (fn_7101153c6c), creates it from the description's (+0xc78) file path id (fn_71011520a8, kind 0),
// places it with the default offsets (fn_710115217c), starts it (fn_7101152c0c) and switches it off (fn_7100146e60).
// The second one tests the field at +0xc58 instead of the new effect. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CFXComponent.h"
#include "game/UnkSwitchObject.h"
#include "types.h"


struct UnkObject;

struct UnkSteamJetDesc_A4_b75 {
    u8 pad_0x0[0x60];
    base::global::CStrId path_a;      // 0x60
    base::global::CStrId path_b;      // 0x68
};

struct UnkSteamJetEffects_A4_b75 {
    u8 pad_0x0[0xc50];
    void* object;    // 0xc50
    char* effect_c;  // 0xc58
    char* effect_a;  // 0xc60
    char* effect_b;  // 0xc68
    u8 pad_0xc70[0x8];
    UnkSteamJetDesc_A4_b75* desc;  // 0xc78
};

// Places, starts and switches off a new effect.
static inline __attribute__((always_inline)) void PlaceSteamJetEffect(UnkSteamJetEffects_A4_b75* self, char* effect,
                                                                      char* UnkSteamJetEffects_A4_b75::*field) {
    fn_710115217c(self->object, effect, 0, 1, &g_unk_7101d470e0,
                  base::math::CVector3D(g_unk_7101647790),
                  base::math::CVector3D(g_unk_710164779c), nullptr);
    fn_7101152c0c(static_cast<UnkObject*>(self->object), self->*field, 1, 1);
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(self->*field), false);
}

template <char* UnkSteamJetEffects_A4_b75::*Effect, base::global::CStrId UnkSteamJetDesc_A4_b75::*Path, bool TestField>
static inline __attribute__((always_inline)) void CreateSteamJetEffect(UnkSteamJetEffects_A4_b75* self) {
    if (!self->object)
        return;
    if (char* old = self->*Effect) {
        fn_7101153c6c(self->object, old);
        self->*Effect = nullptr;
    }
    char* effect =
        fn_71011520a8(static_cast<char*>(self->object), 0, reinterpret_cast<char*>(&(self->desc->*Path)), 1);
    self->*Effect = effect;
    if (TestField ? !self->effect_c : !effect)
        return;
    PlaceSteamJetEffect(self, effect, Effect);
}

void fn_710089f634(UnkSteamJetEffects_A4_b75* self) {
    CreateSteamJetEffect<&UnkSteamJetEffects_A4_b75::effect_b, &UnkSteamJetDesc_A4_b75::path_b, false>(self);
}

void fn_710089f55c(UnkSteamJetEffects_A4_b75* self) {
    CreateSteamJetEffect<&UnkSteamJetEffects_A4_b75::effect_a, &UnkSteamJetDesc_A4_b75::path_a, true>(self);
}
