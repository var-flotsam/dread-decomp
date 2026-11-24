// CPullOffGrapplePointComponent unit (tools/tu_map.py): after the grapple point's own read, reads "Used" from
// the component's blackboard section (resetting the point when it is missing) and the name of the last
// animation the activatable object played ("ActivatableObjectLastAnim"); when there is one, resolves the
// object's link and sends that animation to the object's character class with mode 0. Names and layouts
// are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CGrapplePointComponent.h"
#include "game/CPullableGrapplePointComponent.h"
#include "game/UnkTU437.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern UnkProperty g_unk_7172724fd8;

struct UnkGrapplePoint_b49;
struct UnkGrappleUsed_A4_b76;
struct UnkPullableGrapplePoint_b54;
void CGrapplePointComponent_LoadFromBlackboard(UnkGrapplePoint_b49* self, CBlackboard* blackboard);
CStrId fn_7100822a1c(UnkGrappleUsed_A4_b76* self);

// A string id's data as far as its hash goes.
struct UnkStrIdHash_l82de94 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkPullOffObject_l82de94 {
    u8 pad_0x0[0x220];
    void* field_0x220;  // its character class
};

// What the link's handle refers to.
struct UnkPullOffHandle_l82de94 {
    u8 pad_0x0[0x10];
    UnkPullOffObject_l82de94* object;  // 0x10
};

struct UnkPullOffValue_l82de94 {
    CStrId id;
    u64 field_0x8;
};

// A table of values keyed by property name.
struct UnkPullOffTable_l82de94 {
    UnkPullOffValue_l82de94* values;
    u64* keys;
    u32 count;

    // The value stored under `property`'s name (one past the end when there is none).
    UnkPullOffValue_l82de94* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return &values[index];
    }
};

struct UnkPullOffGrapplePoint_l82de94 {
    u8 pad_0x0[0x58];
    UnkPullOffTable_l82de94 used;  // 0x58
    u8 pad_0x70[0xa0 - 0x70];
    UnkPullOffTable_l82de94 last_anim;  // 0xa0
    u8 pad_0xb8[0xd0 - 0xb8];
    u8 source[0x28];        // 0xd0
    s32 state;              // 0xf8
    base::UnkRefHandle link;  // 0x100

    // The link's handle, resolved from the source on first use (state 1 -> 2).
    base::UnkRefHandle* Link() {
        if (state == 1) {
            {
                base::UnkRefPtrMember fetched = fn_7100353be8(source);
                fn_710031d208(&link, reinterpret_cast<base::UnkRefHandle*>(&fetched));
            }
            state = 2;
        }
        return &link;
    }
};
static_assert(__builtin_offsetof(UnkPullOffGrapplePoint_l82de94, link) == 0x100, "layout");

void fn_710082de94(UnkPullOffGrapplePoint_l82de94* self, CBlackboard* blackboard) {
    CGrapplePointComponent_LoadFromBlackboard(reinterpret_cast<UnkGrapplePoint_b49*>(self), blackboard);
    CBlackboard::CSection* section =
        fn_71010472e0(blackboard, fn_7100822a1c(reinterpret_cast<UnkGrappleUsed_A4_b76*>(self)));
    if (!section)
        return;
    bool used = false;
    if (!fn_710006d11c(section, &self->used.Find(g_unk_71727235d0)->id, &used)) {
        fn_710082cee4(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self), true);
        fn_710082cf08(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self), false);
        return;
    }
    CStrId anim = CStrId::Empty();
    fn_710051d464(section, &self->last_anim.Find(g_unk_7172724fd8)->id, &anim);
    if (anim.data != &g_unk_7101d47108->empty) {
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        if (self->state == 1) {
            {
                base::UnkRefPtrMember fetched = fn_7100353be8(self->source);
                fn_710031d208(&self->link, reinterpret_cast<base::UnkRefHandle*>(&fetched));
            }
            self->state = 2;
        }
        auto* handle = reinterpret_cast<UnkPullOffHandle_l82de94*>(self->link.ptr);
        if (handle && handle->object) {
            if (void* target = handle->object->field_0x220) {
                u64 hash = reinterpret_cast<UnkStrIdHash_l82de94*>(anim.data)->hash;
                mode.id = 0;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
            }
        }
    }
}
