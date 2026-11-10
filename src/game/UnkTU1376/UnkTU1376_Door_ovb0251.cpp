// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): door functions that match.
// fn_71008cde78 sets the door's flag and, when it is set, runs fn_71008cdf84 on every CDoorLifeComponent in the
// game manager's list for the id g_unk_7172731758; fn_71008cdf84 refreshes the door's cue (fn_71008d1944), its
// light parts and the scenario blackboard; fn_71008cec30 builds four groups of three arrays; fn_71008d5c1c
// whether the player is outside the door's range; fn_71008d1c50 drops the light handles that point at an
// object; fn_71008d2690 plays the cue of the door's state; fn_71008d564c opens a door that is closing.
// Names and layouts are placeholders (see src/game/CDoorLifeComponent.cpp for the unit's other functions).
#include <math.h>
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkSlots.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// Shared helpers: the counted handle a door holds to its light sources (object at +0x10), the
// named values and the section setter of src/game/CDoorLifeComponent.cpp, and the state machine
// at +0x420 (as there).
struct UnkDoorCounted_b0251 : base::UnkRefCounted {
    union {
        void* object;
        u64 raw_object;
    };
};

struct UnkRef {
    UnkDoorCounted_b0251* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

struct UnkDoorHandle_b0251 {
    union {
        UnkDoorCounted_b0251* ptr;
    };
};

UnkRef fn_7100353be8(void* source);

// The handle and source are given as raw byte offsets of the door, so the address calculations stay
// separate from the member reads.
static inline void ResolveHandle_b0251(void* door, u32 state_off, u32 source_off, u32 handle_off) {
    s32* state = reinterpret_cast<s32*>(static_cast<u8*>(door) + state_off);
    if (*state == 1) {
        {
            UnkRef r = fn_7100353be8(static_cast<u8*>(door) + source_off);
            fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(static_cast<u8*>(door) + handle_off),
                          reinterpret_cast<const base::UnkRefHandle*>(&r));
        }
        *state = 2;
    }
}

struct UnkDoorNameValue_b0251 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

static inline void SetSectionBool_b0251(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                        bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

struct UnkDoorOwner_b0251 : UnkWeakTarget {
    u8 pad_0x8[0x1ac];
    u8 field_0x1b4;
};

struct UnkDoorState_b0251 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);
    virtual void vf_0x18(s32 next);
    u64 field_0x8;
    s32 id;
};

// The state machine at +0x420 of a door (as in src/game/CDoorLifeComponent.cpp).
typedef UnkDoorState_b0251* __attribute__((aligned(4))) UnkDoorStatePtr_b0251;

struct UnkDoorMachine_b0251 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkDoorState_b0251* previous, UnkDoorState_b0251* next);
    virtual void vf_0x28(UnkDoorState_b0251* current, UnkDoorState_b0251* next);

    u64 field_0x8;
    UnkDoorState_b0251* current;
    UnkDoorState_b0251* previous;
    u32 count;
    UnkDoorStatePtr_b0251 states[16];
    s32 pending;
    bool field_0xa8;
};

struct UnkDoorLock;
// The door itself, for the callers that only pass it on.
struct UnkDoor_b0251;
void fn_71008d1944(UnkDoor_b0251* self, bool on);

using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkDoorLock;
struct UnkDoorFlag_b0251 {
    u8 pad_0x0[0x2c];
    bool field_0x2c;
};

bool fn_7100de9460(char* self, const base::UnkStrIdMember* key);
void fn_71008cdf84(UnkDoorLock* door);
extern base::UnkStrIdMember g_unk_7172731758 __attribute__((visibility("hidden")));
extern CType g_unk_7172731af8 __attribute__((visibility("hidden")));

struct UnkDoorLink_b0251 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;
};

void fn_71008cde78(UnkDoorFlag_b0251* self, bool on) {
    if (self->field_0x2c == on)
        return;
    self->field_0x2c = on;
    if (!on)
        return;
    if (!fn_7100de9460(reinterpret_cast<char*>(g_unk_7101d47310), &g_unk_7172731758))
        return;
    UnkGameManagerIdList* list = fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172731758));
    for (u32 i = 0; i < list->count; ++i) {
        auto* h = reinterpret_cast<UnkDoorLink_b0251**>(list->data)[i];
        if (!h || !h->object)
            continue;
        UnkReflObject* obj = h->object;
        const CType* type = obj->vf_0x0();
        fn_71008cb078();
        if (fn_710009d50c(type, &g_unk_7172731af8))
            fn_71008cdf84(reinterpret_cast<UnkDoorLock*>(obj));
    }
}

