// CEmmyGrabSamusAttack (unit 800): a grab attack's destructor. Removes its two render items, releases
// its string id, destroys the thirteen property entries (last first), the property list, the
// listener and then CAttack's part. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "game/UnkRenderManager.h"
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a477e0[];  // the class's vtable
extern void* g_unk_7101a47970[];  // the entries' base vtable
extern void* g_unk_7101a478d8[];  // the property list's vtable

// A render item owner: the item proper starts after its vtable.
struct UnkGrabRenderItem_505744 {
    void** vtable;
    u8 item[8];
};

struct UnkGrabBlock_505744 {
    u64 v[8];
};

struct UnkGrabEntry_505744 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkGrabBlock_505744 block;

    void Destroy() {
        for (int i = 0; i < 8; i++)
            block.v[i] = 0;
        vtable = g_unk_7101a47970 + 2;
        fn_71000a1c60(&value);
    }
};

struct UnkGrabAttack_505744 {
    void** vtable;
    u8 pad_0x8[0x20];
    void** list_vtable;            // 0x28
    u64 list_field_0x8;            // 0x30
    u64 list_field_0x10;           // 0x38
    u64 list_field_0x18;           // 0x40
    u32 list_count;                // 0x48
    u8 pad_0x4c[0x94];
    UnkAttackListenerRaw listener;  // 0xe0
    UnkGrabEntry_505744 entries[13];  // 0x100
    u8 pad_0x6b0[0x40];
    UnkGrabRenderItem_505744* render_a;  // 0x6f0
    UnkGrabRenderItem_505744* render_b;  // 0x6f8
    u8 pad_0x700[0xa0];
    UnkAttackStrIdRaw name;        // 0x7a0
};
static_assert(__builtin_offsetof(UnkGrabAttack_505744, entries) == 0x100, "layout");
static_assert(__builtin_offsetof(UnkGrabAttack_505744, render_a) == 0x6f0, "layout");
static_assert(__builtin_offsetof(UnkGrabAttack_505744, name) == 0x7a0, "layout");

static inline void RemoveRenderItem(UnkGrabRenderItem_505744*& holder) {
    if (holder) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(holder->item), 0);
        holder = nullptr;
    }
}

void fn_7100505744(UnkGrabAttack_505744* self) {
    self->vtable = g_unk_7101a477e0 + 2;
    RemoveRenderItem(self->render_a);
    RemoveRenderItem(self->render_b);
    self->name.Release();
    for (int i = 12; i >= 0; --i)
        self->entries[i].Destroy();
    self->listener.Destroy();
    self->list_vtable = g_unk_7101a478d8 + 2;
    self->list_field_0x10 = 0;
    self->list_field_0x18 = 0;
    self->list_count = 0;
    fn_71003606c0(self);
}
