// CTriggerComponent unit (tools/tu_map.py). fn_7100a6317c: the constructor of a 0x60-byte record: an empty id, the
// limit INT_MAX, an empty name (fn_71000003d4), two flags, two -1 defaults, an empty array (step 10,
// flags 0xf, the default allocator) and a cleared flag. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkTriggerRecord {
    CStrId field_0x0;
    s32 field_0x8;
    u8 pad_0xc[4];
    CStrId field_0x10;
    bool field_0x18;
    bool field_0x19;
    u8 pad_0x1a[2];
    f32 field_0x1c;
    f32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u8 pad_0x2c[4];
    u64 field_0x30;
    u64 field_0x38;
    u32 field_0x40;
    bool field_0x44;
    u32 field_0x48;
    base::UnkAllocator* field_0x50;
    bool field_0x58;

    __attribute__((always_inline)) UnkTriggerRecord()
        : field_0x0(g_unk_7101d470e0), field_0x8(0x7fffffff), field_0x10("", true) {
        field_0x18 = true;
        field_0x19 = false;
        field_0x1c = -1.0f;
        field_0x20 = -1.0f;
        field_0x24 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        field_0x38 = 0;
        field_0x40 = 10;
        field_0x44 = true;
        field_0x48 = 0xf;
        field_0x50 = fn_7100083990();
        field_0x58 = false;
    }
};

void fn_7100a6317c(UnkTriggerRecord* self) {
    new (self) UnkTriggerRecord();
}

// A reflected function of the unit: hands the owner (+0x30 of the component at 8 of the reflected object)
// to every element of the argument object's list at +0x30 (fn_71007a804c gives the i-th slot), through
// the element's virtual at 0x18; it returns an empty reflected value.
struct UnkTriggerListElement {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(void* owner);
};

struct UnkTriggerList {
    u8 pad_0x0[0x30];
    u8 list[8];  // 0x30
    u32 count;   // 0x38
};

UnkTriggerListElement** fn_71007a804c(void* list, u32 index);

UnkReflValue fn_7100a6491c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkTriggerList* const* in;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        in = reinterpret_cast<UnkTriggerList* const*>(arg->field_0x0);
    else
        in = reinterpret_cast<UnkTriggerList* const*>(arg);
    UnkTriggerList* list = *in;
    for (u32 i = 0; i < list->count; ++i)
        (*fn_71007a804c(list->list, i))->vf_0x18(*reinterpret_cast<void**>(target + 0x30));
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
