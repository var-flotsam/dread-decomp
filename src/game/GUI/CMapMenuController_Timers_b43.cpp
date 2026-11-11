// GUI::CMapMenuController's unit (tools/tu_map.py): the controller's timers, per frame: while the
// delay at +0x8b0 runs nothing else happens; when the timer at +0x8a8 runs out the map data is told
// (fn_7100ea6488), the state sound for the mode (+0x6f0; key 0x71729f0178 in mode 2, 0x71729f0180
// otherwise) is started with the current entry's object (+0x238, its +0x10) and the timer at +0x8ac
// set to 0.3; when that one runs out the controller moves to its mode (fn_7100b3e5ac). Then the two
// transitions (+0x7d0, +0x6f8) advance; when both are done the mode is entered (fn_7100b3feb8),
// otherwise the game manager's map view (+0x2228) gets the zoom (+0x7c8) and the value at +0x8a0.
// Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU1689.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU2567.h"
#include "types.h"

// As src/game/GUI/CIngameMenu/fn_7100b2de48.cpp and
// src/game/GUI/CGlobalMapComposition/CGlobalMapComposition_Sound.cpp declare them.
struct UnkStateSink;
extern UnkStateSink* g_unk_717275c380;
struct UnkStateKey;
extern UnkStateKey g_unk_71729f0178;
extern UnkStateKey g_unk_71729f0180;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);

// As src/game/GUI/CMapMenuController.cpp and src/game/GUI/CMapMenuController_Close.cpp declare them.
struct UnkMapData;
struct UnkMapMenuControllerClose;

void fn_7100ea6488(UnkMapData* self);

struct UnkMapTimersView_b43a3 {
    u8 pad_0x0[0xb8];
    u32 field_0xb8;
};

struct UnkMapTimersEntry_b43a3 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkMapTimersController_b43a3 {
    u8 pad_0x0[0x238];
    UnkMapTimersEntry_b43a3* entry;  // 0x238
    u8 pad_0x240[0x6f0 - 0x240];
    s32 mode;  // 0x6f0
    u8 pad_0x6f4[0x4];
    u8 transition_0x6f8[0xd0];
    f32 zoom;  // 0x7c8
    u8 pad_0x7cc[0x4];
    u8 transition_0x7d0[0xd0];
    u32 field_0x8a0;
    u8 pad_0x8a4[0x4];
    f32 timer_0x8a8;
    f32 timer_0x8ac;
    f32 delay;  // 0x8b0
};
static_assert(__builtin_offsetof(UnkMapTimersController_b43a3, delay) == 0x8b0, "UnkMapTimersController_b43a3 layout");

void fn_7100b3feb8(UnkMapTimersController_b43a3* self, s32 mode, u32 flags);

static inline UnkMapTimersView_b43a3* MapView_b43a3() {
    return *reinterpret_cast<UnkMapTimersView_b43a3**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2228);
}

void fn_7100b3e45c(UnkMapTimersController_b43a3* self, f32 dt) {
    if (self->delay > 0.0f) {
        self->delay -= dt;
        return;
    }
    if (self->timer_0x8a8 > 0.0f) {
        self->timer_0x8a8 -= dt;
        if (self->timer_0x8a8 <= 0.0f) {
            fn_7100ea6488(fn_7100e8d8b0());
            fn_7100fc9d78(g_unk_717275c380, self->mode == 2 ? &g_unk_71729f0178 : &g_unk_71729f0180,
                          self->entry ? self->entry->object : nullptr);
            self->timer_0x8ac = 0.3f;
        }
    }
    if (self->timer_0x8ac > 0.0f) {
        self->timer_0x8ac -= dt;
        if (self->timer_0x8ac <= 0.0f)
            fn_7100b3e5ac(reinterpret_cast<UnkMapMenuControllerClose*>(self), self->mode, 0);
    }
    bool a = fn_7100eb7470(self->transition_0x7d0, dt);
    bool b = fn_7100eb7470(self->transition_0x6f8, dt);
    if (a && b) {
        fn_7100b3feb8(self, self->mode, 0);
        return;
    }
    fn_7100e94054(MapView_b43a3(), self->zoom);
    UnkMapTimersView_b43a3* view = MapView_b43a3();
    view->field_0xb8 = self->field_0x8a0;
}
