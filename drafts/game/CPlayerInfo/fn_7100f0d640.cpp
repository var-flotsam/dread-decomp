// CPlayerInfo unit: appends an entry (a counted reference to the target's weak handle, a value and a
// zeroed flag word) to the 16-byte entry list at +0x94 (count at +0x90). Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTargetEntry_lf0d640 {
    UnkWeakHandle* handle;
    u32 value;
    u16 flags;
};

struct __attribute__((packed)) UnkTargetList_lf0d640 {
    u8 pad_0x0[0x90];
    u32 count;
    UnkTargetEntry_lf0d640 entries[1];
};

// Holds a counted reference to the handle while it is used.
struct UnkHeldHandle_lf0d640 {
    UnkWeakHandle* handle;
    explicit UnkHeldHandle_lf0d640(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHeldHandle_lf0d640() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

void fn_7100f0d640(UnkTargetList_lf0d640* self, UnkWeakTarget* target, u32 value) {
    UnkHeldHandle_lf0d640 held(GetWeakHandle(target));
    u32 n = self->count;
    UnkTargetEntry_lf0d640* entries = self->entries;
    entries[n].handle = nullptr;
    AssignRef(&entries[n].handle, held.handle);
    entries[n].value = value;
    entries[n].flags = 0;
    ++self->count;
}
