// CCharClassWaterPoolComponent (unit 1396): restores the pool from the current scenario's blackboard section: reads
// the stored move count and, for each entry's object, plays its "move", "move2", ... animations up to that count and
// passes the matching hashes on to the object's owner. Names and layouts are placeholders.
#include <string>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU372.h"
#include "game/UnkTU437.h"
#include "game/UnkTU57.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkReflObjectRef_l908d30;
UnkReflObjectRef_l908d30* fn_71009c0e24(void* owner, u64 id);
extern UnkProperty g_unk_7172734638;

struct UnkHolderRef_l908d30 : base::UnkRefCounted {
    void* target;  // 0x10
};

struct UnkHolderEntry_l908d30 {
    u8 pad_0x0[0x8];
    u8 key[0x28];  // 0x8
    u32 state;     // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38
    u8 pad_0x40[0x8];

    UnkHolderRef_l908d30* Resolve() {
        if (state == 1) {
            {
                base::UnkRefPtrMember fetched = fn_7100353be8(key);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fetched));
            }
            state = 2;
        }
        return static_cast<UnkHolderRef_l908d30*>(handle.ptr);
    }
};
static_assert(sizeof(UnkHolderEntry_l908d30) == 0x48, "layout");

struct UnkMoveTarget_l908d30 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkValue_l908d30 {
    CStrId id;
    u64 field_0x8;
};

struct UnkWaterPool_l908d30 {
    u8 pad_0x0[0xc90];
    UnkHolderEntry_l908d30* entries;  // 0xc90
    u32 count;                        // 0xc98
    u8 pad_0xc9c[0xcd0 - 0xc9c];
    UnkValue_l908d30* values;  // 0xcd0
    u64* keys;                 // 0xcd8
    u32 key_count;             // 0xce0
    u8 pad_0xce4[0xcf8 - 0xce4];
    s32 moves;       // 0xcf8
    s32 field_0xcfc;  // 0xcfc

    u64 IndexOf(const UnkProperty& property) const {
        u64 index = 0;
        u32 n = key_count;
        if (n) {
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        return index;
    }
};
static_assert(__builtin_offsetof(UnkWaterPool_l908d30, entries) == 0xc90, "layout");
static_assert(__builtin_offsetof(UnkWaterPool_l908d30, values) == 0xcd0, "layout");
static_assert(__builtin_offsetof(UnkWaterPool_l908d30, moves) == 0xcf8, "layout");

void fn_7100908d30(UnkWaterPool_l908d30* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section;
    {
        CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        section = fn_71010472e0(blackboard, section_id);
    }
    if (!section)
        return;
    if (!fn_7100395a30(section, &self->values[self->IndexOf(g_unk_7172734638)], &self->moves))
        return;
    self->field_0xcfc = self->moves;
    for (u64 i = 0; i < self->count; ++i) {
        UnkHolderRef_l908d30* ref = self->entries[i].Resolve();
        auto* object = static_cast<UnkMoveTarget_l908d30*>(ref ? ref->target : nullptr);
        void* animation = object ? object->animation : nullptr;
        if (!animation || self->moves < 0)
            continue;
        for (s32 j = 0; j <= self->moves; ++j) {
            CRntString name("move");
            if (j >= 2) {
                std::string digit = std::to_string(j);
                CRntString_AppendCString(&name, digit.c_str());
            }
            u64 hash;
            fn_7100066630(&hash, &name);
            {
                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 0;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
            }
            fn_7101178010(animation, 0xf4061ff25a2163d6, false, (f32)fn_7101177da8(animation, 0xf4061ff25a2163d6));
            UnkHolderRef_l908d30* again = self->entries[i].Resolve();
            if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(again ? again->target : nullptr))) {
                CStrId first(name, true);
                if (fn_71009c0e24(target, first.data->Key())) {
                    CStrId second(name, true);
                    fn_71009c0f1c(target, second.data->Key());
                }
            }
        }
    }
}
