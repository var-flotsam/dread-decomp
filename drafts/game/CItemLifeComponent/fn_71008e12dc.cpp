// CItemLifeComponent unit (tools/tu_map.py unit 1379): the item takes a hit. A "HUSK" hit hides the model parts
// named for the husk collider's suffix; a "HIT" hit counts only when it lands on a matching enabled collider; any
// other hit takes damage. A counted hit tints the item, or kills it on the last one. Names and layouts are placeholders.
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU253.h"
#include "game/UnkTU341.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::math::CVector4D;

struct UnkStruct_71008651c8;
struct UnkStruct_71008651c8_Entry;
using UnkOut_71008651c8 = base::global::CRntVector<UnkStruct_71008651c8_Entry*>;
void CCollisionComponent_CollectRecordsBySecondaryId(UnkStruct_71008651c8* self, void* const* key, UnkOut_71008651c8* out);

// The suffix used when a "HIT" source names none, and the tints by hits left (1 to 3).
__attribute__((visibility("hidden"))) extern const char g_unk_710164b160[];
__attribute__((visibility("hidden"))) extern const CVector4D g_unk_710164b164[3];

// The collision component a hit comes from.
struct UnkHitComponent_l8e12dc {
    virtual const base::reflection::CClass* vf_0x0();
};

struct UnkHitSource_l8e12dc {
    u8 pad_0x0[0x10];
    UnkHitComponent_l8e12dc* collision;  // 0x10
    u8 pad_0x18[0x8];
    CStrId name;  // 0x20
};

struct UnkHitInfo_l8e12dc {
    u8 pad_0x0[0x60];
    UnkHitSource_l8e12dc* source;  // 0x60
    u8 pad_0x68[0xb4 - 0x68];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0xf8 - 0xb8];
    f32 factor;  // 0xf8
};

// A collider as the broadphase sees it (fn_7100d35a98 sets the flag).
struct UnkItemLifeCollider_l8e12dc {
    u8 pad_0x0[0x6a];
    bool enabled;  // 0x6a
};

struct UnkItemLifeOwner_l8e12dc {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
    u8 pad_0x1b6[0x2a0 - 0x1b6];
    UnkCollisionContainer* colliders;  // 0x2a0
};

struct UnkItemLifeDamage_l8e12dc {
    u8 pad_0x0[0xa8];
    u8 factors[0x8];  // 0xa8
};

struct UnkItemLifeTint_l8e12dc {
    u8 pad_0x0[0x240];
    CVector4D color;  // 0x240
};

struct UnkItemLifePart_l8e12dc {
    UnkStringData* name;
};

struct UnkItemLifeModelData_l8e12dc {
    u8 pad_0x0[0x138];
    UnkPowerBombPartList_ovb0127* parts;  // 0x138
};

struct UnkItemLifeModel_l8e12dc {
    u8 pad_0x0[0x50];
    UnkItemLifeModelData_l8e12dc* data;  // 0x50
};

struct UnkItemLife_l8e12dc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0(s32 reason, UnkHitInfo_l8e12dc* hit);  // the item dies
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual bool vf_0x1d8(UnkHitInfo_l8e12dc* hit);  // whether the hit can harm it
    virtual bool vf_0x1e0(UnkHitInfo_l8e12dc* hit);  // takes the hit
    virtual void vf_0x1e8(UnkHitInfo_l8e12dc* hit);  // lets the hit pass
    u8 pad_0x8[0x28];
    UnkItemLifeOwner_l8e12dc* owner;  // 0x30
    u8 pad_0x38[0x54 - 0x38];
    f32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    bool field_0x64;
    u8 pad_0x65[0x70 - 0x65];
    s32 field_0x70;
    u8 pad_0x74[0x310 - 0x74];
    UnkItemLifeDamage_l8e12dc* damage;  // 0x310
    u8 pad_0x318[0x340 - 0x318];
    s32 hits_left;                  // 0x340
    UnkItemLifeTint_l8e12dc* tint;  // 0x348
    CVector4D tint_from;            // 0x350
    CVector4D tint_current;         // 0x360
    CVector4D tint_to;              // 0x370
    f32 tint_speed;                 // 0x380
    f32 tint_time;                  // 0x384
};
static_assert(__builtin_offsetof(UnkItemLife_l8e12dc, hits_left) == 0x340, "UnkItemLife_l8e12dc layout");
static_assert(__builtin_offsetof(UnkItemLife_l8e12dc, tint_time) == 0x384, "UnkItemLife_l8e12dc layout");

// A string id built from a string's text (fn_7100000850).
struct UnkTextStrId_l8e12dc {
    UnkStringData* data;

