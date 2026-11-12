// base::file::CPackManagerDevice's unit (tools/tu_map.py unit 66): puts a load request for a path,
// a flag, a value and two words in the next slot of the pack sets object's request ring.
// Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkStrIdMemberOf;
using base::file::UnkFileDevice;
using base::file::UnkPathRequestArgs;
using base::file::UnkPathSlot;

bool fn_7100089754(UnkFileDevice* self, const UnkStrIdMemberOf<g_unk_7101d47200>* path, bool a, u32 b,
                   const UnkPathRequestArgs* args) {
    fn_710006691c(&self->mutex);
    u32 index = self->next;
    UnkFileDevice* ring = reinterpret_cast<UnkFileDevice*>(reinterpret_cast<u8*>(self) + index * sizeof(UnkPathSlot));
    // keeps the slot address computed before the ring index and count are stored
    asm("" : "+r"(ring));
    UnkPathSlot& slot = ring->slots[0];
    self->next = (index + 1) & 0x7f;
    ++self->queued;
    UnkStrIdMemberOf<g_unk_7101d47200>* id = &slot.path;
    // keeps the id's address separate from the slot's
    asm("" : "+r"(id));
    *id = *path;
    slot.args = *args;
    slot.field_0x8 = a;
    slot.field_0xc = b;
    fn_7100066920(&self->mutex);
    return true;
}