struct UnkDoorVecs_b0251 {
    base::UnkVector<u8> a;
    base::UnkVector<u8> b;
    base::UnkVector<u8> c;
    bool flag;
    UnkDoorVecs_b0251() : a(15), b(15), c(15), flag(false) {}
};

struct UnkDoorVecSet_b0251 {
    UnkDoorVecs_b0251 groups[4];
};

__attribute__((flatten)) void fn_71008cec30(UnkDoorVecSet_b0251* self) {
    new (self) UnkDoorVecSet_b0251;
}

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* name);
void fn_7100fbc4e0(lua_State* L, void* arg);
s32 fn_71010c0d80(lua_State* L, s32 count, char* out, u32 out_size, void* a, void* b);
struct UnkBreakablePart;
struct UnkBreakableTarget;
UnkBreakableTarget** fn_71002a76a4(UnkBreakablePart* part);
void* fn_7100751520(void* key);
void fn_710076351c(void* obj);
extern UnkProperty g_unk_71727310c0;


struct UnkDoorPart_b0251 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    u8 pad_0x8[0x38];
};

struct UnkDoorTarget_b0251 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkDoorOpen_b0251 {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0x620 - 0x38];
    s32 field_0x620;
    u8 pad_0x624[0x644 - 0x624];
    u32 field_0x644;
    u32 field_0x648;
    u32 field_0x64c;
    u32 field_0x650;
    u8 pad_0x654[0x670 - 0x654];
    UnkDoorNameValue_b0251* values;
    u64* keys;
    u32 count;
    u8 pad_0x684[0xbd8 - 0x684];
    UnkDoorPart_b0251* parts;
    u32 part_count;

    __attribute__((always_inline)) UnkDoorNameValue_b0251* Find(const UnkProperty& property) {
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

void fn_71008cdf84(UnkDoorLock* door) {
    UnkDoorOpen_b0251* self = reinterpret_cast<UnkDoorOpen_b0251*>(door);
    fn_71008d1944(reinterpret_cast<UnkDoor_b0251*>(self), true);
    for (UnkDoorPart_b0251 *p = self->parts, *e = p + self->part_count; p != e; ++p) {
        if (p->vf_0x20())
            continue;
        UnkDoorTarget_b0251** t = reinterpret_cast<UnkDoorTarget_b0251**>(fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(p)));
        UnkDoorTarget_b0251* target = *t;
        void* r = fn_7100751520(target ? target->field_0x10 : nullptr);
        if (r)
            fn_710076351c(r);
    }
    if (fn_71010c0bb0(g_unk_7172a01d18, "CurrentScenario.OnDoorOpened")) {
        fn_7100fbc4e0(g_unk_7172a01d18, self->owner);
        char out[0x100];
        fn_71010c0d80(g_unk_7172a01d18, 1, out, 0x100, nullptr, nullptr);
    }
    self->field_0x620 = 0;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    {
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        UnkDoorNameValue_b0251* name = self->Find(g_unk_71727310c0);
        bool value = true;
        SetSectionBool_b0251(blackboard, section_id, &name->name, &value);
    }
    if (self->field_0x644 & 0x08000400) {
        u32 v = self->field_0x64c;
        self->field_0x64c = 0;
        self->field_0x644 = v;
    }
    if (self->field_0x648 & 0x08000400) {
        u32 v = self->field_0x650;
        self->field_0x650 = 0;
        self->field_0x648 = v;
    }
}

struct UnkDoorLife;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkFlagged;
struct UnkShape;
UnkShape* fn_7100976588(UnkFlagged* flagged);

struct UnkDoorShape_b0251 {
    virtual void vf_0x0();
    u8 pad_0x8[0x40 - 0x8];
    f32 min_x, min_y, max_x, max_y;  // 0x40
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    void Update() {
        typedef void (*Fn)(UnkDoorShape_b0251*);
        (*reinterpret_cast<Fn**>(this))[0x98 / 8](this);
    }
};

struct UnkDoorPlayer_b0251 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x200 - 0x68];
    UnkFlagged* movement;
};

struct UnkDoorRange_b0251 {
    u8 pad_0x0[0x698];
    base::math::CVector3D field_0x698;
    f32 field_0x6a4;
    f32 field_0x6a8;
};

