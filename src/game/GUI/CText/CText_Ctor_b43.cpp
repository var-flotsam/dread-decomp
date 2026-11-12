// GUI::CText unit (tools/tu_map.py): a text object's constructor, on its base (fn_71011cd4b4): three
// cleared words, the GUI root's default id (fn_71011b8920), a cleared word and a word of 1, a cleared
// word, two layout texts built empty (fn_7100066af0) and two flags set. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2744.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CStrId;

// As src/game/GUI/CLabel.cpp declares them.
struct UnkLayoutText;

struct UnkText_b43a3 {
    u8 base[0x185];
    bool field_0x185;
    u8 pad_0x186[0xa];
    u64 field_0x190;
    u64 field_0x198;
    u64 field_0x1a0;
    CStrId id;  // 0x1a8
    u64 field_0x1b0;
    u32 field_0x1b8;
    u8 pad_0x1bc[0x34];
    u32 field_0x1f0;
    u8 pad_0x1f4[0x4];
    u8 text_0x1f8[0x20];
    u8 text_0x218[0x20];
    bool field_0x238;
};
static_assert(__builtin_offsetof(UnkText_b43a3, field_0x238) == 0x238, "UnkText_b43a3 layout");

void fn_7100b86798(UnkText_b43a3* self) {
    fn_71011cd4b4(self);
    self->field_0x198 = 0;
    self->field_0x1a0 = 0;
    self->field_0x190 = 0;
    self->id = fn_71011b8920(g_unk_717271e8f8);
    self->field_0x1b0 = 0;
    self->field_0x1b8 = 1;
    self->field_0x1f0 = 0;
    fn_7100066af0(reinterpret_cast<UnkLayoutText*>(self->text_0x218), U"");
    fn_7100066af0(reinterpret_cast<UnkLayoutText*>(self->text_0x1f8), U"");
    self->field_0x185 = true;
    self->field_0x238 = true;
}
