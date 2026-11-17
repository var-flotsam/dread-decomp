// Unit 1699 (tools/tu_map.py): walks a list of overlay objects and calls a virtual setter (100, false) on
// each one named Black, Vignette_T, Vignette_B, Grid or Vignette_HL. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkOverlay_lb51494 {
    void** vtable;
    u8 pad_0x8[0x130 - 0x8];
    CStrId name;  // 0x130

    bool IsNamed(const char* text) const { return name == CStrId(text, true); }

    void Set(s32 value, bool flag) {
        reinterpret_cast<void (*)(UnkOverlay_lb51494*, s32, bool)>(vtable[0x78 / 8])(this, value, flag);
    }
};
static_assert(__builtin_offsetof(UnkOverlay_lb51494, name) == 0x130, "layout");

struct UnkOverlayNode_lb51494 {
    UnkOverlay_lb51494* value;
    UnkOverlayNode_lb51494* next;
};

struct UnkOverlayOwner_lb51494 {
    u8 pad_0x0[0x1d0];
    UnkOverlayNode_lb51494* overlays;  // 0x1d0
};

struct UnkOverlayHolder_lb51494 {
    void** vtable;
    UnkOverlayOwner_lb51494* owner;  // 0x8
};

void fn_7100b51494(UnkOverlayHolder_lb51494* self) {
    for (UnkOverlayNode_lb51494* node = self->owner->overlays; node; node = node->next) {
        UnkOverlay_lb51494* overlay = node->value;
        if (overlay->IsNamed("Black") || overlay->IsNamed("Vignette_T") || overlay->IsNamed("Vignette_B") ||
            overlay->IsNamed("Grid") || overlay->IsNamed("Vignette_HL"))
            overlay->Set(100, false);
    }
}
