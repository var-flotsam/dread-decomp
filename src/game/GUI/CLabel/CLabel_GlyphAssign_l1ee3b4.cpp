// GUI::CLabel's unit (2752): the copy-assignment of a run's glyph array: the glyphs are destroyed, the
// array is reserved to the other's capacity and the other's glyphs are appended one by one. The glyph
// is src/game/GUI/CLabel.cpp's UnkLabelGlyph, read here with the place (0x10, three words) as a member
// with its own copy constructor. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2744.h"
#include "types.h"

using base::global::CStrId;

// Where a glyph is drawn: its code word and position, copied word by word.
struct UnkGlyphPlace_l1ee3b4 {
    u32 code;
    f32 x;
    f32 y;

    UnkGlyphPlace_l1ee3b4(const UnkGlyphPlace_l1ee3b4& other) : code(other.code), x(other.x), y(other.y) {}
};

struct UnkLabelGlyph_l1ee3b4 {
    u32 kind;
    CStrId id;  // 0x8
    UnkGlyphPlace_l1ee3b4 place;  // 0x10
    u32 field_0x1c;
    f32 scale;
    s32 frame;     // 0x24
    void* object;  // 0x28

    UnkLabelGlyph_l1ee3b4(const UnkLabelGlyph_l1ee3b4& other) = default;
    ~UnkLabelGlyph_l1ee3b4() {
        if (object && frame != 0x7fffffff) {
            fn_71011bab80(g_unk_717271e8f8, object, frame);
            object = nullptr;
        }
    }
};
static_assert(sizeof(UnkLabelGlyph_l1ee3b4) == 0x30, "UnkLabelGlyph_l1ee3b4 layout");

using UnkGlyphVector_l1ee3b4 = base::global::CRntVector<UnkLabelGlyph_l1ee3b4>;

// Appends a copy of a glyph (the array grown by its step first).
static inline void AddGlyph(UnkGlyphVector_l1ee3b4* glyphs, const UnkLabelGlyph_l1ee3b4& glyph) {
    u32 n = glyphs->count;
    u32 capacity = glyphs->capacity;
    if (capacity <= n && glyphs->step != 0)
        capacity = glyphs->GrownCapacity(n);
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_71011f05d4(reinterpret_cast<UnkGlyphVector*>(glyphs), capacity);
    new (&glyphs->data[glyphs->count]) UnkLabelGlyph_l1ee3b4(glyph);
    ++glyphs->count;
}

UnkGlyphVector* fn_71011ee3b4(UnkGlyphVector* self_, const UnkGlyphVector* other_) {
    auto* self = reinterpret_cast<UnkGlyphVector_l1ee3b4*>(self_);
    auto* other = reinterpret_cast<const UnkGlyphVector_l1ee3b4*>(other_);
    if (self != other) {
        self->Clear();
        fn_71011f05d4(self_, other->capacity);
        for (const UnkLabelGlyph_l1ee3b4 *it = other->data, *end = it + other->count; it != end; ++it)
            AddGlyph(self, *it);
    }
    return self_;
}
