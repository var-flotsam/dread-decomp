// Unit 133 of tools/tu_map.py (switch object; see src/game_lo/fn_7100145e94.cpp): the same four
// functions as unit 116's (src/game/UnkTU116/UnkTU116_Switch_ovb0168.cpp) with this unit's layout
// (the later members sit 0x58 bytes earlier) and callees. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkLinkTarget127_b66a1;
bool fn_710002589c(void* link, UnkLinkTarget127_b66a1* target, u32 mode, void* extra);
void fn_7100208004(void* obj, bool flag);

struct UnkLinkSource133_ovb0168 {
    void** vtable;
};

struct UnkSwitchObject133_ovb0168 {
    u8 pad_0x0[0x28];
    u8 field_0x28[0xf9];
    u8 field_0x121;
    u8 pad_0x122[0x46];
    void* link;        // 0x168
    void* link_extra;  // 0x170
    u8 pad_0x178[0xbe];
    u8 field_0x236;
    u8 pad_0x237[9];
    u8 field_0x240;
    u8 pad_0x241[3];
    s32 field_0x244;
};
static_assert(__builtin_offsetof(UnkSwitchObject133_ovb0168, field_0x244) == 0x244, "layout");

void fn_7100146730(UnkSwitchObject133_ovb0168* self) {
    u32 on = self->field_0x121;
    if ((on != 0) & (self->field_0x240 == 0))
        fn_7100208004(self->field_0x28, false);
    self->field_0x240 = true;
    self->field_0x244 = on ^ 1;
}

// Binds the link at +0x168 (first word: the bound target) to what the source's virtual at +0x20
// returns, once, remembering the flag at +0x236.
void fn_7100146808(UnkSwitchObject133_ovb0168* self, UnkLinkSource133_ovb0168* source, void* extra, u32 flag, u32 mode) {
    if (self->link != nullptr)
        return;
    // Keeps the link test ahead of the source test.
    asm volatile("" ::: "memory");
    if (source == nullptr || self->link_extra != nullptr)
        return;
    typedef UnkLinkTarget127_b66a1* (*Fn)(UnkLinkSource133_ovb0168*);
    UnkLinkTarget127_b66a1* target = reinterpret_cast<Fn>(source->vtable[0x20 / 8])(source);
    self->field_0x236 = flag & 1;
    fn_710002589c(&self->link, target, mode, extra);
}

// ---- the constructor (the tagged header of src/base/UnkTaggedObjectSubCtors.cpp, a second base at +0x20) ----

struct UnkSonarModel_ovb0085;
struct UnkWidget;
struct UnkWidgetArg;
void fn_710020716c(UnkWidget* self, UnkWidgetArg* a);
void fn_71000255c4(void* member);
void fn_7100025884(void* member, void* owner);

struct UnkTaggedPart133_ovb0168 {
    u8 pad_0x0[0x362];
    u8 field_0x362;
};

struct UnkTaggedObject133_ovb0168 {
    void* vtable;
    u32 flags;
    u32 tag;
    void* resource;
    void* self;       // 0x18
    void* vtable2;    // 0x20
    u8 sub[0x108];    // 0x28
    UnkTaggedPart133_ovb0168* part;  // 0x130
    u8 pad_0x138[0x28];
    UnkTaggedPart133_ovb0168* field_0x160;
    u8 member[0x8];   // 0x168, size unknown
    u8 pad_0x170[0xd0];
    u8 busy;          // 0x240
    u8 pad_0x241[3];
    s32 state;        // 0x244
};
static_assert(__builtin_offsetof(UnkTaggedObject133_ovb0168, member) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkTaggedObject133_ovb0168, state) == 0x244, "layout");

extern char g_unk_7101a1c7d8[];

void fn_7100145bdc(UnkSonarModel_ovb0085* obj, const CStrId* name) {
    UnkTaggedObject133_ovb0168* self = reinterpret_cast<UnkTaggedObject133_ovb0168*>(obj);
    self->flags = 0xff09f000;
    self->tag = 0x600d600d;
    self->resource = nullptr;
    self->vtable = g_unk_7101a1c7d8 + 0x10;
    self->vtable2 = g_unk_7101a1c7d8 + 0x1d0;
    fn_710020716c((UnkWidget*)self->sub, (UnkWidgetArg*)name);
    self->field_0x160 = nullptr;
    fn_71000255c4(self->member);
    u32 flags = self->flags;
    self->state = 1;
    self->busy = 0;
    self->self = self;
    self->flags = (flags & 0xfffffe) | (0x5u << 24) | 1;
    UnkTaggedPart133_ovb0168* part = self->part;
    self->field_0x160 = part;
    part->field_0x362 = 1;
    fn_7100025884(self->member, &self->vtable2);
}

// ---- copy-out of the 0x5c-byte block at +0x40 of the object whose second base is `base2` ----

struct UnkBlock133_ovb0168 {
    u32 words[23];
};

struct UnkBlockOwner133_ovb0168 {
    u8 pad_0x0[0x40];
    UnkBlock133_ovb0168 block;
};

bool fn_71001462b0(void** base2, UnkBlock133_ovb0168* out) {
    if (reinterpret_cast<char*>(base2) + 0x20 != reinterpret_cast<char*>(out)) {
        UnkBlockOwner133_ovb0168* self = reinterpret_cast<UnkBlockOwner133_ovb0168*>(reinterpret_cast<char*>(base2) - 0x20);
        // Keeps the object's address as computed (not folded into the compare's operand).
        asm("" : "+r"(self));
        for (u32 i = 0; i < 23; i++)
            out->words[i] = self->block.words[i];
    }
    return true;
}
