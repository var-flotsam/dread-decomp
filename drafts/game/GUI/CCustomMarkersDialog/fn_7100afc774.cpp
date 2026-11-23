// GUI::CCustomMarkersDialog's unit (tools/tu_map.py unit 1660): the dialog's set-up after the base composition's.
// It keeps weak handles to its list, the list's back button and one button per marker colour (each tinted with
// its minimap colour), its fade screen, marker count, accept and remove widgets. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2566.h"
#include "../../../../src/game_s7/UnkGuiS7.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkPanel_71011dfeb0;
struct UnkRefPtrArray;
void fn_7100afe1d0(UnkRefPtrArray* self, u32 capacity);

// The unit's string ids: the list, the back button, the five marker colour buttons and the button's sprite.
__attribute__((visibility("hidden"))) extern CStrId g_unk_717275d400;
__attribute__((visibility("hidden"))) extern CStrId g_unk_717275d408;
__attribute__((visibility("hidden"))) extern CStrId g_unk_717275d410;
__attribute__((visibility("hidden"))) extern CStrId g_unk_717275d418;
__attribute__((visibility("hidden"))) extern CStrId g_unk_717275d420;
__attribute__((visibility("hidden"))) extern CStrId g_unk_717275d428;
__attribute__((visibility("hidden"))) extern CStrId g_unk_717275d430;
__attribute__((visibility("hidden"))) extern CStrId g_unk_717275d458;

// CMinimapManager::CTunableMinimapManager's marker colours (data/reflection_fields.csv).
struct UnkMinimapTunable_lafc774 {
    u8 pad_0x0[0x1ec];
    CVector3D vMarkerColorA;  // 0x1ec
    CVector3D vMarkerColorB;  // 0x1f8
    CVector3D vMarkerColorC;  // 0x204
    CVector3D vMarkerColorD;  // 0x210
    CVector3D vMarkerColorE;  // 0x21c
};

static inline const UnkMinimapTunable_lafc774& MinimapTunable() {
    return reinterpret_cast<const UnkMinimapTunable_lafc774&>(g_unk_71727ca410);
}

// A counted reference to a weak handle.
struct UnkHandleRef_lafc774 {
    UnkWeakHandle* handle = nullptr;

    UnkHandleRef_lafc774() = default;
    explicit UnkHandleRef_lafc774(UnkWeakHandle* h) : handle(h) {
        if (h)
            ++h->ref_count;
    }
    UnkHandleRef_lafc774(const UnkHandleRef_lafc774& other) { *this = other; }
    UnkHandleRef_lafc774& operator=(const UnkHandleRef_lafc774& other) {
        AssignRef(&handle, other.handle);
        return *this;
    }
    ~UnkHandleRef_lafc774() {
        if (UnkWeakHandle* p = handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

using UnkHandleRefs_lafc774 = base::global::CRntVector<UnkHandleRef_lafc774>;

inline void Clear(UnkHandleRefs_lafc774& refs) {
    for (u64 i = 0; i < refs.count; ++i)
        refs.data[i].~UnkHandleRef_lafc774();
    refs.count = 0;
}

inline void PushBack(UnkHandleRefs_lafc774& refs, const UnkHandleRef_lafc774& ref) {
    u32 n = refs.count;
    u32 capacity = refs.capacity;
    if (capacity <= n && refs.step != 0)
        capacity = (n / refs.step + 1) * refs.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_7100afe1d0(reinterpret_cast<UnkRefPtrArray*>(&refs), capacity);
    new (&refs.data[refs.count]) UnkHandleRef_lafc774(ref);
    ++refs.count;
}

struct UnkCustomMarkersDialog_lafc774;
void fn_7100afe1cc(UnkCustomMarkersDialog_lafc774* self);
void fn_7100afe318(UnkCustomMarkersDialog_lafc774* self);

struct UnkWidget_lafc774;
struct UnkListLink_lafc774 {
    UnkWidget_lafc774* child;
    UnkListLink_lafc774* next;
};

// A display object as this function sees it: the list's children and a button's name and what it calls when
// pressed.
struct UnkWidget_lafc774 {
    virtual const base::reflection::CClass* GetClass();
    u8 pad_0x8[0x128];
    CStrId name;  // 0x130
    u8 pad_0x138[0x98];
    UnkListLink_lafc774* children;  // 0x1d0
    u8 pad_0x1d8[0xb8];
    UnkCustomMarkersDialog_lafc774* on_press_object;  // 0x290
    void (*on_press)(UnkCustomMarkersDialog_lafc774*);  // 0x298
};
static_assert(__builtin_offsetof(UnkWidget_lafc774, name) == 0x130, "button placeholder layout");
static_assert(__builtin_offsetof(UnkWidget_lafc774, children) == 0x1d0, "button placeholder layout");
static_assert(__builtin_offsetof(UnkWidget_lafc774, on_press_object) == 0x290, "button placeholder layout");

struct UnkCustomMarkersDialog_lafc774 : UnkGuiDisplayObject {
    u8 pad_0x198[0x230 - 0x198];
    UnkWeakHandle* marker_count;  // 0x230 "MarkerCount"
    UnkWeakHandle* accept;        // 0x238 "Accept"
    UnkWeakHandle* remove;        // 0x240 "Remove"
    UnkWeakHandle* back;          // 0x248
    UnkWeakHandle* list;          // 0x250
    UnkWeakHandle* fade_screen;   // 0x258 "FadeScreen"
    UnkHandleRefs_lafc774 color_buttons;  // 0x260
};
static_assert(__builtin_offsetof(UnkCustomMarkersDialog_lafc774, marker_count) == 0x230, "dialog placeholder layout");
static_assert(__builtin_offsetof(UnkCustomMarkersDialog_lafc774, color_buttons) == 0x260, "dialog placeholder layout");

// The weak handle of `object` (null when it has no handle slot).
template <class T>
inline UnkWeakHandle* WeakHandleOf(T* object) {
    return GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object));
}

inline UnkGuiDisplayObject* FindChild(UnkGuiDisplayObject* container, const CStrId& id) {
    return fn_71011d98ec(container, id.data->Key());
}

struct UnkMarkerTint_lafc774 {
    CVector3D rgb;
    f32 a;

