// GUI::CList unit: lays out the visible entries (first to last): shows each one and, for buttons,
// places it, its background and its label, sizing them to an equal share of the list when the list
// stretches its entries (+0x260). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CButton.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::math::CVector2D;

struct UnkList_ovb0059;
struct UnkSprite_spr;
UnkGuiDisplayObject* fn_71011c0150(UnkMissionLogButton_ovb0079* self);

struct UnkListEntry_l2004ac {
    void** vtable;
    u8 pad_0x8[0x88];
    f32 height;  // 0x90
    f32 field_0x94;
    f32 scale_y;  // 0x98

    const base::reflection::CClass* GetClass() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkListEntry_l2004ac*)>(vtable[0])(this);
    }
    void vf_0x260() { reinterpret_cast<void (*)(UnkListEntry_l2004ac*)>(vtable[0x260 / 8])(this); }

    UnkGuiDisplayObject* Object() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }
    UnkMissionLogButton_ovb0079* Button() { return reinterpret_cast<UnkMissionLogButton_ovb0079*>(this); }
    UnkSprite_spr* Sprite() { return reinterpret_cast<UnkSprite_spr*>(this); }
};

struct UnkListNode_l2004ac {
    UnkListEntry_l2004ac* item;
    UnkListNode_l2004ac* next;
};

struct UnkListLayout_l2004ac {
    u8 pad_0x0[0x8c];
    f32 width;  // 0x8c
    f32 height;  // 0x90
    f32 scale_x;  // 0x94
    f32 scale_y;  // 0x98
    u8 pad_0x9c[0x1c8 - 0x9c];
    u32 count;  // 0x1c8
    u8 pad_0x1cc[0x4];
    UnkListNode_l2004ac* head;  // 0x1d0
    u8 pad_0x1d8[0x230 - 0x1d8];
    f32 spacing_x;  // 0x230
    f32 spacing_y;  // 0x234
    s32 visible;  // 0x238
    s32 first;  // 0x23c
    s32 last;  // 0x240
    u8 pad_0x244[0x260 - 0x244];
    bool stretch;  // 0x260
    bool field_0x261;

    // The node at `index`, or null past the end.
    UnkListNode_l2004ac* NodeAt(u32 index) {
        u32 n = count;
        if (index < n) {
            UnkListNode_l2004ac* const* link = &head;
            for (;;) {
                UnkListNode_l2004ac* node = *link;
                if (index == 0)
                    return node;
                link = &node->next;
                --n;
                --index;
                if (n == 0)
                    break;
            }
        }
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkListLayout_l2004ac, count) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkListLayout_l2004ac, stretch) == 0x260, "layout");

static inline void Place(UnkGuiDisplayObject* object, const CVector2D& position, u32 h_mode, u32 v_mode) {
    fn_71011cde64(object, &position, h_mode, v_mode);
}

void fn_71012004ac(UnkList_ovb0059* self_) {
    auto* self = reinterpret_cast<UnkListLayout_l2004ac*>(self_);
    if (!self->field_0x261)
        return;
    s32 span = self->last - self->first;
    if (span < 0)
        return;

    f32 width = self->width * self->scale_x;
    f32 entry_height = self->height * self->scale_y / (f32)(span + 1);
    f32 spacing_x = self->spacing_x;
    f32 spacing_y = self->spacing_y;
    CVector2D size = {width - spacing_x, entry_height - spacing_y};
    CVector2D position = {0.0f, 0.0f};
    if (self->stretch)
        position = {spacing_x * 0.5f, spacing_y * 0.5f};

    UnkListNode_l2004ac* node = self->NodeAt(self->first);
    for (s32 i = self->first; i <= self->last; ++i) {
        fn_71011cdc30(node->item->Object(), true);
        UnkListEntry_l2004ac* entry = node->item;
        if (entry && fn_710009d50c(entry->GetClass(), fn_71011be400())) {
            Place(entry->Object(), position, 0, 0);
            Place(fn_71011c0150(entry->Button()), {0.0f, 0.0f}, 0, 0);
            if (UnkGuiDisplayObject* label = fn_71011c08dc(entry->Button()))
                Place(label, {0.0f, 0.0f}, 0, 0);
            if (self->stretch) {
                if (UnkGuiDisplayObject* label = fn_71011c08dc(entry->Button())) {
                    fn_71011cf8d0(reinterpret_cast<UnkSprite_spr*>(label), &size);
                    Place(label, {0.0f, 0.0f}, 1, 1);
                }
                fn_71011cf8d0(entry->Sprite(), &size);
                fn_71011cf8d0(reinterpret_cast<UnkSprite_spr*>(fn_71011c0150(entry->Button())), &size);
                entry->vf_0x260();
            }
        }
        position.y += self->stretch ? entry_height : node->item->height * node->item->scale_y + self->spacing_y;
        node = node->next;
    }
}
