#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"
#include "game/CComponent.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU53.h"
#include "game/UnkTU315.h"

struct UnkParent;
struct UnkPlayer;
extern const u64 g_unk_710164ab48;

struct UnkCountDown_ovb0259;
void fn_71007daa8c(UnkCountDown_ovb0259* self);
void fn_71007dacbc(UnkCountDown_ovb0259* self);
void fn_71007daf04(UnkCountDown_ovb0259* self);
void fn_71007da730(UnkCountDown_ovb0259* self);
void fn_71007db054(UnkCountDown_ovb0259* self, f32 dt);

struct UnkCountDownTunables_ovb0259 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
    u8 pad_0x2c[4];
    base::global::CStrId curve_0x30;
    f32 total;  // 0x38
    u8 pad_0x3c[0x5c];
    f32 field_0x98;
    f32 field_0x9c;
    base::global::CStrId curve_0xa0;
};
extern UnkCountDownTunables_ovb0259 g_unk_717271e848;

struct UnkPlayerView_ovb0259 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkTargetView_ovb0259 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
    u8 pad_0x68[0x348];
    u8 field_0x3b0;
};

struct UnkCountDown_ovb0259 {
    u8 pad_0x0[0x30];
    UnkTargetView_ovb0259* field_0x30;
    u8 pad_0x38[0x41];
    bool field_0x79;
    u8 pad_0x7a[0x46];
    f32 elapsed;  // 0xc0
    u8 pad_0xc4[0x2c];
    bool field_0xf0;
};

static inline f32 LerpClamped(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

static inline f32 CurveAt(UnkCountDownTunables_ovb0259* t, f32 ratio) {
    f32 clamped = ratio <= 0.0f ? 0.0f : (ratio >= 1.0f ? 1.0f : ratio);
    return fn_7100076020(clamped, t->curve_0xa0.Text());
}

void fn_71007da92c(UnkCountDown_ovb0259* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkPlayerView_ovb0259* player = reinterpret_cast<UnkPlayerView_ovb0259*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        UnkTargetView_ovb0259* target = self->field_0x30;
        u32 a = player->field_0x44;
        u32 b = player->field_0x54;
        u32 c = player->field_0x64;
        target->field_0x44 = a;
        target->field_0x54 = b;
        target->field_0x64 = c;
        target->field_0x3b0 = 1;
    }
    if (!self->field_0x79 || !self->field_0xf0)
        return;
    fn_71007daa8c(self);
    fn_71007dacbc(self);
    fn_71007daf04(self);
    fn_71007da730(self);
    fn_71007db054(self, dt);
    UnkCountDownTunables_ovb0259* t = &g_unk_717271e848;
    f32 ratio = self->elapsed / t->total;
    f32 v = CurveAt(t, ratio);
    f32 value = LerpClamped(t->field_0x98, t->field_0x9c, v);
    base::math::CVector4D vec;
    vec.x = value;
    vec.y = 0.0f;
    vec.z = 0.0f;
    vec.w = 0.0f;
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164ab48, &vec);
    self->elapsed = self->elapsed + dt;
}
