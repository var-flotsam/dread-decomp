// CDroppableMissileComponent and CDroppablePowerBombComponent units (1218, 1219): setters that build a string-id
// handle from the given id (fn_7100ae986c) and copy it into the component (+0xa8 or +0xb8). One template for the
// four. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

template <u32 Offset>
static inline __attribute__((always_inline)) void SetHandle_A5_b70(char* self, const CStrId& id) {
    alignas(UnkStrIdHandle) char storage[sizeof(UnkStrIdHandle)];
    UnkStrIdHandle& h = *reinterpret_cast<UnkStrIdHandle*>(storage);
    fn_7100ae986c(&h, id);
    UnkStrIdHandle& dst = *reinterpret_cast<UnkStrIdHandle*>(self + Offset);
    dst.id = h.id;
    dst.field_0x8 = h.field_0x8;
    h.id.~CStrId();
}

void fn_71007bc95c(char* self, const CStrId& id) { SetHandle_A5_b70<0xa8>(self, id); }
void fn_71007bcadc(char* self, const CStrId& id) { SetHandle_A5_b70<0xb8>(self, id); }
void fn_71007bda2c(char* self, const CStrId& id) { SetHandle_A5_b70<0xa8>(self, id); }
void fn_71007bdbac(char* self, const CStrId& id) { SetHandle_A5_b70<0xb8>(self, id); }
