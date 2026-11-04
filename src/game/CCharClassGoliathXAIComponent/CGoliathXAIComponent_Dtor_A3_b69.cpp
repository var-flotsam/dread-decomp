// CGoliathXAIComponent's unit (tools/tu_map.py unit 838): a destructor (vtable 0x7101a4b428) releasing a
// callback list (base::UnkCallbackListShort) at +0x14c80, the attack at +0x14bf0 (fn_7100587280), the
// object at +0x14b60 (fn_7100586078), then the base's destructor fn_7100590b6c. Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "types.h"

extern void* g_unk_7101a4b428[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];

// As src/game/CGoliathXSlamAttack/fn_7100587280.cpp defines it.
struct UnkAttack_7100587280;
void fn_7100587280(UnkAttack_7100587280* self);
void fn_7100586078(char* self);
char* fn_7100590b6c(char* self);  // the base's destructor

struct UnkGoliathXDtor_A3_b69;

template <>
struct base::UnkDtorTraits<UnkGoliathXDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a4b428); }
    static void base(char* self) { fn_7100590b6c(self); }
};

template <>
struct base::UnkCallbackTraits<UnkGoliathXDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

struct UnkGoliathXList_A3_b69 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkCallbackListShort<UnkGoliathXDtor_A3_b69>*>(self + 0x14c80)
            ->~UnkCallbackListShort();
    }
};

struct UnkGoliathXAttack_A3_b69 {
    static void Release(char* self) { fn_7100587280(reinterpret_cast<UnkAttack_7100587280*>(self + 0x14bf0)); }
};

struct UnkGoliathXObject_A3_b69 {
    static void Release(char* self) { fn_7100586078(self + 0x14b60); }
};

__attribute__((flatten)) void fn_7100588210(char* self) {
    base::refl_Dtor<UnkGoliathXDtor_A3_b69, UnkGoliathXList_A3_b69, UnkGoliathXAttack_A3_b69,
                    UnkGoliathXObject_A3_b69>(self);
}
