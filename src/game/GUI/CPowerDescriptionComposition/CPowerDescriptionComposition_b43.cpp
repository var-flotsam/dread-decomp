// GUI::CPowerDescriptionComposition's unit (tools/tu_map.py): its update, after the base one
// (fn_71011d9050): while active (+0xb4) and with both its pages (+0x2d0) and its source (+0x2d8), a
// source with pages hides one arrow (+0x2f0), shows the continue arrow (+0x2e8) playing
// "ContinueArrow_Loop" (its vf_0x1e8) and the bar (+0x2c0), whose length (+0x8c) over the page count
// becomes the step (+0x2e0), and resets the pages (fn_71011ccd20 with 0); one without pages hides the
// bar and both arrows. Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "types.h"

void fn_71011d9050(void* self);  // the base composition's update

struct UnkPowerDescSource_b43a3 {
    u8 pad_0x0[0x18c];
    s32 page_count;
};


struct UnkPowerDescComposition_b43a3 {
    u8 pad_0x0[0xb4];
    bool active;  // 0xb4
    u8 pad_0xb5[0x2c0 - 0xb5];
    UnkGuiDisplayObject* bar;  // 0x2c0
    u8 pad_0x2c8[0x8];
    UnkGuiDisplayObject* pages;  // 0x2d0
    UnkPowerDescSource_b43a3* source;  // 0x2d8
    f32 step;  // 0x2e0
    UnkGuiDisplayObject* continue_arrow;  // 0x2e8
    UnkGuiDisplayObject* arrow;  // 0x2f0
};
static_assert(__builtin_offsetof(UnkPowerDescComposition_b43a3, arrow) == 0x2f0, "UnkPowerDescComposition_b43a3 layout");

static inline void PlayAnimation_b43a3(UnkGuiDisplayObject* object, const char* name, s32 flags) {
    typedef void (*Fn)(UnkGuiDisplayObject*, const char*, s32);
    (*reinterpret_cast<Fn*>(*reinterpret_cast<char**>(object) + 0x1e8))(object, name, flags);
}

void fn_7100b63bc4(UnkPowerDescComposition_b43a3* self) {
    fn_71011d9050(self);
    if (!self->active || !self->pages || !self->source)
        return;
    if (self->source->page_count >= 1) {
        fn_71011cdc30(self->arrow, false);
        fn_71011cdc30(self->continue_arrow, true);
        PlayAnimation_b43a3(self->continue_arrow, "ContinueArrow_Loop", 0);
        fn_71011cdc30(self->bar, true);
        self->step = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(self->bar) + 0x8c) / (f32)self->source->page_count;
        fn_71011ccd20(self->pages, 0.0f);
    } else {
        fn_71011cdc30(self->bar, false);
        fn_71011cdc30(self->arrow, false);
        fn_71011cdc30(self->continue_arrow, false);
    }
}
