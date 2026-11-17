// CDemolitionBlockLifeComponent unit (tools/tu_map.py unit 1374): the component's reset: the life
// component's (fn_710115fc5c), the registry table at +0x258 named after the owner's id, and the three
// id and path pairs at +0x298, +0x2b0 and +0x2c8 built from the unit's path texts. Names and layouts are
// placeholders.
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CLifeComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflRegistryMap;

struct UnkRefl_fn_71008c5414;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_71008c5414>(UnkReflRegistryMap*,
                                                                                      const CStrId&);

extern base::global::CRntString g_unk_7172730d70 __attribute__((visibility("hidden")));
extern base::global::CRntString g_unk_7172730da0 __attribute__((visibility("hidden")));
extern base::global::CRntString g_unk_7172730dd0 __attribute__((visibility("hidden")));

struct UnkDemolitionOwner_l8c5184 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkDemolitionBlockLifeInit {
    u8 pad_0x0[0x18];
    UnkDemolitionOwner_l8c5184* owner;  // 0x18
    u8 pad_0x20[0x258 - 0x20];
    UnkReflRegistryMap table;  // 0x258
    u8 pad_after_table[0x298 - 0x258 - sizeof(UnkReflRegistryMap)];
    UnkStrIdPathResource path_a;  // 0x298
    UnkStrIdPathResource path_b;  // 0x2b0
    UnkStrIdPathResource path_c;  // 0x2c8
};

// Names the registry table (the id is taken by value).
static inline void InitTable(UnkReflRegistryMap* table, CStrId name) {
    base::reflection::refl_RegistryMapInit<UnkRefl_fn_71008c5414>(table, name);
}

void fn_71008c5184(UnkDemolitionBlockLifeInit* self) {
    fn_710115fc5c(reinterpret_cast<char*>(self));
    InitTable(&self->table, self->owner->name);
    fn_71000267f8(&self->path_a, UnkPathIdsFromText(g_unk_7172730d70).ids);
    fn_71000267f8(&self->path_b, UnkPathIdsFromText(g_unk_7172730da0).ids);
    fn_71000267f8(&self->path_c, UnkPathIdsFromText(g_unk_7172730dd0).ids);
}
