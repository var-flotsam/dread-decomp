// A search in the CBTObserverComponent unit: fn_7100ca7494 returns the first hit fn_7100ca2048 gives for
// `query` over the enabled (+0x14) observers in the array at +0x90 (count at +0xa0), or null. Names and
// layouts are placeholders.
#include "types.h"

struct UnkQueryObj;
UnkQueryObj* fn_7100ca2048(void* observer, void* query, u32 flags);

struct UnkBTObserver {
    u8 pad_0x0[0x14];
    bool enabled;
};

struct UnkBTObserverComponent {
    u8 pad_0x0[0x90];
    UnkBTObserver** data;  // 0x90
    u8 pad_0x98[0x8];
    u32 count;  // 0xa0
};

UnkQueryObj* fn_7100ca7494(UnkBTObserverComponent* self, void* query) {
    for (UnkBTObserver** it = self->data, **end = self->data + self->count; it != end; ++it) {
        if ((*it)->enabled) {
            if (UnkQueryObj* hit = fn_7100ca2048(*it, query, 0))
                return hit;
        }
    }
    return nullptr;
}
