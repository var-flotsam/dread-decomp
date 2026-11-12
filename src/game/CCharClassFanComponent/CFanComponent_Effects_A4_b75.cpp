// CCharClassFanComponent unit (tools/tu_map.py, unit 1336): (re)creates one of the fan's two effects on its effect
// object (+0xae0): drops the old one (fn_7101153c6c), and when the effect's file path id in the description (+0xb00)
// is not empty, creates it (fn_71011520a8, kind 3), places it with the default offsets (fn_710115217c) and scales its
// node by the fan's size over a constant (fn_7100207be8). The steam jet's pair is
// src/game/CCharClassSteamJetComponent/CSteamJetComponent_Effects_A4_b75.cpp. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CFXComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern char g_unk_7101647790[];

void fn_7100207be8(UnkXformNode* self, const UnkVec3f* v);

struct UnkFanDesc_A4_b75 {
    u8 pad_0x0[0x60];
    base::global::CStrId path_b;  // 0x60
    base::global::CStrId path_a;  // 0x68
};

struct UnkFanEffects_A4_b75 {
    u8 pad_0x0[0xac0];
    f32 size_b;  // 0xac0
    f32 size_a;  // 0xac4
    u8 pad_0xac8[0x18];
    void* object;    // 0xae0
    char* effect_b;  // 0xae8
    char* effect_a;  // 0xaf0
    u8 pad_0xaf8[0x8];
    UnkFanDesc_A4_b75* desc;  // 0xb00
};

template <char* UnkFanEffects_A4_b75::*Effect, base::global::CStrId UnkFanDesc_A4_b75::*Path,
          f32 UnkFanEffects_A4_b75::*Size>
static inline __attribute__((always_inline)) void CreateFanEffect(UnkFanEffects_A4_b75* self, f32 scale,
                                                                  base::math::CVector3D& v) {
    if (!self->object)
        return;
    if (char* old = self->*Effect) {
        fn_7101153c6c(self->object, old);
        self->*Effect = nullptr;
    }
    base::global::CStrId* path = &(self->desc->*Path);
    if (*reinterpret_cast<const char**>(path) == reinterpret_cast<const char*>(g_unk_7101d47200) + 0x10)
        return;
    char* effect = fn_71011520a8(static_cast<char*>(self->object), 3, reinterpret_cast<char*>(path), 1);
    self->*Effect = effect;
    if (!effect)
        return;
    fn_710115217c(self->object, effect, 0, 1, &g_unk_7101d470e0,
                  base::math::CVector3D(*reinterpret_cast<const base::math::CVector3D*>(g_unk_7101647790)),
                  base::math::CVector3D(g_unk_710164779c), nullptr);
    f32 s = self->*Size / scale;
    UnkXformNode* node = reinterpret_cast<UnkXformNode*>(self->*Effect + 0x28);
    v.x = s;
    v.y = s;
    v.z = s;
    fn_7100207be8(node, reinterpret_cast<const UnkVec3f*>(&v));
}

void fn_710087f138(UnkFanEffects_A4_b75* self) {
    goto start;
    base::math::CVector3D v;
start:
    CreateFanEffect<&UnkFanEffects_A4_b75::effect_a, &UnkFanDesc_A4_b75::path_a, &UnkFanEffects_A4_b75::size_a>(
        self, 800.0f, v);
}

void fn_710087f22c(UnkFanEffects_A4_b75* self) {
    goto start;
    base::math::CVector3D v;
start:
    CreateFanEffect<&UnkFanEffects_A4_b75::effect_b, &UnkFanDesc_A4_b75::path_b, &UnkFanEffects_A4_b75::size_b>(
        self, 1200.0f, v);
}
