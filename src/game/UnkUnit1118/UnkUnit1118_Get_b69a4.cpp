// From unit 1118 of tools/tu_map.py (no reflected class): fn_71006fe07c picks an entry of a list. A
// non-negative index counts into the first child of the given list (when it is enabled and not empty); a
// negative one counts back from the end of the object's own list at +0x218 (when +0x210 is set), never
// reaching the first entry. Null when out of range. Names and layouts are placeholders.
#include "types.h"

struct UnkEntryList_b69a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();                  // enabled
    virtual s32 vf_0x28();                   // count
    virtual UnkEntryList_b69a4* vf_0x30(s32 index);  // entry
};

struct UnkUnit1118Object_b69a4 {
    u8 pad_0x0[0x210];
    bool field_0x210;
    u8 pad_0x211[7];
    UnkEntryList_b69a4 list;  // 0x218
};

UnkEntryList_b69a4* fn_71006fe07c(UnkUnit1118Object_b69a4* self, UnkEntryList_b69a4* from, s32 index) {
    if (index < 0) {
        if (!self->field_0x210)
            return nullptr;
        UnkEntryList_b69a4* list = &self->list;
        index = list->vf_0x28() + index;
        if (index < 1 || index >= list->vf_0x28())
            return nullptr;
        return list->vf_0x30(index);
    }
    if (!from || !from->vf_0x20() || from->vf_0x28() == 0)
        return nullptr;
    from->vf_0x28();
    UnkEntryList_b69a4* first = from->vf_0x30(0);
    if (first->vf_0x28() > index)
        return first->vf_0x30(index);
    return nullptr;
}
