// GUI::CScrollMenuEntryItemRenderer's unit (tools/tu_map.py unit 1716): binds the renderer's widgets after
// the base's: its button, every "ValueLabel<n>" label (blanked and listed) and two sprites, each set up
// the same way. Names and layouts are placeholders.
#include <stdio.h>

#include "base/UnkVtableList.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "types.h"

using base::global::CStrId;

struct UnkLegendEntryRenderer_ovb0135;
void fn_71011d9328(UnkLegendEntryRenderer_ovb0135* self);
void fn_71011cecc4(UnkGuiDisplayObject* self, f32 value);
void fn_71011ced1c(UnkGuiDisplayObject* self, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, bool flag);

static inline const CStrId& TableId(u32 offset) {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<u8*>(&GameManagerIdTable()) + offset);
}

template <const base::reflection::CClass* (*Getter)()>
static inline UnkGuiDisplayObject* CastTo(UnkGuiDisplayObject* obj) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->GetClass(), Getter()) ? obj : nullptr;
}

// A label's text built from UTF-32 text.
struct UnkValueText_b7544c : UnkGuiText {
    explicit UnkValueText_b7544c(const char32_t* text) { fn_7100066a50(this, text); }
    ~UnkValueText_b7544c() { fn_7100066e44(this); }
};

struct UnkValueLabelNode_b7544c {
    UnkGuiDisplayObject* label;
    UnkValueLabelNode_b7544c* next;
};

struct UnkScrollEntryRendererFields_b7544c {
    u8 pad_0x0[0x230];
    UnkGuiDisplayObject* button;                                 // 0x230
    base::UnkVtableList<UnkValueLabelNode_b7544c> value_labels;  // 0x238
    UnkGuiDisplayObject* sprite_a;                               // 0x268
    UnkGuiDisplayObject* sprite_b;                               // 0x270
};
static_assert(__builtin_offsetof(UnkScrollEntryRendererFields_b7544c, sprite_a) == 0x268, "layout");

static inline void SetUpSprite(UnkGuiDisplayObject*& sprite, UnkGuiDisplayObject* found) {
    sprite = CastTo<fn_71011f6890>(found);
    if (sprite) {
        fn_71011cecc4(sprite, 2.0f);
        fn_71011ced1c(sprite, -1.0f, 2.0f, 2.5f, 2.5f, 2.5f, 1.0f, false);
    }
}

void fn_7100b7544c(UnkGuiDisplayObject* self) {
    fn_71011d9328(reinterpret_cast<UnkLegendEntryRenderer_ovb0135*>(self));  // the base's binding
    UnkScrollEntryRendererFields_b7544c* fields = reinterpret_cast<UnkScrollEntryRendererFields_b7544c*>(self);

    fields->button = CastTo<fn_71011be400>(fn_71011d94d4(self, TableId(0x9e8)));
    if (fields->button)
        fn_71011cf2a4(fields->button, false);

    for (s32 i = 0;;) {
        char name[256];
        sprintf(name, "ValueLabel%d", i);
        UnkGuiDisplayObject* label;
        {
            CStrId id(name, true);
            label = CastTo<fn_71011e8dfc>(fn_71011d94d4(self, id));
        }
        if (!label)
            break;
        ++i;
        {
            UnkValueText_b7544c text(U" ");
            fn_71011ec468(label, text, false);
        }
        auto& list = fields->value_labels;
        auto* node = static_cast<UnkValueLabelNode_b7544c*>(list.allocator->vf_0x40(0x10, 8, list.flags));
        if (node) {
            node->label = label;
            node->next = nullptr;
            list.PushBackTailFirst(node);
        }
    }

    SetUpSprite(fields->sprite_a, fn_71011d94d4(self, TableId(0x1980)));
    SetUpSprite(fields->sprite_b, fn_71011d94d4(self, TableId(0x2450)));
}
