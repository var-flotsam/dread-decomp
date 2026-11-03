// Constructors of objects tagged for debugging, from units 125 and 127 to 130 of tools/tu_map.py
// (their destructors are in src/base/UnkTaggedObjectDtors.cpp, "one base, sub-object reset then
// destroyed"): the vtable, the flags word (0xff09f000) and the live tag 0x600d600d, no counted
// resource, the sub-object at +0x20 built from the argument (fn_710020c38c), the self pointer, the
// flags' low bit and top byte (the class's kind), then the sub-object's mode (fn_710020c46c) and a
// copy of its +0x108 into its +0x118. Names and layouts are placeholders.
#include "types.h"

struct UnkTaggedSubArg;

void fn_710020c38c(void* sub, UnkTaggedSubArg* arg);  // the sub-object's constructor
void fn_710020c46c(void* sub, u32 mode);

struct UnkTaggedSubObject {
    void* vtable;
    u32 flags;
    u32 tag;          // 0xc, 0x600d600d while alive
    void* resource;   // 0x10
    void* self;       // 0x18
    u8 sub[0x108];    // 0x20, size unknown
    u64 field_0x128;  // the sub-object's +0x108
    u8 pad_0x130[0x8];
    u64 field_0x138;  // the sub-object's +0x118
};

template <char* Vtable, u32 Kind, u32 Mode>
static inline void ConstructTagged(UnkTaggedSubObject* self, UnkTaggedSubArg* arg) {
    self->resource = nullptr;
    self->flags = 0xff09f000;
    self->tag = 0x600d600d;
    self->vtable = Vtable + 0x10;
    fn_710020c38c(self->sub, arg);
    self->field_0x138 = 0;
    self->self = self;
    self->flags = (self->flags & 0xfffffe) | (Kind << 24) | 1;
    fn_710020c46c(self->sub, Mode);
    self->field_0x138 = self->field_0x128;
}

extern char g_unk_7101a1c030[];
extern char g_unk_7101a1c1b8[];
extern char g_unk_7101a1c4a8[];
void fn_7100142b8c(UnkTaggedSubObject* self, UnkTaggedSubArg* arg) { ConstructTagged<g_unk_7101a1c030, 0x10, 1>(self, arg); }
void fn_7100143b3c(UnkTaggedSubObject* self, UnkTaggedSubArg* arg) { ConstructTagged<g_unk_7101a1c1b8, 0xd, 4>(self, arg); }
void fn_7100144bac(UnkTaggedSubObject* self, UnkTaggedSubArg* arg) { ConstructTagged<g_unk_7101a1c4a8, 0xf, 3>(self, arg); }

// ---- the same with a member at +0x140/+0x148 that registers with the object (units 127, 129) ----

void fn_71000255c4(void* member);                // the member's constructor
void fn_7100025884(void* member, void* owner);  // registers the owner with it
void fn_710020d0dc(void* sub, UnkTaggedSubArg* arg);

// Unit 127: one base, its own sub-object constructor, kind 0x11.
struct UnkTaggedSubObject127 {
    void* vtable;
    u32 flags;
    u32 tag;
    void* resource;
    void* self;
    u8 sub[0x108];    // 0x20
    u64 field_0x128;
    u8 pad_0x130[0x8];
    u64 field_0x138;
    u8 member[0x8];   // 0x140, size unknown
};

extern char g_unk_7101a1c170[];
void fn_710014333c(UnkTaggedSubObject127* self, UnkTaggedSubArg* arg) {
    self->resource = nullptr;
    self->flags = 0xff09f000;
    self->tag = 0x600d600d;
    self->vtable = g_unk_7101a1c170 + 0x10;
    fn_710020d0dc(self->sub, arg);
    self->field_0x138 = 0;
    fn_71000255c4(self->member);
    self->self = self;
    self->flags = (self->flags & 0xfffffe) | (0x11u << 24) | 1;
    self->field_0x138 = self->field_0x128;
    fn_7100025884(self->member, self);
}

// Unit 129: a second base at +0x20 (its vtable further on in the same table), the sub-object at
// +0x28, kind 0xe, mode 2.
struct UnkTaggedSubObject129 {
    void* vtable;
    u32 flags;
    u32 tag;
    void* resource;
    void* self;
    void* vtable2;    // 0x20
    u8 sub[0x108];    // 0x28
    u64 field_0x130;
    u8 pad_0x138[0x8];
    u64 field_0x140;
    u8 member[0x8];   // 0x148, size unknown
};

extern char g_unk_7101a1c2f8[];
void fn_710014419c(UnkTaggedSubObject129* self, UnkTaggedSubArg* arg) {
    self->flags = 0xff09f000;
    self->tag = 0x600d600d;
    self->resource = nullptr;
    self->vtable = g_unk_7101a1c2f8 + 0x10;
    self->vtable2 = g_unk_7101a1c2f8 + 0x178;
    fn_710020c38c(self->sub, arg);
    self->field_0x140 = 0;
    fn_71000255c4(self->member);
    self->self = self;
    self->flags = (self->flags & 0xfffffe) | (0xeu << 24) | 1;
    fn_710020c46c(self->sub, 2);
    self->field_0x140 = self->field_0x130;
    fn_7100025884(self->member, &self->vtable2);
}
