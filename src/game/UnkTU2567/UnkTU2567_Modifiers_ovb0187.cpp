// In unit 2567 of tools/tu_map.py (the minimap's items; src/game/CMinimapManager.cpp lays the item out in full,
// src/game/UnkUnit2567/fn_7100eb7f6c.cpp reads it): advances every alpha modifier and every size modifier
// of an item that has an icon by `dt` (fn_7100eb7470, the bouncing curve's step). Names and layouts are
// placeholders.
#include "game/UnkTU2567.h"
#include "types.h"

struct UnkMinimapItemModifier_ovb0187 {
    u8 pad_0x0[0xd8];
};
static_assert(sizeof(UnkMinimapItemModifier_ovb0187) == 0xd8, "UnkMinimapItemModifier_ovb0187 layout");

struct UnkMinimapItemModifiers_ovb0187 {
    u32 count;
    alignas(4) u8 storage[3 * sizeof(UnkMinimapItemModifier_ovb0187)];

    UnkMinimapItemModifier_ovb0187* begin() { return reinterpret_cast<UnkMinimapItemModifier_ovb0187*>(storage); }
    UnkMinimapItemModifier_ovb0187* end() { return begin() + count; }
};

struct UnkMinimapItem {
    void* icon;
    u8 pad_0x8[0x58];
    UnkMinimapItemModifiers_ovb0187 alpha_modifiers;  // 0x60
    UnkMinimapItemModifiers_ovb0187 size_modifiers;   // 0x2ec
};
static_assert(__builtin_offsetof(UnkMinimapItem, size_modifiers) == 0x2ec, "UnkMinimapItem layout");

void fn_7100eb8978(UnkMinimapItem* item, f32 dt) {
    if (!item->icon)
        return;
    for (UnkMinimapItemModifier_ovb0187& it : item->alpha_modifiers)
        fn_7100eb7470(&it, dt);
    for (UnkMinimapItemModifier_ovb0187& it : item->size_modifiers)
        fn_7100eb7470(&it, dt);
}
