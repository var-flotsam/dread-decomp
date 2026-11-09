// GUI::CMapMenuController's unit (tools/tu_map.py, unit 1689): the controller's set-up, reached
// through its vtable. It sets its own display flags, then finds its widgets by name and keeps a
// weak handle to each in its own slots: the global view and the custom markers (loaded through
// the GUI registry), the crosshair and its "Crosshair-" children, the inspector, the four arrows,
// the headers, the input labels, the item and EMMI counters and the navigation hints (each hint
// moved right of its icon). Last it takes the two text styles of the unit (built by its
// initialiser, src/game_s7/fn_7100b48ca0.cpp) and refreshes the text blocks they changed. The other
// code of the map menu reads these slots (fn_7100b3e5ac uses the input labels). Names and layouts
// are placeholders.
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkGuiRoot.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using UnkGuiObj = UnkGuiDisplayObject;
using unk_static_init::UnkBufferParam;

struct UnkPanel_71011dfeb0;
struct UnkGuiItemState;

// A 2D position returned in memory (src/game/UnkGuiAnchor.cpp).
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
};

// The container's own set-up, before its flags (src/game/GUI/CHUD/fn_71011dfeb0.cpp calls it too).
void fn_71011d9b3c(UnkPanel_71011dfeb0* self);
void fn_71011cf364(void* composition, s32 value);
// The child at a dotted path ("Content.Navigation"), given as the path string's text.
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);
// The position of an item's anchor (src/game/UnkGuiAnchor.cpp).
UnkGuiPos fn_71011f6240(const UnkGuiItemState* state, u32 h, u32 v);
// Sets the object's x (src/game/GUI/CDisplayObject.cpp).
void fn_71011cc748(f32 x, UnkGuiDisplayObject* self);
// Refreshes a text block (the flag arguments as the map menu passes them).
void fn_7100eb7358(void* object, u32 a, u32 b);

// The unit's two text styles (src/game_s7/fn_7100b48ca0.cpp).
extern UnkBufferParam g_unk_7172763b78;
extern UnkBufferParam g_unk_7172763bf8;

namespace {

// A string id's data as this function reads it: the text (+0x10, whose key at +0x30 is the hash
// the lookups use) and the use count bumped on each read of the name (+0x44, not given back).
struct UnkIdData {
    u8 unk_0x0[0x10];
    CRntString text;
    u8 unk_0x40[0x4];
    union {
        u32 use_count;
    };
};
static_assert(__builtin_offsetof(UnkIdData, use_count) == 0x44, "UnkIdData placeholder layout");

u64 Hash(const CStrId& id) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id.data) + 0x30);
}

// A display object as this function sees it: the slot of the weak handle (vf_0x28), a setting by
// number (vf_0x78), the anchor state (+0x78, its vertical anchor +0x7c), the name (+0x130), the
// child list (+0x1d0) and, on the global view, the composition it adds next to itself (+0x230).
struct UnkChildLink;
struct UnkMapWidget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkWeakHandle** vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78(s32 value, bool flag);

    u8 unk_0x8[0x70];
    UnkGuiItemState* AnchorState() { return reinterpret_cast<UnkGuiItemState*>(unk_0x78); }
    u8 unk_0x78[0x4];
    u32 field_0x7c;
    u8 unk_0x80[0xb0];
    UnkIdData* name;
    u8 unk_0x138[0x98];
    UnkChildLink* children;
    u8 unk_0x1d8[0x58];
    UnkGuiDisplayObject* field_0x230;
};
static_assert(__builtin_offsetof(UnkMapWidget, field_0x7c) == 0x7c, "UnkMapWidget placeholder layout");
static_assert(__builtin_offsetof(UnkMapWidget, name) == 0x130, "UnkMapWidget placeholder layout");
static_assert(__builtin_offsetof(UnkMapWidget, children) == 0x1d0, "UnkMapWidget placeholder layout");
static_assert(__builtin_offsetof(UnkMapWidget, field_0x230) == 0x230, "UnkMapWidget placeholder layout");

struct UnkChildLink {
    UnkMapWidget* child;
    UnkChildLink* next;
};

