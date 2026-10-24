// Constructors of a movie extractor subclass. fn_710022323c, from a pair of ids: builds the base
// movie::Extractor (fn_7101239ca0) with the type fn_7100225570 gives for the kind fn_7100220880
// reads from the second id's name, sets the vtable, formats "<first>:/<second>" and passes it to
// vtable slot 0x10. Each id read takes a reference that this function doesn't release.
// fn_7100223318, from a source object: vtable slots 0x28 and 0x30 of the source give the two
// values passed to slot 0x18.
#include <cstdio>

#include "types.h"

// The count's stores may alias the id pointers (the original reloads them after each one).
typedef u32 __attribute__((may_alias)) UnkAliasedCount;

struct UnkIdEntry_710022323c {
    u8 unk_0x0[0x10];
    const char* str;
    u8 unk_0x18[0x2c];
    u32 ref_count;

    void AddRef() { ++*reinterpret_cast<UnkAliasedCount*>(&ref_count); }

    const char* Str() const { return str ? str : ""; }
};

struct UnkIdPair_710022323c {
    UnkIdEntry_710022323c* first;
    UnkIdEntry_710022323c* second;
};

struct UnkExtractor_710022323c {
    void** vtable;

    u64 vf_0x18(void* a, void* b) {
        return reinterpret_cast<u64 (*)(UnkExtractor_710022323c*, void*, void*)>(vtable[3])(this, a, b);
    }
    u64 vf_0x10(const char* path) {
        return reinterpret_cast<u64 (*)(UnkExtractor_710022323c*, const char*)>(vtable[2])(this, path);
    }
};

u32 fn_7100220880(const char* name);
u32 fn_7100225570(u32 type);
void fn_7101239ca0(UnkExtractor_710022323c* self, u32 type);  // movie::Extractor::Extractor
extern void* g_unk_7101a205a0[];

u64 fn_710022323c(UnkExtractor_710022323c* self, UnkIdPair_710022323c* ids) {
    ids->second->AddRef();
    fn_7101239ca0(self, fn_7100225570(fn_7100220880(ids->second->Str())));
    self->vtable = g_unk_7101a205a0 + 2;
    ids->first->AddRef();
    UnkIdPair_710022323c copy = *ids;
    const char* first = copy.first->Str();
    copy.second->AddRef();
    char path[0x100];
    sprintf(path, "%s:/%s", first, ids->second->Str());
    return self->vf_0x10(path);
}

struct UnkSource_7100223318 {
    void** vtable;

    void* vf_0x28() { return reinterpret_cast<void* (*)(UnkSource_7100223318*)>(vtable[5])(this); }
    void* vf_0x30() { return reinterpret_cast<void* (*)(UnkSource_7100223318*)>(vtable[6])(this); }
};

u64 fn_7100223318(UnkExtractor_710022323c* self, UnkSource_7100223318* source, u32 type) {
    fn_7101239ca0(self, fn_7100225570(type));
    self->vtable = g_unk_7101a205a0 + 2;
    void* a = source->vf_0x28();
    void* b = source->vf_0x30();
    return self->vf_0x18(a, b);
}
