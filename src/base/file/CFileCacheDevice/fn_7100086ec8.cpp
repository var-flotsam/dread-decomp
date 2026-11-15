// base::file::CFileCacheDevice unit: answers a query from the size cache at +0x18 when it holds the file,
// otherwise (or for more detailed queries) passes it to the wrapped device. Names and layouts are
// placeholders.
#include "types.h"

struct UnkCachePathData_l086ec8 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkCacheFile_l086ec8 {
    UnkCachePathData_l086ec8* path;
};

struct UnkCacheResult_l086ec8 {
    u8 valid;
    u8 pad_0x1[3];
    u32 flags;
    s64 size;
};

struct UnkCachedDevice_l086ec8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual u32 Query(UnkCacheFile_l086ec8* file, UnkCacheResult_l086ec8* result, const u32* wanted);
};

struct UnkCacheSizes_l086ec8 {
    bool enabled;
    u8 rest[0x97];
};

struct UnkFileCacheDevice_l086ec8 {
    u8 pad_0x0[0x10];
    UnkCachedDevice_l086ec8* device;
    UnkCacheSizes_l086ec8 sizes;  // +0x18
};

u64 fn_7100085610(char* p0, char* p1);

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
u32 fn_7100086ec8(UnkFileCacheDevice_l086ec8* self, UnkCacheFile_l086ec8* file, UnkCacheResult_l086ec8* result,
                  const u32* wanted) {
    if (self->sizes.enabled) {
        u64 key = file->path->hash;
        const s32* size = reinterpret_cast<const s32*>(
            fn_7100085610(reinterpret_cast<char*>(&self->sizes), reinterpret_cast<char*>(&key)));
        u32 status = 1;
        if (!size)
            return status;
        result->valid = status;
        if (*wanted & 1) {
            result->size = *size;
            result->flags |= 1;
        }
        if (*wanted >= 2)
            self->device->Query(file, result, wanted);
        return 0;
    }
    return self->device->Query(file, result, wanted);
}
