// CCharClassShootActivatorComponent unit: moves the (index, entry) pair an iterator value holds or
// refers to into another, for an array of callback sets each with a string id.
// Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/UnkCallbackBase.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

// A string id whose move releases the old string, then takes the other's and leaves it the empty
// string of the pool it last loaded.
struct UnkMovedStrId_l894510 : CStrId {
    void Take(UnkMovedStrId_l894510& other) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &pool->empty;
    }
};

// A callback set with the refcounted owner it belongs to.
struct UnkCallbackSet_l894510 : UnkCallbackBase {
    virtual void vf8();
    base::UnkRefHandle owner;

    UnkCallbackSet_l894510& operator=(UnkCallbackSet_l894510&& other) {
        fn_710031d208(&owner, &other.owner);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
        other.field_0x30 = 0;
        fn_7100ad3588(&other.callbacks);
        if (base::UnkRefCounted* p = other.owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.owner.ptr = nullptr;
        }
        return *this;
    }
};

struct UnkEntry_l894510 {
    UnkCallbackSet_l894510 set;
    UnkMovedStrId_l894510 id;
};

using UnkEntryValue_l894510 = base::UnkIterValue<UnkEntry_l894510, u32>;

UnkEntryValue_l894510* fn_7100894510(UnkEntryValue_l894510* self, UnkEntryValue_l894510* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        UnkEntry_l894510& src = other->value.element;
        UnkEntry_l894510& dst = self->Element();
        dst.set = static_cast<UnkCallbackSet_l894510&&>(src.set);
        dst.id.Take(src.id);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        UnkEntry_l894510& src = *other->ref.element;
        UnkEntry_l894510& dst = self->Element();
        dst.set = static_cast<UnkCallbackSet_l894510&&>(src.set);
        dst.id.Take(src.id);
    }
    return self;
}
