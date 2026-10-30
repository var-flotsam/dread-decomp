// The collision container's lookups (game/UnkCollisionRecord.h): by name, the object of the first
// record whose first id is `name`, refreshed first through the container's vf_0x170 when the
// record's flag (the low byte of +0x18) is set, or null; and an object's record index
// (fn_710086427c).
#include "game/UnkCollisionRecord.h"

typedef void (*UnkCollisionRefresh)(UnkCollisionContainer* self, UnkCollisionRecord* record, s32 a);

UnkCollisionObject* fn_71008640a4(UnkCollisionContainer* self, const base::global::CStrId* name) {
    for (u32 i = 0; i < self->records.count; ++i) {
        UnkCollisionRecord* record = &self->records.data[i];
        if (record->field_0x0.data == name->data) {
            if (static_cast<u8>(record->field_0x18))
                (*reinterpret_cast<UnkCollisionRefresh**>(self))[0x170 / 8](self, record, 0);
            return record->field_0x28;
        }
    }
    return nullptr;
}

// The index of the record holding `object`, or -1.
s32 fn_710086427c(UnkCollisionContainer* self, const UnkCollisionObject* object) {
    for (u32 i = 0; i < self->records.count; ++i)
        if (self->records.data[i].field_0x28 == object)
            return i;
    return -1;
}
