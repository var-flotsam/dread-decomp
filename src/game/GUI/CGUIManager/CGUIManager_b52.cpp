// More of the GUI manager's unit (see CGUIManager.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkGuiRoot;
using base::global::CStrId;
void* fn_71011b882c(UnkGuiRoot*, const CStrId*);  // as src/game/GUI/CLabel.cpp declares it
const base::global::CRntString** fn_710109c444(void* vec, u32 index);

struct UnkGuiRoot;
struct UnkGlyphMetrics;  // src/game/GUI/CLabel.cpp

// The root's font names: a vector of string pointers at +0x188.
struct UnkGuiFontNames_b52 {
    u8 pad_0x0[0x188];
    u8 names[0x8];
    u32 count;
};

// A glyph's metrics in font `font` (signature as src/game/GUI/CLabel.cpp declares it): this version
// only builds (and drops) a string id from the font's name when the root has the font, and always
// fails.
bool fn_71011b89dc(UnkGuiRoot* root, u8 font, const base::global::CStrId* font_id, u32 code, UnkGlyphMetrics* out) {
    auto* self = reinterpret_cast<UnkGuiFontNames_b52*>(root);
    // The font lookup (CLabel.cpp declares it with the id; CGUIManager.cpp defines it without).
    auto lookup = reinterpret_cast<void* (*)(UnkGuiRoot*, const base::global::CStrId*)>(fn_71011b882c);
    if (lookup(root, font_id) && font < self->count) {
        const base::global::CRntString* name = *fn_710109c444(self->names, font);
        alignas(8) u8 storage[sizeof(base::global::CStrId)];
        auto* id = reinterpret_cast<base::global::CStrId*>(storage);
        fn_7100000850(id, *name, true);
        id->~CStrId();
    }
    return false;
}
