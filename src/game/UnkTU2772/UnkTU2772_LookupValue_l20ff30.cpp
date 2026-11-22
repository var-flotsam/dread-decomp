// UnkTU2772 (nn::atk): resolves a packed id (kind in the top byte, index in the low 24 bits) through the
// reference table at +4 to a block of type 0x2201 and returns a value from the data it points to, or -1.
// Names and layouts are placeholders.
#include "game/UnkAtkRef.h"
#include "game/UnkAtkRefTable.h"
#include "types.h"

struct UnkAtkHolder {
    u32 field_0x0;
    s32 table;  // 0x4
};
struct UnkAtkTypedBlock {
    u8 pad_0x0[0xc];
    u16 type;  // 0xc
    u16 pad_0xe;
    s32 dataOffset;  // 0x10
};
struct UnkAtkBlockData {
    u8 pad_0x0[0x20];
    s32 value;  // 0x20
};

s32 fn_710120ff30(const UnkAtkHolder* self, u32 id) {
    if ((id >> 24) != 1)
        return -1;
    u32 index = id & 0xffffff;
    const UnkAtkRefTable* table = reinterpret_cast<const UnkAtkRefTable*>(reinterpret_cast<const u8*>(self) + self->table);
    if (index >= table->count)
        return -1;
    const UnkAtkTypedBlock* block =
        reinterpret_cast<const UnkAtkTypedBlock*>(reinterpret_cast<const u8*>(table) + table->refs[index].offset);
    if (block == nullptr)
        return -1;
    if (block->type != 0x2201)
        return -1;
    const UnkAtkBlockData* data =
        reinterpret_cast<const UnkAtkBlockData*>(reinterpret_cast<const u8*>(block) + block->dataOffset);
    return data->value;
}
