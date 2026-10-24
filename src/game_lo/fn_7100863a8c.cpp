// Unregisters the object stored under a string id: finds the record with that id, detaches its
// object from a global tracker, removes the record (the last one takes its place), drops the
// object from a pointer list and hands it to a lazily created manager.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

void* fn_71000839a0(u64 size, u32 flags);

struct UnkObj_7100863a8c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(s32 value);
};

// A string id moved by taking the pointer and leaving the source empty.
struct UnkId_7100863a8c {
    UnkStringData* data;

    UnkId_7100863a8c(UnkId_7100863a8c&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkRecord_7100863a8c {
    UnkId_7100863a8c field_0x0;
    UnkId_7100863a8c field_0x8;
    UnkId_7100863a8c field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    UnkObj_7100863a8c* field_0x28;

    UnkRecord_7100863a8c();   // fn_710072676c
    ~UnkRecord_7100863a8c();  // fn_7100726674
    UnkRecord_7100863a8c(UnkRecord_7100863a8c&& other) = default;
};

// UnkVector::GrowAt for these records, out of line.
UnkRecord_7100863a8c& fn_71007253b8(base::UnkVector<UnkRecord_7100863a8c>* self, u32 index);

struct UnkTracker_7100863a8c;
bool fn_7100d5a930(UnkTracker_7100863a8c* self, UnkObj_7100863a8c* obj);
void fn_7100d5abfc(UnkTracker_7100863a8c* self, UnkObj_7100863a8c* obj);

struct UnkManager_7100863a8c {
    u8 unk_0x0[0x6b0];
};
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkObj_7100863a8c* obj);
extern UnkManager_7100863a8c* g_unk_71726a4788;  // the manager instance (reached through the GOT)

static inline UnkManager_7100863a8c* GetManager() {
    if (!g_unk_71726a4788) {
        UnkManager_7100863a8c* m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return g_unk_71726a4788;
}

struct UnkStruct_7100863a8c {
    u8 unk_0x0[0x48];
    base::UnkVector<UnkRecord_7100863a8c> records;
    u8 unk_0x70[0x28];
    base::UnkVector<UnkObj_7100863a8c*> objects;
};

void fn_7100863a8c(UnkStruct_7100863a8c* self, const CStrId* id) {
    u32 i;
    for (i = 0; i < self->records.count; ++i) {
        if (fn_71007253b8(&self->records, i).field_0x0.data == id->data)
            goto found;
    }
    return;

found:
    UnkObj_7100863a8c* obj = fn_71007253b8(&self->records, i).field_0x28;
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
            self->records.data[i].~UnkRecord_7100863a8c();
            new (&self->records.data[i])
                UnkRecord_7100863a8c(static_cast<UnkRecord_7100863a8c&&>(self->records.data[last]));
        }
        for (u32 j = last; self->records.count > j; ++j)
            self->records.data[j].~UnkRecord_7100863a8c();
        self->records.count = last;
    }
    self->objects.Remove(obj);
    fn_7100d46c60(GetManager(), obj);
}
