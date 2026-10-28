// Clears the done flag at 0x3a, then, when the id at 0x30 isn't the empty one and the owner
// at 0x18 has its object at 0x280, looks the id's entry up there and calls its slot 0x40 with
// the flag at 0x38 (setting the done flag).
#include "types.h"
#include "base/global/CStrId.h"

struct UnkTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(u8 flag);
};
struct UnkLookupResult {
    UnkTarget* target;
};
struct UnkIdData {
    u8 unk_0x0[0x30];
    u64 key;
};
struct UnkOwnerObjects {
    u8 unk_0x0[0x280];
    void* field_0x280;
};
struct UnkSelf {
    u8 unk_0x0[0x18];
    UnkOwnerObjects* owner;
    u8 unk_0x20[0x10];
    UnkIdData* id;
    u8 field_0x38;
    u8 unk_0x39;
    u8 done;
};

void fn_7100394bd0(UnkSelf* self);
UnkLookupResult* fn_7101152b24(void* objects, u64 key);

// NON_MATCHING(minor): the flag load and the slot load swap order in the call.
void fn_7100378d84(UnkSelf* self) {
    fn_7100394bd0(self);
    self->done = 0;
    UnkIdData* id = self->id;
    if ((u64)id == (u64)&g_unk_7101d47108->empty)
        return;
    UnkOwnerObjects* owner = self->owner;
    if (!owner || !owner->field_0x280)
        return;
    UnkLookupResult* found = fn_7101152b24(owner->field_0x280, id->key);
    bool ok = false;
    if (found) {
        UnkTarget* target = found->target;
        target->vf_0x40(self->field_0x38);
        ok = true;
    }
    self->done = ok;
}
