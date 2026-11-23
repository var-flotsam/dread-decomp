// base::tunable::CTunable's unit: copy-assignment of a registry of named objects (base/UnkNamedRegistry.h)
// and the two words and the pointer after it: the old keys are destroyed, the storage is resized to
// the other's capacity when it differs, then the object pointers and keys are copied.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkRegistryKey138fa0 : base::UnkNamedEntry {
    ~UnkRegistryKey138fa0() { CRntString_Destroy(reinterpret_cast<base::global::CRntString*>(this)); }
};

struct UnkRegistryAssign138fa0 {
    base::UnkNamedObject** objects;
    UnkRegistryKey138fa0* entries;
    u32 count;
    u32 capacity;
    u32 step;
    u32 field_0x1c;
    void* field_0x20;
};

void fn_71001390a0(UnkRegistryAssign138fa0* self);

UnkRegistryAssign138fa0* fn_7100138fa0(UnkRegistryAssign138fa0* self, const UnkRegistryAssign138fa0* other) {
    if (self != other) {
        if (self->entries)
            for (u32 i = 0; i < self->count; ++i)
                self->entries[i].~UnkRegistryKey138fa0();
        self->count = 0;
        if (other->count != self->capacity) {
            fn_71001390a0(self);
            fn_710007547c(reinterpret_cast<base::UnkNamedRegistry*>(self), other->count);
        }
        memcpy(self->objects, other->objects, (u64)other->count << 3);
        self->count = other->count;
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            CRntString_CopyConstruct(&self->entries[i], &other->entries[i]);
    }
    return self;
}
