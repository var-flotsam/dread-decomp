// CEmmySancAIComponent's unit (tools/tu_map.py unit 793): the component's destructor (vtable 0x7101a46180):
// two callback lists (base::UnkCallbackListShort) at +0x2a770 and +0x2a730, an array of 0x40-byte
// polymorphic elements at +0x2a700, then the base's destructor fn_710053f510. Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a46180[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];

// As src/game/CEmmyCaveAIComponent/fn_71004f0428.cpp declares it.
struct UnkComponent;
void fn_710053f510(UnkComponent* self);

struct UnkSancDtor_A3_b69;

template <>
struct base::UnkDtorTraits<UnkSancDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a46180); }
    static void base(char* self) { fn_710053f510(reinterpret_cast<UnkComponent*>(self)); }
};

template <>
struct base::UnkCallbackTraits<UnkSancDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

struct UnkSancElem_A3_b69 {
    virtual ~UnkSancElem_A3_b69();
    u8 field_0x8[0x38];
};

template <u32 Offset>
struct UnkSancList_A3_b69 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkCallbackListShort<UnkSancDtor_A3_b69>*>(self + Offset)->~UnkCallbackListShort();
    }
};

struct UnkSancElems_A3_b69 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkVector<UnkSancElem_A3_b69>*>(self + 0x2a700)->Free();
    }
};

__attribute__((flatten)) void fn_71004ff2cc(char* self) {
    base::refl_Dtor<UnkSancDtor_A3_b69, UnkSancList_A3_b69<0x2a770>, UnkSancList_A3_b69<0x2a730>,
                    UnkSancElems_A3_b69>(self);
}
