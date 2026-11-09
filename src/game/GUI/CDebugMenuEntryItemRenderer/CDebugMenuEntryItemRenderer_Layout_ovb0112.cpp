// GUI::CDebugMenuEntryItemRenderer's unit (tools/tu_map.py): fn_7100affe40 names a display object
// (the id at +0x130), attaches it to `parent` (fn_71011cd424) and hands it to the root
// (fn_71011b5598) under CList's condition (src/game/GUI/CList/CList_Selection_ovb0059.cpp);
// fn_7100aff9fc lays out the row of entry `index` of the menu data: the label (vf_0x40) and the
// entry's name (+0x30 of the element fn_7100affc6c gives) are copied, and a name starting
// "[SUBMENU]" (fn_710007f240) splits the row 70/30 between the text (+0x230) and the submenu marker
// (+0x238); otherwise the text takes the whole row and the marker is hidden. Names and layouts are
// placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "base/math/CVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector2D;

bool fn_71011ceac0(UnkGuiDisplayObject* self);
bool fn_71011ceb34(UnkGuiDisplayObject* self);
void fn_71011b5598(UnkGuiRoot* root, UnkGuiDisplayObject* object);
void fn_71011cd424(UnkGuiDisplayObject* self, UnkGuiDisplayObject* parent);

struct UnkDebugMenuItem_ovb0112 {
    UnkGuiDisplayObject* Object() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }
    u8 pad_0x0[0x130];
    CStrId id;  // 0x130
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[0x2];
    bool field_0x187;
    bool field_0x188;
};

void fn_7100affe40(UnkGuiRoot* root, UnkDebugMenuItem_ovb0112* item, const CStrId* id, UnkGuiDisplayObject* parent) {
    if (!item)
        return;
    item->id = *id;
    if (parent)
        fn_71011cd424(item->Object(), parent);
    if (!item->field_0x187 &&
        (item->field_0x188 || item->field_0x184 ||
         (fn_71011cde14(item->Object())->field_0x3c &&
          (fn_71011ceac0(item->Object()) || fn_71011ceb34(item->Object())))))
        fn_71011b5598(root, item->Object());
}

struct UnkClone_fn_7100affc6c;
using UnkMenuEntrySlot_ovb0112 = UnkCloneElem<UnkClone_fn_7100affc6c, 8>;
extern template UnkMenuEntrySlot_ovb0112& base::UnkVector<UnkMenuEntrySlot_ovb0112>::GrowAt(u32);

struct UnkSprite_spr;
void fn_71011cf8d0(UnkSprite_spr* self, const base::math::CVector2D* size);

struct UnkMenuEntry_ovb0112 {
    u8 pad_0x0[0x30];
    CRntString name;  // 0x30
};

struct UnkMenuEntryData_ovb0112 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual u32 Count();                // vf_0x38
    virtual CRntString Label(u32 index);  // vf_0x40
    u8 pad_0x8[0x8];
    base::UnkVector<UnkMenuEntrySlot_ovb0112> entries;  // 0x10
};

struct UnkDebugMenuEntryItemRenderer_ovb0112 {
    u8 pad_0x0[0x230];
    UnkGuiDisplayObject* text;     // 0x230
    UnkGuiDisplayObject* submenu;  // 0x238
};

static inline void Place(UnkGuiDisplayObject* o, CVector2D& v, u32 h_mode) {
    v = {0.0f, 0.0f};
    fn_71011cde64(o, &v, h_mode, 1);
}

static inline void Resize(UnkGuiDisplayObject* o, CVector2D& v, f32 x, f32 y) {
    v = {x, y};
    fn_71011cf8d0(reinterpret_cast<UnkSprite_spr*>(o), &v);
}

// The label on 70% of the row and the submenu marker on the rest.
static inline void SetSubmenuLayout(UnkDebugMenuEntryItemRenderer_ovb0112* self, CVector2D& v) {
    Place(self->text, v, 2);
    Resize(self->text, v, 0.7f, 1.0f);
    fn_71011cdce8(self->submenu, true);
    Place(self->submenu, v, 3);
    Resize(self->submenu, v, 0.3f, 1.0f);
}

// The label on the whole row, the submenu marker hidden.
static inline void SetPlainLayout(UnkDebugMenuEntryItemRenderer_ovb0112* self, CVector2D& v) {
    Place(self->text, v, 2);
    Resize(self->text, v, 1.0f, 1.0f);
    if (self->submenu)
        fn_71011cdce8(self->submenu, false);
}

void fn_7100aff9fc(UnkDebugMenuEntryItemRenderer_ovb0112* self, UnkMenuEntryData_ovb0112* data, u32 index) {
    if (!data || !self->text)
        return;
    if (data->Count() <= index)
        return;
    CRntString label = data->Label(index);
    UnkMenuEntry_ovb0112* entry = *reinterpret_cast<UnkMenuEntry_ovb0112**>(data->entries.GrowAt(index).data);
    CRntString name(entry->name);
    CRntString text(label);
    CVector2D submenu_size;
    CVector2D plain_size;
    if (self->submenu && name.data && *name.data) {
        if (fn_710007f240(reinterpret_cast<char*>(&name), "[SUBMENU]")) {
            SetSubmenuLayout(self, submenu_size);
        } else {
            SetPlainLayout(self, plain_size);
        }
    } else {
        SetPlainLayout(self, plain_size);
    }
}
