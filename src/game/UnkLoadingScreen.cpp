// From unit 2557 of tools/tu_map.py (0x7100e0f028-0x7100e140c0, no reflected class; it reflects the
// ELoadingScreen enum and names "LoadingScreen", "LoadinScreenTip%d" and the loading animations):
// the loading screen. Names and layouts are placeholders.
// Holds fn_7100e10aac (closes the loading screen).
#include "base/UnkVector.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkRenderManager.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

// A fade colour (the same definition as src/game/UnkScreenFade.cpp's).
struct UnkFadeColor {
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    UnkFadeColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

// A bit mask passed by value (copied into a temporary), and its two guarded constants.
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkLoadingMaskOf {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag;

// The object behind +0x20 whose node at +0x18 shows the fade.
struct UnkLoadingFade {
    u8 unk_0x0[0x18];
    u8 node[1];
};

struct UnkLoadingScreen {
    s32 field_0x0;
    u8 unk_0x4[0x14];
    void* field_0x18;  // a render object handed back to the render manager (pool 3)
    UnkLoadingFade* field_0x20;
    s32 field_0x28;  // the scene state; 0x50 once restored
    u8 unk_0x2c[0x1bc];
    bool field_0x1e8;
};
static_assert(__builtin_offsetof(UnkLoadingScreen, field_0x1e8) == 0x1e8, "UnkLoadingScreen placeholder layout");

// The scene-state singleton (0x150 bytes, created on first use; the type is src/game_hi's
// UnkStruct_0x128).
struct UnkStruct_0x128;
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100ef3494(UnkStruct_0x128* self);
void fn_7100f011c4(UnkStruct_0x128* self, u32 id);
void fn_7100f00714(UnkStruct_0x128* self);
static inline UnkStruct_0x128* SceneState() {
    UnkStruct_0x128* s = g_unk_7101d47320;
    if (!s) {
        s = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(s);
        g_unk_7101d47320 = s;
    }
    return s;
}

// The music manager (game/sound/CMusicManager.h), created on first use.
extern void* g_unk_71726bc560;
void fn_7100fd7b44(void* manager, UnkLoadingMask channels, UnkLoadingMask mask, f32 time);
void fn_7100fdca28(void* manager, u32 value);
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

struct UnkItemBase_71011a91c4;
bool fn_71002199d8(UnkRenderManager* self, UnkItemBase_71011a91c4* item, u32 pool);
void fn_71002199b0(UnkRenderManager* self);
void fn_71002199b4(UnkRenderManager* self);
void fn_7100219974(UnkRenderManager* self, u32 value);
void* fn_71000810b0(u32 value);
void fn_7100209968(void* node, f32 duration, const UnkFadeColor* to, const UnkFadeColor* from);
void fn_7100209fec(void* node, bool visible);
void fn_7100e13238(UnkLoadingScreen* self);
void fn_71011b6b5c(UnkGuiRoot* root, u32 value);
extern void* g_unk_71727c0708;  // a render object the loading screen keeps (pool 0)

// Closes the loading screen: the fade node goes from black to clear over 0.3, then is hidden; the
// render objects go back to the render manager, the scene state is restored once (state 0x50) with
// the music brought back on all channels, and the music and the GUI root are reset.
// NON_MATCHING(minor): scheduling only: the fade's 0.3 duration is materialised after the second
// colour's constant and store instead of before them (its constant then lands in x8, not x9).
void fn_7100e10aac(UnkLoadingScreen* self) {
    fn_71002199b0(g_unk_7101d47300);
    void* node = self->field_0x20->node;
    UnkFadeColor to(0.0f, 0.0f, 0.0f, 0.0f);
    UnkFadeColor from(0.0f, 0.0f, 0.0f, 1.0f);
    fn_7100209968(node, 0.3f, &to, &from);
    fn_71002199b4(g_unk_7101d47300);
    fn_71000810b0(1000);
    fn_7100219974(g_unk_7101d47300, 0);
    fn_7100209fec(self->field_0x20->node, false);
    self->field_0x1e8 = false;
    fn_7100e13238(self);
    if (g_unk_71727c0708) {
        fn_71002199d8(g_unk_7101d47300, static_cast<UnkItemBase_71011a91c4*>(g_unk_71727c0708), 0);
        g_unk_71727c0708 = nullptr;
    }
    if (self->field_0x28 != 0x50) {
        fn_7100f011c4(SceneState(), self->field_0x28);
        fn_7100f00714(SceneState());
        self->field_0x28 = 0x50;
        fn_7100fd7b44(MusicManager(), UnkLoadingMask(4), UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), 0.2f);
    }
    if (self->field_0x18) {
        fn_71002199d8(g_unk_7101d47300, static_cast<UnkItemBase_71011a91c4*>(self->field_0x18), 3);
        self->field_0x18 = nullptr;
    }
    fn_7100fd7b44(MusicManager(), UnkLoadingMask(10), UnkLoadingMask(0x100), 0.2f);
    self->field_0x0 = 0;
    fn_7100fdca28(MusicManager(), 0);
    fn_71011b6b5c(g_unk_717271e8f8, 0);
}
