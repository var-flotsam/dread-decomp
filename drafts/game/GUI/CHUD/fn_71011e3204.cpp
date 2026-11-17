// GUI::CHUD (unit 2751): when the power bomb is owned, starts a power bomb counter animation over the
// given time from the current and maximum ammo, appended to the HUD's animations. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkInventory;

struct UnkHudOwned_3204 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

// An owning pointer, as src/game/GUI/CHUD/CHUD_PushOwned.cpp spells it.
struct UnkHudOwnedRef_3204 {
    UnkHudOwned_3204* ptr;

    explicit UnkHudOwnedRef_3204(UnkHudOwned_3204* p) : ptr(p) {}
    ~UnkHudOwnedRef_3204() {
        UnkHudOwned_3204* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x10();
    }
};

struct UnkHudOwnedPtr;
void fn_71011e7700(base::global::CRntVector<UnkHudOwnedPtr>* self, UnkHudOwnedPtr* value);

// Appends an owned object to the array (fn_71011e7700 takes it over).
static inline void Append(base::global::CRntVector<UnkHudOwnedRef_3204>* array, UnkHudOwnedRef_3204&& value) {
    fn_71011e7700(reinterpret_cast<base::global::CRntVector<UnkHudOwnedPtr>*>(array),
                  reinterpret_cast<UnkHudOwnedPtr*>(&value));
}

// The power bomb animation's vtable.
extern char g_unk_7101cb62e8[];

struct UnkHud_3204 {
    u8 pad_0x0[0x340];
    base::global::CRntVector<UnkHudOwnedRef_3204> animations;  // 0x340
};

// The animation's clock: its length and the time gone.
struct UnkAnimTime_3204 {
    f32 duration;
    f32 elapsed;
};

// The animation: the HUD, its time and progress, and the ammo it counts from.
struct UnkPowerBombAnim_3204 {
    char* vtable;
    UnkHud_3204* hud;  // 0x8
    UnkAnimTime_3204 time;  // 0x10
    f32 speed;         // 0x18
    bool done;         // 0x1c
    s32 current;       // 0x20
    s32 max;           // 0x24
    f32 shown;         // 0x28
    f32 field_0x2c;

    UnkPowerBombAnim_3204(UnkHud_3204* hud_, f32 duration_, s32 current_, s32 max_)
        : vtable(g_unk_7101cb62e8 + 0x10), hud(hud_), time(), done(false), current(current_), max(max_) {
        time.duration = duration_;
        field_0x2c = 0.0f;
        speed = 1.0f;
        shown = current_;
    }
};
static_assert(sizeof(UnkPowerBombAnim_3204) == 0x30, "UnkPowerBombAnim_3204 layout");

static inline UnkQualifiedId& Qid_3204(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_71011e3204(UnkHud_3204* self, f32 duration) {
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return;
    if (!fn_71011428bc(inventory, Qid_3204(g_unk_717275a818)))  // ITEM_WEAPON_POWER_BOMB
        return;
    s32 current = fn_71011442a0(inventory, Qid_3204(g_unk_717275a258), 0);
    s32 max = fn_71011442a0(inventory, Qid_3204(g_unk_717275a298), 0);
    Append(&self->animations, UnkHudOwnedRef_3204(reinterpret_cast<UnkHudOwned_3204*>(
                                  new UnkPowerBombAnim_3204(self, duration, current, max))));
}
