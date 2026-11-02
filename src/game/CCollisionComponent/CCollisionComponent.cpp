// The collision container's record of an object (game/UnkCollisionRecord.h): the first record
// holding `object`, refreshed first through the container's vf_0x170 when its flag (the low byte
// of +0x18) is set, or null; beside fn_71008640a4's lookups.
#include "game/UnkCollisionRecord.h"

typedef void (*UnkCollisionRefresh)(UnkCollisionContainer* self, UnkCollisionRecord* record, s32 a);

UnkCollisionRecord* fn_710086411c(UnkCollisionContainer* self, const UnkCollisionObject* object) {
    for (u32 i = 0; i < self->records.count; ++i) {
        UnkCollisionRecord* record = &self->records.data[i];
        if (record->field_0x28 == object) {
            if (static_cast<u8>(record->field_0x18))
                (*reinterpret_cast<UnkCollisionRefresh**>(self))[0x170 / 8](self, record, 0);
            return record;
        }
    }
    return nullptr;
}
