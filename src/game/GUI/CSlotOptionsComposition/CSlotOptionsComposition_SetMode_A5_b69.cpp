// GUI::CSlotOptionsComposition: stores a mode at +0x230, then builds a GUI event of kind 0 for itself
// and hands it to its own vf_0x1a8. The layout is a placeholder.
#include "game/UnkGuiEvent.h"

struct UnkSlotOptions_A5_b69 : UnkGuiDisplayObject {
    u8 pad_0x198[0x98];
    u32 field_0x230;
};
static_assert(__builtin_offsetof(UnkSlotOptions_A5_b69, field_0x230) == 0x230, "layout");

void fn_7100b7c648(UnkSlotOptions_A5_b69* self, u32 mode) {
    self->field_0x230 = mode;
    s32 kind = 0;
    UnkGuiEvent event;
    fn_710120235c(&event, self, &kind, nullptr);
    (*reinterpret_cast<void (***)(UnkGuiDisplayObject*, UnkGuiEvent*)>(self))[0x1a8 / 8](self, &event);
    fn_7101202488(&event);
}
