// GUI::CList (tools/tu_map.py unit 2759): a container whose children (the list at +0x1d0, counted at
// +0x1c8) are the entries; one is selected (+0x244, the previous one at +0x248) and the selection is
// applied by fn_71011ff97c. Here: the factory and the second constructor, selecting by name, the
// handlers for focus, events and children (laying out the visible entries is fn_71012004ac). Names and layouts
// are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71011ff97c;
struct UnkSmBase_A6_b62;
struct UnkSmArg_A6_b62;
struct UnkGuiEvent_b49 {
    u8 pad_0x0[0x20];
    u64 kind;  // 0x20
};
struct UnkSprite_spr;
struct UnkGuiButtonFocus_ovb0000;

void fn_71011ff97c(UnkStruct_71011ff97c* self, bool quiet);
void fn_71011d99e4(UnkGuiDisplayObject* self, UnkGuiEvent_b49* event);
struct UnkPanel_71011dfeb0;
bool fn_71011ceac0(UnkGuiDisplayObject* self);
bool fn_71011ceb34(UnkGuiDisplayObject* self);
void fn_71011b5598(UnkGuiRoot* root, UnkGuiDisplayObject* object);
void fn_71011cf8d0(UnkSprite_spr* self, const base::math::CVector2D* size);

extern void* g_unk_7101cb6d08[];  // the list's vtable
extern CStrId g_unk_7172a10df0 __attribute__((visibility("hidden")));  // the selection sound's key

struct UnkListItem_ovb0059 {
    void** vtable;
    u8 pad_0x8[0x84];
    f32 field_0x8c;
    f32 height;  // 0x90
    f32 field_0x94;
    f32 scale_y;  // 0x98
    u8 pad_0x9c[0x18];
    bool field_0xb4;
    u8 pad_0xb5[0x7b];
    CStrId id;  // 0x130
    u8 pad_0x138[0x49];
    bool field_0x181;

    void vf_0xd8(UnkGuiEvent* event) {
        reinterpret_cast<void (*)(UnkListItem_ovb0059*, UnkGuiEvent*)>(vtable[0xd8 / 8])(this, event);
    }
    // Whether the entry can not be selected.
    bool Disabled() { return field_0xb4 && field_0x181; }
};

struct UnkListNode_ovb0059 {
    UnkListItem_ovb0059* item;
    UnkListNode_ovb0059* next;
};

struct UnkList_ovb0059 {
    void** vtable;
    u8 pad_0x8[0x84];
    f32 field_0x8c;
    f32 field_0x90;
    f32 field_0x94;
    f32 field_0x98;
    u8 pad_0x9c[0x84];
    UnkGuiDisplayObject* parent;  // 0x120
    u8 pad_0x128[0x8];
    CStrId name;  // 0x130
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[0x2];
    bool field_0x187;
    bool field_0x188;
    u8 pad_0x189[0x3f];
    u32 count;  // 0x1c8
    u8 pad_0x1cc[0x4];
    UnkListNode_ovb0059* head;  // 0x1d0
    u8 pad_0x1d8[0x58];
    f32 spacing_x;  // 0x230
    f32 spacing_y;  // 0x234
    s32 visible;  // 0x238
    s32 first;  // 0x23c
    s32 last;  // 0x240
    s32 selected;  // 0x244
    s32 previous;  // 0x248
    u8 pad_0x24c[0x4];
    CStrId selected_name;  // 0x250
    u64 field_0x258;
    bool field_0x260;
    bool field_0x261;
    bool field_0x262;
    bool field_0x263;
    bool field_0x264;

    void vf_0x18() { reinterpret_cast<void (*)(UnkList_ovb0059*)>(vtable[0x18 / 8])(this); }
    void vf_0x1e8(const char* event, bool flag) {
        reinterpret_cast<void (*)(UnkList_ovb0059*, const char*, bool)>(vtable[0x1e8 / 8])(this, event, flag);
    }
    UnkGuiDisplayObject* Object() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }

    // The entry at `index`, or null past the end.
    UnkListItem_ovb0059* ItemAt(u32 index) {
        u32 n = count;
        if (index < n) {
            UnkListNode_ovb0059* const* link = &head;
            for (;;) {
                UnkListNode_ovb0059* node = *link;
                if (index == 0)
                    return node ? node->item : nullptr;
                link = &node->next;
                --n;
                --index;
                if (n == 0)
                    break;
            }
        }
        return nullptr;
    }

    // Selects the entry whose id is `id`.
    void SelectById(CStrId id) {
        s32 i = 0;
        for (UnkListNode_ovb0059* n = head; n; n = n->next, ++i) {
            if (n->item->id.data == id.data) {
                previous = -1;
                selected = i;
                break;
            }
        }
    }

    // Plays `event` on the parent container, or on the list itself.
    void Notify(const char* event) {
        UnkGuiDisplayObject* p = parent;
        if (p && fn_710009d50c(p->GetClass(), fn_71011d7b84())) {
            reinterpret_cast<UnkList_ovb0059*>(p)->vf_0x1e8(event, false);
            return;
        }
        vf_0x1e8(event, false);
    }

    void Init() {
        vtable = &g_unk_7101cb6d08[2];
        spacing_x = 0.0f;
        spacing_y = 0.02f;
        selected = -1;
        previous = -1;
        new (&selected_name) CStrId(g_unk_7101d470e0);
        field_0x258 = 0;
        field_0x260 = false;
        field_0x261 = true;
        field_0x262 = false;
        field_0x263 = false;
        field_0x264 = false;
    }
};
static_assert(__builtin_offsetof(UnkList_ovb0059, count) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkList_ovb0059, selected_name) == 0x250, "layout");

