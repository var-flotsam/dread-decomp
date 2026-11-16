// Unit 324 of tools/tu_map.py: a media instance adds a listener (a context and a callback) to its
// list at +0x720 under the lock at +0x960, unless the same pair is already listed. Names and
// layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkListener_l21ff8c {
    void* context;
    void (*callback)();

    ~UnkListener_l21ff8c() {}

    bool operator==(const UnkListener_l21ff8c& other) const {
        return callback == other.callback && context == other.context;
    }
};

struct UnkMedia_l21ff8c {
    u8 pad_0x0[0x720];
    base::global::CRntVector<UnkListener_l21ff8c> listeners;  // 0x720
    u8 pad_0x748[0x218];
    nn::os::MutexType lock;  // 0x960
};
static_assert(__builtin_offsetof(UnkMedia_l21ff8c, lock) == 0x960, "UnkMedia_l21ff8c layout");

// The listed pair equal to `key`, or the end of the list.
static UnkListener_l21ff8c* Find(base::global::CRntVector<UnkListener_l21ff8c>& list, UnkListener_l21ff8c key) {
    u32 count = list.count;
    UnkListener_l21ff8c* it = list.data;
    for (u32 n = count; n != 0; --n, ++it) {
        if (*it == key)
            return it;
    }
    return &list.data[count];
}

void fn_710021ff8c(void* obj, void* pair) {
    UnkMedia_l21ff8c* self = static_cast<UnkMedia_l21ff8c*>(obj);
    const UnkListener_l21ff8c& listener = *static_cast<const UnkListener_l21ff8c*>(pair);
    fn_710006691c(&self->lock);
    base::global::CRntVector<UnkListener_l21ff8c>& list = self->listeners;
    if (Find(list, listener) == &list.data[list.count])
        list.PushBackCopy(listener);
    fn_7100066920(&self->lock);
}
