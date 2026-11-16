// CPlayerInfo's unit (tools/tu_map.py unit 2592): removes the flag entry whose weak handle refers to
// `source` from the player info's inline list at +0x90 (later entries move down one place).
// Names and layouts are placeholders.
#include <string.h>

#include <algorithm>

#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#pragma pack(push, 4)
struct UnkFlagEntry_ovb0071 {
    UnkWeakHandle* handle;
    u32 value;
    bool flag;
    bool field_0xd;
    u8 pad_0xe[2];
};
#pragma pack(pop)

struct UnkPlayerInfo_ovb0071;

// An inline vector of up to four entries: its count, then raw storage for the entries.
struct UnkFlagList_lf0d88c {
    u32 count;
    u8 storage[4 * sizeof(UnkFlagEntry_ovb0071)];

    UnkFlagEntry_ovb0071* begin() { return reinterpret_cast<UnkFlagEntry_ovb0071*>(storage); }
    UnkFlagEntry_ovb0071* end() { return begin() + count; }
};

struct UnkPlayerInfoFlags_lf0d88c {
    u8 pad_0x0[0x90];
    UnkFlagList_lf0d88c flags;  // 0x90
};

static inline UnkWeakTarget* Target(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

static inline void Assign(UnkFlagEntry_ovb0071* self, const UnkFlagEntry_ovb0071* other) {
    CopyWeakRef(&self->handle, other->handle);
    self->value = other->value;
    self->flag = other->flag;
    self->field_0xd = other->field_0xd;
}

static inline void Resize(UnkFlagList_lf0d88c* self, u32 count) {
    if (count < self->count) {
        for (u32 i = count; i < self->count; ++i) {
            UnkWeakHandle* handle = self->begin()[i].handle;
            if (handle && --handle->ref_count == 0)
                handle->vf_0x10();
        }
    } else if (count > self->count) {
        memset(self->end(), 0, (count - self->count) * sizeof(UnkFlagEntry_ovb0071));
    }
    self->count = count;
}

void fn_7100f0d88c(UnkPlayerInfo_ovb0071* player, void* source) {
    UnkFlagList_lf0d88c* flags = &reinterpret_cast<UnkPlayerInfoFlags_lf0d88c*>(player)->flags;
    UnkFlagEntry_ovb0071* it =
        std::find_if(flags->begin(), flags->end(),
                     [source](const UnkFlagEntry_ovb0071& entry) { return Target(entry.handle) == source; });
    for (; it + 1 != flags->end(); ++it)
        Assign(it, it + 1);
    Resize(flags, flags->count - 1);
}
