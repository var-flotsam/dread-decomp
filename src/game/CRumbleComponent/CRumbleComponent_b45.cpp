// CRumbleComponent's unit (tools/tu_map.py unit 1500): two functions of the same shape
// (fn_71009da040 the original, on the list at +0x48; fn_71009da0a8 on the list at +8). Marks (byte
// +0x63) the first 0x68-byte entry whose weak handle (an object pointer and the id it was taken
// with, live while the object's id at +8 still matches and is not -1) names the given object.
// Names and layouts are placeholders.
#include "types.h"

struct UnkRumbleTarget_b45a2 {
    void* object;
    s64 id;
};

struct UnkRumbleEntry_b45a2 {
    UnkRumbleTarget_b45a2* target;
    s64 id;
    u8 pad_0x10[0x53];
    bool field_0x63;
    u8 pad_0x64[4];

    void* Get() const {
        if (!target || target->id != id || id == -1)
            return nullptr;
        return target->object;
    }
};
static_assert(sizeof(UnkRumbleEntry_b45a2) == 0x68, "layout");

struct UnkRumbleList_b45a2 {
    UnkRumbleEntry_b45a2* data;
    u32 count;
};

static inline __attribute__((always_inline)) void UnkRumbleMark_b45a2(UnkRumbleList_b45a2* list,
                                                                      void* object) {
    if (!object)
        return;
    for (UnkRumbleEntry_b45a2 *it = list->data, *end = it + list->count; it != end; ++it) {
        if (it->Get() == object) {
            it->field_0x63 = true;
            return;
        }
    }
}

void fn_71009da040(u8* self, void* object) {
    UnkRumbleMark_b45a2(reinterpret_cast<UnkRumbleList_b45a2*>(self + 0x48), object);
}

void fn_71009da0a8(u8* self, void* object) {
    UnkRumbleMark_b45a2(reinterpret_cast<UnkRumbleList_b45a2*>(self + 8), object);
}
