// Unit 232 (data/tu_map.csv, no reliable name): the link of a resource block, the inverse of its
// release (UnkTU232_Reloc_ovb0252.cpp): the header's offsets turn into pointers, then the texture and
// the second resource its two paths name are loaded and kept. Names and layouts are placeholders.
#include "base/UnkAssetResRef.h"
#include "base/UnkCountedResource.h"
#include "base/UnkResKey.h"
#include "base/UnkStrIdPathPair.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU239.h"
#include "types.h"

using base::global::CFilePathStrId;

// The block's header: its strings are stored as offsets from the header until it is linked.
struct UnkLinkHeader_l1aaf80 {
    u8 pad_0x0[0x8];
    const char* texture_path;  // 0x8
    u8 pad_0x10[0x18];
    const char* field_0x28;
    const char* field_0x30;  // the second resource's path
};

// The second resource, as far as it is read here.
struct UnkLinkResource_l1aaf80 {
    u8 pad_0x0[0x70];
    void* field_0x70;
};

struct UnkLinkOwner_l1aaf80 {
    bool linked;  // 0x0
    u8 pad_0x1[0x7];
    void* loader;                    // 0x8
    UnkLinkHeader_l1aaf80* header;   // 0x10
    UnkAssetResRef texture;   // 0x18
    UnkAssetResRef second;    // 0x20
    void* field_0x28;
    void* field_0x30;
};

// Turns an offset from the header (none when zero) into a pointer.
static inline void Link(const char*& str, u64 base) {
    if (str)
        str = reinterpret_cast<const char*>(reinterpret_cast<u64>(str) + base);
}

static inline UnkAssetResRef Load(void* loader, const UnkResKey& key) {
    return fn_7100017878(loader, reinterpret_cast<const UnkStrIdPathPair*>(&key));
}

void fn_71001aaf80(UnkLinkOwner_l1aaf80* self) {
    if (!self->header || self->linked)
        return;
    u64 base = reinterpret_cast<u64>(self->header);
    Link(self->header->texture_path, base);
    Link(self->header->field_0x30, base);
    Link(self->header->field_0x28, base);

    UnkResKey texture_key(g_unk_716e4540f8.name, CFilePathStrId(self->header->texture_path, true));
    self->texture = Load(self->loader, texture_key);
    UnkResKey second_key(g_unk_716e454278.name, CFilePathStrId(self->header->field_0x30, true));
    self->second = Load(self->loader, second_key);

    self->field_0x28 = fn_71001b363c(reinterpret_cast<UnkCountedObject259*>(self->texture.ptr));
    self->field_0x30 = reinterpret_cast<UnkLinkResource_l1aaf80*>(self->second.ptr)->field_0x70;
    self->linked = true;
}
