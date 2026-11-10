// CKraidSpikeMovablePlatformComponent unit (tools/tu_map.py unit 931): the constructor (fn_7100609010): the base
// (fn_71009516a0), the vtable, the platform's fields cleared (the config at +0x250 through the state at +0x280),
// then the class-entry pass of base::core::CBaseObject (fn_7100023854). Names and layouts are placeholders.
#include "types.h"

struct UnkNode;
void fn_7100023854(UnkNode* node);
void fn_71009516a0(void* self);
extern void* g_unk_7101a55bd0[];

struct UnkSpikePlatformCtor_ovb0020 {
    void** vtable;
    u8 unk_0x8[0x248];
    void* config;  // 0x250
    f32 field_0x258;
    f32 field_0x25c;
    bool field_0x260;
    f32 field_0x264;
    bool field_0x268;
    bool field_0x269;
    f32 field_0x26c;
    f32 field_0x270;
    f32 field_0x274;
    f32 field_0x278;
    f32 field_0x27c;
    u32 state;  // 0x280
};
static_assert(__builtin_offsetof(UnkSpikePlatformCtor_ovb0020, state) == 0x280, "layout");

void fn_7100609010(UnkSpikePlatformCtor_ovb0020* self) {
    fn_71009516a0(self);
    self->vtable = g_unk_7101a55bd0 + 2;
    self->config = nullptr;
    self->field_0x258 = 0.0f;
    self->field_0x25c = 0.0f;
    self->field_0x260 = false;
    self->field_0x264 = 0.0f;
    self->field_0x268 = false;
    self->field_0x269 = false;
    self->field_0x26c = 0.0f;
    self->field_0x270 = 0.0f;
    self->field_0x274 = 0.0f;
    self->field_0x278 = 0.0f;
    self->field_0x27c = 0.0f;
    self->state = 0;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
