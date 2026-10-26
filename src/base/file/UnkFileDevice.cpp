// The constructor of a file device object (unit 66, engine/base; see include/base/file/UnkFileDevice.h).
#include "base/UnkDefaultAllocator.h"
#include "base/file/UnkFileDevice.h"

using namespace base::file;

extern char g_unk_7101a1b0e8[];  // its vtable (GOT 0x7101cbc488)
void fn_710008b88c(void* base_object);

void fn_71000893a8(UnkFileDevice* self) {
    typedef u64 __attribute__((may_alias)) word;
    u16* flags = &self->field_0x1118;
    asm("" : "+r"(flags));
    self->vtable = g_unk_7101a1b0e8 + 0x10;
    fn_710008b88c(self->base_0x8);
    self->field_0xb8 = 0;
    UnkPathSlot* slot = self->slots;
    do {
        new (&slot->path) base::global::CFilePathStrId("", true);
        slot->field_0x8 = 0;
        slot->field_0xc = 9;
        slot->field_0x10 = 0;
        slot->field_0x18 = 0;
        ++slot;
    } while (slot != &self->slots[128]);
    self->field_0x10c0 = 0;
    self->field_0x10c8 = 0;
    fn_71000668f8(&self->mutex);
    self->field_0x1110 = 0;
    *flags = 0;
    *reinterpret_cast<u32*>(self->tail_0x1324 + 0x10) = 0;
    *reinterpret_cast<word*>(self->tail_0x1324) = 0;
    *reinterpret_cast<word*>(self->tail_0x1324 + 8) = 0;
}
