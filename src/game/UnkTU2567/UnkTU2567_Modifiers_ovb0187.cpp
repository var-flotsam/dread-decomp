// In unit 2567 of tools/tu_map.py (the minimap's items; src/game/CMinimapManager.cpp lays the item out in full,
// src/game/UnkUnit2567/fn_7100eb7f6c.cpp reads it): advances every alpha modifier and every size modifier
// of an item that has an icon by `dt` (fn_7100eb7470, the bouncing curve's step). Names and layouts are
// placeholders.
#include "game/UnkTU2567.h"
#include "types.h"

struct UnkMinimapItemModifier {
    u8 pad_0x0[0xd8];
};
static_assert(sizeof(UnkMinimapItemModifier) == 0xd8, "UnkMinimapItemModifier layout");

struct UnkMinimapItemModifiers {
    u32 count;
    alignas(4) u8 storage[3 * sizeof(UnkMinimapItemModifier)];

    UnkMinimapItemModifier* begin() { return reinterpret_cast<UnkMinimapItemModifier*>(storage); }
    UnkMinimapItemModifier* end() { return begin() + count; }
};

struct UnkMinimapItem {
    void* icon;
    u8 pad_0x8[0x58];
    UnkMinimapItemModifiers alpha_modifiers;  // 0x60
    UnkMinimapItemModifiers size_modifiers;   // 0x2ec
};
static_assert(__builtin_offsetof(UnkMinimapItem, size_modifiers) == 0x2ec, "UnkMinimapItem layout");

void fn_7100eb8978(UnkMinimapItem* item, f32 dt) {
    if (!item->icon)
        return;
    for (UnkMinimapItemModifier& it : item->alpha_modifiers)
        fn_7100eb7470(&it, dt);
    for (UnkMinimapItemModifier& it : item->size_modifiers)
        fn_7100eb7470(&it, dt);
}
