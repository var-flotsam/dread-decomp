// GUI::CHUD (unit 2751): starts a HUD count animation over the given time, from the two counts the
// player's object at +0x1c0 holds (+0x54 and +0x50), and adds it to the HUD's animations.
// Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "game/GUI/CHUD.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkHudOwned_33a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

// An owning pointer, as src/game/GUI/CHUD/CHUD_PushOwned.cpp spells it.
struct UnkHudOwnedRef_33a8 {
    union {
        UnkHudOwned_33a8* ptr;
    };
    explicit UnkHudOwnedRef_33a8(UnkHudOwned_33a8* p) : ptr(p) {}
    ~UnkHudOwnedRef_33a8() {
        UnkHudOwned_33a8* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x10();
    }
};

struct UnkHudOwnedPtr;

// Hands a new object to the array (the array's out-of-line EmplaceBack).
static inline void Add(base::global::CRntVector<UnkHudOwnedPtr>* list, UnkHudOwned_33a8* object) {
    UnkHudOwnedRef_33a8 owned(object);
    fn_71011e7700(list, reinterpret_cast<UnkHudOwnedPtr*>(&owned));
}

// The count animation's vtable (reached through the GOT).
extern void* g_unk_7101cb6318[];

struct UnkCounts_33a8 {
    u8 pad_0x0[0x50];
    f32 field_0x50;
    f32 field_0x54;
};

struct UnkPlayerView_33a8 {
    u8 pad_0x0[0x1c0];
    UnkCounts_33a8* counts;  // 0x1c0
};

struct UnkHud_33a8 {
    u8 pad_0x0[0x340];
    base::global::CRntVector<UnkHudOwnedPtr> animations;  // 0x340
};

struct UnkTimer_33a8 {
    f32 duration;
    f32 elapsed;
};

// What every HUD animation has: its vtable, its owner and a timer.
struct UnkHudAnim_33a8 {
    void** vtable;
    UnkHud_33a8* owner;
    UnkTimer_33a8 timer;  // 0x10

    UnkHudAnim_33a8(UnkHud_33a8* hud, f32 time) : owner(hud), timer() { timer.duration = time; }
};

// A HUD animation of a count going from one value to another.
struct UnkHudCountAnim_33a8 : UnkHudAnim_33a8 {
    f32 speed;   // 0x18
    bool done;   // 0x1c
    s32 from;    // 0x20
    s32 to;      // 0x24
    f32 value;   // 0x28
    f32 field_0x2c;

    UnkHudCountAnim_33a8(UnkHud_33a8* hud, f32 time, s32 start, s32 end)
        : UnkHudAnim_33a8(hud, time), done(false), from(start), to(end), field_0x2c(0.0f) {
        vtable = &g_unk_7101cb6318[2];
        speed = 1.0f;
        value = start;
    }
};
// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71011e33a8(UnkHud_33a8* self, f32 time) {
    UnkPlayerView_33a8* player = reinterpret_cast<UnkPlayerView_33a8*>(fn_7100dec788(g_unk_7101d47310));
    UnkCounts_33a8* counts = player->counts;
    if (!counts)
        return;
    s32 from = (s32)roundf(counts->field_0x54);
    s32 to = (s32)roundf(counts->field_0x50);
    Add(&self->animations, reinterpret_cast<UnkHudOwned_33a8*>(new UnkHudCountAnim_33a8(self, time, from, to)));
}
