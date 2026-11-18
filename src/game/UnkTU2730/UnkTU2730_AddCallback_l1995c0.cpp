// Unit 2730 (tools/tu_map.py): registers a callback with the callback registry unless the same one (object and
// function) is already listed. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU2730.h"
#include "types.h"

// A listed callback as the registry copies it: field by field.
struct UnkCallbackEntry_l1995c0 {
    void* object;
    void (*fn)();

    UnkCallbackEntry_l1995c0(const UnkCallbackEntry_l1995c0& o) : object(o.object), fn(o.fn) {}

    bool operator==(const UnkCallbackEntry_l1995c0& o) const { return fn == o.fn && object == o.object; }
};

struct UnkCallbackRegistry_l1995c0 {
    u8 pad_0x0[0x30];
    base::global::CRntVector<UnkCallbackEntry_l1995c0> callbacks;  // 0x30
};

// The listed entry equal to `entry`, or the end of the list.
static inline UnkCallbackEntry_l1995c0* Find(base::global::CRntVector<UnkCallbackEntry_l1995c0>& list,
                                             const UnkCallbackEntry_l1995c0& entry) {
    for (u32 i = 0; i < list.count; ++i)
        if (list.data[i] == entry)
            return &list.data[i];
    return &list.data[list.count];
}

void fn_71011995c0(void* owner, const UnkCallback_kr& callback) {
    auto* self = static_cast<UnkCallbackRegistry_l1995c0*>(owner);
    const auto& entry = reinterpret_cast<const UnkCallbackEntry_l1995c0&>(callback);
    base::global::CRntVector<UnkCallbackEntry_l1995c0>& list = self->callbacks;
    if (Find(list, entry) != &list.data[list.count])
        return;
    list.PushBackCopy(entry);
}
