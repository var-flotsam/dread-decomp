// CActorDef's unit (tools/tu_map.py unit 345): two identical lookups over the definition's array of
// entry pointers (+0xa0, count +0xa8): the first entry whose id (+0x8) is `*id`, or null. Names are
// placeholders.
#include "types.h"

struct UnkActorDefEntry345 {
    void* field_0x0;
    u64 id;  // 0x8
};

struct UnkActorDef345 {
    u8 pad_0x0[0xa0];
    UnkActorDefEntry345** entries;  // 0xa0
    u32 count;                      // 0xa8
};

static inline UnkActorDefEntry345* FindById345(const UnkActorDef345* self, const u64* id) {
    UnkActorDefEntry345** it = self->entries;
    UnkActorDefEntry345** end = it + self->count;
    for (; it != end; ++it) {
        UnkActorDefEntry345* entry = *it;
        if (entry->id == *id) {
            return entry;
        }
    }
    return nullptr;
}

UnkActorDefEntry345* fn_7100255334(const UnkActorDef345* self, const u64* id) {
    return FindById345(self, id);
}

UnkActorDefEntry345* fn_7100255368(const UnkActorDef345* self, const u64* id) {
    return FindById345(self, id);
}
