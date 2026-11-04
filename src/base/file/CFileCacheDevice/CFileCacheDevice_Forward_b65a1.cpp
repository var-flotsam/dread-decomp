// base::file::CFileCacheDevice unit (tools/tu_map.py, the name from reflection): passes its five
// arguments (two of them bools) on to the virtual at slot 0x80 of the device held at +0x10.
// Names are placeholders.
#include "types.h"

struct UnkCachedDevice_b65a1 {
    void** vtable;
};

struct UnkFileCacheDevice_b65a1 {
    u8 pad_0x0[0x10];
    UnkCachedDevice_b65a1* device;
};

void fn_7100087230(UnkFileCacheDevice_b65a1* self, u64 a, u64 b, bool c, bool d, u64 e) {
    typedef void (*Fn)(UnkCachedDevice_b65a1*, u64, u64, bool, bool, u64);
    UnkCachedDevice_b65a1* device = self->device;
    reinterpret_cast<Fn>(device->vtable[0x80 / 8])(device, a, b, c, d, e);
}
