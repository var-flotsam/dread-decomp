// GUI::CButton unit: selects what a button's label names for the given state (one of the four ids at
// +0x230: states 0, 1, 3, 2). A label with a "." is a path to a child of the button's container (+0x270, its
// object at +0x10); a child inside a GUI::CList selects itself in the list (by name when it is the flag
// +0xb4's kind, else by moving the list to its first or last entry) and takes the focus when the list can;
// any other target plays the select sound and takes the focus. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkGuiRoot.h"
#include "game/sound/SGUISoundEventsSelector.h"

using base::global::CStrId;

struct UnkList_ovb0059;
void CList_SelectItemById(UnkList_ovb0059* self, const CStrId* id);
char* fn_7101201658(char* p0);

// A GUI::CList as this function uses it.
struct UnkButtonList_l1bf744 {
    void** vtable;
    u8 pad_0x8[0x1c0];
    s32 count;  // 0x1c8
    u8 pad_0x1cc[0x78];
    s32 index;  // 0x244
    s32 field_0x248;

    void vf_0x150(UnkGuiEvent* ev) {
        reinterpret_cast<void (*)(UnkButtonList_l1bf744*, UnkGuiEvent*)>(vtable[0x150 / 8])(this, ev);
    }
    void vf_0x158(UnkGuiEvent* ev) {
        reinterpret_cast<void (*)(UnkButtonList_l1bf744*, UnkGuiEvent*)>(vtable[0x158 / 8])(this, ev);
    }
};
static_assert(__builtin_offsetof(UnkButtonList_l1bf744, field_0x248) == 0x248, "layout");

// Selects the list's entry with the given name.
inline void SelectByName(UnkButtonList_l1bf744* list, CStrId name) {
    CList_SelectItemById(reinterpret_cast<UnkList_ovb0059*>(list), &name);
}

// The object a path names, as far as this function reads it.
struct UnkButtonTarget_l1bf744 {
    u8 pad_0x0[0xb4];
    bool field_0xb4;
    u8 pad_0xb5[0x6b];
    UnkGuiDisplayObject* parent;  // 0x120
    u8 pad_0x128[0x8];
    CStrId name;  // 0x130
};
static_assert(__builtin_offsetof(UnkButtonTarget_l1bf744, name) == 0x130, "layout");

struct UnkButtonContainerRef_l1bf744 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* object;  // 0x10
};

struct UnkButtonSelect_l1bf744 {
    u8 pad_0x0[0x120];
    UnkGuiDisplayObject* parent;  // 0x120
    u8 pad_0x128[0x108];
    // The label for each state: 0, 1, 3 and 2 in that order.
    CStrId label_0;  // 0x230
    CStrId label_1;  // 0x238
    CStrId label_3;  // 0x240
    CStrId label_2;  // 0x248
    u8 pad_0x250[0x20];
    UnkButtonContainerRef_l1bf744* container;  // 0x270
};
static_assert(__builtin_offsetof(UnkButtonSelect_l1bf744, container) == 0x270, "layout");

// An id that starts empty (the pool's empty string, making the pool if there is none yet) and can take a
// reference to another; released as a CStrId.
struct UnkLabelId_l1bf744 {
    base::global::UnkStringData* data;

    UnkLabelId_l1bf744() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkLabelId_l1bf744() { Id().~CStrId(); }
    CStrId& Id() { return *reinterpret_cast<CStrId*>(this); }
    void Take(const CStrId& id) {
        base::global::UnkStringData* other = id.data;
        if (data != other) {
            ++other->ref_count;
            data = other;
        }
    }
};

// The object's class (vtable slot 0) is read before the getter runs.
template <auto Getter>
inline bool IsKindOf(UnkGuiDisplayObject* obj) {
    const base::reflection::CClass* cls = obj->GetClass();
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(cls),
                         reinterpret_cast<const base::reflection::CType*>(Getter()));
}

inline void Focus(UnkGuiDisplayObject* obj) {
    fn_71011cf2a4(obj, true);
    fn_71011b6bbc(g_unk_717271e8f8, obj, false);
}

inline void PlaySelectSound(UnkButtonSelect_l1bf744* self) {
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0188, self);
}

void fn_71011bf744(UnkGuiButtonIds* button, u32 state) {
    auto* self = reinterpret_cast<UnkButtonSelect_l1bf744*>(button);
    if (UnkGuiDisplayObject* parent = self->parent) {
        if (IsKindOf<fn_71011fe904>(parent))
            fn_7101201658(reinterpret_cast<char*>(parent));
    }
    UnkLabelId_l1bf744 label;
    if (state == 0) {
        label.Take(self->label_0);
    } else {
        switch (state) {
        case 1:
            label.Take(self->label_1);
            break;
        case 2:
            label.Take(self->label_2);
            break;
        case 3:
            label.Take(self->label_3);
            break;
        }
    }
    if (!self->container)
        return;
    UnkGuiDisplayObject* container = self->container->object;
    if (!container || !IsKindOf<fn_71011d7b84>(container))
        return;
    ++label.data->reads;
    if (label.data == &g_unk_7101d47108->empty || !CRntString_Contains(&label.data->String(), ".")) {
        // A plain name: a child of the container.
        if (UnkGuiDisplayObject* obj = fn_71011d94d4(container, label.Id())) {
            PlaySelectSound(self);
            Focus(obj);
        }
        return;
    }
    ++label.data->reads;
    UnkGuiDisplayObject* obj = fn_71011d9508(container, &label.data->text);
    if (!obj)
        return;
    auto* target = reinterpret_cast<UnkButtonTarget_l1bf744*>(obj);
    UnkGuiDisplayObject* list_obj = target->parent;
    if (!list_obj || !IsKindOf<fn_71011fe904>(list_obj)) {
        PlaySelectSound(self);
        Focus(obj);
        return;
    }
    auto* list = reinterpret_cast<UnkButtonList_l1bf744*>(list_obj);
    list->field_0x248 = -1;
    if (target->field_0xb4) {
        SelectByName(list, target->name);
    } else {
        UnkGuiEvent ev;
        s32 kind = 0x2b;
        fn_710120235c(&ev, reinterpret_cast<UnkGuiDisplayObject*>(self), &kind, nullptr);
        if (state == 1) {
            list->index = 0;
            list->vf_0x158(&ev);
        } else if (state == 0) {
            list->index = list->count - 1;
            list->vf_0x150(&ev);
        }
        fn_7101202488(&ev);
    }
    if (fn_71011cde14(list_obj)->field_0x3c)
        Focus(list_obj);
}
