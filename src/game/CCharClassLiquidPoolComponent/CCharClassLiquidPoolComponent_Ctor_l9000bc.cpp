// CCharClassLiquidPoolComponent (tools/tu_map.py unit 1394): fn_71009000bc, the constructor. On the damage
// trigger base (fn_7100a45a30): the vtable, a callback bound to this object, five empty paths, an empty
// 2D bounding box and a flag in the base. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CCharClassBaseDamageTriggerComponent.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;

struct UnkBaseDamageTrigger3;
struct UnkWaterPoolComponent_b53;

void fn_71009052e8(void* self);

struct UnkLiquidPool_l9000bc {
    void** vtable;
    u8 pad_0x8[0x98 - 0x8];
    u32 field_0x98;
    u8 pad_0x9c[0xbe8 - 0x9c];
    void* registered_in;  // 0xbe8
    void* callback_owner;  // 0xbf0
    void (*callback)(void*);  // 0xbf8
    void* owned;  // 0xc00
    u64 field_0xc08;
    u16 field_0xc10;
    CFilePathStrId paths[5];  // 0xc18
    u64 field_0xc40[7];
    f32 min_x;  // 0xc78
    f32 min_y;
    f32 max_x;  // 0xc80
    f32 max_y;
    void* shared;  // 0xc88
};
static_assert(__builtin_offsetof(UnkLiquidPool_l9000bc, shared) == 0xc88, "layout");

void fn_71009000bc(UnkWaterPoolComponent_b53* object) {
    UnkLiquidPool_l9000bc* self = reinterpret_cast<UnkLiquidPool_l9000bc*>(object);
    fn_7100a45a30(reinterpret_cast<UnkBaseDamageTrigger3*>(self));
    self->vtable = &g_unk_7101a80ae8[2];
    self->registered_in = nullptr;
    self->callback_owner = self;
    self->callback = fn_71009052e8;
    self->owned = nullptr;
    self->field_0xc08 = 9;
    self->field_0xc10 = 0;
    new (&self->paths[0]) CFilePathStrId();
    new (&self->paths[1]) CFilePathStrId();
    new (&self->paths[2]) CFilePathStrId();
    new (&self->paths[3]) CFilePathStrId();
    new (&self->paths[4]) CFilePathStrId();
    for (s32 i = 0; i < 7; ++i)
        self->field_0xc40[i] = 0;
    self->min_x = 3.4028235e38f;
    self->min_y = 3.4028235e38f;
    self->max_x = -3.4028235e38f;
    self->max_y = -3.4028235e38f;
    self->shared = nullptr;
    self->field_0x98 = 0x4000;
}
