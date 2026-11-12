// Unit 2611's functions (tools/tu_map.py; no class hint, its last function reads "SubareaSetupID"): a
// record of a sub-area setup (the sub-area names, the six kinds' string ids, a vector of string ids at
// +0x150) with its copy/move constructor, the kind walkers, the flag setter, the reset that rebuilds
// the id vector, and the unit's static initialiser. Names and layouts are placeholders.
#include <new>

#include "../../game_s7/UnkStaticInitS7.h"
#include "base/UnkCloneRecords.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CHeatRoomComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2611.h"
#include "types.h"

using namespace unk_static_init;
using base::global::CStrId;
using base::global::UnkStringData;

void fn_7100fab0dc(char* p0);
void fn_7100fa4eb8(void* dst, void* src);
void fn_7100fa64bc(void* self);
void fn_7100fa5c40(void* self, void* arg);

struct UnkIdNine_ovb0259 {
    UnkStringData* ids[9];
};

static inline void CopyId(UnkStringData*& dst, UnkStringData* src) {
    dst = src;
    if (&g_unk_7101d47108->empty != src)
        ++src->ref_count;
}

void fn_7100fab330(UnkIdNine_ovb0259* self, UnkIdNine_ovb0259* other) {
    for (int i = 0; i < 9; ++i)
        fn_7100fab0dc(reinterpret_cast<char*>(self) + i * 8);
    CopyId(self->ids[0], other->ids[0]);
    CopyId(self->ids[1], other->ids[1]);
    CopyId(self->ids[2], other->ids[2]);
    CopyId(self->ids[3], other->ids[3]);
    CopyId(self->ids[4], other->ids[4]);
    CopyId(self->ids[5], other->ids[5]);
    CopyId(self->ids[6], other->ids[6]);
    CopyId(self->ids[7], other->ids[7]);
    CopyId(self->ids[8], other->ids[8]);
}

struct UnkGeneratorRecord_ovb0120;

struct UnkRecord_ovb0259 {
    UnkStringData* id;  // 0x0
    void* field_0x8;
    void* data;  // 0x10
    u32 count;   // 0x18
    u32 capacity;
    u32 step;
    u8 field_0x24;
    u32 flags;       // 0x28
    u64 allocator;   // 0x30
    u8 sub[0xd0];    // 0x38
    UnkIdNine_ovb0259 ids;  // 0x108
    base::UnkStrIdVec kinds;  // 0x150
    u32 field_0x178;
    u32 field_0x17c;
    u16 field_0x180;
};
static_assert(__builtin_offsetof(UnkRecord_ovb0259, kinds) == 0x150, "UnkRecord_ovb0259 layout");
static_assert(__builtin_offsetof(UnkRecord_ovb0259, field_0x178) == 0x178, "UnkRecord_ovb0259 layout");

void fn_7100fab18c(UnkGeneratorRecord_ovb0120* self_, UnkGeneratorRecord_ovb0120* other_) {
    UnkRecord_ovb0259* self = reinterpret_cast<UnkRecord_ovb0259*>(self_);
    UnkRecord_ovb0259* other = reinterpret_cast<UnkRecord_ovb0259*>(other_);
    self->id = other->id;
    if (&g_unk_7101d47108->empty != self->id)
        ++self->id->ref_count;
    self->field_0x8 = other->field_0x8;
    self->data = other->data;
    self->count = other->count;
    self->capacity = other->capacity;
    self->step = other->step;
    self->field_0x24 = other->field_0x24;
    self->flags = other->flags;
    self->allocator = other->allocator;
    other->data = nullptr;
    other->count = 0;
    other->capacity = 0;
    fn_7100fa4eb8(self->sub, other->sub);
    fn_7100fab330(&self->ids, &other->ids);
    new (&self->kinds) base::UnkStrIdVec(other->kinds);
    self->field_0x178 = other->field_0x178;
    self->field_0x17c = other->field_0x17c;
    self->field_0x180 = 0;
}

struct UnkMask {
    u32 bits;
    UnkMask() : bits(0) {}
    UnkMask(u32 b) : bits(b) {}
    UnkMask(const UnkMask& o) : bits(o.bits) {}
};
struct UnkKindFlags_ovb0259 {
    u8 pad_0x0[0x178];
    u32 flags;  // 0x178
};

struct UnkSubArea;

void fn_7100facf04(UnkSubArea* area, UnkMask mask, bool set) {
    UnkKindFlags_ovb0259* self = reinterpret_cast<UnkKindFlags_ovb0259*>(area);
    u32* flags = &self->flags;
    const u32* mp = &mask.bits;
#define STEP_SET(b) \
    if (*mp & (1u << b)) { u32 f = *flags; if (!(f & (1u << b))) *flags = f | (1u << b); }
#define STEP_CLR(b) \
    if (*mp & (1u << b)) { u32 f = *flags; if (f & (1u << b)) *flags = f & ~(1u << b); }
    if (set) {
        STEP_SET(0) STEP_SET(1) STEP_SET(2) STEP_SET(3) STEP_SET(4) STEP_SET(5) STEP_SET(6) STEP_SET(7) STEP_SET(8)
    } else {
        STEP_CLR(0) STEP_CLR(1) STEP_CLR(2) STEP_CLR(3) STEP_CLR(4) STEP_CLR(5) STEP_CLR(6) STEP_CLR(7) STEP_CLR(8)
    }
#undef STEP_SET
#undef STEP_CLR
}

