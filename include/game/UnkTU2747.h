#pragma once

// UnkTU2747: functions of this translation unit that other files call (tools/tu_map.py unit 2747,
// 0x71011c198c-0x71011d6a04). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkChangeMenuStateButton_b48a3;
struct UnkGallery_A5_b75;
struct UnkGuiButtonFocus_ovb0000;
struct UnkGuiEvent_b48a3;
void fn_71011cf364(void* composition, s32 value);
void fn_71011cd4b4(void* self);  // The display object's set-up (derived classes call it from their constructors).
void fn_71011ccfb4(char*);  // The display object's destructor.
void fn_71011cef74(void* composition, void* owner);
void fn_71011cf39c(UnkChangeMenuStateButton_b48a3* self, UnkGuiEvent_b48a3* event);
void fn_71011cf630(UnkGuiButtonFocus_ovb0000* self);
void fn_71011cf70c(UnkGallery_A5_b75* self);
void fn_71011cff5c(void* a, void* b, void* c, bool d, bool e, bool f);
