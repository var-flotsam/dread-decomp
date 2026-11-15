// CMissionLog's unit (tools/tu_map.py): the mission log's constructor: the display object base constructor
// (fn_71011d8d24), the vtable, two record arrays (flags 5, reserving 0x47 and 0xd entries), the cleared members
// around the callback slots, then the object's own slot 3 (vf_0x18). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/GUI/CMissionLog.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkElem38;
template <class Elem>
struct UnkElemArray;
using UnkElem38Array = UnkElemArray<UnkElem38>;

void fn_7100b5c28c(UnkElem38Array* self, u32 capacity);
void fn_7100b5cea0();

struct UnkMissionLogCtor_b5af1c {
    void** vtable;
    u8 pad_0x8[0x228];
    u64 field_0x230;
    u64 field_0x238;
    u64 field_0x240;
    void* field_0x248_data;
    u64 field_0x248_count;
    u32 field_0x248_step;
    bool field_0x248_grow;
    u32 field_0x248_flags;
    base::UnkAllocator* field_0x248_allocator;
    void* field_0x270_data;
    u64 field_0x270_count;
    u32 field_0x270_step;
    bool field_0x270_grow;
    u32 field_0x270_flags;
    base::UnkAllocator* field_0x270_allocator;
    u64 field_0x298;
    u8 pad_0x2a0[0x4];
    bool field_0x2a4;
    u64 field_0x2a8;
    u32 field_0x2b0;
    u8 pad_0x2b4[0x4];
    u64 field_0x2b8;
    u8 pad_0x2c0[0x18];
    u64 field_0x2d8[6];
    u64 field_0x308;
    void* field_0x310;
    void* field_0x318;
};
static_assert(__builtin_offsetof(UnkMissionLogCtor_b5af1c, field_0x248_data) == 0x248, "layout");
static_assert(__builtin_offsetof(UnkMissionLogCtor_b5af1c, field_0x270_data) == 0x270, "layout");
static_assert(__builtin_offsetof(UnkMissionLogCtor_b5af1c, field_0x2d8) == 0x2d8, "layout");
static_assert(__builtin_offsetof(UnkMissionLogCtor_b5af1c, field_0x318) == 0x318, "layout");

void fn_7100b5af1c(UnkMissionLogCtor_b5af1c* self) {
    fn_71011d8d24(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a9d220 + 0x10);
    self->field_0x240 = 0;
    self->field_0x238 = 0;
    self->field_0x248_data = nullptr;
    self->field_0x230 = 0;
    self->field_0x248_count = 0;
    self->field_0x248_step = 10;
    self->field_0x248_grow = true;
    self->field_0x248_flags = 5;
    self->field_0x248_allocator = fn_7100083990();
    fn_7100b5c28c(reinterpret_cast<UnkElem38Array*>(&self->field_0x248_data), 0x47);
    self->field_0x270_data = nullptr;
    self->field_0x270_count = 0;
    self->field_0x270_step = 10;
    self->field_0x270_grow = true;
    self->field_0x270_flags = 5;
    self->field_0x270_allocator = fn_7100083990();
    fn_7100b5c28c(reinterpret_cast<UnkElem38Array*>(&self->field_0x270_data), 0xd);
    self->field_0x298 = 0;
    self->field_0x2a4 = false;
    self->field_0x2a8 = 0;
    self->field_0x2b0 = 0;
    self->field_0x2b8 = 0;
    for (u32 i = 0; i < 6; ++i)
        self->field_0x2d8[i] = 0;
    self->field_0x318 = reinterpret_cast<void*>(fn_7100b5cea0);
    self->field_0x308 = 0;
    self->field_0x310 = self;
    reinterpret_cast<void (*)(UnkMissionLogCtor_b5af1c*)>(self->vtable[3])(self);
}
