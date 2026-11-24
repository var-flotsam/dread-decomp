// CCharClassShootActivatorComponent unit: inserts a copy of an entry (a callback set and a string id) at an
// index of the entry array, moving the later entries up one place (growing the array first); appending is
// fn_710089566c. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassShootActivatorComponent.h"
#include "types.h"

using base::global::CStrId;

// A callback set's base: an owned array of callbacks and a count.
struct UnkShootSetBase_l8959d4 {  // vtable g_unk_7101a20998
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkShootSetBase_l8959d4() : field_0x30(0) {}
};

// A callback set with the refcounted owner it belongs to.
struct UnkShootSet_l8959d4 : UnkShootSetBase_l8959d4 {  // vtable g_unk_7101a33fa0
    virtual void vf8();
    base::UnkRefHandle owner;

    UnkShootSet_l8959d4(UnkShootSet_l8959d4&& other);  // fn_71007b1928
    UnkShootSet_l8959d4(const UnkShootSet_l8959d4& other) {
        owner.ptr = nullptr;
        owner.Set(other.owner.ptr);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
    ~UnkShootSet_l8959d4() {
        if (base::UnkRefCounted* p = owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkShootEntry_l8959d4 {
    UnkShootSet_l8959d4 set;
    CStrId id;
};
static_assert(sizeof(UnkShootEntry_l8959d4) == 0x48, "layout");

using UnkShootEntryVector_l8959d4 = base::global::CRntVector<UnkShootEntry_l8959d4>;

void fn_71008959d4(UnkShootEntryVector_l8959d4* v, const UnkShootEntry_l8959d4* value, u32 index) {
    if (index == v->count) {
        CCharClassShootActivatorComponent_EntryArrayPushBackCopy(reinterpret_cast<UnkEntryVector*>(v), reinterpret_cast<const UnkEntry*>(value));
        return;
    }
    u32 n = v->capacity;
    if (n <= v->count && v->step != 0)
        n = (v->count / v->step + 1) * v->step;
    CCharClassShootActivatorComponent_ReserveCallbackEntryVector(reinterpret_cast<UnkEntryVector*>(v), n);
    // The entries from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = v->count - 1; i >= index; --i) {
        UnkShootEntry_l8959d4* from = &v->data[i];
        UnkShootEntry_l8959d4* to = from + 1;
        new (&to->set) UnkShootSet_l8959d4(static_cast<UnkShootSet_l8959d4&&>(from->set));
        CStrId::MoveConstructReloading(&to->id, from->id);
        v->data[i].~UnkShootEntry_l8959d4();
        if (i == 0)
            break;
    }
    new (&v->data[index]) UnkShootEntry_l8959d4(*value);
    ++v->count;
}
