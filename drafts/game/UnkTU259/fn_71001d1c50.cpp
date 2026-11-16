// Unit 259 (the graphics system): groups a list of draw entries by the object they draw and queues one
// slot per entry for each of that object's ready render commands. Names and layouts are placeholders.
#include "types.h"

struct UnkRenderTargetA1;
struct UnkRenderCmd_b0247;
struct UnkGfxSubmitIdentity;
bool fn_7100196e80(UnkRenderCmd_b0247* cmd);
void fn_71001c47c0(UnkGfxSubmitIdentity* self);

struct UnkCmdMaterial_l1d1c50 {
    u8 pad_0x0[0x18];
    u32 mask;  // 0x18
};

// A render command of an object's list.
struct UnkRenderCmd_l1d1c50 {
    u8 pad_0x0[0x8];
    UnkCmdMaterial_l1d1c50* material;  // 0x8
};

// An object's render commands.
struct UnkCmdList_l1d1c50 {
    u8 pad_0x0[0x28];
    u32 count;  // 0x28
    u8 pad_0x2c[0x44];
    UnkRenderCmd_l1d1c50** items;  // 0x70
};

struct UnkDrawObject_l1d1c50 {
    u8 pad_0x0[0x78];
    UnkCmdList_l1d1c50* commands;  // 0x78
};

struct UnkDrawEntry_l1d1c50 {
    UnkDrawObject_l1d1c50* object;
    u8 field_0x8[0x8];
};

// A queued slot (0x50 bytes); the callback gets the slot itself.
struct UnkQueuedSlot_l1d1c50 {
    s32 id;
    u8 pad_0x4[0x4];
    UnkCmdList_l1d1c50* list;  // 0x8
    u32 mask;  // 0x10
    u8 pad_0x14[0x4];
    UnkRenderCmd_l1d1c50* command;  // 0x18
    UnkCmdMaterial_l1d1c50* material;  // 0x20
    u32 field_0x28;
    u8 pad_0x2c[0x4];
    void* entry_data;  // 0x30
    u8 pad_0x38[0x8];
    void (*callback)(UnkGfxSubmitIdentity* slot);  // 0x40
    bool field_0x48;
    u8 pad_0x49[0x7];
};
static_assert(sizeof(UnkQueuedSlot_l1d1c50) == 0x50, "slot size");

struct UnkGfx_l1d1c50 {
    u8 pad_0x0[0x3178178];
    UnkQueuedSlot_l1d1c50 slots[128];  // 0x3178178
    u32 slot_count;  // 0x317a978
    u32 mask;  // 0x317a97c
};
static_assert(__builtin_offsetof(UnkGfx_l1d1c50, mask) == 0x317a97c, "layout");

void fn_71001d1c50(UnkRenderTargetA1* target, void* p, u32 n) {
    UnkGfx_l1d1c50* self = reinterpret_cast<UnkGfx_l1d1c50*>(target);
    UnkDrawEntry_l1d1c50** entries = static_cast<UnkDrawEntry_l1d1c50**>(p);
    self->mask = 0;
    if (n == 0)
        return;
    u32 count = n < 128 ? n : 128;

    // Runs of entries that draw the same object.
    u32 run_sizes[128];
    u32 runs = 0;
    UnkDrawObject_l1d1c50* last = nullptr;
    for (u64 i = 0; i < count; ++i) {
        UnkDrawObject_l1d1c50* object = entries[i]->object;
        if (last == object) {
            ++run_sizes[runs - 1];
        } else {
            run_sizes[runs++] = 1;
            last = object;
        }
    }
    if (runs == 0)
        return;

    u32& slot_count = self->slot_count;
    u32& mask = self->mask;
    s32 start = 0;
    for (u64 r = 0; r < runs; ++r) {
        UnkCmdList_l1d1c50* list = entries[start]->object->commands;
        u32 size = run_sizes[r];
        u64 commands = list->count;
        for (u64 j = 0; j < commands; ++j) {
            UnkRenderCmd_l1d1c50* command = list->items[j];
            if (!fn_7100196e80(reinterpret_cast<UnkRenderCmd_b0247*>(command)))
                continue;
            for (u32 k = 0; k < size; ++k) {
                u32 index = slot_count;
                if (index > 126)
                    break;
                slot_count = index + 1;
                UnkQueuedSlot_l1d1c50& slot = self->slots[index];
                UnkCmdMaterial_l1d1c50* material = command->material;
                slot.id = 10;
                slot.list = list;
                slot.mask = material->mask;
                slot.field_0x48 = true;
                slot.field_0x28 = 0;
                slot.callback = fn_71001c47c0;
                slot.command = command;
                slot.material = material;
                slot.entry_data = entries[start + k]->field_0x8;
                mask |= material->mask;
            }
        }
        start += size;
    }
}
