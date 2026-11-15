// Unit 324 (media instance): picks the entry of list `kind` whose name starts with a 3-character
// prefix, or the first entry, as the current one. Names and layouts are placeholders.
#include <algorithm>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntString.h"
#include "game/UnkTU324.h"
#include "types.h"

bool fn_710007ee58(const base::global::CRntString* self, const char* str, u32 n);

struct UnkMediaEntry324 {
    u8 pad_0x0[0x38];
    base::global::CRntString name;  // 0x38
};

struct UnkMediaEntryList324 {
    UnkMediaEntry324** entries;  // 0x0
    u32 count;                   // 0x8
    u8 pad_0xc[0x1c];
};

struct UnkMediaSelect324 {
    u8 pad_0x0[0x50];
    UnkMediaEntry324* current[8];  // 0x50
    u8 pad_0x90[0x640];
    UnkMediaEntryList324 lists[8];  // 0x6d0
    u8 pad_0x810[0x150];
    nn::os::MutexType lock;  // 0x960
};
static_assert(__builtin_offsetof(UnkMediaSelect324, lists) == 0x6d0, "UnkMediaSelect324 layout");
static_assert(__builtin_offsetof(UnkMediaSelect324, lock) == 0x960, "UnkMediaSelect324 layout");

u32 fn_710021f704(UnkMediaSelect324* self, s32 kind, const char* prefix) {
    fn_710006691c(&self->lock);
    s64 index = kind;
    if (prefix) {
        UnkMediaEntry324** it = std::find_if(self->lists[index].entries, self->lists[index].entries + self->lists[index].count, [prefix](UnkMediaEntry324* entry) {
            const char* name = entry->name.data;
            return name && *name && fn_710007ee58(&entry->name, prefix, 3);
        });
        if (it != self->lists[index].entries + self->lists[index].count) {
            self->current[index] = *it;
            fn_7100066920(&self->lock);
            return 1;
        }
    }
    fn_710006691c(&self->lock);
    u32 result;
    if (self->lists[index].count) {
        self->current[index] = self->lists[index].entries[0];
        result = 1;
    } else {
        result = 5;
    }
    fn_7100066920(&self->lock);
    fn_7100066920(&self->lock);
    return result;
}