struct UnkSubareaSetupState;
void fn_7100fac2d4(UnkSubareaSetupState* self, u32 index, bool enable, bool force, UnkSubareaSetupState* other,
                   bool arg5);

void fn_7100fac120(UnkSubArea* area, UnkMask mask, bool a, bool b, void* other, bool c) {
    UnkSubareaSetupState* self = reinterpret_cast<UnkSubareaSetupState*>(area);
    UnkSubareaSetupState* from = static_cast<UnkSubareaSetupState*>(other);
#define STEP(i)     if (mask.bits & (1u << i)) fn_7100fac2d4(self, i, a, b, from, c);
    STEP(0) STEP(1) STEP(2) STEP(3) STEP(4) STEP(5) STEP(6) STEP(7) STEP(8)
#undef STEP
}

// The definition the setup chain is read under (src/game/UnkSubareaSetupState.cpp names it the same).
struct UnkSetupNamedDef : UnkPropertyEx<0x160> {};

struct UnkNamePart_ovb0259 {
    u8 pad_0x0[0x44];
    union {
        s32 counter;  // 0x44: reloaded after the increment
    };
};

struct UnkNameSet_ovb0259 {
    UnkStringData* id;  // 0x0
    u8 pad_0x8[0x148];
    UnkNamePart_ovb0259** parts;  // 0x150
    u32 count;                    // 0x158
};

struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);
void fn_710007f564(void* self, const char* str);
CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

void fn_7100fac824(UnkNameSet_ovb0259* self, CBlackboard* blackboard) {
    UnkTypedValue v;
    base::UnkNamedEntry entry;
    fn_710007e704(&entry, g_unk_710162e2ed);
    base::global::CRntString* text = reinterpret_cast<base::global::CRntString*>(&entry);
    for (UnkNamePart_ovb0259 **p = self->parts, **end = p + self->count; p != end; ++p) {
        if (text->length != 0)
            fn_710007f564(text, ">");
        ++(*p)->counter;
        fn_710007f3b4(text, reinterpret_cast<const base::global::CRntString*>(reinterpret_cast<char*>(*p) + 0x10));
    }
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0x608) {
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        UnkQualifiedName* qualified = reinterpret_cast<UnkQualifiedName*>(g_unk_71729ee018.field_0x10);
        if (!qualified->str || !qualified->str[0])
            fn_7100ae9d7c(qualified, reinterpret_cast<UnkIdSource*>(&g_unk_71729ee018), g_unk_7101d470e0);
        UnkSubId_7100fc4b34 key = fn_71003327a4(qualified, reinterpret_cast<const CStrId*>(self));
        if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
            if (!blackboard->field_0xe8) {
                v = {fn_7100007f84(), text};
                fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&key.id), &v);
            }
        }
    }
    fn_710007ecd4(text);
}

struct UnkRecordKinds_ovb0259 {
    u8 pad_0x0[0x8];
    void* field_0x8;
    u8 pad_0x10[0x140];
    CStrId* data;  // 0x150
    u32 count;     // 0x158
    u32 capacity;
    u32 step;
};

bool fn_7100fab880(UnkNameSet_ovb0259* self_, bool flag) {
    UnkRecordKinds_ovb0259* self = reinterpret_cast<UnkRecordKinds_ovb0259*>(self_);
    if (!self->field_0x8)
        return false;
    base::UnkStrIdVec* kinds = reinterpret_cast<base::UnkStrIdVec*>(&self->data);
    for (u32 i = 0; i < self->count; ++i)
        self->data[i].Reset();
    self->count = 0;
    const CStrId& key = GameManagerIdTable().field_0xd80;
    u32 n = self->count;
    u32 c = self->capacity;
    if (c <= n && self->step != 0)
        c = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    kinds->Reserve(c);
    CStrId* slot = &self->data[self->count];
    UnkStringData* d = key.data;
    __builtin_memcpy(slot, &d, 8);
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
    ++self->count;
    void* sub = reinterpret_cast<char*>(self) + 0x38;
    fn_7100fa64bc(sub);
    fn_7100fa5c40(sub, self->field_0x8);
    if (flag)
        fn_7100fac824(self_, &g_unk_7101d47310->field_0x968);
    return true;
}

extern HeaderIds g_unk_71729ee1a0 __attribute__((visibility("hidden")));
UnkEnumType* fn_710009ffd4();

void fn_7100fad9d0() {
    InitHeaderIds(&g_unk_71729ee1a0);
    InitPropertyEx(&g_unk_71729ee018, g_unk_7101a889c8, g_unk_71726db788, g_unk_7101a8d830, fn_7100ae9cc8,
                   "SubareaSetupID", fn_710009ffd4(), fn_71004fd2cc, fn_7100a24a50);
}
