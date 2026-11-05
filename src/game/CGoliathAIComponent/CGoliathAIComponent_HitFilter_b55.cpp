// CGoliathAIComponent unit (tools/tu_map.py, the name from reflection): a hit filter. When the hit
// info's flag (bit 6 of +0xb5) is set and the owner's collision container (+0x2a0) maps the hit
// target's +0x10 to a non-empty id whose text contains the text of the id at 0x71726e6740, it
// clears *out and returns 1; otherwise it defers to fn_71011061ac. Both ids' counts at +0x44 are
// raised and never lowered, as in the original. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkCollisionContainer;
CStrId* fn_7100864018(UnkCollisionContainer* self, void* object);

struct UnkGoliathIdData_b55 {
    u8 pad_0x0[0x10];
    const char* text;  // a CRntString starts here
    u8 pad_0x18[0x2c];
    union {
        s32 count;  // a union member: the original reloads around its stores
    };
};

extern UnkGoliathIdData_b55* g_unk_71726e6740 __attribute__((visibility("hidden")));

struct UnkGoliathOwner_b55 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkGoliathAI_b55 {
    u8 pad_0x0[0x30];
    UnkGoliathOwner_b55* owner;
};

struct UnkGoliathHitInfo_b55 {
    u8 pad_0x0[0xb5];
    u8 flags;
};

struct UnkGoliathHitTarget_b55 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

u32 fn_71011061ac(UnkGoliathAI_b55* self, u32* out, void* arg2, UnkGoliathHitInfo_b55* info,
                  UnkGoliathHitTarget_b55** hit);

u32 fn_71005916b4(UnkGoliathAI_b55* self, u32* out, void* arg2, UnkGoliathHitInfo_b55* info,
                  UnkGoliathHitTarget_b55** hit) {
    if (info->flags & 0x40) {
        if (UnkCollisionContainer* collision = self->owner->field_0x2a0) {
            UnkGoliathHitTarget_b55* target = *hit;
            if (target && target->field_0x10) {
                CStrId* id = fn_7100864018(collision, target->field_0x10);
                UnkGoliathIdData_b55** data = reinterpret_cast<UnkGoliathIdData_b55**>(id);
                ++(*data)->count;
                UnkGoliathIdData_b55* d = *data;
                if (d != reinterpret_cast<UnkGoliathIdData_b55*>(&g_unk_7101d47108->empty)) {
                    const base::global::CRntString* str =
                        reinterpret_cast<const base::global::CRntString*>(&d->text);
                    UnkGoliathIdData_b55* other = g_unk_71726e6740;
                    ++other->count;
                    if (fn_710007fb9c(str, other->text ? other->text : "")) {
                        *out = 0;
                        return 1;
                    }
                }
            }
        }
    }
    return fn_71011061ac(self, out, arg2, info, hit);
}