bool fn_71008d5c1c(UnkDoorLife* door) {
    UnkDoorRange_b0251* self = reinterpret_cast<UnkDoorRange_b0251*>(door);
    UnkDoorPlayer_b0251* player = reinterpret_cast<UnkDoorPlayer_b0251*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    base::math::CVector2D door_pos;
    fn_710007b430(&door_pos, &self->field_0x698);
    base::math::CVector2D player_pos;
    f32 px = player->x;
    f32 py = player->y;
    f32 pz = player->z;
    base::math::CVector3D p3 = {px, py, pz};
    fn_710007b430(&player_pos, &p3);
    f32 dy = fabsf(door_pos.y - player_pos.y);
    f32 dx = fabsf(door_pos.x - player_pos.x);
    f32 hx = self->field_0x6a4 * 0.5f;
    f32 hy = self->field_0x6a8 * 0.5f;
    if (player->movement) {
        if (UnkDoorShape_b0251* shape = reinterpret_cast<UnkDoorShape_b0251*>(fn_7100976588(player->movement))) {
            if (shape->dirty)
                shape->Update();
            hx += (shape->max_x - shape->min_x) * 0.5f + 1.0f;
            hy += (shape->max_y - shape->min_y) * 0.5f + 1.0f;
        }
    }
    bool over_x = dx > hx;
    bool over_y = dy > hy;
    return over_y | over_x;
}

struct UnkDoorHandles_b0251 {
    u8 pad_0x0[0x390];
    u8 source_a[0x28];
    s32 state_a;
    u8 pad_0x3bc[4];
    UnkDoorHandle_b0251 handle_a;
    u8 pad_0x3c8[8];
    u8 source_b[0x28];
    s32 state_b;
    u8 pad_0x3fc[4];
    UnkDoorHandle_b0251 handle_b;
    u8 pad_0x408[0x430 - 0x408];
    UnkDoorState_b0251* current;
    u8 pad_0x438[0xa14 - 0x438];
    u32 field_0xa14;
};

static inline void Drop_b0251(UnkDoorHandle_b0251* handle) {
    if (UnkDoorCounted_b0251* held = handle->ptr) {
        if (--held->ref_count == 0)
            delete held;
        handle->ptr = nullptr;
    }
}

bool fn_71008d1c50(UnkDoorHandles_b0251* self, void* target) {
    ResolveHandle_b0251(self, 0x3b8, 0x390, 0x3c0);
    bool a = false;
    {
        void* held = self->handle_a.ptr ? self->handle_a.ptr->object : nullptr;
        if (held == target) {
            Drop_b0251(&self->handle_a);
            a = true;
            self->state_a = 0;
        }
    }
    ResolveHandle_b0251(self, 0x3f8, 0x3d0, 0x400);
    void* held = self->handle_b.ptr ? self->handle_b.ptr->object : nullptr;
    if (held == target) {
        Drop_b0251(&self->handle_b);
        self->state_b = 0;
    } else if (!a) {
        return false;
    }
    UnkDoorState_b0251* current = self->current;
    fn_71008d1944(reinterpret_cast<UnkDoor_b0251*>(self), current && current->id == 1);
    self->field_0xa14 = 0;
    return false;
}

void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void* object, u64 id);
struct UnkDoorLock;
struct UnkDoorLife_b44a2;
void fn_71008d586c(UnkDoorLock* door, bool locked, u32 mode, CStrId* name);
void fn_71008d498c(UnkDoorLife_b44a2* self, u32 a, u32 b, u32 c);
bool fn_71008d0ef4(void*);

struct UnkDoorFlow_b0251 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_b0251* owner;
    u8 pad_0x38[0x40c - 0x38];
    s32 field_0x40c;
};

void fn_71008d2690(UnkDoorFlow_b0251* self) {
    switch (self->field_0x40c) {
    case 1:
        fn_71008d0ef4(self);
        if (void* actor = fn_71002b3b84(self->owner))
            fn_71009c0f1c(actor, 0x6909d27d2c787fba);
        break;
    case 2:
        fn_71008d498c(reinterpret_cast<UnkDoorLife_b44a2*>(self), 0, 0, 0);
        if (void* actor = fn_71002b3b84(self->owner))
            fn_71009c0f1c(actor, 0xe43bbd07b9e307b9);
        break;
    case 3: {
        {
            CStrId id = g_unk_7101d470e0;
            fn_71008d586c(reinterpret_cast<UnkDoorLock*>(self), false, 0, &id);
        }
        if (void* actor = fn_71002b3b84(self->owner))
            fn_71009c0f1c(actor, 0xe43bbd07b9e307b9);
        break;
    }
    }
}

using base::color::CColor4F;
using base::math::CVector4D;

