// Registers everything once (flag 0xc8): passes each record to a virtual hook, then adds each
// collision object to the global collision tracker, as a collider group or as a single collider
// depending on its reflected class.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

UnkReflObject*& fn_7100862088(base::UnkVector<UnkReflObject*>* self, u32 index);

struct UnkTracker_71008618cc;
void fn_7100d59994(UnkTracker_71008618cc* self, UnkReflObject* collider);
void fn_7100d5a304(UnkTracker_71008618cc* self, UnkReflObject* group);

struct UnkStruct_71008618cc;
struct UnkVtbl_71008618cc {
    void* pad_0x0[46];
    void (*vf_0x170)(UnkStruct_71008618cc* self, UnkCollisionRecord* record, bool flag);
};

struct UnkStruct_71008618cc {
    UnkVtbl_71008618cc* vtbl;
    u8 unk_0x8[0x40];
    base::UnkVector<UnkCollisionRecord> records;
    u8 unk_0x70[0x28];
    base::UnkVector<UnkReflObject*> objects;
    u8 unk_0xc0[8];
    bool registered;  // 0xc8
};

void fn_71008618cc(UnkStruct_71008618cc* self) {
    if (self->registered)
        return;
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    if (!sub)
        return;
    UnkTracker_71008618cc* tracker = static_cast<UnkTracker_71008618cc*>(sub->field_0x820);
    if (!tracker)
        return;
    for (u32 i = 0; i < self->records.count; ++i) {
        UnkCollisionRecord& record = fn_71007253b8(&self->records, i);
        self->vtbl->vf_0x170(self, &record, true);
    }
    for (u32 i = 0; i < self->objects.count; ++i) {
        UnkReflObject* obj = fn_7100862088(&self->objects, i);
        if (fn_710009d52c(obj->vf_0x0(), fn_7100d454fc())) {
            UnkReflObject* group =
                obj ? (fn_710009d50c(obj->vf_0x0(), fn_7100d454fc()) ? obj : nullptr) : nullptr;
            fn_7100d5a304(tracker, group);
        } else {
            UnkReflObject* collider =
                obj ? (fn_710009d50c(obj->vf_0x0(), fn_7100d361c0()) ? obj : nullptr) : nullptr;
            fn_7100d59994(tracker, collider);
        }
    }
    self->registered = true;
}
