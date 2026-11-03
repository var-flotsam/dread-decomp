// CAINavigationComponent unit (tools/tu_map.py unit 1113). fn_71006f540c and fn_71006f549c pass an
// argument on to every entry of the navigation set the component reaches through +0xb0 (a count at
// +0x20 and the entries from +0x24), through vtable slot 0x68 or 0x70; both report true. Names and
// layouts are placeholders.
#include "types.h"

struct UnkNavEntry_b50 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68(u32 value);
    virtual void vf_0x70(u32 value);
};

struct UnkNavSet_b50 {
    u8 pad_0x0[0x20];
    u32 count;
    UnkNavEntry_b50* entries[1];  // 0x24
} __attribute__((packed));

struct UnkNavComponent_b50 {
    u8 pad_0x0[0xb0];
    UnkNavSet_b50** set;
};

// The end of `count` entries from `begin` (computed on the address, as the original does).
static inline UnkNavEntry_b50** End_b50(UnkNavEntry_b50** begin, u32 count) {
    return reinterpret_cast<UnkNavEntry_b50**>(reinterpret_cast<u64>(begin) + (u64)count * 8);
}

template <void (UnkNavEntry_b50::*Method)(u32)>
static inline bool ForEachNavEntry_b50(UnkNavComponent_b50* self, u32 value) {
    if (UnkNavSet_b50** ref = self->set) {
        if (UnkNavSet_b50* set = *ref) {
            UnkNavEntry_b50** begin = set->entries;
            for (UnkNavEntry_b50** it = begin; it != End_b50(begin, set->count); ++it) {
                if (UnkNavEntry_b50* entry = *it)
                    (entry->*Method)(value);
            }
        }
    }
    return true;
}

bool fn_71006f540c(UnkNavComponent_b50* self, u32 value) {
    return ForEachNavEntry_b50<&UnkNavEntry_b50::vf_0x68>(self, value);
}

bool fn_71006f549c(UnkNavComponent_b50* self, u32 value) {
    return ForEachNavEntry_b50<&UnkNavEntry_b50::vf_0x70>(self, value);
}
