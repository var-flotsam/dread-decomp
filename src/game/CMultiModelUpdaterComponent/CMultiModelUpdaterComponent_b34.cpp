// CMultiModelUpdaterComponent's destructor body (vtable g_unk_7101a6f6b8): the id table at 0x80 is
// cleared (fn_71000db40c), both handles still held are released through the registry at 0xa0, then
// the base (fn_7100792448). The shape of fn_71009b99e8 (CChangeStageNavMeshItemComponent).
#include "base/global/CStrId.h"
#include "game/CCharClassModelUpdaterComponent.h"
#include "types.h"

using base::global::UnkStringPool;

template <UnkStringPool** Pool, bool Reread = Pool == &g_unk_7101d47108>
struct UnkReleasedStrIdT;
using UnkFilePathReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47200>;
template <class Value>
struct UnkIdTable;
void fn_71000db40c(UnkIdTable<UnkFilePathReleasedStrId>* self);

extern void* g_unk_7101a6f6b8[];

struct UnkModelRegistry {
    void** vtable;
    void Release(void* handle) {
        reinterpret_cast<void (*)(UnkModelRegistry*, void*)>(vtable[0x48 / 8])(this, handle);
    }
};

struct UnkModelTable {
    void* values;
};

struct CMultiModelUpdaterComponent;

struct CMultiModelUpdaterComponent {
    void** vtable;
    u8 unk_0x8[0x78];
    UnkModelTable table;  // 0x80
    void* handle;         // 0x88
    u8 unk_0x90[0x10];
    UnkModelRegistry* registry;  // 0xa0
};

void fn_71007937f0(CMultiModelUpdaterComponent* self) {
    self->vtable = g_unk_7101a6f6b8 + 2;
    UnkModelTable* table = &self->table;
    fn_71000db40c(reinterpret_cast<UnkIdTable<UnkFilePathReleasedStrId>*>(table));
    if (self->handle)
        self->registry->Release(self->handle);
    if (table->values)
        self->registry->Release(table->values);
    fn_7100792448(self);
}
