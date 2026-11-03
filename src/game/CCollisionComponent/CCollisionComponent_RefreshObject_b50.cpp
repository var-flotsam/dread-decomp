// CCollisionComponent unit (game/UnkCollisionRecord.h): when forced or the container is active
// (+0xc8), refreshes the record whose object is `object` through the container's vf_0x170.
// Names and layouts are placeholders.
#include "game/UnkCollisionRecord.h"

typedef void (*UnkCollisionRefresh_b50)(void* self, UnkCollisionRecord* record, bool force);

struct UnkCollisionContainer_b50 {
    UnkCollisionRefresh_b50* vtable;
    u8 unk_0x8[0x40];
    base::UnkVector<UnkCollisionRecord> records;  // 0x48
    u8 unk_0x70[0x58];
    bool active;  // 0xc8
};

void fn_71008621d8(UnkCollisionContainer_b50* self, UnkCollisionObject* object, bool force) {
    if (!force && !self->active)
        return;
    for (u32 i = 0; i < self->records.count; ++i) {
        UnkCollisionRecord& record = fn_71007253b8(&self->records, i);
        if (record.field_0x28 == object) {
            self->vtable[0x170 / 8](self, &record, force);
            return;
        }
    }
}
