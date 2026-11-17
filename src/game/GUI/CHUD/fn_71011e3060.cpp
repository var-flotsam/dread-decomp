// GUI::CHUD (unit 2751): unless the Hyper Suit is owned, starts a missile counter animation over the
// given time from the current and maximum ammo, appended to the HUD's animations. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/GUI/CHUD.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkInventory;

struct UnkHudOwned_3060 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

// An owning pointer, as src/game/GUI/CHUD/CHUD_PushOwned.cpp spells it.
struct UnkHudOwnedRef_3060 {
    UnkHudOwned_3060* ptr;

    explicit UnkHudOwnedRef_3060(UnkHudOwned_3060* p) : ptr(p) {}
    ~UnkHudOwnedRef_3060() {
        UnkHudOwned_3060* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x10();
    }
};

struct UnkHudOwnedPtr;

// Appends an owned object to the array (fn_71011e7700 takes it over).
static inline void Append(base::global::CRntVector<UnkHudOwnedRef_3060>* array, UnkHudOwnedRef_3060&& value) {
    fn_71011e7700(reinterpret_cast<base::global::CRntVector<UnkHudOwnedPtr>*>(array),
                  reinterpret_cast<UnkHudOwnedPtr*>(&value));
}

// The missile animation's vtable.
extern char g_unk_7101cb62b8[];

struct UnkHud_3060 {
    u8 pad_0x0[0x340];
    base::global::CRntVector<UnkHudOwnedRef_3060> animations;  // 0x340
};

// The animation's clock: its length and the time gone.
struct UnkAnimTime_3060 {
    f32 duration;
    f32 elapsed;
};

// What the HUD's counter animations share: the HUD, the clock, its speed and whether it is over.
struct UnkHudAnim_3060 {
    char* vtable;
    UnkHud_3060* hud;       // 0x8
    UnkAnimTime_3060 time;  // 0x10
    f32 speed;              // 0x18
    bool done;              // 0x1c

    UnkHudAnim_3060(UnkHud_3060* hud_, f32 duration) : hud(hud_), time() {
        time.duration = duration;
        done = false;
    }
};

// The missile animation: the ammo it counts from and the value it shows.
struct UnkMissileAnim_3060 : UnkHudAnim_3060 {
    s32 current;  // 0x20
    s32 max;      // 0x24
    f32 shown;    // 0x28
    f32 field_0x2c;

    UnkMissileAnim_3060(UnkHud_3060* hud_, f32 duration, s32 current_, s32 max_)
        : UnkHudAnim_3060(hud_, duration), current(current_), max(max_) {
        vtable = g_unk_7101cb62b8 + 0x10;
        field_0x2c = 0.0f;
        speed = 1.0f;
        shown = current_;
    }
};
static_assert(sizeof(UnkMissileAnim_3060) == 0x30, "UnkMissileAnim_3060 layout");

static inline UnkQualifiedId& Qid_3060(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71011e3060(UnkHud_3060* self, f32 duration) {
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return;
    if (fn_71011428bc(inventory, Qid_3060(g_unk_717275a418)))  // ITEM_HYPER_SUIT
        return;
    s32 current = fn_71011442a0(inventory, Qid_3060(g_unk_717275a1d8), 0);
    s32 max = fn_71011442a0(inventory, Qid_3060(g_unk_717275a218), 0);
    Append(&self->animations, UnkHudOwnedRef_3060(reinterpret_cast<UnkHudOwned_3060*>(
                                  new UnkMissileAnim_3060(self, duration, current, max))));
}
