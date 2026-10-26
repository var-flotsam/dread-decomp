// Looks for a hit among the 0x48-byte entries of the object two links down the owner's chain: first
// takes the pending handle of the record at +0x10 of the object at +0x29558 when its state (+0x5d8)
// is 1 (hands it to the sink at +0x5e0, releases the reference and moves to state 2), then walks the
// record's entries (when its object at +0x30 answers 0x10 to vtable slot 0x18) calling
// fn_7100116de0 on each until one reports true.
#include "base/UnkRefHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkEntry {
    u8 pad_0x0[0x48];
};

struct UnkTarget {
    void** vtable;
    u8 pad_0x8[0xa0];
    UnkEntry* field_0xa8;
    u32 field_0xb0;
    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkTarget*)>(vtable[3])(this); }
};

struct UnkLinkObject {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkTracker {
    u8 pad_0x0[0x30];
    UnkTarget* field_0x30;
};

// The handle the record refers to (base::UnkRefCounted's layout); its target is at +0x10.
struct UnkSink : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkRecord {
    u8 pad_0x0[0x5b0];
    u8 field_0x5b0[0x28];
    s32 field_0x5d8;
    u8 pad_0x5dc[4];
    base::UnkRefHandle field_0x5e0;
};

struct UnkRoot {
    u8 pad_0x0[0x10];
    UnkRecord* field_0x10;
};

struct UnkSelf {
    u8 pad_0x0[0x29558];
    UnkRoot* field_0x29558;
};

UnkTracker* fn_710028aca8(void* link);
bool fn_7100116de0(UnkEntry* entry, void* arg, u32 a, u32 b, f32 scale);

bool fn_71005528fc(UnkSelf* self, void* arg) {
    UnkRoot* root = self->field_0x29558;
    if (!root)
        return false;
    UnkRecord* record = root->field_0x10;
    if (!record)
        return false;
    if (record->field_0x5d8 == 1) {
        {
            UnkRefPtr ref = fn_710023b580(record->field_0x5b0);
            // The reference's pointer is the handle's.
            fn_7100234f78(&record->field_0x5e0, reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        record->field_0x5d8 = 2;
    }
    UnkSink* sink = static_cast<UnkSink*>(record->field_0x5e0.ptr);
    if (!sink || !sink->field_0x10)
        return false;
    UnkTracker* tracker = fn_710028aca8(sink->field_0x10);
    if (!tracker)
        return false;
    UnkTarget* target = tracker->field_0x30;
    if (!target)
        return false;
    if (target->vf_0x18() != 0x10 || target->field_0xb0 == 0)
        return false;
    for (UnkEntry* entry = target->field_0xa8; entry != target->field_0xa8 + target->field_0xb0; ++entry) {
        if (fn_7100116de0(entry, arg, 0, 1, 1.0f))
            return true;
    }
    return false;
}
