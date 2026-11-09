// CCreditsMode::CTunableCredits's unit (tools/tu_map.py unit 2724). Names and layouts are
// placeholders.
#include "../../../game_s7/UnkGuiS7.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CRntString;

void* fn_7100deab70(UnkGameManager* self, const CRntString& name);

struct UnkCreditsRgb_ovb0070 {
    f32 r, g, b;
    // Copied component by component.
    void Set(const UnkCreditsRgb_ovb0070& o) {
        r = o.r;
        g = o.g;
        b = o.b;
    }
};

// A credits line: its text and its shadow, coloured by the tunable colour `kind` selects.
struct UnkCreditsLine_ovb0070 {
    unk_gui::UnkGuiObj* text;
    unk_gui::UnkGuiObj* shadow;
    u32 kind;
};

struct UnkCreditsNode_ovb0070 {
    UnkCreditsLine_ovb0070* line;
    UnkCreditsNode_ovb0070* next;
};

struct UnkCreditsMenu_ovb0070 {
    u8 pad_0x0[0xa0];
    UnkCreditsNode_ovb0070* lines;
};

// The tunable colours by line kind.
extern UnkCreditsRgb_ovb0070* g_unk_7101cb4fb0[] __attribute__((visibility("hidden")));

struct UnkCreditsColor_ovb0070 {
    UnkCreditsRgb_ovb0070 rgb;
    f32 a;
    UnkCreditsColor_ovb0070(UnkCreditsRgb_ovb0070 rgb_, f32 a_) : rgb(rgb_), a(a_) {}
    UnkCreditsColor_ovb0070(f32 r, f32 g, f32 b, f32 a_) : rgb{r, g, b}, a(a_) {}
};

static inline void SetColor(unk_gui::UnkGuiObj* object, const UnkCreditsColor_ovb0070& color) {
    unk_gui::fn_71011cfc24(object, reinterpret_cast<const unk_gui::UnkColor&>(color), 1);
}

// Recolours every line of the "CREDITS" menu: the text by its kind, the shadow black.
__attribute__((always_inline)) static inline void UpdateColors() {
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager)
        return;
    UnkCreditsMenu_ovb0070* menu;
    {
        CRntString name("CREDITS");
        menu = static_cast<UnkCreditsMenu_ovb0070*>(fn_7100deab70(manager, name));
    }
    if (!menu)
        return;
    for (UnkCreditsNode_ovb0070* node = menu->lines; node; node = node->next) {
        UnkCreditsLine_ovb0070* line = node->line;
        if (!line || !line->text)
            continue;
        SetColor(line->text, UnkCreditsColor_ovb0070(*g_unk_7101cb4fb0[line->kind], 1.0f));
        if (!line->shadow)
            continue;
        SetColor(line->shadow, UnkCreditsColor_ovb0070(0.0f, 0.0f, 0.0f, 1.0f));
    }
}

struct UnkTunableCredits_ovb0070 {
    u8 pad_0x0[0x24];
    UnkCreditsRgb_ovb0070 field_0x24;
    UnkCreditsRgb_ovb0070 field_0x30;
    UnkCreditsRgb_ovb0070 field_0x3c;
};

void fn_710118618c(UnkTunableCredits_ovb0070* self, const UnkCreditsRgb_ovb0070* value) {
    self->field_0x24.Set(*value);
    UpdateColors();
}

void fn_710118634c(UnkTunableCredits_ovb0070* self, const UnkCreditsRgb_ovb0070* value) {
    self->field_0x30.Set(*value);
    UpdateColors();
}

void fn_710118650c(UnkTunableCredits_ovb0070* self, const UnkCreditsRgb_ovb0070* value) {
    self->field_0x3c.Set(*value);
    UpdateColors();
}
