// CCentralUnitAIComponent's unit (tools/tu_map.py, unit 614): after the base step, tells the owner's
// sub-object at +0x220 two things (the id table's id with 2 unless an animation already uses it, then a
// zero), switches the game manager's music id and fades music, then stops channel 7. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CStrId;

char* fn_71003696d4(char* self);
void fn_7100df5cec(UnkGameManager* manager, void* target);
struct UnkMgr_0xf18;
void fn_7100fdb9e0(UnkMgr_0xf18* manager, u32 kind, u32 flag, f32 a, f32 b);
extern void* g_unk_71726bc560;
extern const u8 g_unk_710164b958;
extern const u8 g_unk_7172769740;
extern CStrId g_unk_71726c40e8 __attribute__((visibility("hidden")));

struct UnkIdHolder_l403c5c {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkIdTable_l403c5c {
    UnkIdTable_l403c5c() asm("fn_7100031894");
    ~UnkIdTable_l403c5c() asm("fn_71000407f0");
    u8 pad_0x0[0x1d78];
    UnkIdHolder_l403c5c* field_0x1d78;
};

inline UnkIdTable_l403c5c& IdTable_l403c5c() {
    static UnkIdTable_l403c5c s_table;
    return s_table;
}

// The channel mask as src/game/UnkLoadingScreen.cpp defines it, and its two guarded constants.
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkLoadingMaskOf {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag;
void fn_7100fd7b44(void* manager, UnkLoadingMask channels, UnkLoadingMask mask, f32 time);

static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

struct UnkOwner_l403c5c {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkSelf_l403c5c {
    u8 pad_0x0[0x30];
    UnkOwner_l403c5c* owner;
};

void fn_7100403c5c(UnkSelf_l403c5c* self) {
    fn_71003696d4(reinterpret_cast<char*>(self));
    if (void* target = self->owner->field_0x220) {
        base::reflection::UnkReflTypeIdArg value;
        base::reflection::UnkReflTypeIdArg arg;
        if (!fn_710117ae38(static_cast<UnkAnimationComponent*>(target), IdTable_l403c5c().field_0x1d78->id, false))
            base::reflection::refl_EmitU32(self->owner->field_0x220, IdTable_l403c5c().field_0x1d78->id,
                                           0xf4061ff25a2163d6, 2, &g_unk_710164b958, &g_unk_7172769740);
        // The second send's arguments live for the whole block (their own stack slots).
        void* receiver = self->owner->field_0x220;
        value.id = 0;
        arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        fn_7101176690(receiver, 0xf3925904d50f96b3, 0xcf7107181622cb23, &value, &g_unk_710164b958,
                      &g_unk_7172769740, &arg);
    }
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0x2238.data != g_unk_71726c40e8.data) {
        fn_7100df5cec(manager, &g_unk_71726c40e8);
        fn_7100fd7b44(MusicManager(), UnkLoadingMask(4), UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), 0.2f);
    }
    fn_7100fdb9e0(static_cast<UnkMgr_0xf18*>(MusicManager()), 7, 0, -1.0f, 0.0f);
}
