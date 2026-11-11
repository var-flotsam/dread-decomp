// GUI::CPowerDescriptionButton's unit (tools/tu_map.py unit 1710, named by reflection): the
// constructor, on GUI::CButton's (fn_71011bf0ec): the vtable, two empty strings (+0x2a8, +0x2d8; the
// string's default constructor fn_710007e6c4), two empty string ids and three cleared words. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CPowerDescriptionButton.h"
#include "types.h"

using base::global::CStrId;

struct UnkGuiButton;
void fn_710007e6c4(void*);               // the string's default constructor

// An empty id: the string id constructor for "" (the length folded) inlined.
__attribute__((always_inline)) static inline void InitEmptyId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

struct UnkPowerDescriptionButton {
    char* vtable;
    u8 base_0x8[0x2a0];
    u8 field_0x2a8[0x30];  // strings (base::global::CRntString's layout)
    u8 field_0x2d8[0x30];
    CStrId field_0x308;
    CStrId field_0x310;
    u64 field_0x318;
    u64 field_0x320;
    u64 field_0x328;
};
static_assert(__builtin_offsetof(UnkPowerDescriptionButton, field_0x328) == 0x328, "layout");

void fn_7100b626f0(UnkPowerDescriptionButton* self) {
    fn_71011bf0ec(reinterpret_cast<UnkGuiButton*>(self));
    self->vtable = g_unk_7101a9dcb0 + 0x10;
    fn_710007e6c4(self->field_0x2a8);
    fn_710007e6c4(self->field_0x2d8);
    InitEmptyId(&self->field_0x308);
    InitEmptyId(&self->field_0x310);
    self->field_0x318 = 0;
    self->field_0x320 = 0;
    self->field_0x328 = 0;
}