    explicit UnkTextStrId_l8e12dc(const CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_l8e12dc() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    const char* Text() const {
        ++data->reads;
        return data->text ? data->text : g_unk_710162e2ed;
    }
};

// The id's text; reading it counts a use of the string.
static inline const CRntString& NameString(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// Whether the name contains `str` (an empty name doesn't); reading it counts a use of the string.
static inline bool NameHas(UnkStringData* const& name, const char* str) {
    ++name->reads;
    if (name == &g_unk_7101d47108->empty)
        return false;
    return CRntString_Contains(&name->String(), str);
}

// The id of a husk collider's suffix (from its last '_' on), upper-cased.
static inline UnkTextStrId_l8e12dc HuskSuffixId(const CStrId& name) {
    const CRntString& text = NameString(name);
    u32 start = CRntString_FindLastChar(&NameString(name), '_', -1);
    u32 length = text.length - start;
    UnkTunableLine suffix(text.data + start, length);
    char upper[0x100];
    upper[0] = '\0';
    const char* str = suffix.Data();
    u32 n = __builtin_strlen(str);
    if (n) {
        for (u32 i = 0; i < n; ++i) {
            char c = str[i];
            upper[i] = c >= 'a' && c <= 'z' ? c - 0x20 : c;
        }
        upper[n] = '\0';
    }
    return UnkTextStrId_l8e12dc(*UnkTunableLine(upper).Str());
}

// Whether a "HIT" hit counts: when the collision component has no "HUSK" collider, or the one whose name has the
// suffix (g_unk_710164b160 when the source names none) is disabled.
static inline bool HuskAllows(UnkOut_71008651c8& husks, UnkTunableLine& suffix) {
    if (husks.count == 0)
        return true;
    if (suffix.IsEmpty())
        CRntString_AssignCString(suffix.Str(), g_unk_710164b160);
    for (u64 i = 0; i < husks.count; ++i) {
        UnkCollisionObject* object = reinterpret_cast<UnkCollisionRecord*>(husks.data[i])->field_0x28;
        auto* record = reinterpret_cast<UnkCollisionRecord*>(husks.GrowAt(i));
        if (NameHas(record->field_0x0.data, suffix.Data())) {
            if (!object || !reinterpret_cast<UnkItemLifeCollider_l8e12dc*>(object)->enabled)
                return true;
            return false;
        }
    }
    return false;
}

bool fn_71008e12dc(UnkItemLife_l8e12dc* self, UnkHitInfo_l8e12dc* hit) {
    if (self->field_0x64 || self->field_0x70 == 2 || self->owner->field_0x1b5 || !fn_710024fbc8(self))
        return false;
    UnkHitComponent_l8e12dc* collision = hit->source->collision;
    if (!collision)
        return false;
    if (!fn_710009d50c(collision->vf_0x0(), fn_710085fc44()))
        return false;
    CStrId tag = *fn_7100864018(reinterpret_cast<UnkCollisionContainer*>(collision), hit->source);

    bool counted;
    if (tag == CStrId("HUSK", true)) {
        // The husk collider goes off and the model parts named for it are hidden.
        CStrId name = hit->source->name;
        if (UnkCollisionObject* collider = fn_71008640a4(self->owner->colliders, &name))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), false);
        UnkTextStrId_l8e12dc suffix = HuskSuffixId(name);
        auto* model = reinterpret_cast<UnkItemLifeModel_l8e12dc*>(
            fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(self->owner)));
        if (model) {
            if (UnkItemLifeModelData_l8e12dc* data = model->data) {
                for (u32 i = 0; i < fn_71001c1440(data->parts); ++i) {
                    auto* part = reinterpret_cast<UnkItemLifePart_l8e12dc*>(fn_71001c1450(data->parts, i));
                    ++part->name->reads;
                    if (part->name == &g_unk_7101d47108->empty)
                        continue;
                    if (CRntString_Contains(&part->name->String(), suffix.Text())) {
                        u64 key = part->name->Key();
                        fn_7101172780(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
                    }
                }
            }
        }
        counted = false;
    } else if (tag == CStrId("HIT", true) && self->vf_0x1d8(hit)) {
        // The source's name ends in "hit<suffix>": the hit counts unless it lands on an enabled "HUSK" collider
        // whose name has that suffix.
        UnkTunableLine name(NameString(hit->source->name));
        u32 start = CRntString_Find(name.Str(), "hit", 0) + 3;
        u32 length = name.Length() - start;
        UnkTunableLine suffix(name.Data() + start, length);
        UnkOut_71008651c8 husks(0xf);
        {
            CStrId husk("HUSK", true);
            CCollisionComponent_CollectRecordsBySecondaryId(reinterpret_cast<UnkStruct_71008651c8*>(collision), reinterpret_cast<void* const*>(&husk),
                          &husks);
        }
        counted = HuskAllows(husks, suffix);
        husks.Free();
    } else {
        f32 factor = fn_71008c2de8(reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(self->damage->factors), hit->flags);
        fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), factor * hit->factor);
        if (hit->flags & 0x20)
            fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), self->field_0x54);
        if (self->vf_0x1d8(hit)) {
            counted = self->vf_0x1e0(hit);
        } else {
            self->vf_0x1e8(hit);
            counted = false;
        }
    }
    if (!counted)
        return false;

    s32 hits = --self->hits_left;
    if (hits <= 0) {
        self->vf_0x1c0(0, hit);
    } else if (UnkItemLifeTint_l8e12dc* tint = self->tint) {
        // The tint for the hits left: the first for one, the last for three or more.
        s32 stage = hits - 1;
        stage = stage < 2 ? stage : 2;
        stage = stage > 0 ? stage : 0;
        u32 index = stage;
        CVector4D color = tint->color;
        fn_710007b518(&self->tint_to, &color);
        fn_710007b518(&self->tint_current, &self->tint_to);
        fn_710007b518(&self->tint_from, &self->tint_current);
        CVector4D target = g_unk_710164b164[index];
        fn_710007b518(&self->tint_current, &self->tint_from);
        fn_710007b518(&self->tint_to, &target);
        self->tint_speed = 0.5f;
        self->tint_time = 0.0f;
    }
    return true;
}
