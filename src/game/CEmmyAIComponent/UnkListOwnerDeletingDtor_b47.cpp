// Deleting destructors of two classes whose secondary base at +8 is a list (refl_ListDtor's layout,
// nodes linked at +0x20, freed through the allocator at +0x28 of the object): the class's vtable, the
// list base's vtable and node frees, then operator delete (the list's count and head stores are
// dropped). fn_710055fc44 is in the CEmmyAIComponent unit (tools/tu_map.py unit 811), fn_710051c5dc in
// the behaviortree::CEmmySearchingAction unit (unit 805). Names are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkTU811.h"
#include "game/behaviortree/CEmmySearchingAction.h"

extern char g_unk_7101a48b80[];

template <auto* Vtable>
struct UnkListBaseTag_b47;

template <auto* Vtable>
struct base::UnkDtorTraits<UnkListBaseTag_b47<Vtable>> {
    static char* vtable() { return reinterpret_cast<char*>(Vtable); }
};

template <auto* Vtable, auto* ListVtable>
static inline void ListOwnerDeletingDtor_b47(char* self) {
    *reinterpret_cast<char**>(self) = reinterpret_cast<char*>(Vtable) + 0x10;
    base::refl_ListDtor<UnkListBaseTag_b47<ListVtable>, 0x20, 0x20>(self + 8);
    __builtin_operator_delete(self);
}

void fn_710055fc44(char* self) { ListOwnerDeletingDtor_b47<g_unk_7101a48b80, g_unk_7101a48bc0>(self); }
void fn_710051c5dc(char* self) { ListOwnerDeletingDtor_b47<g_unk_7101a47f48, g_unk_7101a47f88>(self); }
