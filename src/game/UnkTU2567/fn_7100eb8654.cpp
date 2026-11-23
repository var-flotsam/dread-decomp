// Unit 2567 (the minimap's items): resets an item's selected alpha and size modifiers: each gets the
// default curve settings and is snapped back to its off value. Names and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkTimedCurve.h"
#include "base/global/CRntString.h"
#include "game/UnkBufferStringPool.h"
#include "types.h"

using UnkString20 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

// The values a curve runs between and how long it takes.
struct UnkCurveRange_eb8654 {
    f32 off_value = 1.0f;
    f32 on_value = 1.0f;
    f32 duration = 1.0f;
};

// A curve's settings: its range and how it is shaped.
struct UnkCurveSettings_eb8654 {
    UnkCurveRange_eb8654 range;
    UnkString20 modifiers;
    u32 mode = 0;
};
static_assert(sizeof(UnkCurveSettings_eb8654) == 0x80, "UnkCurveSettings_eb8654 layout");

// One modifier: a curve that runs between an off and an on value (as fn_7100eb7358 drives it).
struct UnkModifier_eb8654 {
    UnkTimedCurve curve;
    UnkCurveRange_eb8654 range;  // 0x50
    UnkString20 modifiers;  // 0x60
    u32 mode;       // 0xc8
    u8 pad_0xcc[4];
    f32 value;      // 0xd0
    bool forward;   // 0xd4
    bool on;        // 0xd5

    // Takes `settings`; returns whether the on value or the shaping changed.
    bool Set(const UnkCurveSettings_eb8654& settings) {
        bool same = settings.range.on_value == range.on_value &&
                    CRntString_KeysDiffer(settings.modifiers.Str(), modifiers.Str());
        range = settings.range;
        CRntString_Assign(modifiers.Str(), settings.modifiers.Str());
        mode = settings.mode;
        return !same;
    }

    // Jumps straight to the off value.
    void SnapOff() {
        f32 target = range.off_value;
        fn_7100075f78(target, target, 0.0f, &curve, &g_unk_7101d58d10);
        value = target;
        on = false;
    }
};
static_assert(__builtin_offsetof(UnkModifier_eb8654, mode) == 0xc8, "UnkModifier_eb8654 layout");
static_assert(sizeof(UnkModifier_eb8654) == 0xd8, "UnkModifier_eb8654 layout");

struct UnkMinimapModifiers;
void fn_7100ea9b04(UnkMinimapModifiers* self, u32 size);

// Up to three modifiers, the count first.
struct UnkModifiers_eb8654 {
    u32 count;
    alignas(4) u8 storage[3 * sizeof(UnkModifier_eb8654)];

    UnkModifier_eb8654* data() { return reinterpret_cast<UnkModifier_eb8654*>(storage); }

    void Resize(u32 size) { fn_7100ea9b04(reinterpret_cast<UnkMinimapModifiers*>(this), size); }
    // The modifier at `index`, growing the list to reach it.
    UnkModifier_eb8654& At(u32 index) {
        u32 size = index + 1;
        if (size > count)
            Resize(size);
        return data()[index];
    }
};

struct UnkMinimapItem_eb8654 {
    u8 pad_0x0[0x58];
    u32 alpha_index;  // 0x58
    u32 size_index;   // 0x5c
    UnkModifiers_eb8654 alpha_modifiers;  // 0x60
    UnkModifiers_eb8654 size_modifiers;   // 0x2ec
};
static_assert(__builtin_offsetof(UnkMinimapItem_eb8654, size_modifiers) == 0x2ec, "UnkMinimapItem_eb8654 layout");

static const u32 kNoModifier = 0xffffffff;

static inline void Reset(UnkModifiers_eb8654* list, u32 index) {
    u32 size = index + 1;
    if (size > list->count)
        list->Resize(size);
    UnkModifier_eb8654* modifiers = list->data();
    UnkCurveSettings_eb8654 settings = UnkCurveSettings_eb8654();
    UnkModifier_eb8654& modifier = modifiers[index];
    modifier.Set(settings);
    if (modifier.on)
        modifier.SnapOff();
}

void fn_7100eb8654(UnkMinimapItem_eb8654* item) {
    if (item->alpha_index != kNoModifier) {
        Reset(&item->alpha_modifiers, item->alpha_index);
        item->alpha_modifiers.At(item->alpha_index).SnapOff();
    }
    if (item->size_index != kNoModifier) {
        Reset(&item->size_modifiers, item->size_index);
        item->size_modifiers.At(item->size_index).SnapOff();
    }
}
