// CCentralUnitAIComponent's unit: once the handle source at +0x14758 is resolved (state 1 becomes 2), looks
// at the handle's object's component; when the component's object holds an entry fn_71010662f0 accepts for
// the id g_unk_71726c40f0, and fn_71007aa7c4 agrees, registers a callback with the manager. Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void* fn_71004067a4(void* owner);
bool fn_71010662f0(void* entry, void* owner, const CStrId* id);
bool fn_71007aa7c4(void* component);
void fn_710040782c();
extern void* g_unk_7101d47330;
extern CStrId g_unk_71726c40f0 __attribute__((visibility("hidden")));

struct UnkCallback_cu {
    void* object;
    void (*fn)();
};
void fn_71011995c0(void* owner, const UnkCallback_cu& callback);

struct UnkCountedCU : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

struct UnkRefCU {
    UnkCountedCU* ptr;
    ~UnkRefCU() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};
UnkRefCU fn_7100353be8(void* from);

struct UnkCUEntryList {
    u8 pad_0x0[0x150];
    u8* entries;
    u32 count;
};

struct UnkCUComponent {
    u8 pad_0x0[0x200];
    UnkCUEntryList* list;
};

struct UnkCUOwner {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkCUSource {
    u8 pad_0x0[0x14758];
    u8 items[0x28];  // 0x14758
    u32 state;       // 0x14780
    u32 pad_0x14784;
    union {
        UnkCountedCU* handle;  // 0x14788
        base::UnkRefHandle handle_slot;
    };
};
static_assert(__builtin_offsetof(UnkCUSource, state) == 0x14780, "layout");
static_assert(__builtin_offsetof(UnkCUSource, handle) == 0x14788, "layout");

void fn_71004065f4(UnkCUSource* self) {
    if (self->state == 1) {
        {
            UnkRefCU fresh = fn_7100353be8(self->items);
            fn_710031d208(&self->handle_slot, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    UnkCountedCU* c = self->handle;
    if (c && c->raw_0x10) {
        asm("" ::: "memory");
        UnkCUComponent* component = static_cast<UnkCUComponent*>(fn_71004067a4(c->field_0x10));
        if (component) {
            UnkCUEntryList* list = component->list;
            u8* entry = list->entries;
            bool found = false;
            if (list->count) {
                u64 bytes = u64(list->count) * 0x30;
                do {
                    CStrId id = g_unk_71726c40f0;
                    found = fn_71010662f0(entry, reinterpret_cast<UnkCUOwner*>(self)->field_0x30, &id);
                    entry += 0x30;
                    bytes -= 0x30;
                } while (bytes && !found);
            }
            if (fn_71007aa7c4(component))
                fn_71011995c0(g_unk_7101d47330, UnkCallback_cu{self, fn_710040782c});
        }
    }
}