UnkMapWidget* AsWidget(UnkGuiDisplayObject* object) {
    return reinterpret_cast<UnkMapWidget*>(object);
}

// A slot holding a weak handle to a widget.
struct UnkWidgetSlot {
    UnkWeakHandle* handle;

    UnkGuiDisplayObject* Get() const {
        return handle ? reinterpret_cast<UnkGuiDisplayObject*>(handle->target) : nullptr;
    }
    // The target, reading the handle without testing it (as the original does where it has just
    // stored one).
    UnkGuiDisplayObject* Target() const { return reinterpret_cast<UnkGuiDisplayObject*>(handle->target); }
    // Keeps the weak handle of `object` (nothing when it has no handle slot).
    void Set(UnkGuiDisplayObject* object) {
        AssignRef(&handle, GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object)));
    }
};

// The handle of a crosshair child, in a fixed array packed after its 32-bit count.
struct __attribute__((packed)) UnkPackedHandle {
    UnkWeakHandle* handle;
};

// A text block refreshed when its style changes (+0x50 the style's numbers, +0x60 its font name,
// +0xc8 its last word, +0xd5 whether it is in use).
struct UnkTextBlock {
    u8 unk_0x0[0x50];
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    u8 unk_0x5c[0x4];
    CRntString font;
    u8 unk_0x90[0x38];
    u32 field_0xc8;
    u8 unk_0xcc[0x9];
    bool in_use;
};
static_assert(__builtin_offsetof(UnkTextBlock, font) == 0x60, "UnkTextBlock placeholder layout");
static_assert(__builtin_offsetof(UnkTextBlock, field_0xc8) == 0xc8, "UnkTextBlock placeholder layout");
static_assert(__builtin_offsetof(UnkTextBlock, in_use) == 0xd5, "UnkTextBlock placeholder layout");

// Copies a style into a text block; refreshes the block when it is in use and the style changed
// (its second number, its font name or its third number).
void ApplyStyle(UnkTextBlock* block, const UnkBufferParam* style) {
    const CRntString* font = reinterpret_cast<const CRntString*>(style->field_0x10);
    bool changed;
    if (style->field_0x4 == block->field_0x54 && !fn_710007f220(font, &block->font))
        changed = style->field_0x8 != block->field_0x58;
    else
        changed = true;
    block->field_0x50 = style->field_0x0;
    block->field_0x54 = style->field_0x4;
    block->field_0x58 = style->field_0x8;
    fn_710007eaa0(&block->font, font);
    block->field_0xc8 = style->field_0x78;
    if (changed && block->in_use)
        fn_7100eb7358(block, 0, 1);
}

}  // namespace

