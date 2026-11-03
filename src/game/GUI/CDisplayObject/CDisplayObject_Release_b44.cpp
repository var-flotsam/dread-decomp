// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): releases what the object set up
// (when +0x180 says it did): vf_0x40, the GUI root is told the object lost the focus when it has it,
// the object at +0x128 and each condition (+0x100 many, from +0x104, an unaligned array of pointers,
// as in CDisplayObject.cpp) are deleted through their vtables, the count and the flag are cleared, then
// vf_0x48. Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

struct UnkGuiOwned_b44a3 {
    virtual ~UnkGuiOwned_b44a3();
};

struct UnkGuiReleaseView_b44a3 {
    u8 pad_0x0[0x100];
    u32 condition_count;  // 0x100
    u8 conditions[1];     // 0x104
    u8 pad_0x105[0x23];
    UnkGuiOwned_b44a3* field_0x128;
    u8 pad_0x130[0x50];
    bool field_0x180;

    UnkGuiOwned_b44a3** begin() { return reinterpret_cast<UnkGuiOwned_b44a3**>(conditions); }
    UnkGuiOwned_b44a3** end() { return begin() + condition_count; }
};
static_assert(__builtin_offsetof(UnkGuiReleaseView_b44a3, field_0x180) == 0x180, "placeholder layout");

void fn_71011cd340(UnkGuiDisplayObject* self) {
    UnkGuiReleaseView_b44a3* view = reinterpret_cast<UnkGuiReleaseView_b44a3*>(self);
    if (!view->field_0x180)
        return;
    self->vf_0x40();
    if (self->field_0x181)
        fn_71011b5d94(g_unk_717271e8f8, self);
    if (view->field_0x128) {
        delete view->field_0x128;
        view->field_0x128 = nullptr;
    }
    for (UnkGuiOwned_b44a3** it = view->begin(); it != view->end(); ++it)
        delete *it;
    view->condition_count = 0;
    view->field_0x180 = false;
    self->vf_0x48();
}
