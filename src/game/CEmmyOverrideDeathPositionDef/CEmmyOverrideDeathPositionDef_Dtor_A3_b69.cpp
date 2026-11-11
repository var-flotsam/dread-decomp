// CEmmyOverrideDeathPositionDef's unit (tools/tu_map.py unit 810): the destructor of a class without a
// base destructor call (vtable 0x7101a48248) holding two callback lists (base::UnkCallbackListShort, with
// the vtables src/clones/clones_00.cpp gives fn_710023ac4c's copy) at +8 and +0x48, destroyed in reverse
// order. Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU341.h"
#include "types.h"

extern void* g_unk_7101a48248[];
extern void* g_unk_7101a48270[];
extern void* g_unk_7101a48298[];

struct UnkDeathPositionLists_A3_b69;

template <>
struct base::UnkCallbackTraits<UnkDeathPositionLists_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

struct UnkDeathPositionDef_A3_b69 {
    char* vtable;
    base::UnkCallbackListShort<UnkDeathPositionLists_A3_b69> first;
    base::UnkCallbackListShort<UnkDeathPositionLists_A3_b69> second;
    ~UnkDeathPositionDef_A3_b69() { vtable = reinterpret_cast<char*>(g_unk_7101a48248) + 0x10; }
};
static_assert(sizeof(UnkDeathPositionDef_A3_b69) == 0x88, "UnkDeathPositionDef_A3_b69 layout");

__attribute__((flatten)) void fn_7100531ffc(UnkDeathPositionDef_A3_b69* self) {
    self->~UnkDeathPositionDef_A3_b69();
}


// The destructor of the class with vtable 0x7101a48298 (fn_7100535078; its copy constructor is
// fn_7100563c14): an array of 0x100-byte objects at +0xb8, a list at +0x70, an array of 0x40-byte
// polymorphic elements at +0x48 and a list at +8.
struct UnkDefBElem_A3_b69 {
    virtual ~UnkDefBElem_A3_b69();
    u8 field_0x8[0x38];
};
static_assert(sizeof(UnkDefBElem_A3_b69) == 0x40, "UnkDefBElem_A3_b69 layout");

struct UnkDefBDtor_A3_b69 {
    char* vtable;
    base::UnkCallbackListShort<UnkDeathPositionLists_A3_b69> first;
    base::UnkVector<UnkDefBElem_A3_b69> elems;
    base::UnkCallbackListShort<UnkDeathPositionLists_A3_b69> second;
    bool flag;
    base::UnkVector<UnkObject_0x100> objects;
    void Destroy() {
        vtable = reinterpret_cast<char*>(g_unk_7101a48298) + 0x10;
        objects.Free();
        second.~UnkCallbackListShort();
        elems.Free();
        first.~UnkCallbackListShort();
    }
};
static_assert(__builtin_offsetof(UnkDefBDtor_A3_b69, objects) == 0xb8, "UnkDefBDtor_A3_b69 layout");

__attribute__((flatten)) void fn_7100535078(UnkDefBDtor_A3_b69* self) {
    self->Destroy();
}

// The destructor of the class with vtable 0x7101a48270 (fn_71005337e4): an array of string ids at +0xb0,
// an array of 0x40-byte polymorphic elements at +0x88 and the lists at +0x48 and +8.
struct UnkDefCDtor_A3_b69 {
    char* vtable;
    base::UnkCallbackListShort<UnkDeathPositionLists_A3_b69> first;
    base::UnkCallbackListShort<UnkDeathPositionLists_A3_b69> second;
    base::UnkVector<UnkDefBElem_A3_b69> elems;
    base::UnkVector<base::UnkStrIdMember> ids;
    void Destroy() {
        vtable = reinterpret_cast<char*>(g_unk_7101a48270) + 0x10;
        ids.Free();
        elems.Free();
        second.~UnkCallbackListShort();
        first.~UnkCallbackListShort();
    }
};
static_assert(__builtin_offsetof(UnkDefCDtor_A3_b69, ids) == 0xb0, "UnkDefCDtor_A3_b69 layout");

__attribute__((flatten)) void fn_71005337e4(UnkDefCDtor_A3_b69* self) {
    self->Destroy();
}
