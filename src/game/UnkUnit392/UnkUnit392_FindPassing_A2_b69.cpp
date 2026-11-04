// Unit 392 (query system): the first non-null entry of the owner's list that the filter check
// fn_71002bee84 passes (result 1) for the context at +0x10, or null. The list is re-read after each check.
#include "types.h"

struct UnkUnit392Filter_b69;

struct UnkUnit392PtrList_b69 {
    u8 unk_0x0[0x8];
    UnkUnit392Filter_b69** data;  // +0x8
    u32 count;                    // +0x10
};

struct UnkUnit392Query_b69 {
    u8 unk_0x0[0x8];
    UnkUnit392PtrList_b69* list;  // +0x8
    void* context;                // +0x10
};

int fn_71002bee84(UnkUnit392Filter_b69* filter, void* context, void* arg);

UnkUnit392Filter_b69* fn_71002c5818(UnkUnit392Query_b69* self, void* arg) {
    for (UnkUnit392Filter_b69** it = self->list->data; it != self->list->data + self->list->count; ++it) {
        UnkUnit392Filter_b69* e = *it;
        if (e && fn_71002bee84(e, self->context, arg) == 1)
            return e;
    }
    return nullptr;
}
