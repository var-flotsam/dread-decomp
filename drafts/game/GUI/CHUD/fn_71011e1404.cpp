// GUI::CHUD (unit 2751): copies the HUD's minimap frame into the minimap's view settings (the set
// in use): its size in pixels, a scale of one and its anchored position, centred on the screen.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/tunable/CTunable.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2755.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::math::CVector2D;

// A 2D position returned in memory (its user-provided copy keeps it out of registers).
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
};

f32 fn_71011cdf7c(UnkGuiDisplayObject* object);

extern const CVector2D g_unk_7101647788;
__attribute__((visibility("hidden"))) extern CVector2D g_unk_7101cb6378;

// The minimap frame: its anchoring at +0x78 and its size at +0x8c.
struct UnkHudFrame_1404 {
    u8 pad_0x0[0x78];
    u8 anchor[0x4];  // 0x78
    u32 vertical;    // 0x7c
    u8 pad_0x80[0xc];
    CVector2D size;  // 0x8c

    const UnkGuiItemState* AnchorState() const { return reinterpret_cast<const UnkGuiItemState*>(anchor); }
};
static_assert(__builtin_offsetof(UnkHudFrame_1404, size) == 0x8c, "UnkHudFrame_1404 layout");

struct UnkHudRef_1404 {
    u8 pad_0x0[0x10];
    UnkHudFrame_1404* target;  // 0x10
};

struct UnkHud_1404 {
    u8 pad_0x0[0x2c8];
    UnkHudRef_1404* minimap;  // 0x2c8
};

// One set of the minimap's view settings.
struct UnkMinimapSettings_1404 {
    u8 pad_0x0[0xc];
    CVector2D position;  // 0xc
    CVector2D size;      // 0x14
    f32 scale;           // 0x1c
    u8 pad_0x20[0x18];
};
static_assert(sizeof(UnkMinimapSettings_1404) == 0x38, "UnkMinimapSettings_1404 layout");

struct UnkMinimap_1404 {
    u8 pad_0x0[0x98];
    UnkMinimapSettings_1404 settings[2];  // 0x98, 0xd0
    u8 pad_0x108[0xd63d38];
    u8 timer[0xb0];  // 0xd63e40

    UnkMinimapSettings_1404& Current() { return fn_710002fb70(timer) ? settings[1] : settings[0]; }
};
static_assert(__builtin_offsetof(UnkMinimap_1404, timer) == 0xd63e40, "UnkMinimap_1404 layout");

static inline UnkHudFrame_1404* Target(UnkHudRef_1404* ref) {
    return ref ? ref->target : nullptr;
}

void fn_71011e1404(UnkHud_1404* self) {
    if (!Target(self->minimap))
        return;
    UnkMinimap_1404* minimap = static_cast<UnkMinimap_1404*>(g_unk_7101d47310->field_0x2228);
    u32 screen_width = fn_7100132fc0();
    u32 screen_height = fn_7100132fd0();
    CVector2D screen = {static_cast<f32>(screen_width), static_cast<f32>(screen_height)};
    // The frame's size in screen units, from the screen's origin, in pixels.
    CVector2D origin = {0.0f, 0.0f};
    CVector2D offset = {origin.x / screen.x * 2.0f, origin.y / screen.y * 2.0f};
    UnkHudFrame_1404* frame = self->minimap->target;
    CVector2D size = {(frame->size.x - offset.x) * screen.x, (frame->size.y - offset.y) * screen.y};
    g_unk_7101cb6378 = size;
    minimap->Current().size = size;
    minimap->Current().scale = 1.0f;

    UnkHudFrame_1404* anchor = Target(self->minimap);
    UnkGuiPos anchored = fn_71011f6240(anchor->AnchorState(), 1, anchor->vertical);
    f32 y = fn_71011cdf7c(reinterpret_cast<UnkGuiDisplayObject*>(Target(self->minimap)));
    CVector2D position = {anchored.x * 2.0f - g_unk_7101647788.x, y * 2.0f - g_unk_7101647788.y};
    minimap->Current().position = position;
}
