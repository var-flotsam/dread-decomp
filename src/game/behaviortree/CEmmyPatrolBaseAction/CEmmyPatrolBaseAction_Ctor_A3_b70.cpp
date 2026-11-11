// behaviortree::CEmmyPatrolBaseAction unit (tools/tu_map.py unit 803): the constructor of the class whose
// destructor is src/game/behaviortree/CEmmyPatrolAction/CEmmyPatrolAction_Dtor_A3_b69.cpp (vtable
// 0x7101a47c98): base constructor fn_710051dab0, an id of -1 at +0x2c, the node pool at +0x30
// (fn_7100515270: growth 10, flags 4), two arrays at +0xa0 and +0xc8 (growth 20, flags 4), a
// registration at +0xf8 (no set yet, this object and fn_7100516248 as its callback), the point, wait and
// flag at +0x110..+0x11c cleared, the member at +0x120 (fn_71006fbadc), which then takes the point
// wrapped in a temporary (vtable 0x7101a47d80) through fn_7100515388. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/behaviortree/CEmmyStateAction.h"
#include "types.h"

extern void* g_unk_7101a47c98[];
extern void* g_unk_7101a47d80[];

struct UnkComponent;
// base::refl_PoolListCtor<.., 0, 0x10>, instantiated in src/clones (UnkClone_fn_7100515270).
void fn_7100515270(void* self, u32 step, u32 count, u32 flags, base::UnkAllocator* allocator);
char* fn_71006fbadc(char* p0);
void fn_7100515388(char* p0, char* p1);

struct UnkPatrolBase_b54;
struct UnkVec3;
u32 fn_7100516248(UnkPatrolBase_b54* self, const UnkVec3* point);

template <class T>
struct UnkPatrolArray_A3_b70 {
    T* data = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 20;
    bool field_0x14 = true;
    u32 flags = 4;
    base::UnkAllocator* allocator = fn_7100083990();
};

struct UnkPatrolXY_A3_b70 {
    u32 x;
    u32 y;
};

struct UnkPatrolPoint_A3_b70 {
    void* vtable;
    u32 x;
    u32 y;
    explicit UnkPatrolPoint_A3_b70(const UnkPatrolXY_A3_b70& p) : vtable(g_unk_7101a47d80 + 2), x(p.x), y(p.y) {}
};

struct UnkPatrolBaseAction_A3_b70 {
    void** vtable;
    u8 pad_0x8[0x24];
    s32 field_0x2c;
    u8 pool[0x70];
    UnkPatrolArray_A3_b70<u32> field_0xa0;
    UnkPatrolArray_A3_b70<u32> field_0xc8;
    u32 field_0xf0;
    void* set;
    void* owner;
    u32 (*callback)(UnkPatrolBase_b54*, const UnkVec3*);
    UnkPatrolXY_A3_b70 point;
    u32 wait;
    bool flag;
    u8 pad_0x11d[3];
    u8 member[8];
};

void fn_7100515180(UnkPatrolBaseAction_A3_b70* self) {
    fn_710051dab0(reinterpret_cast<UnkComponent*>(self));
    self->vtable = g_unk_7101a47c98 + 2;
    self->field_0x2c = -1;
    fn_7100515270(self->pool, 10, 0, 4, nullptr);
    new (&self->field_0xa0) UnkPatrolArray_A3_b70<u32>;
    new (&self->field_0xc8) UnkPatrolArray_A3_b70<u32>;
    self->field_0xf0 = 0;
    self->set = nullptr;
    self->owner = self;
    self->callback = fn_7100516248;
    UnkPatrolXY_A3_b70* xy = &self->point;
    xy->x = 0;
    xy->y = 0;
    self->wait = 0;
    self->flag = false;
    fn_71006fbadc(reinterpret_cast<char*>(self->member));
    UnkPatrolPoint_A3_b70 point(*xy);
    fn_7100515388(reinterpret_cast<char*>(self->member), reinterpret_cast<char*>(&point));
}
