// CHearingSensor unit (unit 2580 of tools/tu_map.py): a destructor that restores its vtable and releases a
// shared object (+0x10) whose reference count sits at +0x30. Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7101caa308[];  // the object's vtable

struct UnkHearingShared {
    virtual ~UnkHearingShared();
    u8 pad_0x8[0x28];
    s32 ref_count;
};

struct UnkHearingHolder {
    char* vtable;
    u8 pad_0x8[0x8];
    UnkHearingShared* shared;
};

void fn_7100f03818(UnkHearingHolder* self) {
    self->vtable = g_unk_7101caa308 + 0x10;
    UnkHearingShared* shared = self->shared;
    if (shared && --shared->ref_count == 0)
        delete shared;
}
