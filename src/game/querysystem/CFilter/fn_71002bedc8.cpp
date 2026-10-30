// querysystem::CFilter's constructor (unit 383 of tools/tu_map.py, the name from reflection): the
// filters' base. Sets the vtable, names the filter "XXXX" (a placeholder the derived constructors
// replace) and clears the flag at +0x10. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

extern char g_unk_7101a21348[];  // CFilter's vtable

struct UnkQueryParam {
    char* vtable;
    UnkStrId name;
    bool field_0x10;
};

void fn_71002bedc8(UnkQueryParam* self) {
    self->vtable = g_unk_7101a21348 + 0x10;
    new (&self->name) UnkStrId("XXXX", true);
    self->field_0x10 = false;
}
