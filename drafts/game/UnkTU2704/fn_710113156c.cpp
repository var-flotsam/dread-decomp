// CSpawnPointComponent unit (2704): erases the entry at the given index from the 0x40-byte entry array
// (UnkVector::EraseAt): destroys it, moves the later entries down one place, destroying each moved-from one.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_710111fd80(void*, void*);

struct UnkEntry_l13156c {
    virtual ~UnkEntry_l13156c();
    u8 pad_0x8[0x38];
};

void fn_710113156c(base::UnkVector<UnkEntry_l13156c>* self, const u32* index) {
    u32 first = *index;
    if (self->count > first) {
        u64 i = first;
        self->data[i].~UnkEntry_l13156c();
        u32 last = self->count - 1;
        if (first < last) {
            do {
                fn_710111fd80(&self->data[i], &self->data[i + 1]);
                self->data[i + 1].~UnkEntry_l13156c();
                ++i;
                last = self->count - 1;
            } while (i < last);
        }
        self->count = last;
    }
}
