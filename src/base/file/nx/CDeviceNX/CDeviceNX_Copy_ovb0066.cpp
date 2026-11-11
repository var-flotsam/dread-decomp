// base::file::nx::CDeviceNX's unit (tools/tu_map.py unit 38): copying a file: it is read into a buffer
// (fn_710002ed98, 8-byte aligned, flags 0x16) and written out under the destination path by
// fn_710002eec8, then the buffer is deleted. Kept apart from fn_710002ed98's file so its unused
// argument is still passed. Names and layouts are placeholders.
#include "base/file/nx/CDeviceNX.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

u32 fn_710002ed98(void* table, base::global::CRntString* path, void* a, u32 b, void* c, u32 d, u32 e);

class UnkSaveBuffer_ovb0066 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void* vf_0x28();
    virtual u64 vf_0x30();
};

s32 fn_710002ecdc(void* table, CRntString* path, void* dest, bool commit) {
    UnkSaveBuffer_ovb0066* buffer = nullptr;
    s32 ret = fn_710002ed98(table, path, &buffer, 0, nullptr, 8, 0x16);
    if (ret == 0) {
        UnkSaveBuffer_ovb0066* held = buffer;
        void* data = held->vf_0x28();
        u64 size = held->vf_0x30();
        ret = fn_710002eec8(table, dest, data, size, commit, true);
        if (held) {
            held->vf_0x8();
            buffer = nullptr;
        }
    }
    return ret;
}
