// Applies a list selection change: sends "deselected" to the previously selected item and
// "selected" to the new one, optionally plays the selection sound, and scrolls to the new item.
#include "types.h"

struct UnkEvent_0x28 {
    u8 data[0x28];
};

struct UnkItem_71011ff97c {
    virtual void vf_0x0();
    void Call(u32 off, UnkEvent_0x28* ev);
};

struct UnkItemVt_71011ff97c {
    void* slots[0xd0 / 8];
    void (*vf_0xd0)(UnkItem_71011ff97c*, UnkEvent_0x28*);
    void (*vf_0xd8)(UnkItem_71011ff97c*, UnkEvent_0x28*);
    void* slots2[(0x1e0 - 0xe0) / 8];
    void (*vf_0x1e0)(UnkItem_71011ff97c*, bool);
};

static inline UnkItemVt_71011ff97c* Vt(UnkItem_71011ff97c* it) {
    return *reinterpret_cast<UnkItemVt_71011ff97c**>(it);
}

struct UnkNode_71011ff97c {
    UnkItem_71011ff97c* item;
    UnkNode_71011ff97c* next;
};

struct UnkScale_71011ff97c {
    u8 unk_0x0[0x18];
    f32 field_0x18;
    u8 unk_0x1c[0x4];
    f32 field_0x20;
};

struct UnkStruct_71011ff97c {
    u8 unk_0x0[0x1c8];
    s32 count;
    u8 unk_0x1cc[0x4];
    UnkNode_71011ff97c* head;
    u8 unk_0x1d8[0x6c];
    s32 selected;
    s32 previous;
    u8 unk_0x24c[0xc];
    void* field_0x258;
    bool field_0x260;
    u8 unk_0x261[0x2];
    bool field_0x263;

    UnkItem_71011ff97c* At(s32 index) {
        UnkNode_71011ff97c* n = head;
        for (u32 i = 0; i < (u32)index; ++i)
            n = n->next;
        return n->item;
    }
};

void fn_710120235c(UnkEvent_0x28* ev, UnkItem_71011ff97c* sender, const s32* type, void* extra);
void fn_7101202488(UnkEvent_0x28* ev);
void fn_7100fc9d78(void* mgr, void* sound, UnkStruct_71011ff97c* self);
void* fn_71011d94d4(void* root, void* name);
UnkScale_71011ff97c* fn_71011cde14(void* obj);
void fn_71011cca3c(void* obj, f32 value);
extern void* g_unk_717275c380;
extern char g_unk_71729f0220[];
extern char g_unk_7172a10de8[] __attribute__((visibility("hidden")));

void fn_71011ff97c(UnkStruct_71011ff97c* self, bool quiet) {
    if (self->count == 0)
        return;
    if (self->previous >= 0 && self->previous < self->count) {
        UnkItem_71011ff97c* it = self->At(self->previous);
        s32 type = 3;
        UnkEvent_0x28 ev;
        fn_710120235c(&ev, it, &type, nullptr);
        Vt(it)->vf_0xd8(it, &ev);
        if (!quiet && self->field_0x263)
            Vt(it)->vf_0x1e0(it, false);
        fn_7101202488(&ev);
    }
    if (self->selected >= 0 && self->selected < self->count) {
        UnkItem_71011ff97c* it = self->At(self->selected);
        s32 type = 2;
        UnkEvent_0x28 ev;
        fn_710120235c(&ev, it, &type, nullptr);
        Vt(it)->vf_0xd0(it, &ev);
        if (!quiet && self->field_0x263)
            Vt(it)->vf_0x1e0(it, false);
        fn_7101202488(&ev);
    }
    if (!self->field_0x260 && quiet && self->selected != -1)
        fn_7100fc9d78(g_unk_717275c380, g_unk_71729f0220, self);
    if (!self->field_0x258)
        return;
    void* target = fn_71011d94d4(self->field_0x258, g_unk_7172a10de8);
    if (!target)
        return;
    UnkScale_71011ff97c* s = fn_71011cde14(target);
    fn_71011cca3c(target, s->field_0x18 * s->field_0x20 * (f32)self->selected);
}
