// Units 118, 126, 136, 137 and 138 of tools/tu_map.py: constructors of objects tagged for debugging, as in
// src/base/UnkTaggedObjectSubCtors_b40.cpp but without a vtable: the flags word (0xff09f000), the live
// tag 0x600d600d, no counted resource, the widget sub-object at +0x18 built with the caller's argument,
// the self pointer, the flags' kind bits set (bit 0 kept set), and +0x128 (unit 137: +0x178; unit 136: none) copied from +0x120.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkWidget;
struct UnkWidgetArg;
void fn_71001f402c(UnkWidget* self, UnkWidgetArg* a);
void fn_710020ae7c(UnkWidget* self, UnkWidgetArg* a);
void fn_71001f2a08(UnkWidget* self, UnkWidgetArg* a);
void fn_710020a18c(UnkWidget* self, UnkWidgetArg* a);
void fn_71002094dc(UnkWidget* self, UnkWidgetArg* a);

struct UnkTaggedWidgetObject {
    u32 flags;
    u32 tag;          // 0x600d600d, 0xdeaddead once destroyed
    void* resource;   // 0x8
    void* self;       // 0x10
    u8 sub[0x108];    // 0x18
    u64 field_0x120;
    u64 field_0x128;
    u8 pad_0x130[0x48];
    u64 field_0x178;
};

template <void (*Ctor)(UnkWidget*, UnkWidgetArg*), u32 KeepMask, u32 Kind, u64 UnkTaggedWidgetObject::*Copy = &UnkTaggedWidgetObject::field_0x128>
static inline void ConstructTaggedWidget(UnkTaggedWidgetObject* self, UnkWidgetArg* arg) {
    self->flags = 0xff09f000;
    self->tag = 0x600d600d;
    self->resource = nullptr;
    Ctor(reinterpret_cast<UnkWidget*>(self->sub), arg);
    self->self = self;
    self->flags = (self->flags & KeepMask) | Kind | 1;
    if (Copy != nullptr)
        self->*Copy = self->field_0x120;
}

void fn_7100141aac(UnkTaggedWidgetObject* self, UnkWidgetArg* arg) {
    ConstructTaggedWidget<fn_71001f402c, 0xfffffe, 0x17u << 24>(self, arg);
}

void fn_7100147d9c(UnkTaggedWidgetObject* self, UnkWidgetArg* arg) {
    ConstructTaggedWidget<fn_710020ae7c, 0xfffe, 0xa0cu << 16>(self, arg);
}

void fn_71001431ec(UnkTaggedWidgetObject* self, UnkWidgetArg* arg) {
    ConstructTaggedWidget<fn_71001f2a08, 0xfffffe, 0x12u << 24>(self, arg);
}

// As src/base/spatial/UnkModelInstanceSet.cpp declares it (the argument is the object's name).
struct UnkModelInstance;

void fn_7100147c4c(UnkModelInstance* instance, const base::global::CStrId* name) {
    ConstructTaggedWidget<fn_710020a18c, 0xfffe, 0x90cu << 16, &UnkTaggedWidgetObject::field_0x178>(
        reinterpret_cast<UnkTaggedWidgetObject*>(instance),
        reinterpret_cast<UnkWidgetArg*>(const_cast<base::global::CStrId*>(name)));
}

void fn_7100147b0c(UnkTaggedWidgetObject* self, UnkWidgetArg* arg) {
    ConstructTaggedWidget<fn_71002094dc, 0xfffe, 0x80cu << 16, nullptr>(self, arg);
}
