// Pre-filters an incoming damage event: clears a pending state, then checks the damage-source
// and screw-attack tracks of the owner's character class (zeroing or blocking the damage), and
// reports whether the owner's shield currently absorbs it (2) or not (0).
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"

using base::global::CStrId;

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id copied out of a track and released without a null check.
struct UnkIdCopy_7101105e44 {
    UnkStringData* data;
    explicit UnkIdCopy_7101105e44(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkIdCopy_7101105e44() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
        }
    }
};

struct UnkStrUse_7101105e44 {
    u8 unk_0x0[0x10];
    const char* str;
    u8 unk_0x18[0x2c];
    union {
        u32 use_count;
    };
};

struct UnkShield_7101105e44;

struct UnkState_7101105e44 {
    u8 unk_0x0[0x330];
    bool field_0x330;
};

struct UnkStateOwner_7101105e44 {
    u8 unk_0x0[0x1c0];
    UnkState_7101105e44* field_0x1c0;
};

struct UnkActor_7101105e44 {
    u8 unk_0x0[0x1b8];
    UnkShield_7101105e44* shield;
    u8 unk_0x1c0[0x60];
    UnkAnimationComponent* char_class;
    u8 unk_0x228[0xe8];
    bool field_0x310;
};

struct UnkDamage_7101105e44 {
    u8 unk_0x0[0x20];
    bool field_0x20;
    u8 unk_0x21[0x93];
    u32 flags;
};

struct UnkStruct_7101105e44 {
    u8 unk_0x0[0x30];
    UnkActor_7101105e44* actor;
    u8 unk_0x38[0x28];
    bool field_0x60;
    u8 unk_0x61[0x10b3];
    u32 field_0x1114;
    u8 unk_0x1118[0xc8];
    bool field_0x11e0;
};

void fn_7100d7add4(UnkActor_7101105e44* actor, bool value);
u32 fn_7100414070(const char* str);
void fn_71008c1538(UnkDamage_7101105e44* d, f32 amount);
bool fn_7101161088(UnkShield_7101105e44* shield);

static inline const char* UseStr_7101105e44(const UnkIdCopy_7101105e44& id) {
    UnkStrUse_7101105e44* s = reinterpret_cast<UnkStrUse_7101105e44*>(id.data);
    ++s->use_count;
    return s->str ? s->str : "";
}

u32 fn_7101105e44(UnkStruct_7101105e44* self, UnkDamage_7101105e44* d) {
    if (self->actor->field_0x310) {
        fn_7100d7add4(self->actor, false);
        self->field_0x11e0 = false;
        self->field_0x1114 = 0;
    }
    self->field_0x60 = true;
    if (!(d->flags & 0x20000030)) {
        UnkStateOwner_7101105e44* o =
            reinterpret_cast<UnkStateOwner_7101105e44*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)));
        if (o && o->field_0x1c0 && o->field_0x1c0->field_0x330) {
            fn_71008c1538(d, 999999.0f);
            return 0;
        }
    }
    if (self->actor->char_class) {
        UnkTrackQuery q = {fn_7100c135a8(), nullptr, -1.0f};
        fn_710117b08c(self->actor->char_class, &q);
        if (q.result) {
            u32 mask;
            {
                UnkIdCopy_7101105e44 id(q.result->sources);
                mask = fn_7100414070(UseStr_7101105e44(id));
            }
            if (!(d->flags & mask))
                return 2;
        }
        if (fn_710117b348(self->actor->char_class, fn_7100c2bcd8())) {
            UnkTrackQuery q2 = {fn_7100bf0758(), nullptr, -1.0f};
            fn_710117b08c(self->actor->char_class, &q2);
            if (!q2.result || !(d->flags & 0x800040)) {
                fn_71008c1538(d, 0.0f);
                d->field_0x20 = true;
            }
        }
    }
    UnkShield_7101105e44* shield = self->actor->shield;
    if (shield && fn_7101161088(shield))
        return 2;
    return 0;
}
