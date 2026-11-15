// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eea3fc requests the sub-area
// named by an id: with no change running (+0x110 clear) it is handled at once (fn_7100ee4550), otherwise a
// request (a copy of the id and three flags, the first set) is appended to the ring buffer of 64 at +0x420. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::global::CStrId;

struct UnkPoppedRequest;

struct UnkRequest_lea3fc {
    CStrId id;
    u8 field_0x8;
    u8 field_0x9;
    u8 field_0xa;
    u8 pad_0xb[5];
};
static_assert(sizeof(UnkRequest_lea3fc) == 0x10, "layout");

struct UnkRequestQueue_lea3fc {
    u8 pad_0x0[0x110];
    u32 field_0x110;
    u8 pad_0x114[0x30c];
    UnkRequest_lea3fc* write;  // 0x420
    u64 count;                 // 0x428
    UnkRequest_lea3fc storage[64];  // 0x430
};
static_assert(__builtin_offsetof(UnkRequestQueue_lea3fc, write) == 0x420, "layout");
static_assert(__builtin_offsetof(UnkRequestQueue_lea3fc, storage) == 0x430, "layout");

void fn_7100eea3fc(void* manager, const CStrId* id, bool a, bool b) {
    UnkRequestQueue_lea3fc* self = static_cast<UnkRequestQueue_lea3fc*>(manager);
    if (self->field_0x110 == 0) {
        fn_7100ee4550(static_cast<CSubAreaManager*>(manager), reinterpret_cast<const UnkPoppedRequest*>(id), a, b);
        return;
    }
    CStrId key(*id);
    UnkRequest_lea3fc* slot = self->write;
    new (&slot->id) CStrId(key);
    slot->field_0x8 = 1;
    slot->field_0x9 = a;
    slot->field_0xa = b;
    UnkRequest_lea3fc* next = self->write + 1;
    self->write = next == &self->storage[64] ? &self->storage[0] : next;
    ++self->count;
}
