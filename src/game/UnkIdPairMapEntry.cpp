// From unit 2604 of tools/tu_map.py (0x7100f8af0c-0x7100f92a44, no reflected class): an entry of
// three string ids (a name, a file path and another name) with a hash map whose nodes hold pairs
// of ids. fn_7100f8db84 empties the map: it hands every node to the map's allocator, zeroes the
// buckets and the count. Names and layouts are placeholders.
// Holds fn_7100f8da4c (the entry's destructor).
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdPairMap;
// Empties the map (its nodes go back to the allocator at +0).
void fn_7100f8db84(UnkIdPairMap* self);

struct UnkIdPairMap {
    base::UnkAllocator* allocator;  // frees the nodes
    u32 field_0x8;
    u32 count;
    base::UnkVector<void*> buckets;
    void* field_0x38;
    void* nodes;  // a list linked at +0x28

    ~UnkIdPairMap() {
        fn_7100f8db84(this);
        buckets.Free();
    }
};
static_assert(__builtin_offsetof(UnkIdPairMap, nodes) == 0x40, "UnkIdPairMap placeholder layout");

struct UnkIdPairMapEntry {
    base::UnkStrIdMember name;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    base::UnkStrIdMember field_0x10;
    u8 pad_0x18[0x18];
    UnkIdPairMap map;
};
static_assert(__builtin_offsetof(UnkIdPairMapEntry, map) == 0x30, "UnkIdPairMapEntry placeholder layout");

// Empties the map, then destroys the members (inlined). Takes the entry untyped, as the map
// iterators of src/game_7100ece7fc/fn_7100f79844.cpp and src/game_s7/fn_7100f798fc.cpp declare it.
__attribute__((flatten)) void fn_7100f8da4c(void* value) {
    UnkIdPairMapEntry* self = static_cast<UnkIdPairMapEntry*>(value);
    fn_7100f8db84(&self->map);
    self->~UnkIdPairMapEntry();
}
