// CNailongAIComponent's unit (tools/tu_map.py unit 937): the component's destructor (vtable 0x7101a562b0):
// arrays of string ids at +0x14898 and +0x14848 and of id records at +0x14870, a refcounted pointer at +0x14830, a callback
// list (base::UnkCallbackListShort) at +0x147e8, the members at +0x14760 (fn_7100624974, a
// refl_LinkedDtor instance in src/clones/clones_02.cpp) and +0x146c8 (fn_710060b114), then the base's
// destructor fn_7100369380. Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a562b0[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];
void fn_7100369380(void*);  // the base's destructor
void fn_7100624974(char* self);
struct UnkAttack_710060b114;
void fn_710060b114(UnkAttack_710060b114* self);  // as src/game/CNailongThornsAttack/fn_710060b114.cpp defines it

struct UnkNailongDtor_A4_b70;

template <>
struct base::UnkDtorTraits<UnkNailongDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a562b0); }
    static void base(char* self) { fn_7100369380(self); }
};

template <>
struct base::UnkCallbackTraits<UnkNailongDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

// A string id and 0x10 bytes of plain data.
struct UnkNailongIdRecord_A4_b70 {
    base::UnkStrIdMember id;
    u8 field_0x8[0x10];
};

template <class T, u32 Offset>
struct UnkNailongIds_A4_b70 {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<T>*>(self + Offset)->Free(); }
};

struct UnkNailongList_A4_b70 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkCallbackListShort<UnkNailongDtor_A4_b70>*>(self + 0x147e8)->~UnkCallbackListShort();
    }
};

struct UnkNailongMembers_A4_b70 {
    static void Release(char* self) {
        fn_7100624974(self + 0x14760);
        fn_710060b114(reinterpret_cast<UnkAttack_710060b114*>(self + 0x146c8));
    }
};

__attribute__((flatten)) void fn_710061fb68(char* self) {
    base::refl_Dtor<UnkNailongDtor_A4_b70, UnkNailongIds_A4_b70<base::UnkStrIdMember, 0x14898>, UnkNailongIds_A4_b70<UnkNailongIdRecord_A4_b70, 0x14870>,
                    UnkNailongIds_A4_b70<base::UnkStrIdMember, 0x14848>, base::UnkRefPtrAt<0x14830>, UnkNailongList_A4_b70,
                    UnkNailongMembers_A4_b70>(self);
}
