// UnkTU2656: unless the gui is busy, subtracts a frame time from the fade time, starts a fade of a colour
// on the node at +0x18 and, while time is left, asks the player's input component for an input request.
// Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU286.h"
#include "types.h"

struct UnkInputComponent;

f32 fn_7100080e30(UnkGfxFrameSource* timer);
void fn_71008724b4(f32 time, UnkInputComponent* self, void* arg, bool reset);
__attribute__((visibility("hidden"))) extern char g_unk_71729fe170[];

struct UnkFadeColor {
    f32 r, g, b, a;

    UnkFadeColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct UnkFadeOwner_l053850 {
    u8 pad_0x0[0x18];
    u8 node;
};

struct UnkFadeUser_l053850 {
    UnkFadeOwner_l053850* owner;
    u8 pad_0x8[0x20];
    u32 field_0x28;
};

struct UnkRgb_l053850 {
    f32 r, g, b;
};

struct UnkFadePlayer_l053850 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;
};

static inline f32 UnkClampPositive(f32 x) { return x <= 0.0f ? 0.0f : x; }

void fn_7101053850(UnkFadeUser_l053850* self, const UnkRgb_l053850* color, bool request, f32 time, f32 alpha) {
    if (fn_7101198688(g_unk_7101d47330))
        return;
    if (fn_7100080e30(g_unk_7101d47328) <= time)
        time -= fn_7100080e30(g_unk_7101d47328);
    f32 r = UnkClampPositive(color->r);
    f32 g = UnkClampPositive(color->g);
    f32 b = UnkClampPositive(color->b);
    UnkFadeColor to(r, g, b, 0.0f);
    UnkFadeColor from(r, g, b, UnkClampPositive(alpha));
    fn_7100209968(&self->owner->node, time, &to, &from);
    if (time > 0.0f && request) {
        UnkFadePlayer_l053850* player = reinterpret_cast<UnkFadePlayer_l053850*>(fn_7100dec788(g_unk_7101d47310));
        if (player && player->input)
            fn_71008724b4(time, player->input, g_unk_71729fe170, false);
    }
    self->field_0x28 = 1;
}
