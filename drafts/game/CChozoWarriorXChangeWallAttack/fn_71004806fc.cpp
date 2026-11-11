// CChozoWarriorXChangeWallAttack's start: after the base start, when the owner's AI (+0x240) is a
// CChozoWarriorXAIComponent and the owner tracks an object (+0x220), sends it a mode-2 request under the
// id the AI reports (fn_710049c59c) and 0xf4061ff25a2163d6; then hooks the callback fn_7100481424 up to
// the tracked object as src/game/CBigFistAttack/CBigFistAttack_Start_A2_b72.cpp does. Names and layouts
// are placeholders.
#include "../../../src/game_5821e4/CAttackDtor.h"
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_7100481424();
// Returned in memory (it has a copy constructor).
struct UnkStrIdOut_b0266 {
    UnkAttackStrIdRaw raw;
    UnkStrIdOut_b0266(const UnkStrIdOut_b0266&);

    void Release() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (raw.data && raw.data != &pool->empty) {
                --raw.data->ref_count;
                if (raw.data->ref_count == 0) {
                    fn_71000803a4(pool, raw.data);
                    pool = g_unk_7101d47108;
                }
            }
            raw.data = &pool->empty;
        }
    }
};
UnkStrIdOut_b0266 fn_710049c59c(UnkReflObject* ai);

struct UnkChangeWallLink_b0266 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkChangeWallStart_b0266 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x38];
    UnkChangeWallLink_b0266 link;  // 0x48
    UnkWeakHandle* handle;         // 0x60
};

static inline UnkReflObject* Ai_b0266(char* owner) { return *reinterpret_cast<UnkReflObject**>(owner + 0x240); }

void fn_71004806fc(UnkChangeWallStart_b0266* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkReflObject* ai = Ai_b0266(self->owner);
    bool is_wx = ai && fn_710009d50c(ai->vf_0x0(), fn_710048db44());
    char* owner = self->owner;
    if (is_wx) {
        if (void* target = *reinterpret_cast<void**>(owner + 0x220)) {
            UnkStrIdOut_b0266 id = fn_710049c59c(ai);
            u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id.raw.data) + 0x30);
            id.Release();
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 2;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, hash, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
            owner = self->owner;
        }
    }
    UnkChangeWallLink_b0266* link = &self->link;
    link->context = self;
    link->callback = fn_7100481424;
    UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(owner + 0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        // keeps self (not &self->handle) live across the calls
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
}
