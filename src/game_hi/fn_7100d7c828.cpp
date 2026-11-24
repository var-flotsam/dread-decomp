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

struct UnkMask_d7c828 {
    u32 value;
    UnkMask_d7c828() : value(~GetEmptyFlags_d7c828().value & 0xff) {}
    UnkMask_d7c828(const UnkMask_d7c828& o) : value(o.value) {}
};

inline UnkMask_d7c828& GetDefaultMask_d7c828() {
    static UnkMask_d7c828 s;
    return s;
}

static inline UnkTimelineComponent* Timeline(UnkActor* actor) {
    return static_cast<UnkTimelineComponent*>(actor->field_0x2e8);
}

static inline void AddDefault(UnkTimelineComponent* comp, const u64* key) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = GetDefaultMask_d7c828().value;
    fn_7100a2cb7c(comp, key, &mask.id, false, -1.0f);
}

void fn_7100d7c828(UnkActor* self, const u64* key, bool flag) {
    if (!Timeline(self))
        return;
    u64 copy = *key;  // fn_7100a2be94 gets its own copy of the key
    if (fn_7100a2be94(Timeline(self), &copy))
        return;
    if (flag)
        fn_7100a2b7b8(Timeline(self), key);
    if (CTimelineComponent_HasTrack(Timeline(self), key))
        return;
    AddDefault(Timeline(self), key);
}
