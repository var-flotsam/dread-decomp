// UnkTU267 (unit 267): sorts a list of entries into three arrays by their kind: the payloads of one kind,
// the entries whose payload's settings enable both flags, and the rest. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

struct UnkSettings_l1e8ad4 {
    u8 pad_0x0[0xa9];
    bool field_0xa9;
    u8 pad_0xaa[0x6];
    bool field_0xb0;
};

struct UnkPayload_l1e8ad4 {
    u8 pad_0x0[0x88];
    UnkSettings_l1e8ad4* settings;  // 0x88
};

struct UnkEntry_l1e8ad4 {
    s32 kind;
    s32 subkind;
    u8 pad_0x8[0x8];
    UnkPayload_l1e8ad4* payload;  // 0x10
};

struct UnkSorter_l1e8ad4 {
    u8 pad_0x0[0x8];
    CRntVector<UnkPayload_l1e8ad4*> payloads;  // 0x8
    CRntVector<UnkEntry_l1e8ad4*> others;      // 0x30
    CRntVector<UnkEntry_l1e8ad4*> flagged;     // 0x58
};

static inline bool IsFlagged(UnkEntry_l1e8ad4* entry) {
    if (entry->subkind != 0x4000 || !entry->payload)
        return false;
    UnkSettings_l1e8ad4* settings = entry->payload->settings;
    return settings->field_0xa9 && settings->field_0xb0;
}

void fn_71001e8ad4(UnkSorter_l1e8ad4* self, UnkEntry_l1e8ad4** entries, u32 count) {
    self->payloads.Clear();
    self->others.Clear();
    self->flagged.Clear();
    for (u32 i = 0; i < count; ++i) {
        UnkEntry_l1e8ad4* entry = entries[i];
        switch (entry->kind) {
        case 0x10: {
            UnkPayload_l1e8ad4* payload = entry->payload;
            self->payloads.PushBack(payload);
            break;
        }
        case 0x20:
            if (IsFlagged(entry))
                self->flagged.PushBack(entry);
            else
                self->others.PushBack(entry);
            break;
        case 0x2:
        case 0x800:
        case 0x2000:
            self->others.PushBack(entry);
            break;
        }
    }
}