struct UnkMapMenuController_b39950 {
    u8 unk_0x0[0x230];
    bool field_0x230;
    u8 unk_0x231[0x3];
    u32 field_0x234;
    UnkWidgetSlot global_view;     // +0x238 "GlobalViewComposition" (registry)
    UnkWidgetSlot custom_markers;  // +0x240 "custommarkercomposition" (registry)
    UnkWidgetSlot content;         // +0x248 "Content" (a container)
    UnkWidgetSlot inspector;       // +0x250 "Inspector" (a container)
    UnkWidgetSlot inspector_label; // +0x258 "Inspector-Label" in it (a label)
    u8 unk_0x260[0x8];
    UnkWidgetSlot crosshair;       // +0x268 "CrosshairComposition"
    u32 crosshair_count;           // +0x270
    UnkPackedHandle crosshairs[12];  // +0x274 its "Crosshair-" children
    u8 unk_0x2d4[0x4];
    UnkWidgetSlot arrows[4];       // +0x2d8 "Arrow_U", "Arrow_D", "Arrow_L", "Arrow_R"
    UnkWidgetSlot header_c_label;  // +0x2f8
    UnkWidgetSlot header_r_bg;     // +0x300
    UnkWidgetSlot header_r_icon;   // +0x308
    UnkWidgetSlot header_r_label;  // +0x310
    UnkWidgetSlot header_l_bg;     // +0x318
    UnkWidgetSlot header_l_icon;   // +0x320
    UnkWidgetSlot header_l_label;  // +0x328
    UnkWidgetSlot global_map;      // +0x330 "GlobalMap"
    UnkWidgetSlot input_label;     // +0x338 "CLabelAInput" (a label)
    UnkWidgetSlot back_label;      // +0x340 "Back" (a label)
    UnkWidgetSlot hint_move;       // +0x348 "Menucontrols-Move" (a label)
    UnkWidgetSlot hint_recenter;   // +0x350 "Menucontrols-Recenter" (a label)
    UnkWidgetSlot hint_fast_mode;  // +0x358 "Menucontrols-FastMode" (a label)
    UnkWidgetSlot emmi_list;       // +0x360 "EmmiList"
    UnkWidgetSlot emmi_remaining;  // +0x368 "CLabel-EMMIRemaining" in it (a label)
    UnkWidgetSlot item_discovered; // +0x370 "ItemDiscovered"
    UnkWidgetSlot item_percentage; // +0x378 "CLabel-Percentage" in it (a label)
    UnkWidgetSlot item_count;      // +0x380 "CLabel-Items" in it (a label)
    UnkWidgetSlot highlight_icons; // +0x388 "CLabelHighlightIcons"
    UnkWidgetSlot navigation;      // +0x390 "Content.Navigation" (a container)
    u8 unk_0x398[0x28];
    UnkTextBlock text_b;           // +0x3c0, styled by g_unk_7172763bf8
    u8 unk_0x498[0x180];
    UnkTextBlock text_a;           // +0x618, styled by g_unk_7172763b78
};
static_assert(__builtin_offsetof(UnkMapMenuController_b39950, global_view) == 0x238, "map menu placeholder layout");
static_assert(__builtin_offsetof(UnkMapMenuController_b39950, crosshair_count) == 0x270, "map menu placeholder layout");
static_assert(__builtin_offsetof(UnkMapMenuController_b39950, crosshairs) == 0x274, "map menu placeholder layout");
static_assert(__builtin_offsetof(UnkMapMenuController_b39950, arrows) == 0x2d8, "map menu placeholder layout");
static_assert(__builtin_offsetof(UnkMapMenuController_b39950, global_map) == 0x330, "map menu placeholder layout");
static_assert(__builtin_offsetof(UnkMapMenuController_b39950, navigation) == 0x390, "map menu placeholder layout");
static_assert(__builtin_offsetof(UnkMapMenuController_b39950, text_b) == 0x3c0, "map menu placeholder layout");
static_assert(__builtin_offsetof(UnkMapMenuController_b39950, text_a) == 0x618, "map menu placeholder layout");

namespace {

UnkGuiDisplayObject* AsObject(UnkMapMenuController_b39950* self) {
    return reinterpret_cast<UnkGuiDisplayObject*>(self);
}

// `object` if it is a display-object container, else null.
UnkGuiDisplayObject* IfContainer(UnkGuiDisplayObject* object) {
    return fn_710009d50c(object->GetClass(), fn_71011d7b84()) ? object : nullptr;
}

// `object` if it is a label, else null.
UnkGuiDisplayObject* IfLabel(UnkGuiDisplayObject* object) {
    return fn_710009d50c(object->GetClass(), fn_71011e8dfc()) ? object : nullptr;
}

// The child of `container` with this name, if it is a label.
UnkGuiDisplayObject* FindLabel(UnkGuiDisplayObject* container, const char* name) {
    CStrId id(name, true);
    UnkGuiDisplayObject* child = fn_71011d94d4(container, id);
    return child ? IfLabel(child) : nullptr;
}

// Keeps the child of `container` with this name hash in `slot`, if there is one.
void BindChild(UnkWidgetSlot* slot, UnkGuiDisplayObject* container, const CStrId& name) {
    if (UnkGuiDisplayObject* child = fn_71011d98ec(container, Hash(name)))
        slot->Set(child);
}

void BindChild(UnkWidgetSlot* slot, UnkGuiDisplayObject* container, const char* name) {
    UnkGuiDisplayObject* child;
    {
        CStrId id(name, true);
        child = fn_71011d98ec(container, Hash(id));
    }
    if (child)
        slot->Set(child);
}

// Keeps a navigation hint label and moves it right of its icon.
void BindHint(UnkWidgetSlot* slot, UnkGuiDisplayObject* navigation, const char* name) {
    UnkGuiDisplayObject* child;
    {
        CStrId id(name, true);
        child = fn_71011d98ec(navigation, Hash(id));
        child = child ? IfLabel(child) : nullptr;
    }
    if (!child)
        return;
    slot->Set(child);
    UnkMapWidget* label = AsWidget(slot->Get());
    UnkGuiPos pos = fn_71011f6240(label->AnchorState(), 2, label->field_0x7c);
    fn_71011cc748(pos.x + 0.012f, reinterpret_cast<UnkGuiDisplayObject*>(label));
}

// Sets a numbered setting (vf_0x78) of the child with this name, if there is one.
void SetChildSetting(UnkGuiDisplayObject* container, const char* name, s32 value) {
    UnkGuiDisplayObject* child;
    {
        CStrId id(name, true);
        child = fn_71011d94d4(container, id);
    }
    if (child)
        AsWidget(child)->vf_0x78(value, true);
}

// A composition loaded through the GUI registry, kept in `slot`.
void Load(UnkWidgetSlot* slot, const char* name) {
    UnkGuiRoot* root = g_unk_717271e8f8;
    CRntString path(name);
    UnkGuiHandle loaded = fn_71011b69d8(root, path);
    slot->Set(loaded.ptr->target);
}

}  // namespace

