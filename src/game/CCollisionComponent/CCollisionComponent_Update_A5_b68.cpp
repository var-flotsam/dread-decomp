// CCollisionComponent's unit: the collision container's update. After the base update (fn_71010c424c) and
// fn_71008615b4, when both flags +0xc9 and +0xc8 are set, goes over the records (game/UnkCollisionRecord.h): a
// flagged record (+0x18) whose object's vf_0x58 holds is refreshed through the container's vf_0x170 with 1, any
// other flagged record has its word at +0x1c cleared. Names are placeholders.
#include "game/CCollisionComponent.h"
#include "game/CComponent.h"
#include "game/UnkCollisionRecord.h"

struct UnkParent;

typedef void (*UnkCollisionRefresh)(UnkCollisionContainer* self, UnkCollisionRecord* record, s32 a);

// The record's +0x18 as the update reads it: the flag and the word after it.
struct UnkCollisionRecordState_A5_b68 {
    u8 pad_0x0[0x18];
    bool flag;
    u8 pad_0x19[3];
    u32 field_0x1c;
};

static inline u32 ObjectTest_A5_b68(UnkCollisionObject* o) {
    return (*reinterpret_cast<u32 (***)(UnkCollisionObject*)>(o))[0x58 / 8](o);
}

void fn_7100861af8(UnkCollisionContainer* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    fn_71008615b4(self);
    u8* bytes = reinterpret_cast<u8*>(self);
    if (!bytes[0xc9] || !bytes[0xc8])
        return;
    for (u32 i = 0; i < self->records.count; ++i) {
        UnkCollisionRecord* record = &fn_71007253b8(&self->records, i);
        UnkCollisionRecordState_A5_b68* state = reinterpret_cast<UnkCollisionRecordState_A5_b68*>(record);
        if (!state->flag)
            continue;
        if (ObjectTest_A5_b68(record->field_0x28))
            (*reinterpret_cast<UnkCollisionRefresh**>(self))[0x170 / 8](self, record, 1);
        else
            state->field_0x1c = 0;
    }
}
