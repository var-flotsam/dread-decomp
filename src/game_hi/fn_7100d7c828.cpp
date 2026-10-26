// Asks the object's optional component at +0x2e8 to handle a key, with a default mask and -1 parameter.
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

struct UnkKey {
    u64 value;
    UnkKey(const UnkKey& o) : value(o.value) {}
};

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

struct UnkComponent;
bool fn_7100a2be94(UnkComponent* self, UnkKey key);
void fn_7100a2b7b8(UnkComponent* self, const UnkKey& key);
bool fn_7100a2cfe8(UnkComponent* self, const UnkKey& key);

static inline void AddDefault(UnkComponent* comp, const UnkKey& key) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = GetDefaultMask_d7c828().value;
    fn_7100a2cb7c(comp, &key, &mask, -1.0f, 0);
}

struct UnkOwner {
    char pad[0x2e8];
    UnkComponent* field_0x2e8;
};

void fn_7100d7c828(UnkOwner* self, const UnkKey& key, bool flag) {
    if (!self->field_0x2e8)
        return;
    if (fn_7100a2be94(self->field_0x2e8, key))
        return;
    if (flag)
        fn_7100a2b7b8(self->field_0x2e8, key);
    if (fn_7100a2cfe8(self->field_0x2e8, key))
        return;
    AddDefault(self->field_0x2e8, key);
}
