// CInputComponent unit: constructs an input map: its name string, an empty slot (-1) with its two
// pointers, 119 bindings that start unbound (0xfe), a trailing unbound binding and an enabled flag.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkInputBinding_l8717d4 {
    s32 id;
    u32 values[4];

    UnkInputBinding_l8717d4() : id(0xfe), values() {}
};
static_assert(sizeof(UnkInputBinding_l8717d4) == 0x14, "");

struct UnkInputMap_l8717d4 {
    base::global::CRntString name;
    s32 slot;
    bool active;
    void* field_0x38;
    void* field_0x40;
    UnkInputBinding_l8717d4 bindings[119];
    s32 default_id;
    u32 default_value;
    u32 enabled;
};
static_assert(__builtin_offsetof(UnkInputMap_l8717d4, bindings) == 0x48, "");
static_assert(__builtin_offsetof(UnkInputMap_l8717d4, enabled) == 0x99c, "");

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_71008717d4(UnkInputMap_l8717d4* self, const char* key) {
    new (&self->name) base::global::CRntString(key);
    self->slot = -1;
    self->active = false;
    self->field_0x38 = nullptr;
    self->field_0x40 = nullptr;
    for (UnkInputBinding_l8717d4& binding : self->bindings)
        new (&binding) UnkInputBinding_l8717d4();
    self->default_id = 0xfe;
    self->default_value = 0;
    self->enabled = 1;
}
