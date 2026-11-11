// Unit 295: hands an item list's handles to the renderer and draws with a position copy, then finishes the
// renderer. Names and layouts are placeholders.
#include "game/UnkTU167.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkDrawItem_b260 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void Prepare(u32 flag);  // vf_0x60
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void* GetHandle();  // vf_0x88
};

struct UnkItemList_b260 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual UnkDrawItem_b260** GetItems(u32* count);  // vf_0x68
};

struct UnkRenderer_b260 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Draw(void* list, u64 a, void* b, f32 t, u32 c, u32 d, UnkVec3f* v);  // vf_0x20
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void SetHandles(void** handles, u32 count);  // vf_0x38
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void Finish();  // vf_0x50
};

struct UnkDrawParent_b260 {
    u8 unk_0x0[0x108];
    void* field_0x108;
};

struct UnkDrawer_b260 {
    u8 unk_0x0[8];
    UnkDrawParent_b260* parent;  // 0x8
    UnkRenderer_b260* renderer;  // 0x10
};

void fn_710020f240(UnkDrawer_b260* self, UnkItemList_b260* list, u64 a2, u64 a3, u64 a4, void* arg, u64 a6, u64 a7,
                   bool flag_a, bool flag_b, const UnkVec3f* vec, f32 t) {
    u32 zero = 0;
    u32 n;
    void** handles = fn_7100150b50(&zero);
    void* first;
    u32 count;
    if (list) {
        n = 0;
        UnkDrawItem_b260** items = list->GetItems(&n);
        u64 i = 0;
        if (n) {
            do {
                UnkDrawItem_b260* item = items[i];
                item->Prepare(1);
                handles[i] = item->GetHandle();
                ++i;
            } while (i < n);
        }
        count = (u32)i;
        first = self->parent->field_0x108;
    } else {
        first = nullptr;
        count = 0;
    }
    self->renderer->SetHandles(handles, count);
    UnkRenderer_b260* r = self->renderer;
    UnkVec3f v = *vec;
    f32 tt = t;
    u32 fa = flag_a, fb = flag_b;
    r->Draw(first, 0, arg, tt, fa, fb, &v);
    self->renderer->Finish();
}
