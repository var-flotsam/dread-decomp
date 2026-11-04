// The pack constructor of tools/tu_map.py unit 65 (the packs of src/base/file/CPackManagerDevice_3.cpp):
// keeps the device, a copy of the path id (a CFilePathStrId) and the file, builds the reader at +0x28,
// takes the next serial number and hands the file to the reader. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include <new>

#include "types.h"

struct UnkMountedPack;
struct UnkPackManagerView;
struct UnkPackPathKey;

extern void* g_unk_7101a1b058[];                                      // the pack's vtable
extern s32 g_unk_7101d5954c __attribute__((visibility("hidden")));    // the next pack serial number
void fn_710008c7d4(void* reader);
void fn_710008cbe8(void* reader, void* file);

struct UnkMountedPack_A1b71 {
    void* vtable;
    s32 field_0x8;
    u8 pad_0xc[0x4];
    UnkPackManagerView* device;           // 0x10
    base::global::CFilePathStrId path;    // 0x18
    void* file;                           // 0x20
    u8 reader[0xa0];                      // 0x28
    bool locked;                          // 0xc8
    u8 pad_0xc9[3];
    s32 file_refs;                        // 0xcc
    s32 serial;                           // 0xd0
};

void fn_7100088dfc(UnkMountedPack* pack, UnkPackManagerView* device, const UnkPackPathKey* path, void* file) {
    UnkMountedPack_A1b71* self = reinterpret_cast<UnkMountedPack_A1b71*>(pack);
    self->field_0x8 = 0;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1b058) + 0x10;
    self->device = device;
    new (&self->path) base::global::CFilePathStrId(*reinterpret_cast<const base::global::CFilePathStrId*>(path));
    self->file = file;
    fn_710008c7d4(self->reader);
    self->locked = false;
    self->file_refs = 0;
    self->serial = g_unk_7101d5954c++;
    fn_710008cbe8(self->reader, self->file);
}
