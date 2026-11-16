// base::file::CPackManagerDevice's unit (unit 66): deletes every entry of an owner's list that is
// idle and no longer referenced, taking it out of the list first. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// An entry of the list: slot 4 says whether it is busy; field_0xcc counts its users.
struct UnkEntry_l08b01c {
    virtual ~UnkEntry_l08b01c();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    u8 pad_0x8[0xc4];
    s32 field_0xcc;
};
static_assert(__builtin_offsetof(UnkEntry_l08b01c, field_0xcc) == 0xcc, "");

struct UnkOwner_l08b01c {
    u8 pad_0x0[0x30];
    base::global::CRntVector<UnkEntry_l08b01c*> entries;  // 0x30
};

void fn_710008b01c(UnkOwner_l08b01c* self) {
    if (self->entries.count == 0)
        return;
    base::global::CRntVector<UnkEntry_l08b01c*> unused(0x1b);
    UnkEntry_l08b01c** end = self->entries.data + self->entries.count;
    for (UnkEntry_l08b01c** it = self->entries.data; it != end; ++it) {
        UnkEntry_l08b01c* entry = *it;
        if (!entry->vf_0x20() && entry->field_0xcc <= 0)
            unused.PushBack(entry);
    }
    for (u32 i = 0; i < unused.count; ++i) {
        UnkEntry_l08b01c* entry = unused.data[i];
        self->entries.Remove(entry);
        delete entry;
    }
    unused.Free();
}
