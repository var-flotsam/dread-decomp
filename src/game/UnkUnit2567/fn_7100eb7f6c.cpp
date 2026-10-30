// In unit 2567 of tools/tu_map.py (the minimap's items; src/game/CMinimapManager.cpp lays the item
// out in full): whether an item is drawn, that is, it has an icon, the product of its alpha
// modifiers' factors is positive (when it has any) and so is its size times its size modifiers'.
// Only the members read here are laid out; names are placeholders.
#include "types.h"

struct UnkMinimapItemModifier {
    u8 pad_0x0[0xd0];
    f32 factor;
    u8 pad_0xd4[0x4];
};
static_assert(sizeof(UnkMinimapItemModifier) == 0xd8, "UnkMinimapItemModifier layout");

// Raw storage, as src/game/CMinimapManager.cpp's UnkMinimapModifiers (the emptiness test stays a
// comparison of the two ends).
struct UnkMinimapItemModifiers {
    u32 count;
    alignas(4) u8 storage[3 * sizeof(UnkMinimapItemModifier)];

    const UnkMinimapItemModifier* begin() const { return reinterpret_cast<const UnkMinimapItemModifier*>(storage); }
    const UnkMinimapItemModifier* end() const { return begin() + count; }
};

struct UnkMinimapItem {
    void* icon;
    u8 pad_0x8[0x28];
    f32 size;  // 0x30
    u8 pad_0x34[0x2c];
    UnkMinimapItemModifiers alpha_modifiers;  // 0x60
    UnkMinimapItemModifiers size_modifiers;   // 0x2ec
};
static_assert(__builtin_offsetof(UnkMinimapItem, size_modifiers) == 0x2ec, "UnkMinimapItem layout");

bool fn_7100eb7f6c(const UnkMinimapItem* item) {
    if (!item->icon)
        return false;
    if (item->alpha_modifiers.begin() != item->alpha_modifiers.end()) {
        f32 alpha = 1.0f;
        for (const UnkMinimapItemModifier& it : item->alpha_modifiers)
            alpha *= it.factor;
        if (!(alpha > 0.0f))
            return false;
    }
    f32 size = item->size;
    for (const UnkMinimapItemModifier& it : item->size_modifiers)
        size *= it.factor;
    return size > 0.0f;
}
