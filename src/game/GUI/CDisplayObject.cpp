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
