// Unit 2538: resets a pack request: when the path (+0x58) is not the empty one, clears the request's
// string and counters, releases the pack held for it (fn_7101199f08), re-registers the callback and
// asks the pack manager for the path again (as src/game/UnkTU2666/UnkTU2666_lua_ovb0163.cpp does),
// then sets the path to the global one. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2201.h"
#include "game/UnkTU2730.h"
#include <new>

#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkMaskedId {
    union {
        CStrId id;
    };
    u8 pad_0x8[0x18];
    UnkMaskedId();
    ~UnkMaskedId() { id.~CStrId(); }
};
inline UnkMaskedId::UnkMaskedId() { fn_7100c939dc(this); }

struct UnkPackManager_b0163;
struct UnkPackRefTarget_b0163 {
    void** vtable;
    s32 ref_count;  // 0x8
};
struct UnkPackRef_b0163 {
    UnkPackRefTarget_b0163* ptr;
    ~UnkPackRef_b0163() {
        if (UnkPackRefTarget_b0163* p = ptr) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
                reinterpret_cast<void (*)(UnkPackRefTarget_b0163*)>(p->vtable[0x10 / 8])(p);
        }
    }
};

struct UnkCallback_kr {
    void* object;
    void (*fn)();
};

UnkPackRef_b0163 fn_7101199f08(UnkPackManager_b0163* self, const CFilePathStrId& path);
UnkPackRef_b0163 fn_7101197008(UnkPackManager_b0163* self, const CFilePathStrId& path, const UnkMaskedId& id,
                                  bool flag);
void fn_7100da6748();

union UnkPackSlot_b0163 {
    UnkPackSlot_b0163() {}
    ~UnkPackSlot_b0163() {}
    UnkPackRef_b0163 ref;
};

struct UnkUnaligned64_b0163 {
    u64 value;
} __attribute__((packed));

struct UnkPackRequest_b0163 {
    u8 pad_0x0[0x58];
    CFilePathStrId path;  // 0x58
    bool pending;  // 0x60
    u8 pad_0x61[7];
    u8 name[0x30];  // 0x68
    u32 count;  // 0x98
    UnkUnaligned64_b0163 field_0x9c;
};

void fn_7100da635c(char* self_) {
    UnkPackRequest_b0163* self = reinterpret_cast<UnkPackRequest_b0163*>(self_);
    using namespace base::global;
    if (self->path.data != &g_unk_7101d47200->empty) {
        self->field_0x9c.value = 0;
        
        fn_710007e77c(reinterpret_cast<CRntString*>(self->name), g_unk_710162e2ed);
        self->count = 0;
        UnkPackSlot_b0163 first;
        UnkPackSlot_b0163 second;
        if (self->pending)
        {
            new (&first.ref) UnkPackRef_b0163(
                fn_7101199f08(static_cast<UnkPackManager_b0163*>(g_unk_7101d47330), self->path));
            first.ref.~UnkPackRef_b0163();
        }
        {
            UnkCallback_kr callback = {self, fn_7100da6748};
            fn_71011995c0(g_unk_7101d47330, callback);
        }
        {
            auto* manager = static_cast<UnkPackManager_b0163*>(g_unk_7101d47330);
            UnkMaskedId id;
            new (&second.ref) UnkPackRef_b0163(fn_7101197008(manager, self->path, id, false));
            second.ref.~UnkPackRef_b0163();
        }
        if (self->path.data != g_unk_7101d470e8.data) {
            UnkStringData* d = g_unk_7101d470e8.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (self->path.data != &pool->empty) {
                --self->path.data->ref_count;
                if (self->path.data->ref_count == 0)
                    fn_71000803a4(pool, self->path.data);
            }
            self->path.data = d;
        }
    }
    self->pending = false;
}
