// From unit 330 of tools/tu_map.py (0x7100224d44-0x710022554c, no reflected class): under the
// object's mutex (+0x960), calls fn_710021b8a4 on each entry of the array at +0x60, tells the
// object at +0xad8 (its vf_0x40, with null) and calls fn_71002207bc on the part at +0xae0. Names and
// layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "types.h"

void fn_710021b8a4(void* entry);
void fn_71002207bc(void* part);

class UnkUnit330Target {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(void* arg);
};

struct UnkUnit330Object {
    u8 pad_0x0[0x60];
    void** entries;  // 0x60
    u32 entry_count;  // 0x68
    u8 pad_0x6c[0x960 - 0x6c];
    nn::os::MutexType mutex;  // 0x960
    u8 pad_0x980[0xad8 - 0x980];
    UnkUnit330Target* target;  // 0xad8
    u8 part[0x8];              // 0xae0, size unknown
};

void fn_7100224e4c(UnkUnit330Object* self) {
    nn::os::LockMutex(&self->mutex);
    for (void **it = self->entries, **end = it + self->entry_count; it != end; ++it)
        fn_710021b8a4(*it);
    self->target->vf_0x40(nullptr);
    fn_71002207bc(self->part);
    fn_7100066920(&self->mutex);
}
