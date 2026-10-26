// Unregisters the object stored under a string id: finds the record with that id, detaches its
// object from a global tracker, removes the record (the last one takes its place), drops the
// object from a pointer list and hands it to a lazily created manager.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkTracker_7100863a8c;
bool fn_7100d5a930(UnkTracker_7100863a8c* self, UnkCollisionObject* obj);
void fn_7100d5abfc(UnkTracker_7100863a8c* self, UnkCollisionObject* obj);

struct UnkManager_7100863a8c {
    u8 unk_0x0[0x6b0];
};
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* obj);
extern UnkManager_7100863a8c* g_unk_71726a4788;  // the manager instance (reached through the GOT)

static inline UnkManager_7100863a8c* GetManager() {
    if (!g_unk_71726a4788) {
        UnkManager_7100863a8c* m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return g_unk_71726a4788;
}

void fn_7100863a8c(UnkCollisionContainer* self, const CStrId* id) {
    u32 i;
    for (i = 0; i < self->records.count; ++i) {
        if (fn_71007253b8(&self->records, i).field_0x0.data == id->data)
            goto found;
    }
    return;

found:
    UnkCollisionObject* obj = fn_71007253b8(&self->records, i).field_0x28;
    if (!obj)
        return;
    if (UnkTracker_7100863a8c* tracker =
            static_cast<UnkTracker_7100863a8c*>(g_unk_7101d47310->field_0x608->field_0x820)) {
        if (fn_7100d5a930(tracker, obj)) {
            fn_7100d5abfc(tracker, obj);
            obj->vf_0x40(4);
        }
    }
    if (self->records.count > i) {
        u32 last = self->records.count - 1;
        if (last > i) {
            self->records.data[i].~UnkCollisionRecord();
            new (&self->records.data[i])
                UnkCollisionRecord(static_cast<UnkCollisionRecord&&>(self->records.data[last]));
        }
        for (u32 j = last; self->records.count > j; ++j)
            self->records.data[j].~UnkCollisionRecord();
        self->records.count = last;
    }
    self->objects.Remove(obj);
    fn_7100d46c60(GetManager(), obj);
}
