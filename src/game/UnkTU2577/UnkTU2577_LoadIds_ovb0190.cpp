// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee96f0: loads a list of ids: each is passed to the GUI cache (fn_710119e4ec) and added to the loaded list at +0x338, then every spawn group component of the entities (fn_7101119514) is told about each id (fn_710111b040). Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkSubAreaGuiCache.h"
#include "types.h"

using base::global::CStrId;

struct CSubAreaManager;

struct UnkIdSource_b190 {
    u8 pad_0x0[0x28];
    base::UnkStrIdVec ids;  // 0x28
};

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x338];
    base::UnkStrIdVec loaded;  // 0x338
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, loaded) == 0x338, "layout");

struct UnkSubAreaEntity {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};
struct UnkSubAreaEntityList {
    UnkSubAreaEntity** data;
    u32 count;
};

extern UnkSubAreaGuiCache* g_unk_71727254f8;
void fn_71000839c0(bool current_thread);
void fn_7100083a10();
void fn_710119e4ec(UnkSubAreaGuiCache* self, const CStrId* id, bool flag);
void fn_710109d0a0();
UnkSubAreaEntityList* fn_7101119514();
void fn_710111b040(base::reflection::UnkReflObject* component, const CStrId* id);

void fn_7100ee96f0(UnkSubAreaMgr_b190* self, UnkIdSource_b190* source) {
    fn_71000839c0(true);
    if (source->ids.count) {
        for (const CStrId *it = source->ids.data, *end = it + source->ids.count; it != end; ++it) {
            UnkSubAreaGuiCache* cache = g_unk_71727254f8;
            if (!cache) {
                cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
                fn_710119d484(cache);
                g_unk_71727254f8 = cache;
            }
            fn_710119e4ec(cache, it, true);
            self->loaded.PushBack(*it);
        }
        fn_710109d0a0();
    }
    fn_7100083a10();
    UnkSubAreaEntityList* entities = fn_7101119514();
    for (u32 i = 0; i < entities->count; ++i) {
        UnkSubAreaEntity* e = entities->data[i];
        base::reflection::UnkReflObject* component = e ? e->field_0x10 : nullptr;
        if (component && !fn_710009d50c(component->vf_0x0(), fn_7101117478()))
            component = nullptr;
        for (const CStrId *it = source->ids.data, *end = it + source->ids.count; it != end; ++it)
            fn_710111b040(component, it);
    }
}