struct UnkDoorLock;
struct UnkUnit2186Object;
UnkUnit2186Object* fn_71008d5d2c(void* owner);
struct UnkLightingComponent2;
void fn_71008e7e98(UnkLightingComponent2* self, const u64* id, void* arg, f32 value);
extern u64 g_unk_710164b098 __attribute__((visibility("hidden")));
struct UnkStruct_71727313a4 {
    CColor4F colours[3];
};
extern UnkStruct_71727313a4 g_unk_71727313a4;
void fn_71001c2ec0(void* target, u32 a, u32 b, CVector4D* colour, u32 c, f32 time);
bool fn_710002fb70(void* timer);
struct UnkEnumType;
extern UnkProperty g_unk_7172731160;
extern UnkEnumType g_unk_7172731200;

struct UnkDoorGfx_b0251 {
    u8 pad_0x0[0x138];
    void* slots;
};
struct UnkDoorHolder_b0251 {
    u8 pad_0x0[0x50];
    UnkDoorGfx_b0251* gfx;
};
struct UnkDoorOwner7_b0251 {
    u8 pad_0x0[0x228];
    UnkDoorHolder_b0251* holder;
};

struct UnkDoorNotify_b0251 {
    u8 pad_0x0[0x30];
    UnkDoorOwner7_b0251* owner;
    u8 pad_0x38[0x40c - 0x38];
    s32 field_0x40c;
    u8 pad_0x410[0x420 - 0x410];
    UnkDoorMachine_b0251 machine;
    u8 pad_0x4d0[0x624 - 0x4d0];
    f32 field_0x624;
    u8 pad_0x628[0x670 - 0x628];
    UnkDoorNameValue_b0251* values;
    u64* keys;
    u32 count;
    u8 pad_0x684[0x6ac - 0x684];
    u8 field_0x6ac;
    u8 pad_0x6ad[0x6e8 - 0x6ad];
    CColor4F field_0x6e8;
    CColor4F field_0x6f8;
    CColor4F field_0x708;
    u8 pad_0x718[0xa10 - 0x718];
    u32 field_0xa10;
    u8 pad_0xa14[4];
    UnkGuiKeyList field_0xa18;
};

bool fn_71008d564c(UnkDoorLock* door, u32 value, const CStrId* id) {
    UnkDoorNotify_b0251* self = reinterpret_cast<UnkDoorNotify_b0251*>(door);
    if (id->data != &g_unk_7101d47108->empty)
        fn_710002ffb4(&self->field_0xa18, const_cast<CStrId*>(id));
    if (fn_710002fb70(&self->field_0xa18))
        return false;
    self->field_0x6ac = 0;
    if (!self->machine.current || self->machine.current->id != 3)
        return false;
    self->field_0x624 = 0.6f;
    UnkDoorHolder_b0251* holder = self->owner->holder;
    if (holder && holder->gfx) {
        CColor4F colour(1.0f, 1.0f, 1.0f, 1.0f);
        if (self->field_0xa10 != 0xffffffff)
            fn_71001c2ec0(holder->gfx->slots, self->field_0xa10, 0, reinterpret_cast<CVector4D*>(&colour), 1, 0.21000001f);
        fn_7100030728(&self->field_0x6e8, &g_unk_71727313a4.colours[0]);
        fn_7100030728(&self->field_0x6f8, &g_unk_71727313a4.colours[1]);
        fn_7100030728(&self->field_0x708, &g_unk_71727313a4.colours[2]);
        if (UnkLightingComponent2* light = reinterpret_cast<UnkLightingComponent2*>(fn_71008d5d2c(self->owner))) {
            base::math::UnkVector4D v = fn_7100030878(&colour);
            fn_71008e7e98(light, &g_unk_710164b098, &v, 0.0f);
        }
    }
    if (value & 1) {
        UnkGameManager* manager = g_unk_7101d47310;
        self->field_0x40c = 0x7fffffff;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 i = 0;
        u32 n = self->count;
        if (n) {
            u64* k = self->keys;
            u64 key = (u64)g_unk_7172731160.name.data;
            for (; i != n; ++i) {
                if (k[i] == key)
                    break;
            }
        }
        UnkDoorNameValue_b0251* name = &self->values[i];
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!blackboard->field_0xe8) {
                UnkTypedValue tv;
                tv.type = reinterpret_cast<const base::reflection::CType*>(&g_unk_7172731200);
                tv.value = &self->field_0x40c;
                fn_7100332c80(s, &name->name, &tv);
            }
        }
    }
    return true;
}
