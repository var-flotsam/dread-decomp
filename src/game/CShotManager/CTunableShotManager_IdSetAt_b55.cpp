// Moving "set element at index" of reflected fixed arrays of string ids: the element is released
// as a destructor would, then takes the value's id, which is left empty. CShotManager::
// CTunableShotManager's unit (tools/tu_map.py unit 2576) has one over file-path ids (pool
// g_unk_7101d47200), unit 2612 one over plain ids (g_unk_7101d47108). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "types.h"

template <base::global::UnkStringPool*& Pool>
struct UnkIdElem_b55 {
    base::UnkStrIdMemberOf<Pool> id;
};

template <base::global::UnkStringPool*& Pool>
static inline void IdSetAt_b55(UnkIdElem_b55<Pool>* data, const u32* index, UnkIdElem_b55<Pool>* value) {
    using Id = base::UnkStrIdMemberOf<Pool>;
    Id& e = data[*index].id;
    e.~Id();
    new (&e) Id(static_cast<Id&&>(value->id));
}

void fn_7100edb3dc(UnkIdElem_b55<g_unk_7101d47200>* data, const u32* index, UnkIdElem_b55<g_unk_7101d47200>* value) {
    IdSetAt_b55(data, index, value);
}
void fn_7100fb1288(UnkIdElem_b55<g_unk_7101d47108>* data, const u32* index, UnkIdElem_b55<g_unk_7101d47108>* value) {
    IdSetAt_b55(data, index, value);
}
