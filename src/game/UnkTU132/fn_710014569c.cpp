// Unit 132: the constructor of a 0x160-byte tagged model object built with its name: the vtable, the
// flags word and live tag, no counted resource, the widget sub-object at +0x20, the self pointer, kind 4,
// then +0x150 copied from +0x128. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU132.h"
#include "types.h"

struct UnkWidget;
struct UnkWidgetArg;
void fn_710020694c(UnkWidget* self, UnkWidgetArg* a);

struct UnkTaggedModel_l14569c {
    void* vtable;
    u32 flags;
    u32 tag;          // 0xc, 0x600d600d while alive
    void* resource;   // 0x10
    void* self;       // 0x18
    u8 sub[0x108];    // 0x20
    u64 field_0x128;  // the sub-object's +0x108
    u8 pad_0x130[0x20];
    u64 field_0x150;
    bool field_0x158;
    u8 pad_0x159[3];
    u32 field_0x15c;
};
static_assert(sizeof(UnkTaggedModel_l14569c) == 0x160, "UnkTaggedModel_l14569c layout");

void fn_710014569c(void* object, base::global::CStrId* name) {
    UnkTaggedModel_l14569c* self = static_cast<UnkTaggedModel_l14569c*>(object);
    self->resource = nullptr;
    self->flags = 0xff09f000;
    self->tag = 0x600d600d;
    self->vtable = g_unk_7101a1c630 + 0x10;
    fn_710020694c(reinterpret_cast<UnkWidget*>(self->sub), reinterpret_cast<UnkWidgetArg*>(name));
    self->field_0x158 = false;
    self->field_0x15c = 1;
    self->self = self;
    self->flags = (self->flags & 0xfffffe) | (4u << 24) | 1;
    self->field_0x150 = self->field_0x128;
}
