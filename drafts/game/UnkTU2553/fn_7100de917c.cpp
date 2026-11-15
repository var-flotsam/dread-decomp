// Unit 2553 (CGameManager::CTunableGameManager): gives the manager an empty list for each string id
// in the id registry (the function-local static of fn_7100de8f78). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;

struct UnkRefRegistry {
    u32 count;
    u8 data[256 * sizeof(CStrId)];  // capacity unknown
    UnkRefRegistry() : count(0) {}
    ~UnkRefRegistry();  // fn_7100de4928
    CStrId* begin() { return reinterpret_cast<CStrId*>(data); }
    CStrId* end() { return begin() + count; }
};

static UnkRefRegistry& GetRegistry_e917c() {
    static UnkRefRegistry s;
    return s;
}

template <class V>
struct UnkIdMap;
void fn_7100e06048(UnkIdMap<UnkOwnedList>* self, u32 capacity);

struct UnkTU2553Manager {
    u8 pad_0x0[0x2450];
    UnkOwnedList* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

void fn_7100de917c(UnkTU2553Manager* self) {
    UnkRefRegistry& registry = GetRegistry_e917c();
    for (CStrId *it = registry.begin(), *e = registry.end(); it != e; ++it) {
        if (self->count == self->capacity)
            fn_7100e06048(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(&self->values), self->step + self->count);
        CStrId* key = &self->keys[self->count];
        new (key) CStrId(*it);
        UnkOwnedList* list = &self->values[self->count];
        new (list) UnkOwnedList();
        ++self->count;
    }
}