static inline void Apply(UnkList_ovb0059* self) {
    fn_71011ff97c(reinterpret_cast<UnkStruct_71011ff97c*>(self), false);
}

void fn_7101200790(UnkList_ovb0059* self);
void fn_71012004ac(UnkList_ovb0059* self);
void fn_7101201b44(UnkList_ovb0059* self) {
    fn_71011d8d24(self);
    self->Init();
    self->vf_0x18();
}

void fn_71012015a0(UnkList_ovb0059* self, const CStrId* id) {
    self->SelectById(*id);
    Apply(self);
}

void fn_7101201a70(UnkList_ovb0059* self, UnkSmArg_A6_b62* arg) {
    fn_71011d9a44(reinterpret_cast<UnkSmBase_A6_b62*>(self), arg);
    UnkWeakHandle* handle = *reinterpret_cast<UnkWeakHandle**>(arg);
    void* target = handle ? *reinterpret_cast<void**>(reinterpret_cast<u8*>(handle) + 0x10) : nullptr;
    if (target == self->ItemAt(self->selected))
        fn_7101200790(self);
}

void fn_71012019a4(UnkList_ovb0059* self, UnkGuiEvent_b49* event) {
    fn_71011d99e4(self->Object(), event);
    if (event->kind != 2)
        return;
    if (!self->field_0x262) {
        self->selected = -1;
        self->previous = -2;
    } else {
        s32 sel = self->selected;
        s32 clamped = sel < 0 ? -1 : sel;
        s32 before = sel - 1;
        self->previous = sel >= -1 ? before : clamped;
        UnkListItem_ovb0059* item = self->ItemAt(sel);
        if (item && !item->Disabled()) {
            fn_7101200790(self);
            return;
        }
    }
    Apply(self);
}

void fn_71012018cc(UnkList_ovb0059* self) {
    fn_71011d9b3c(reinterpret_cast<UnkPanel_71011dfeb0*>(self));
    if (!self->field_0x262) {
        self->selected = -1;
        self->previous = -1;
    }
    bool overflow;
    if ((s32)self->count > self->visible) {
        for (UnkListNode_ovb0059* n = self->head; n; n = n->next)
            fn_71011cdc30(reinterpret_cast<UnkGuiDisplayObject*>(n->item), false);
        overflow = true;
    } else {
        overflow = false;
        self->visible = self->count;
    }
    if (!self->field_0x262) {
        self->first = 0;
        self->last = self->visible - 1;
    }
    if (overflow || self->field_0x260) {
        fn_71012004ac(self);
        if (self->field_0x260) {
            self->selected = 0;
            Apply(self);
        }
    }
}

void fn_71011ff64c(UnkList_ovb0059* self) {
    fn_71011cf630(reinterpret_cast<UnkGuiButtonFocus_ovb0000*>(self));
    if (!self->field_0x260)
        fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_7172a10df0);
    self->previous = -1;
    s32 kind = 0x2b;
    UnkGuiEvent event;
    fn_710120235c(&event, self->Object(), &kind, nullptr);
    s32 sel = self->selected;
    if (sel >= 0 && sel < (s32)self->count) {
        UnkListNode_ovb0059* n = self->head;
        for (u32 i = 0; i < (u32)sel; ++i)
            n = n->next;
        n->item->vf_0xd8(&event);
    }
    self->Notify("OnSelectionLost");
    fn_7101202488(&event);
}

void fn_71011ff79c(UnkList_ovb0059* self) {
    fn_71011d9ab4(reinterpret_cast<UnkGuiButtonFocus_ovb0000*>(self));
    if (!self->field_0x260)
        fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_7172a10df0);
    if (self->selected_name.data != &g_unk_7101d47108->empty) {
        self->SelectById(self->selected_name);
        self->selected_name = g_unk_7101d470e0;
    }
    if (self->selected < 0) {
        self->previous = self->selected;
        self->selected = 0;
        UnkListItem_ovb0059* first = self->count && self->head ? self->head->item : nullptr;
        if (first && !first->Disabled()) {
            fn_7101200790(self);
            self->Notify("OnSelected");
            return;
        }
    }
    Apply(self);
    self->Notify("OnSelected");
}

UnkList_ovb0059* fn_7101201668() {
    UnkGuiRoot* root = g_unk_717271e8f8;
    UnkList_ovb0059* list;
    {
        CStrId name("ListComposition", true);
        list = static_cast<UnkList_ovb0059*>(fn_71000839a0(0x268, 0xc));
        fn_71011d8d24(list);
        list->Init();
        if (list)
            list->vf_0x18();
        list->name = name;
        if (!list->field_0x187 &&
            (list->field_0x188 || list->field_0x184 ||
             (fn_71011cde14(list->Object())->field_0x3c &&
              (fn_71011ceac0(list->Object()) || fn_71011ceb34(list->Object())))))
            fn_71011b5598(root, list->Object());
    }
    fn_71011cdce8(list->Object(), true);
    {
        base::math::CVector2D origin = {0.0f, 0.0f};
        fn_71011cde64(list->Object(), &origin, 1, 1);
    }
    {
        base::math::CVector2D size = {1.0f, 0.85f};
        fn_71011cf8d0(reinterpret_cast<UnkSprite_spr*>(list), &size);
    }
    return list;
}
