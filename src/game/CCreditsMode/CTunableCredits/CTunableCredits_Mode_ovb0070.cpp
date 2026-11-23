// CCreditsMode::CTunableCredits's unit (tools/tu_map.py unit 2724). Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CRntString.h"
#include "game/CBossAIComponent.h"
#include "game/UnkCreditsNode_ovb0070.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkMusicMode.h"
#include "game/UnkObj_71011a50cc.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2538.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2737.h"
#include "game/UnkTU99.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "game/UnkMask.h"
#include "base/UnkVtableList.h"

// The music manager (game/sound/CMusicManager.h), created on first use.
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

using base::global::CRntString;

struct UnkVec;

struct UnkCreditsPosition_ovb0070 {
    u64 xy;
    u32 z;
};

// A credits line: its text and shadow objects and the text it shows.
struct UnkCreditsLine_ovb0070 {
    UnkGuiDisplayObject* text;
    UnkGuiDisplayObject* shadow;
    u32 kind;
    u8 pad_0x14[0x14];
    UnkGuiText string;  // 0x28
};

struct UnkCreditsMode_ovb0070 {
    u8 pad_0x0[0x50];
    f32 time;   // 0x50
    u32 flags;  // 0x54: bit 0 scrolling, 1 waiting, 2 ending, 3 music started, 5 to the rewards
    f32 wait;   // 0x58
    f32 field_0x5c;
    UnkGuiDisplayObject* objects[4];  // 0x60
    u8 pad_0x80[0x10];
    base::UnkVtableList<UnkCreditsNode_ovb0070> lines;  // 0x90, the list of lines
    u8 pad_0xc0[0x9b0];
    UnkWeakHandle* field_0xa70;
    UnkWeakHandle* field_0xa78;
};
static_assert(__builtin_offsetof(UnkCreditsMode_ovb0070, field_0xa70) == 0xa70, "layout");

// Starts the end of the credits (once): hides the object at +0xa70, fades the music channels 2 and
// 3 out over 2 s and starts the game manager's screen effect.
void fn_7101184028(UnkCreditsMode_ovb0070* self) {
    if (self->flags & 4)
        return;
    fn_71011cdce8(self->field_0xa70 ? reinterpret_cast<UnkGuiDisplayObject*>(self->field_0xa70->target) : nullptr, false);
    self->flags = (self->flags & ~0x10u) | 4;
    f32 time = 2.0f;
    fn_7100fd7b44(static_cast<UnkMusicMgr_b0019*>(MusicManager()), UnkMask(0xc),
                  UnkMaskOf<UnkMusicMaskTag>::All(), time);
    void* effects = g_unk_7101d47310->field_0x1040;
    UnkCreditsPosition_ovb0070 position;
    position.xy = 0;
    position.z = 0;
    fn_7101053850(effects, reinterpret_cast<UnkVec*>(&position), 1, time, 1.0f);
    self->field_0x5c = 2.5f;
}

void fn_71011843d0(UnkCreditsMode_ovb0070* self, f32 dt);

struct UnkSoundMgr;
struct UnkTunable;

struct UnkCreditsMenu_ovb0070_ovb0070 {
    u8 pad_0x0[0x60];
    u32 state;
};

// The credits' update: the clock, the end (back to the main menu or on to the rewards once its
// time runs out), the wait before the end, the scrolling (fn_71011843d0), unit 2727's object, and
// once past 0.25 s the "StaffRoll" music.
void fn_71011841b4(UnkCreditsMode_ovb0070* self, f32 dt) {
    self->time += dt;
    u32 flags = self->flags;
    if (flags & 4) {
        self->field_0x5c -= dt;
        if (self->field_0x5c <= 0.0f) {
            UnkGameManager* manager = g_unk_7101d47310;
            if (flags & 0x20) {
                fn_7100deac9c(manager, CRntString("REWARDS"));
            } else {
                auto* menu = static_cast<UnkCreditsMenu_ovb0070_ovb0070*>(fn_7100deab70(manager, CRntString("MAINMENU")));
                if (menu)
                    menu->state = 7;
                fn_7100deac9c(g_unk_7101d47310, CRntString("MAINMENU"));
            }
        }
    } else if (flags & 2) {
        if (self->wait > 0.0f)
            self->wait -= dt;
        else
            fn_7101184028(self);
    }
    if (self->flags & 1)
        fn_71011843d0(self, dt);
    if (!g_unk_71727bc260)
        g_unk_71727bc260 = new (fn_71000839a0(0x98, 0x3f)) UnkObj_71011a50cc();
    fn_71011a5798(g_unk_71727bc260, 0, dt);
    if (!(self->flags & 8) && self->time > 0.25f) {
        self->flags |= 8;
        void* music = MusicManager();
        UnkSoundMgr* sound = g_unk_7101d4d820;
        {
            CRntString name("StaffRoll");
            UnkMusicMode mode;
            mode.kind = 0x4e;
            fn_7100fd90d8(0.0f, 0.0f, music, &name, 7, &mode, 0, 0, nullptr);
        }
        f32 volume = *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_7101d68580) + 0x2c);
        fn_71000fbe1c(reinterpret_cast<UnkRampTable*>(reinterpret_cast<u8*>(sound) + 8), 1, volume, 0.3f);
    }
}