// NON_MATCHING(major): written for the port from the original's behaviour (12 KB); not compared
// instruction by instruction.
void fn_7100b39950(UnkMapMenuController_b39950* self) {
    UnkGuiDisplayObject* const me = AsObject(self);
    fn_71011d9b3c(reinterpret_cast<UnkPanel_71011dfeb0*>(self));
    fn_71011cdce8(me, false);
    fn_71011cf2a4(me, true);
    fn_71011cf364(self, 0);
    fn_71011cde44(me, 2);

    SetChildSetting(me, "Black", 0x34);
    SetChildSetting(me, "Grid", 0x33);
    SetChildSetting(me, "Vignette_B", 0x32);

    if (!self->global_view.Get()) {
        Load(&self->global_view, "GlobalViewComposition");
        fn_71011cdce8(self->global_view.Get(), false);
        // The original reads the handle without testing it.
        AsWidget(self->global_view.Target())->vf_0x78(0, true);
        fn_71011cde44(self->global_view.Get(), 2);
        fn_71011d968c(me, self->global_view.Get());
        fn_71011d968c(me, AsWidget(self->global_view.Target())->field_0x230);
    }
    if (!self->custom_markers.Get()) {
        Load(&self->custom_markers, "custommarkercomposition");
        fn_71011cdce8(self->custom_markers.Get(), false);
        fn_71011cf2a4(self->custom_markers.Get(), false);
        fn_71011cf364(self->custom_markers.Get(), 0);
        fn_71011cde44(self->custom_markers.Get(), 2);
        fn_71011d968c(me, self->custom_markers.Get());
    }

    {
        UnkGuiDisplayObject* area_inspector;
        {
            CStrId id("AreaInspector", true);
            area_inspector = fn_71011d98ec(me, Hash(id));
        }
        if (area_inspector)
            fn_71011cdc30(area_inspector, false);
    }

    {
        UnkGuiDisplayObject* crosshair;
        {
            CStrId id("CrosshairComposition", true);
            crosshair = fn_71011d98ec(me, Hash(id));
        }
        if (crosshair) {
            self->crosshair.Set(crosshair);
            for (u32 i = 0; i < self->crosshair_count; ++i)
                ResetRef(&self->crosshairs[i].handle);
            self->crosshair_count = 0;
            UnkMapWidget* parent = AsWidget(self->crosshair.Target());
            for (UnkChildLink* l = parent->children; l; l = l->next) {
                UnkIdData* name = l->child->name;
                ++name->use_count;
                if (l->child->name == reinterpret_cast<UnkIdData*>(&g_unk_7101d47108->empty))
                    continue;
                if (!fn_710007fb9c(&l->child->name->text, "Crosshair-"))
                    continue;
                UnkWeakHandle* h = GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(l->child));
                UnkPackedHandle& item = self->crosshairs[self->crosshair_count];
                item.handle = nullptr;
                if (h) {
                    UnkWeakHandle* slot = nullptr;
                    AssignRef(&slot, h);  // the pushed copy's reference
                    item.handle = slot;
                }
                ++self->crosshair_count;
            }
        }
    }

    CStrId content("Content", true);
    CStrId transport_labels("TransportLabels", true);
    if (UnkGuiDisplayObject* child = fn_71011d98ec(me, Hash(content))) {
        if (IfContainer(child))
            self->content.Set(child);
    }

    CStrId inspector("Inspector", true);
    CStrId inspector_label("Inspector-Label", true);
    if (UnkGuiDisplayObject* child = fn_71011d98ec(me, Hash(inspector))) {
        if (IfContainer(child)) {
            self->inspector.Set(child);
            if (UnkGuiDisplayObject* label = fn_71011d98ec(child, Hash(inspector_label))) {
                if (IfLabel(label))
                    self->inspector_label.Set(label);
            }
        }
    }

    CStrId arrow_u("Arrow_U", true);
    CStrId arrow_d("Arrow_D", true);
    CStrId arrow_l("Arrow_L", true);
    CStrId arrow_r("Arrow_R", true);
    BindChild(&self->arrows[0], me, arrow_u);
    BindChild(&self->arrows[1], me, arrow_d);
    BindChild(&self->arrows[2], me, arrow_l);
    BindChild(&self->arrows[3], me, arrow_r);
    self->field_0x234 = 0;

    BindChild(&self->global_map, me, "GlobalMap");
    BindChild(&self->header_c_label, me, "Header-C-Label");
    BindChild(&self->header_r_bg, me, "Header-R-Bg");
    BindChild(&self->header_r_icon, me, "Header-R-Icon");
    BindChild(&self->header_r_label, me, "Header-R-Label");
    BindChild(&self->header_l_bg, me, "Header-L-Bg");
    BindChild(&self->header_l_icon, me, "Header-L-Icon");
    BindChild(&self->header_l_label, me, "Header-L-Label");

    if (UnkGuiDisplayObject* label = FindLabel(me, "CLabelAInput"))
        self->input_label.Set(label);
    if (UnkGuiDisplayObject* label = FindLabel(me, "Back"))
        self->back_label.Set(label);

    {
        UnkGuiDisplayObject* items;
        {
            CStrId id("ItemDiscovered", true);
            items = fn_71011d98ec(me, Hash(id));
        }
        if (items) {
            self->item_discovered.Set(items);
            if (UnkGuiDisplayObject* label = FindLabel(self->item_discovered.Get(), "CLabel-Percentage"))
                self->item_percentage.Set(label);
            if (UnkGuiDisplayObject* label = FindLabel(self->item_discovered.Get(), "CLabel-Items"))
                self->item_count.Set(label);
        }
    }
    {
        UnkGuiDisplayObject* emmis;
        {
            CStrId id("EmmiList", true);
            emmis = fn_71011d98ec(me, Hash(id));
        }
        if (emmis) {
            self->emmi_list.Set(emmis);
            if (UnkGuiDisplayObject* label = FindLabel(self->emmi_list.Get(), "CLabel-EMMIRemaining"))
                self->emmi_remaining.Set(label);
        }
    }
    {
        UnkGuiDisplayObject* navigation;
        {
            CRntString path("Content.Navigation");
            navigation = fn_71011d9508(me, reinterpret_cast<const char* const*>(&path));
            navigation = navigation ? IfContainer(navigation) : nullptr;
        }
        if (navigation) {
            self->navigation.Set(navigation);
            BindHint(&self->hint_move, self->navigation.Get(), "Menucontrols-Move");
            BindHint(&self->hint_recenter, self->navigation.Get(), "Menucontrols-Recenter");
            BindHint(&self->hint_fast_mode, self->navigation.Get(), "Menucontrols-FastMode");
        }
    }
    self->field_0x230 = false;

    BindChild(&self->highlight_icons, me, "CLabelHighlightIcons");

    ApplyStyle(&self->text_a, &g_unk_7172763b78);
    ApplyStyle(&self->text_b, &g_unk_7172763bf8);
}
