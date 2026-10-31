// From base::file::CPackManagerDevice's unit (tools/tu_map.py unit 66; the device is laid out in
// src/base/file/CPackManagerDevice.cpp, which keeps its types to itself, so this file reads it
// through its own views): whether a pack with the given path (an id compared by its data) is
// mounted, and whether it is in use (its vf_0x20, the reference count, non-zero). Both under the
// device's mutex. Names are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkPackInUseView {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual u32 vf_0x20();  // its reference count
    u8 unk_0x8[0x10];
    CFilePathStrId path;  // +0x18
};

struct UnkPackDeviceInUseView {
    u8 pad_0x0[0x30];
    UnkPackInUseView** packs;  // +0x30
    u32 pack_count;       // +0x38
    u8 pad_0x3c[0x2c];
    nn::os::MutexType mutex;  // +0x68
};

bool fn_710008aa08(UnkPackDeviceInUseView* self, const CFilePathStrId* path) {
    fn_710006691c(&self->mutex);
    bool in_use = false;
    for (UnkPackInUseView **it = self->packs, **end = it + self->pack_count; it != end; ++it) {
        if ((*it)->path.data == path->data) {
            in_use = (*it)->vf_0x20() != 0;
            break;
        }
    }
    fn_7100066920(&self->mutex);
    return in_use;
}

bool fn_710008a990(UnkPackDeviceInUseView* self, const CFilePathStrId* path) {
    fn_710006691c(&self->mutex);
    bool found = false;
    for (UnkPackInUseView **it = self->packs, **end = it + self->pack_count; it != end; ++it) {
        if ((*it)->path.data == path->data) {
            found = true;
            break;
        }
    }
    fn_7100066920(&self->mutex);
    return found;
}
