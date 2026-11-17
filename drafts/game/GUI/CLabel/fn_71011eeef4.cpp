// GUI::CLabel's unit (2752): passes the label's depth on to its runs: every run of every group gets the
// depth, and so does the object of each of its glyphs (the glyph array grows to the index it is read at,
// as the engine's growing accessor does). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU287.h"
#include "types.h"

using base::global::CStrId;

void fn_71011cedec(UnkGuiDisplayObject* self);
void fn_710020c248(void* self, s32 value);

// The object made for a glyph: the scene node its depth is set on.
struct UnkGlyphObject_l1eeef4 {
    u8 unk_0x0[0x18];
    u8 node;
};

// A glyph as src/game/GUI/CLabel.cpp lays it out, with its empty form.
struct UnkLabelGlyph_l1eeef4 {
    u32 kind;
    CStrId id;  // 0x8
    u8 font;
    u16 code;
    f32 x;
    f32 y;
    u32 field_0x1c;
    f32 scale;
    s32 frame;                        // 0x24
    UnkGlyphObject_l1eeef4* object;  // 0x28

    UnkLabelGlyph_l1eeef4()
        : kind(0x7fffffff), id("", true), font(0), code(0), x(0.0f), y(0.0f), field_0x1c(0), scale(1.0f),
          frame(0x7fffffff), object(nullptr) {}
};
static_assert(sizeof(UnkLabelGlyph_l1eeef4) == 0x30, "UnkLabelGlyph_l1eeef4 layout");

using UnkGlyphVector_l1eeef4 = base::global::CRntVector<UnkLabelGlyph_l1eeef4>;

struct UnkRunNode_l1eeef4 {
    u8 unk_0x0[0x20];
    u8 field_0x20;
};

struct UnkLabelRun_l1eeef4 {
    u8 unk_0x0[0x40];
    UnkGlyphVector_l1eeef4 glyphs;  // 0x40
    UnkRunNode_l1eeef4* field_0x68;
};
static_assert(__builtin_offsetof(UnkLabelRun_l1eeef4, field_0x68) == 0x68, "UnkLabelRun_l1eeef4 layout");

struct UnkLabelRunNode_l1eeef4 {
    UnkLabelRun_l1eeef4* run;
    UnkLabelRunNode_l1eeef4* next;
};
struct UnkLabelGroup_l1eeef4 {
    u8 unk_0x0[0x18];
    UnkLabelRunNode_l1eeef4* runs;
};
struct UnkLabelGroupNode_l1eeef4 {
    UnkLabelGroup_l1eeef4* group;
    UnkLabelGroupNode_l1eeef4* next;
};

struct UnkLabel_l1eeef4 {
    u8 unk_0x0[0x13c];
    s32 field_0x13c;
    s32 field_0x140;  // 0x7fffffff when unset: field_0x13c is used instead
    u8 unk_0x144[0x18c];
    UnkLabelGroupNode_l1eeef4* groups;  // +0x2d0
};
static_assert(__builtin_offsetof(UnkLabel_l1eeef4, groups) == 0x2d0, "UnkLabel_l1eeef4 layout");

static inline void Reserve(UnkGlyphVector_l1eeef4* glyphs, u32 capacity) {
    fn_71011f05d4(reinterpret_cast<UnkGlyphVector*>(glyphs), capacity);
}

// The glyph at `index`; a growing array is first grown to hold it, with empty glyphs.
static inline UnkLabelGlyph_l1eeef4& GlyphAt(UnkGlyphVector_l1eeef4* glyphs, u64 index) {
    if (glyphs->field_0x14 && glyphs->count <= index) {
        u32 capacity = glyphs->capacity;
        if (capacity <= index && glyphs->step != 0)
            capacity = ((u32)index / glyphs->step + 1) * glyphs->step;
        Reserve(glyphs, capacity);
        u64 n = index + 1;
        Reserve(glyphs, n);
        for (u64 i = glyphs->count; i < n; ++i)
            new (&glyphs->data[i]) UnkLabelGlyph_l1eeef4();
        glyphs->count = n;
    }
    return glyphs->data[index];
}

void fn_71011eeef4(UnkGuiDisplayObject* label) {
    UnkLabel_l1eeef4* self = reinterpret_cast<UnkLabel_l1eeef4*>(label);
    fn_71011cedec(label);
    s32 depth = self->field_0x140;
    if (depth == 0x7fffffff)
        depth = self->field_0x13c;
    for (UnkLabelGroupNode_l1eeef4* group = self->groups; group; group = group->next) {
        for (UnkLabelRunNode_l1eeef4* node = group->group->runs; node; node = node->next) {
            UnkLabelRun_l1eeef4* run = node->run;
            fn_710020c248(&run->field_0x68->field_0x20, depth);
            u32 count = run->glyphs.count;
            for (u64 i = 0; i < count; ++i) {
                UnkLabelGlyph_l1eeef4& glyph = GlyphAt(&run->glyphs, i);
                if (glyph.object)
                    fn_710020ad90(&glyph.object->node, depth);
            }
        }
    }
}
