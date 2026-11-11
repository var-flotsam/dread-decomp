// Unit 116 of tools/tu_map.py (switch object; see src/game_lo/UnkSwitchApplyPending_b43.cpp): its
// busy-flag setter, the binder of its link at +0x1c0 (as fn_7100143790 does for unit 127), the
// constructor and a copy-out of a 0x5c-byte block. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU275.h"
#include "game/UnkTU32.h"
#include "types.h"

using base::global::CStrId;

struct UnkLinkTarget127_b66a1;

struct UnkLinkSource116_ovb0168 {
    void** vtable;
};

struct UnkSwitchObject116_ovb0168 {
    u8 pad_0x0[0x28];
    u8 field_0x28[0xf9];
    u8 field_0x121;
    u8 pad_0x122[0x9e];
    void* link;        // 0x1c0
    void* link_extra;  // 0x1c8
    u8 pad_0x1d0[0xbe];
    u8 field_0x28e;
    u8 pad_0x28f[9];
    u8 field_0x298;
    u8 pad_0x299[3];
    s32 field_0x29c;
};
static_assert(__builtin_offsetof(UnkSwitchObject116_ovb0168, field_0x29c) == 0x29c, "layout");

void fn_71001416d8(UnkSwitchObject116_ovb0168* self) {
    u32 on = self->field_0x121;
    if ((on != 0) & (self->field_0x298 == 0))
        fn_71001f7388(self->field_0x28, false);
    self->field_0x298 = true;
    self->field_0x29c = on ^ 1;
}

// Binds the link at +0x1c0 (first word: the bound target) to what the source's virtual at +0x20
// returns, once, remembering the flag at +0x28e.
static inline void BindLink(UnkSwitchObject116_ovb0168* self, UnkLinkSource116_ovb0168* source, void* extra,
                            u32 flag, u32 mode) {
    if (self->link != nullptr)
        return;
    // Keeps the link test ahead of the source test.
    asm volatile("" ::: "memory");
    if (source == nullptr || self->link_extra != nullptr)
        return;
    typedef UnkLinkTarget127_b66a1* (*Fn)(UnkLinkSource116_ovb0168*);
    UnkLinkTarget127_b66a1* target = reinterpret_cast<Fn>(source->vtable[0x20 / 8])(source);
    self->field_0x28e = flag & 1;
    fn_710002589c(&self->link, target, mode, extra);
}

void fn_7100140f00(UnkSwitchObject116_ovb0168* self, UnkLinkSource116_ovb0168* source, void* extra, u32 flag, u32 mode) {
    BindLink(self, source, extra, flag, mode);
}
void fn_71001417b0(UnkSwitchObject116_ovb0168* self, UnkLinkSource116_ovb0168* source, void* extra, u32 flag, u32 mode) {
    BindLink(self, source, extra, flag, mode);
}

// ---- the constructor (the tagged header of src/base/UnkTaggedObjectSubCtors.cpp, a second base at +0x20) ----

struct UnkCreatedSceneObj116_ovb0168;
struct UnkWidget;
struct UnkWidgetArg;
void fn_71001f4dcc(UnkWidget* self, UnkWidgetArg* a);

struct UnkTaggedPart116_ovb0168 {
    u8 pad_0x0[0x139];
    u8 field_0x139;
};

struct UnkTaggedObject116_ovb0168 {
    void* vtable;
    u32 flags;
    u32 tag;
    void* resource;
    void* self;       // 0x18
    void* vtable2;    // 0x20
    u8 sub[0x108];    // 0x28
    UnkTaggedPart116_ovb0168* part;  // 0x130
    u8 pad_0x138[0x80];
    UnkTaggedPart116_ovb0168* field_0x1b8;
    u8 member[0x8];   // 0x1c0, size unknown
    u8 pad_0x1c8[0xd0];
    u8 busy;          // 0x298
    u8 pad_0x299[3];
    s32 state;        // 0x29c
};
static_assert(__builtin_offsetof(UnkTaggedObject116_ovb0168, member) == 0x1c0, "layout");

extern char g_unk_7101a1be28[];

void fn_7100140bbc(UnkTaggedObject116_ovb0168* self, const CStrId* name) {
    self->flags = 0xff09f000;
    self->tag = 0x600d600d;
    self->resource = nullptr;
    self->vtable = g_unk_7101a1be28 + 0x10;
    self->vtable2 = g_unk_7101a1be28 + 0x1d0;
    fn_71001f4dcc((UnkWidget*)self->sub, (UnkWidgetArg*)name);
    self->field_0x1b8 = nullptr;
    fn_71000255c4(self->member);
    u32 flags = self->flags;
    self->state = 1;
    self->busy = 0;
    self->self = self;
    self->flags = (flags & 0xfffffe) | (0x18u << 24) | 1;
    UnkTaggedPart116_ovb0168* part = self->part;
    self->field_0x1b8 = part;
    part->field_0x139 = 1;
    fn_7100025884(self->member, &self->vtable2);
}

// ---- copy-out of the 0x5c-byte block at +0x40 of the object whose second base is `base2` ----

struct UnkBlock116_ovb0168 {
    u32 words[23];
};

struct UnkBlockOwner116_ovb0168 {
    u8 pad_0x0[0x40];
    UnkBlock116_ovb0168 block;
};

bool fn_71001412bc(void** base2, UnkBlock116_ovb0168* out) {
    if (reinterpret_cast<char*>(base2) + 0x20 != reinterpret_cast<char*>(out)) {
        UnkBlockOwner116_ovb0168* self = reinterpret_cast<UnkBlockOwner116_ovb0168*>(reinterpret_cast<char*>(base2) - 0x20);
        // Keeps the object's address as computed (not folded into the compare's operand).
        asm("" : "+r"(self));
        for (u32 i = 0; i < 23; i++)
            out->words[i] = self->block.words[i];
    }
    return true;
}
