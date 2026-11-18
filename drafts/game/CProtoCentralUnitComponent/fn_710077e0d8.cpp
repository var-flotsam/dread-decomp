// CProtoCentralUnitComponent's unit (tools/tu_map.py unit 1181): the activation. After the central unit's
// own (fn_7100775fe0), the entity linked at +0xa00 is played a fixed animation id through its character
// class (+0x220) unless it already plays it, and its weak target gets the same id (fn_71009c0f1c).
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU372.h"
#include "game/UnkTU437.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkWeakTarget;
struct UnkCentralUnit_l775fe0;

void fn_7100775fe0(UnkCentralUnit_l775fe0* self, s32 mode);

struct UnkProtoLinked_l77e0d8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* character;  // 0x220
};

struct UnkProtoLinkHandle_l77e0d8 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;  // 0x10
};

// A lazily resolved link to another entity: the key is resolved to a handle on first use.
struct UnkProtoLink_l77e0d8 {
    u8 pad_0x0[0x8];
    u8 key[0x28];               // 0x8
    u32 state;                  // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38: an UnkProtoLinkHandle_l77e0d8

    void Resolve() {
        if (state == 1) {
            {
                base::UnkRefPtrMember ref = fn_7100353be8(key);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
    }
    UnkProtoLinkHandle_l77e0d8* Handle() { return reinterpret_cast<UnkProtoLinkHandle_l77e0d8*>(handle.ptr); }
    UnkWeakTarget* Get() {
        Resolve();
        return Handle() ? Handle()->target : nullptr;
    }
    // The linked entity, once known to be there.
    UnkProtoLinked_l77e0d8* Linked() {
        Resolve();
        return reinterpret_cast<UnkProtoLinked_l77e0d8*>(Handle()->target);
    }
};
static_assert(sizeof(UnkProtoLink_l77e0d8) == 0x40, "UnkProtoLink_l77e0d8 layout");

struct UnkProtoCentralUnit_l77e0d8 {
    u8 pad_0x0[0xa00];
    UnkProtoLink_l77e0d8 link;  // 0xa00
};

void fn_710077e0d8(UnkProtoCentralUnit_l77e0d8* self, s32 mode) {
    fn_7100775fe0(reinterpret_cast<UnkCentralUnit_l775fe0*>(self), mode);
    UnkProtoLinked_l77e0d8* linked = reinterpret_cast<UnkProtoLinked_l77e0d8*>(self->link.Get());
    if (!linked)
        return;
    const u64 id = 0x6909d27d2c787fba;
    if (UnkAnimationComponent* character = linked->character) {
        if (!fn_710117ae38(character, id, false))
            UnkSpitSend(self->link.Linked()->character, 0xa0e33be014f44bd7, 0xf4061ff25a2163d6);
    }
    if (void* target = fn_71002b3b84(self->link.Get()))
        fn_71009c0f1c(target, id);
}
