// GUI::CDisplayObject's unit (the class name from reflection; layout in game/UnkGuiDisplayObject.h):
// applying a list of 0x18-byte entries to an object one by one, each through an iterator over the
// list, and telling the object once if any of them changed it. Names are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkGuiListEntry {
    u8 data[0x18];
};

struct UnkGuiEntryList {
    UnkGuiListEntry* data;
    u32 count;
};

struct UnkGuiEntryIter {
    const UnkGuiEntryList* list;
    UnkGuiListEntry* entry;
};

// Applies the entry the iterator is at; true when that changed the object.
bool fn_71011cd5dc(UnkGuiDisplayObject* self, const UnkGuiEntryIter* it);

void fn_71011cd424(UnkGuiDisplayObject* self, const UnkGuiEntryList* list) {
    UnkGuiEntryIter it{list, list->data};
    bool changed = false;
    for (UnkGuiListEntry* entry = it.entry; entry != list->data + list->count;) {
        changed |= fn_71011cd5dc(self, &it);
        it.entry = ++entry;
    }
    if (changed)
        self->vf_0x80();
}

// Sets the flag at +0x185; a change runs vf_0xa0 the first time, marks the derived flags stale and
// sets change bit 4.
void fn_71011d0088(UnkGuiDisplayObject* self, bool flag) {
    if (self->field_0x185 != flag) {
        self->field_0x185 = flag;
        if (!self->field_0x184) {
            self->field_0x184 = true;
            self->vf_0xa0();
        }
        self->field_0x183 = true;
        self->field_0x144 |= 4;
    }
}

// The conditions an object checks (+0x100: how many, from +0x104, an unaligned array of pointers),
// each through its vf_0x20; and its state object at +0x30. Only these fields are laid out here.
struct UnkGuiCondition {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
};
struct UnkGuiObjectSub_0x30 {
    u8 pad_0x0[0x11];
    bool field_0x11;
};
struct UnkGuiConditionView {
    u8 pad_0x0[0x30];
    UnkGuiObjectSub_0x30* field_0x30;
    u8 pad_0x38[0xc8];
    u32 condition_count;  // 0x100
    u8 conditions[1];     // 0x104

    UnkGuiCondition** begin() { return reinterpret_cast<UnkGuiCondition**>(conditions); }
    UnkGuiCondition** end() { return begin() + condition_count; }
};

// Whether any of the object's conditions fails.
bool fn_71011ceac0(UnkGuiDisplayObject* self) {
    UnkGuiConditionView* view = reinterpret_cast<UnkGuiConditionView*>(self);
    for (UnkGuiCondition** it = view->begin(); it != view->end(); ++it)
        if (!(*it)->vf_0x20())
            return true;
    return false;
}

// Whether the object has a state object at +0x30 whose flag at +0x11 is clear.
bool fn_71011ceb34(UnkGuiDisplayObject* self) {
    UnkGuiConditionView* view = reinterpret_cast<UnkGuiConditionView*>(self);
    if (view->field_0x30)
        return !view->field_0x30->field_0x11;
    return false;
}
