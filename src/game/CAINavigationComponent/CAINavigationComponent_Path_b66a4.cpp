// CAINavigationComponent's unit (tools/tu_map.py): the current record's path object, when it is
// ready (its vf_0x20). The record's kind (+0x10) picks the source as in fn_71006f4f28
// (CAINavigationComponent_Target_b65a4.cpp): kind 1 and kind 7 keep it inside their object
// (+0x12bf8, +0x12c08), kind 2 answers it with fn_71006fce10. Names and layouts are placeholders.
#include "types.h"

struct UnkNavPath_b66a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
};

struct UnkNavRecord_b66a4 {
    u8 pad_0x0[0x10];
    volatile s32 kind;  // 0x10 (the original re-reads it for each test)
};

struct UnkNavHead_b66a4 {
    u8 pad_0x0[0x10];
    UnkNavRecord_b66a4* record;  // 0x10
};

struct UnkNavKind2_b66a4;
UnkNavPath_b66a4* fn_71006fce10(UnkNavKind2_b66a4* obj);

struct UnkNavTable_b66a4 {
    UnkNavHead_b66a4* head;    // 0x0
    char* kind1;               // 0x8
    UnkNavKind2_b66a4* kind2;  // 0x10
    u8 pad_0x18[0x20];
    char* kind7;  // 0x38
};

struct UnkNavComponent_b66a4 {
    u8 pad_0x0[0xb0];
    UnkNavTable_b66a4* table;  // 0xb0
};

UnkNavPath_b66a4* fn_71006f4e78(UnkNavComponent_b66a4* self) {
    UnkNavTable_b66a4* table = self->table;
    UnkNavHead_b66a4* head = table->head;
    UnkNavPath_b66a4* path;
    if (!head || !head->record)
        goto none;
    {
        UnkNavRecord_b66a4* record = head->record;
        if (record->kind == 1 && table->kind1)
            path = reinterpret_cast<UnkNavPath_b66a4*>(table->kind1 + 0x12bf8);
        else if (record->kind == 7 && table->kind7)
            path = reinterpret_cast<UnkNavPath_b66a4*>(table->kind7 + 0x12c08);
        else if (record->kind == 2 && table->kind2) {
            path = fn_71006fce10(table->kind2);
            if (!path)
                goto none;
        } else
            goto none;
    }
    if (path->vf_0x20())
        goto done;
none:
    path = nullptr;
done:
    return path;
}
