// GUI::CScrollMenuEntryItemRenderer's unit (tools/tu_map.py 1716): fills the item's labels (the list at +0x248)
// for entry `index` of a GUI::CMenuEntryDataSet. Without the entry every label is blanked; otherwise the
// entry's flag (vf_0x48) is kept at +0x278 and, by the mode at +0x240, the first label gets the entry's name
// or the name is split at '@' (fn_7100b6f1a8) over the labels in turn, the labels left over blanked.
// Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkBlockFreeList.h"
#include "base/UnkNodeOwner.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "types.h"

using base::global::CRntString;

struct UnkClone_fn_7100affc6c;
using UnkMenuEntrySlot_ovb0112 = UnkCloneElem<UnkClone_fn_7100affc6c, 8>;
extern template UnkMenuEntrySlot_ovb0112& base::global::CRntVector<UnkMenuEntrySlot_ovb0112>::GrowAt(u32);

struct UnkWideTextObj;
void fn_7100066c00(UnkWideTextObj* self, const base::global::CRntString* src);

struct UnkStrListOwner_A6_b63;
void fn_7100b6f3c0(UnkStrListOwner_A6_b63* self);

extern void* g_unk_7101a9e928[];

// A list of strings over a block pool: nodes holding a string, linked at +0x30.
struct UnkScrollStrList_lb75894 {
    void** vtable;
    u32 count;
    u32 pad_0xc;
    UnkListNode<0x30>* head;  // 0x10
    UnkListNode<0x30>* tail;
    u32 field_0x20;
    u32 pad_0x24;
    base::UnkAllocator* allocator;  // 0x28
    u64 field_0x30;
    void* pool_vtable;              // 0x38
    base::UnkAllocator* pool_allocator;  // 0x40
    UnkBlockFreeList free_list;     // 0x48
    u32 field_0x70;
    u8 field_0x74;

    UnkScrollStrList_lb75894()
        : count(0), head(nullptr), tail(nullptr), field_0x20(0x10), allocator(fn_7100083990()), field_0x30(0) {
        vtable = g_unk_7101a9e928 + 2;
        pool_vtable = g_unk_7101a1a4c8 + 0x10;
        pool_allocator = nullptr;
        fn_710008377c(&free_list);
        field_0x70 = 0;
        field_0x74 = 0;
    }
    ~UnkScrollStrList_lb75894() { fn_7100b6f3c0(reinterpret_cast<UnkStrListOwner_A6_b63*>(this)); }
};

void fn_7100b6f1a8(const char* text, const char* separator, UnkScrollStrList_lb75894* out);

// A label text of 32-bit characters, built from UTF-32 text or from a string.
struct UnkScrollText_lb75894 {
    UnkGuiText text;

    explicit UnkScrollText_lb75894(const char32_t* str) { fn_7100066a50(this, str); }
    explicit UnkScrollText_lb75894(const CRntString* str) {
        fn_7100066c00(reinterpret_cast<UnkWideTextObj*>(this), str);
    }
    ~UnkScrollText_lb75894() { fn_7100066e44(this); }
};

struct UnkScrollMenuEntry_lb75894 {
    u8 pad_0x0[0x30];
    CRntString name;  // 0x30
};

struct UnkScrollMenuData_lb75894 {
    virtual const base::reflection::CType* vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual u32 Count();                  // vf_0x38
    virtual CRntString Label(u32 index);  // vf_0x40
    virtual bool vf_0x48(u32 index);
    u8 pad_0x8[0x8];
    base::global::CRntVector<UnkMenuEntrySlot_ovb0112> entries;  // 0x10
};

struct UnkScrollLabelNode_lb75894 {
    UnkGuiDisplayObject* label;
    UnkScrollLabelNode_lb75894* next;
};

struct UnkScrollMenuEntryItemRenderer_lb75894 {
    u8 pad_0x0[0x240];
    u32 mode;  // 0x240
    u8 pad_0x244[4];
    UnkScrollLabelNode_lb75894* labels;  // 0x248
    u8 pad_0x250[0x28];
    bool flag_0x278;
};

void fn_7100b75894(UnkScrollMenuEntryItemRenderer_lb75894* self, UnkScrollMenuData_lb75894* data, u32 index) {
    if (!data || data->Count() <= index) {
        for (UnkScrollLabelNode_lb75894* node = self->labels; node; node = node->next) {
            UnkGuiDisplayObject* label = node->label;
            UnkScrollText_lb75894 blank(U" ");
            fn_71011ec468(label, blank.text, false);
        }
        return;
    }
    UnkScrollMenuData_lb75894* set =
        fn_710009d50c(data->vf_0x0(), reinterpret_cast<const base::reflection::CType*>(fn_7101201d00())) ? data
                                                                                                        : nullptr;
    CRntString label = set->Label(index);
    UnkScrollMenuEntry_lb75894* entry =
        *reinterpret_cast<UnkScrollMenuEntry_lb75894**>(set->entries.GrowAt(index).data);
    CRntString name(entry->name);
    self->flag_0x278 = set->vf_0x48(index);
    if (self->mode == 0)
        return;
    if (self->mode == 1) {
        UnkGuiDisplayObject* first = self->labels->label;
        UnkScrollText_lb75894 text(&name);
        fn_71011ec468(first, text.text, false);
        return;
    }
    UnkScrollStrList_lb75894 parts;
    fn_7100b6f1a8(name.data, "@", &parts);
    UnkListNode<0x30>* part = parts.head;
    for (UnkScrollLabelNode_lb75894* node = self->labels; node; node = node->next) {
        UnkGuiDisplayObject* label = node->label;
        UnkScrollText_lb75894 text = part ? UnkScrollText_lb75894(reinterpret_cast<const CRntString*>(part))
                                          : UnkScrollText_lb75894(U" ");
        fn_71011ec468(label, text.text, false);
        if (part)
            part = part->next;
    }
}
