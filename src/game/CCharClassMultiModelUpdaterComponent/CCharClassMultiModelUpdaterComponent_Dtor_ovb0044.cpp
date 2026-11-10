// CCharClassMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1195): the deleting destructor of
// the multi-model updater (vtable g_unk_7101a6f6b8): the body of fn_71007937f0
// (src/game/CMultiModelUpdaterComponent/CMultiModelUpdaterComponent_b34.cpp), then operator delete
// (fn_7100083c00). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringPool;

template <UnkStringPool** Pool, bool Reread = Pool == &g_unk_7101d47108>
struct UnkReleasedStrIdT;
using UnkFilePathReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47200>;
template <class Value>
struct UnkIdTable;
void fn_71000db40c(UnkIdTable<UnkFilePathReleasedStrId>* self);

extern void* g_unk_7101a6f6b8[];

struct UnkModelRegistry_ovb0044 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void Release(void* handle);  // 0x48
};

struct UnkMultiModelUpdater_ovb0044 {
    void** vtable;
    u8 unk_0x8[0x78];
    void* table;   // 0x80, its values first
    void* handle;  // 0x88
    u8 unk_0x90[0x10];
    UnkModelRegistry_ovb0044* registry;  // 0xa0
};

struct CMultiModelUpdaterComponent;
void fn_7100792448(CMultiModelUpdaterComponent* self);

void fn_710079385c(UnkMultiModelUpdater_ovb0044* self) {
    self->vtable = g_unk_7101a6f6b8 + 2;
    void** table = &self->table;
    fn_71000db40c(reinterpret_cast<UnkIdTable<UnkFilePathReleasedStrId>*>(table));
    if (self->handle)
        self->registry->Release(self->handle);
    if (*table)
        self->registry->Release(*table);
    fn_7100792448(reinterpret_cast<CMultiModelUpdaterComponent*>(self));
    fn_7100083c00(self);
}
