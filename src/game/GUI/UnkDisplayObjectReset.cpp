// The same routine in three GUI classes' units (GUI::CDebugMenuEntryItemRenderer, fn_7100aff9d4;
// GUI::CLegendMenuEntryItemRenderer, fn_7100b33550; GUI::CButton, fn_71011c0098), as fn_71006ef540
// (src/game/CYamplotXAIComponent/fn_71006ef540.cpp) does for nodes: the base routine fn_71011d90f4 runs,
// then two words of the object are cleared (at +0x230 or +0x278). Names are placeholders.
#include "types.h"

template <u32 Offset>
struct UnkGuiResetObject {
    u8 pad_0x0[Offset];
    u64 first;
    u64 second;
};

// The display object's routine (not decompiled yet).
struct UnkGuiDisplayBase;
void fn_71011d90f4(UnkGuiDisplayBase* self);

template <u32 Offset>
__attribute__((always_inline)) static inline void Reset(UnkGuiResetObject<Offset>* self) {
    fn_71011d90f4(reinterpret_cast<UnkGuiDisplayBase*>(self));
    self->second = 0;
    self->first = 0;
}

void fn_7100aff9d4(UnkGuiResetObject<0x230>* self) { Reset(self); }
void fn_7100b33550(UnkGuiResetObject<0x230>* self) { Reset(self); }
void fn_71011c0098(UnkGuiResetObject<0x278>* self) { Reset(self); }