    UnkMarkerTint_lafc774(const CVector3D& rgb_, f32 a_) : rgb(rgb_), a(a_) {}
};

// Tints the button's sprite with a marker colour.
inline void TintSprite(UnkWidget_lafc774* button, const CVector3D& rgb) {
    UnkMarkerTint_lafc774 color(rgb, 1.0f);
    UnkGuiDisplayObject* sprite = FindChild(reinterpret_cast<UnkGuiDisplayObject*>(button), g_unk_717275d458);
    if (sprite && fn_710009d50c(sprite->GetClass(), fn_71011f6890()))
        unk_gui::fn_71011cfc24(sprite, reinterpret_cast<const unk_gui::UnkColor&>(color), true);
}

void fn_7100afc774(UnkCustomMarkersDialog_lafc774* self) {
    fn_71011d9b3c(reinterpret_cast<UnkPanel_71011dfeb0*>(self));

    UnkGuiDisplayObject* object = FindChild(self, g_unk_717275d400);
    if (!object || !fn_710009d50c(object->GetClass(), fn_71011fe904()))
        return;
    UnkWidget_lafc774* list = reinterpret_cast<UnkWidget_lafc774*>(object);
    AssignRef(&self->list, WeakHandleOf(list));

    Clear(self->color_buttons);
    for (UnkListLink_lafc774* link = list->children; link; link = link->next) {
        UnkWidget_lafc774* button = link->child;
        if (!button || !fn_710009d50c(button->GetClass(), fn_71011be400()))
            continue;
        if (button->name == g_unk_717275d408) {
            AssignRef(&self->back, WeakHandleOf(button));
            button->on_press_object = self;
            button->on_press = fn_7100afe1cc;
            continue;
        }
        if (button->name == g_unk_717275d410)
            TintSprite(button, MinimapTunable().vMarkerColorA);
        else if (button->name == g_unk_717275d418)
            TintSprite(button, MinimapTunable().vMarkerColorB);
        else if (button->name == g_unk_717275d420)
            TintSprite(button, MinimapTunable().vMarkerColorC);
        else if (button->name == g_unk_717275d428)
            TintSprite(button, MinimapTunable().vMarkerColorD);
        else if (button->name == g_unk_717275d430)
            TintSprite(button, MinimapTunable().vMarkerColorE);
        PushBack(self->color_buttons, UnkHandleRef_lafc774(WeakHandleOf(button)));
        button->on_press_object = self;
        button->on_press = fn_7100afe318;
    }

    if (UnkGuiDisplayObject* fade = fn_71011d98ec(self, CStrId("FadeScreen", true).data->Key())) {
        AssignRef(&self->fade_screen, WeakHandleOf(fade));
        UnkWeakHandle* handle = self->fade_screen;
        fn_71011cdc30(handle ? reinterpret_cast<UnkGuiDisplayObject*>(handle->target) : nullptr, false);
    }
    if (UnkGuiDisplayObject* count = fn_71011d98ec(self, CStrId("MarkerCount", true).data->Key()))
        AssignRef(&self->marker_count, WeakHandleOf(count));
    if (UnkGuiDisplayObject* accept = fn_71011d98ec(self, CStrId("Accept", true).data->Key()))
        AssignRef(&self->accept, WeakHandleOf(accept));
    if (UnkGuiDisplayObject* remove = fn_71011d98ec(self, CStrId("Remove", true).data->Key()))
        AssignRef(&self->remove, WeakHandleOf(remove));
}
