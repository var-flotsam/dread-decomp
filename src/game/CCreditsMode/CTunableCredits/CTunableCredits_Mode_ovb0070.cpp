// CCreditsMode::CTunableCredits's unit (tools/tu_map.py unit 2724). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/sound/CMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_71726bc560;
// The channel mask as src/game/UnkLoadingScreen.cpp defines it (passed through a copy), and its two
// guarded constants.
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
void fn_7100fd7b44(void* manager, UnkLoadingMask channels, UnkLoadingMask mask, f32 time);

// The music manager (game/sound/CMusicManager.h), created on first use.
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

using base::global::CRntString;

struct UnkVec;
void fn_7101053850(void* effects, UnkVec* position, u32 flag, f32 duration, f32 scale);

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

struct UnkCreditsNode_ovb0070 {
    UnkCreditsLine_ovb0070* line;
    UnkCreditsNode_ovb0070* next;
};

// The list of lines (+0x90): its nodes come from `allocator`.
struct UnkCreditsLineList_ovb0070 {
    void** vtable;
    u32 count;
    UnkCreditsNode_ovb0070* head;
    UnkCreditsNode_ovb0070* tail;
    base::UnkAllocator* allocator;
    u32 flags;
};

struct UnkCreditsMode_ovb0070 {
    u8 pad_0x0[0x50];
    f32 time;   // 0x50
    u32 flags;  // 0x54: bit 0 scrolling, 1 waiting, 2 ending, 3 music started, 5 to the rewards
    f32 wait;   // 0x58
    f32 field_0x5c;
    UnkGuiDisplayObject* objects[4];  // 0x60
    u8 pad_0x80[0x10];
    UnkCreditsLineList_ovb0070 lines;  // 0x90
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
    fn_7100fd7b44(MusicManager(), UnkLoadingMask(0xc), UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), time);
    void* effects = g_unk_7101d47310->field_0x1040;
    UnkCreditsPosition_ovb0070 position;
    position.xy = 0;
    position.z = 0;
    fn_7101053850(effects, reinterpret_cast<UnkVec*>(&position), 1, time, 1.0f);
    self->field_0x5c = 2.5f;
}

void* fn_7100deab70(UnkGameManager* self, const CRntString& name);
void fn_7100deac9c(UnkGameManager* self, const CRntString& name);
void fn_71011843d0(UnkCreditsMode_ovb0070* self, f32 dt);

struct UnkObj_71011a50cc {
    u8 unk_0x0[0x98];
    UnkObj_71011a50cc();  // fn_71011a50cc
};
extern UnkObj_71011a50cc* g_unk_71727bc260;
void fn_71011a5798(UnkObj_71011a50cc* self, u32 index, f32 value);

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
struct UnkTunable;
extern UnkTunable g_unk_7101d68580;  // base::snd::CSoundSystemATK's tunable; a float at +0x2c
void fn_71000fbe1c(void* self, u32 a, f32 b, f32 c);
// A mode word passed by address (the word after it is not set).
struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};
void fn_7100fd90d8(f32 a, f32 b, void* music, const CRntString* name, u32 c, const UnkMusicMode* d, u32 e, u32 f, void* g);

struct UnkCreditsMenu_ovb0070 {
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
                auto* menu = static_cast<UnkCreditsMenu_ovb0070*>(fn_7100deab70(manager, CRntString("MAINMENU")));
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
        fn_71000fbe1c(reinterpret_cast<u8*>(sound) + 8, 1, volume, 0.3f);
    }
}
