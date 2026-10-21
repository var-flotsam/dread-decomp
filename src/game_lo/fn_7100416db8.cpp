// Constructor: runs the base constructor, stores the class's vtable, clears a 16-bit field and
// sets the file-path string id at +0x500 to the empty path (creating the pool if needed).
#include "base/global/CFilePathStrId.h"
#include "types.h"

extern char g_unk_7101a34218[];  // vtable

struct UnkObject_7100416db8 {
    char* vtable;
    u8 pad_0x8[0x4fc - 0x8];
    u16 field_0x4fc;
    u8 pad_0x4fe[0x2];
    base::global::CFilePathStrId field_0x500;  // constructed in place
};

void fn_7100413084(UnkObject_7100416db8* self);  // base constructor

void fn_7100416db8(UnkObject_7100416db8* self) {
    fn_7100413084(self);
    self->vtable = g_unk_7101a34218 + 0x10;
    self->field_0x4fc = 0;
    // The empty path: the id is cleared, the pool created if needed, then the empty string set.
    self->field_0x500.data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->field_0x500.data = &g_unk_7101d47200->empty;
}
