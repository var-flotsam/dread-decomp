// CPlayerMovement unit: two lazily cached lookups. Each returns the pointer kept at +0x7f0, filling it
// on first use from the owner's (+0x30) object at +0x2a0 through fn_710086406c with a type at
// 0x7172a03e28. Names and layouts are placeholders.
#include "types.h"

struct UnkPlayerMovementSource;
extern char g_unk_7172a03e28[] __attribute__((visibility("hidden")));
void* fn_710086406c(UnkPlayerMovementSource* source, const void* type);

struct UnkPlayerMovementOwner {
    u8 unk_0x0[0x2a0];
    UnkPlayerMovementSource* source;
};

struct UnkPlayerMovementCache {
    u8 unk_0x0[0x30];
    UnkPlayerMovementOwner* owner;
    u8 unk_0x38[0x7f0 - 0x38];
    void* cached;
};

static inline void* GetCached(UnkPlayerMovementCache* self) {
    if (self->cached == nullptr) {
        UnkPlayerMovementSource* source = self->owner->source;
        if (source == nullptr)
            return nullptr;
        self->cached = fn_710086406c(source, g_unk_7172a03e28);
    }
    return self->cached;
}

void* fn_71010d7a6c(UnkPlayerMovementCache* self) {
    return GetCached(self);
}

void* fn_71010d7aac(UnkPlayerMovementCache* self) {
    return GetCached(self);
}
