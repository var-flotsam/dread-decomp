// GUI::CHUD (unit 2751): tints every child of the HUD white or clear (by `shown`; the child with name
// hash 0x6b2c8fa02cae947e by `highlight`, or not at all when `keep` is set) over a quarter second, and
// keeps `shown` at +0x234. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkColor_1e50 {
    f32 r, g, b, a;
    UnkColor_1e50(f32 r, f32 g, f32 b, f32 a) : r(r), g(g), b(b), a(a) {}
};

void fn_71011ce488(UnkGuiDisplayObject* self, const UnkColor_1e50* color);

// A display object as far as its name id (+0x130).
struct UnkNamedObject_1e50 {
    u8 pad_0x0[0x130];
    UnkStringData* name;  // 0x130
};

struct UnkChildLink_1e50 {
    UnkGuiDisplayObject* child;
    UnkChildLink_1e50* next;
};

struct UnkHud_1e50 {
    u8 pad_0x0[0x1d0];
    UnkChildLink_1e50* children;  // 0x1d0
    u8 pad_0x1d8[0x5c];
    bool shown;  // 0x234
};
static_assert(__builtin_offsetof(UnkHud_1e50, shown) == 0x234, "UnkHud_1e50 layout");

static inline u64 NameHash(UnkGuiDisplayObject* object) {
    return reinterpret_cast<UnkNamedObject_1e50*>(object)->name->Key();
}

// Best graded form (16.4%): the original unswitches the loop on `shown` and tests `keep` and
// `highlight` inside; this one unswitches on `keep`.
void fn_71011e1e50(UnkHud_1e50* self, bool shown, bool keep, bool highlight) {
    for (UnkChildLink_1e50* link = self->children; link; link = link->next) {
        bool lit;
        if (NameHash(link->child) != 0x6b2c8fa02cae947e) {
            lit = shown;
        } else {
            if (keep)
                continue;
            lit = highlight;
        }
        UnkColor_1e50 color = lit ? UnkColor_1e50(1.0f, 1.0f, 1.0f, 1.0f) : UnkColor_1e50(0.0f, 0.0f, 0.0f, 0.0f);
        fn_71011ce488(link->child, &color);
        fn_71011ce208(link->child, 0.25f);
    }
    self->shown = shown;
}