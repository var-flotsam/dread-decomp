// Asks the object's optional component at +0x2e8 to handle a key, with a default mask and -1 parameter.
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkByteFlags {
    u32 value;
    UnkByteFlags() : value(0) {}
};

inline UnkByteFlags& GetEmptyFlags_d7c828() {
    static UnkByteFlags s;
    return s;
}

struct UnkMask {
    u32 value;
    UnkMask() : value(~GetEmptyFlags_d7c828().value & 0xff) {}
    UnkMask(const UnkMask& o) : value(o.value) {}
};

inline UnkMask& GetDefaultMask_d7c828() {
    static UnkMask s;
    return s;
}

bool fn_7100a2cfe8(UnkTimelineComponent* self, const u64* key);

static inline UnkTimelineComponent* Timeline(UnkActor* actor) {
    return static_cast<UnkTimelineComponent*>(actor->field_0x2e8);
}

static inline void AddDefault(UnkTimelineComponent* comp, const u64* key) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = GetDefaultMask_d7c828().value;
    fn_7100a2cb7c(comp, key, &mask, -1.0f, 0);
}

void fn_7100d7c828(UnkActor* self, const u64* key, bool flag) {
    if (!Timeline(self))
        return;
    u64 copy = *key;  // fn_7100a2be94 gets its own copy of the key
    if (fn_7100a2be94(Timeline(self), &copy))
        return;
    if (flag)
        fn_7100a2b7b8(Timeline(self), key);
    if (fn_7100a2cfe8(Timeline(self), key))
        return;
    AddDefault(Timeline(self), key);
}
