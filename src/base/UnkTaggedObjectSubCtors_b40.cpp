// Unit 131 of tools/tu_map.py (0x710014520c-0x710014569c, no reflected class): a tagged object's
// constructor, the same code as fn_710014333c (src/base/UnkTaggedObjectSubCtors.cpp) with this unit's
// vtable and sub-object constructor (fn_7100205bc4); fn_7100e13928 calls it with the object's name.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU131.h"
#include "game/UnkTU32.h"
#include "types.h"

using base::global::CStrId;

struct UnkCreatedSceneObj;

struct UnkWidget;
struct UnkWidgetArg;
void fn_7100205bc4(UnkWidget* self, UnkWidgetArg* a);

struct UnkTaggedSubObject131 {
    void* vtable;
    u32 flags;
    u32 tag;
    void* resource;
    void* self;
    u8 sub[0x100];    // 0x20
    u8 pad_0x120[0x8];
    u64 field_0x128;
    u64 field_0x130;
    u8 member[0x8];   // 0x138, size unknown
};

void fn_710014520c(UnkCreatedSceneObj* obj, const CStrId* name) {
    UnkTaggedSubObject131* self = (UnkTaggedSubObject131*)obj;
    self->resource = nullptr;
    self->flags = 0xff09f000;
    self->tag = 0x600d600d;
    self->vtable = g_unk_7101a1c5e8 + 0x10;
    fn_7100205bc4((UnkWidget*)self->sub, (UnkWidgetArg*)name);
    self->field_0x130 = 0;
    fn_71000255c4(self->member);
    self->self = self;
    self->flags = (self->flags & 0xfffffe) | (0x3u << 24) | 1;
    self->field_0x130 = self->field_0x128;
    fn_7100025884(self->member, self);
}
