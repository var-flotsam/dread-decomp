// CBlackboard unit (tools/tu_map.py): finds a key in the key array (+0x10, count +0x18) and passes
// the entry at the same position of the array at +0x8 (one past the end when the key is missing) to
// the virtual at slot 6. Names and layout are placeholders.
#include "types.h"

class UnkKeyedSlots_b48a3 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(u64* entry);

    u64* entries;
    u64* keys;
    u32 count;

    u64 IndexOf(const u64* key) const {
        for (u64 i = 0; i < count; ++i) {
            if (keys[i] == *key)
                return i;
        }
        return count;
    }
};

void fn_7101048280(UnkKeyedSlots_b48a3* self, const u64* key) {
    u64 i = self->IndexOf(key);
    self->vf_0x30(&self->entries[i]);
}
