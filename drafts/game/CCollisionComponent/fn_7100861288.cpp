// CCollisionComponent unit (tools/tu_map.py unit 1319): starting the component once: after the base's
// start, every object of the list at +0x70 and every registered object with no parent (+0x164) is
// listed at +0x98. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CActorComponent.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

struct UnkCollider_l861288 : UnkCollisionObject {
    u8 pad_0x8[0x15c];
    u32 parent_count;  // 0x164
};
static_assert(__builtin_offsetof(UnkCollider_l861288, parent_count) == 0x164, "layout");

struct UnkCollisionComponent_l861288 {
    u8 pad_0x0[0x48];
    base::global::CRntVector<UnkCollisionRecord> records;   // 0x48
    base::global::CRntVector<UnkCollisionObject*> objects;  // 0x70
    base::global::CRntVector<UnkCollisionObject*> active;   // 0x98
    u8 pad_0xc0[0x41];
    bool started;  // 0x101
};
static_assert(__builtin_offsetof(UnkCollisionComponent_l861288, started) == 0x101, "layout");

void fn_7100861288(UnkCollisionComponent_l861288* self) {
    if (self->started)
        return;
    self->started = true;
    fn_710024facc(reinterpret_cast<UnkSmBase_b60*>(self));
    for (UnkCollisionObject** it = self->objects.begin(); it != self->objects.end(); ++it) {
        UnkCollisionObject* object = *it;
        self->active.PushBack(object);
    }
    for (UnkCollisionRecord& record : self->records) {
        UnkCollisionObject* object = record.field_0x28;
        if (static_cast<UnkCollider_l861288*>(object)->parent_count == 0)
            self->active.PushBack(object);
    }
}
