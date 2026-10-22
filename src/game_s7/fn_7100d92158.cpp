// Registers a set of descriptors with the registry, three of them with a factory object
// (created on first use). Names and layouts are placeholders.
#include "base/global/CStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

void* fn_71000839a0(u64 size, u32 flags);  // allocation

// An id released in two steps (the original reloads the id and its count after the decrement).
template <UnkStringPool** Pool>
struct UnkEntryId_7100d92158 {
    UnkStringData* data;

    ~UnkEntryId_7100d92158() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkDesc_7100d92158;

// A registry entry (built out of line by fn_710002003c).
struct UnkEntry_7100d92158 {
    UnkDesc_7100d92158* desc;
    void* factory;
    UnkEntryId_7100d92158<&g_unk_7101d47108> id;
    UnkEntryId_7100d92158<&g_unk_7101d47200> path_0x18;
    UnkEntryId_7100d92158<&g_unk_7101d47200> path_0x20;
    u64 field_0x28;
    void* field_0x30;
    u64 field_0x38;
    void* field_0x40;

    UnkEntry_7100d92158(UnkDesc_7100d92158* desc, void* factory);  // fn_710002003c
};

struct UnkRegistry_7100d92158;
extern UnkRegistry_7100d92158* g_unk_7101d4bbc0;
void fn_710002002c(UnkRegistry_7100d92158* registry, const UnkEntry_7100d92158& entry);

// A factory: a vtable and a 0x50-byte member, zeroed and then set up by its own function.
struct UnkFactory_7100d92158 {
    void* vtable;
    u64 field_0x8[10];
};
using UnkFactoryInit_7100d92158 = void (*)(u64* self, void* unk, u32 n);

static inline UnkFactory_7100d92158* Factory(UnkFactory_7100d92158*& instance, void** vtable,
                                             UnkFactoryInit_7100d92158 init) {
    if (!instance) {
        auto* f = static_cast<UnkFactory_7100d92158*>(fn_71000839a0(0x58, 0x3f));
        for (u64& w : f->field_0x8)
            w = 0;
        f->vtable = vtable + 2;
        init(f->field_0x8, nullptr, 0xe);
        instance = f;
    }
    return instance;
}

extern UnkFactory_7100d92158* g_unk_71727ba2b0;
extern UnkFactory_7100d92158* g_unk_71727ba2b8;
extern UnkFactory_7100d92158* g_unk_71727ba2c0;
extern void* g_unk_7101aa9020[];
extern void* g_unk_7101aa9080[];
extern void* g_unk_7101aa90e0[];
void fn_7100d93e5c(u64* self, void* unk, u32 n);
void fn_7100d94bb0(u64* self, void* unk, u32 n);
void fn_7100d95904(u64* self, void* unk, u32 n);
void fn_7100d96658();

extern UnkDesc_7100d92158 g_unk_71727b9630;
extern UnkDesc_7100d92158 g_unk_71727b95b0;
extern UnkDesc_7100d92158 g_unk_71727b9030;
extern UnkDesc_7100d92158 g_unk_71727b9130;
extern UnkDesc_7100d92158 g_unk_71727b91b0;
extern UnkDesc_7100d92158 g_unk_71727b9230;
extern UnkDesc_7100d92158 g_unk_71727b92b0;
extern UnkDesc_7100d92158 g_unk_71727b9330;
extern UnkDesc_7100d92158 g_unk_71727b93b0;
extern UnkDesc_7100d92158 g_unk_71727b9430;
extern UnkDesc_7100d92158 g_unk_71727b94b0;
extern UnkDesc_7100d92158 g_unk_71727b90b0;
extern UnkDesc_7100d92158 g_unk_71727b96b0;
extern UnkDesc_7100d92158 g_unk_71727b9730;
extern UnkDesc_7100d92158 g_unk_71727b97b0;
extern UnkDesc_7100d92158 g_unk_71727b9830;
extern UnkDesc_7100d92158 g_unk_71727b98b0;
extern UnkDesc_7100d92158 g_unk_71727b9930;
extern UnkDesc_7100d92158 g_unk_71727b99b0;
extern UnkDesc_7100d92158 g_unk_71727b9bb0;
extern UnkDesc_7100d92158 g_unk_71727b9c30;
extern UnkDesc_7100d92158 g_unk_71727b9b30;
extern UnkDesc_7100d92158 g_unk_71727b9a30;
extern UnkDesc_7100d92158 g_unk_71727b9ab0;
extern UnkDesc_7100d92158 g_unk_71727b9fb0;
extern UnkDesc_7100d92158 g_unk_71727ba030;
extern UnkDesc_7100d92158 g_unk_71727ba0b0;
extern UnkDesc_7100d92158 g_unk_71727ba130;
extern UnkDesc_7100d92158 g_unk_71727ba1b0;

void fn_7100d92158() {
    UnkFactory_7100d92158* f1 = Factory(g_unk_71727ba2b0, g_unk_7101aa9020, fn_7100d93e5c);
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9630, f1));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b95b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9030, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9130, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b91b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9230, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b92b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9330, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b93b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9430, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b94b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b90b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b96b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9730, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b97b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9830, nullptr));
    UnkFactory_7100d92158* f2 = Factory(g_unk_71727ba2b8, g_unk_7101aa9080, fn_7100d94bb0);
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b98b0, f2));
    UnkFactory_7100d92158* f3 = Factory(g_unk_71727ba2c0, g_unk_7101aa90e0, fn_7100d95904);
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9930, f3));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b99b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9bb0, nullptr));
    UnkEntry_7100d92158 entry(&g_unk_71727b9c30, nullptr);
    entry.field_0x38 = 0;
    entry.field_0x40 = reinterpret_cast<void*>(fn_7100d96658);
    fn_710002002c(g_unk_7101d4bbc0, entry);
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9b30, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9a30, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9ab0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727b9fb0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727ba030, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727ba0b0, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727ba130, nullptr));
    fn_710002002c(g_unk_7101d4bbc0, UnkEntry_7100d92158(&g_unk_71727ba1b0, nullptr));
}
