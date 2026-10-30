// From unit 2611 of tools/tu_map.py (0x7100fa99bc-0x7100fadb10, no reflected class; its last
// function reads "SubareaSetupID"): a sub-area's setup state. A sub-area names up to six kinds of
// things by string id, and each kind goes to its own system: kinds 0, 1, 2 and 4 to the game
// manager's object at +0x608, kind 3 to a render layer (the id's hash) and kind 5 to the object at
// +0x1af8. The state is probably src/game/CSubAreaManager.cpp's UnkSubArea (its name at +0x0, the
// sub-area manager's current and previous ones are of this type, and fn_7100fac120 there walks
// the kinds through fn_7100fac2d4). Names and layouts are placeholders.
// Holds fn_7100fac2d4 (switches one kind on or off), fn_7100facbe8 (hands one kind over between
// two setups' ids) and fn_7100faba64 (applies the chain of setups the blackboard names).
#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A setup: the ids of the six kinds.
struct UnkSubareaSetup {
    u8 pad_0x0[0x30];
    CStrId ids[6];
};

struct UnkSubareaSetupState {
    CStrId name;  // what its blackboard entry is read under
    u8 pad_0x8[0x60];
    CStrId ids[6];      // +0x68: its own id of each kind
    u8 pad_0x98[0x70];
    CStrId current[6];  // +0x108: the id each kind was last switched to
    u8 pad_0x138[0x40];
    u32 field_0x178;    // kinds that are only switched when forced
    u32 field_0x17c;    // kinds that are switched on
};
static_assert(__builtin_offsetof(UnkSubareaSetupState, field_0x17c) == 0x17c, "UnkSubareaSetupState placeholder layout");

// The id's hash (the CRC-64 its pool entry keeps at +0x30).
static inline u64 IdHash(const CStrId& id) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id.data) + 0x30);
}

// The pool's empty entry formed from the pool's address as bytes (fn_7100fac2d4 compares an id
// with it this way round).
static inline const base::global::UnkStringData* EmptyData() {
    return reinterpret_cast<const base::global::UnkStringData*>(reinterpret_cast<const char*>(g_unk_7101d47108) + 0x10);
}

void fn_71011a2fd0(UnkGameManagerSub_0x608* self, const CStrId& id, bool arg2);
void fn_71011a3714(UnkGameManagerSub_0x608* self, u32 kind, const CStrId& from, const CStrId& to, bool arg4);
void fn_71011a3a5c(UnkGameManagerSub_0x608* self, u32 kind, const CStrId& from, const CStrId& to);
void fn_7100219c8c(UnkRenderManager* self, bool enable, u64 from, u64 to, bool arg4);
void fn_7100ca7740(UnkGameManagerSub_0x1af8* self, const CStrId& from, const CStrId& to, bool enable);
void fn_71011a32d8(UnkGameManagerSub_0x608* self, u32 kind, const CStrId& id, bool arg3);
void fn_71011a3574(UnkGameManagerSub_0x608* self, u32 kind, const CStrId& id);
void fn_71011a4944(UnkGameManagerSub_0x608* self);
void fn_71011a4954(UnkGameManagerSub_0x608* self);
void fn_7100219c6c(UnkRenderManager* self, bool enable, u64 layer, u32 arg3, bool arg4);
void fn_7100ca76a0(UnkGameManagerSub_0x1af8* self, const CStrId& id, bool enable);

// The sub-area manager singleton (created on first use; 0xae8 bytes, constructor fn_7100ee1f34):
// only its current and previous sub-areas are read here.
struct UnkSubAreaManagerView {
    u8 pad_0x0[0x3f0];
    UnkSubareaSetupState* current;
    UnkSubareaSetupState* previous;
};
void fn_7100ee1f34(void* self);
extern void* g_unk_7101d47360;
static inline UnkSubAreaManagerView* SubAreaManager() {
    void* instance = g_unk_7101d47360;
    if (!instance) {
        instance = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(instance);
        g_unk_7101d47360 = instance;
    }
    return static_cast<UnkSubAreaManagerView*>(instance);
}

// The kind's current id becomes `id` (the source is picked before the destination is formed).
static inline void SetCurrent(UnkSubareaSetupState* self, u32 index, const CStrId* id) {
    self->current[index] = *id;
}

// Marks the kind on or off. The empty-id path of kind 0 has its own copy, so that its load of the
// mask is the one made at the start (the original's tail is shared).
static inline void SetActive(UnkSubareaSetupState* self, u32 bit, bool enable) {
    self->field_0x17c = !enable ? self->field_0x17c & ~bit : self->field_0x17c | bit;
}

// Switches kind `index` of this state on (`enable`) or off, unless it already is or (without
// `force`) is locked: from the kind's id in `other` (when there is one) to this state's own, and
// back. Kind 1 also does what kind 3 does (the original's case falls through). `arg5` goes to the
// calls for kind 4. Kinds past 5 are left alone.
// NON_MATCHING(minor): kind 4 tests the swap flag with cbz where the original has tbz #0 (the flag
// is 0 or 1); in the inlined CStrId assignments the source is selected into x8 instead of x10, the
// destination's and the source's data pointers are loaded in the other order (no store between
// them), the source's is compared with the empty entry with the operands swapped (an equality
// test), and the tail kinds 2 and 3 share starts at the select instead of the address add.
void fn_7100fac2d4(UnkSubareaSetupState* self, u32 index, bool enable, bool force,
                   UnkSubareaSetupState* other, bool arg5) {
    if (!force && (self->field_0x178 & (1 << index)))
        return;
    u32 bit = 1 << index;
    if (!(((self->field_0x17c & bit) != 0) ^ enable))
        return;
    CStrId& mine = self->ids[index];
    const CStrId& theirs = other ? other->current[index] : g_unk_7101d470e0;
    u64 mine_hash = IdHash(mine);
    u64 their_hash = IdHash(theirs);
    switch (index) {
    case 0:
        if (mine.data != EmptyData()) {
            fn_71011a2fd0(g_unk_7101d47310->field_0x608, mine, true);
            break;
        }
        SetActive(self, bit, enable);
        return;
    case 1: {
        UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608;
        if (other) {
            if (enable)
                fn_71011a3714(p, 2, theirs, mine, false);
            else
                fn_71011a3a5c(p, 2, theirs, mine);
        } else {
            if (enable)
                fn_71011a32d8(p, 2, mine, false);
            else
                fn_71011a3574(p, 2, mine);
        }
        SetCurrent(self, index, enable ? &mine : &g_unk_7101d470e0);
    }
        // fall through
    case 3:
        if (other)
            fn_7100219c8c(g_unk_7101d47300, enable, their_hash, mine_hash, false);
        else
            fn_7100219c6c(g_unk_7101d47300, enable, mine_hash, 0, false);
        SetCurrent(self, index, enable ? &mine : &g_unk_7101d470e0);
        break;
    case 2: {
        UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608;
        if (other) {
            if (enable)
                fn_71011a3714(p, 1, theirs, mine, false);
            else
                fn_71011a3a5c(p, 1, theirs, mine);
        } else {
            if (enable)
                fn_71011a32d8(p, 1, mine, false);
            else
                fn_71011a3574(p, 1, mine);
        }
        SetCurrent(self, index, enable ? &mine : &g_unk_7101d470e0);
        break;
    }
    case 4: {
        UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608;
        if (other) {
            UnkSubAreaManagerView* manager = SubAreaManager();
            bool swap = manager->previous == manager->current && manager->current == self;
            if (enable) {
                if (swap)
                    fn_71011a3714(p, 0, mine, theirs, arg5);
                else
                    fn_71011a3714(p, 0, theirs, mine, arg5);
            } else {
                if (swap)
                    fn_71011a3a5c(p, 0, mine, theirs);
                else
                    fn_71011a3a5c(p, 0, theirs, mine);
            }
        } else {
            if (enable)
                fn_71011a32d8(p, 0, mine, arg5);
            else
                fn_71011a3574(p, 0, mine);
        }
        SetCurrent(self, index, enable ? &mine : &g_unk_7101d470e0);
        fn_71011a4944(g_unk_7101d47310->field_0x608);
        break;
    }
    case 5:
        if (other)
            fn_7100ca7740(g_unk_7101d47310->field_0x1af8, theirs, mine, enable);
        else
            fn_7100ca76a0(g_unk_7101d47310->field_0x1af8, mine, enable);
        SetCurrent(self, index, enable ? &mine : &g_unk_7101d470e0);
        if (UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608)
            fn_71011a4954(p);
        break;
    default:
        return;
    }
    SetActive(self, bit, enable);
}

// Hands kind `index` over between the ids `from` and `to` give it, when `to` has one and their
// hashes differ: with `enable` from `from` to `to`, without in the other direction. Without
// `enable`, `to`'s id (when it has one) also becomes the kind's current id. Whether `to` has an id
// is read before the copy is made (that is what leaves the original's repeated test).
void fn_7100facbe8(UnkSubareaSetupState* self, u32 index, const UnkSubareaSetup* from,
                   const UnkSubareaSetup* to, bool enable) {
    CStrId a = from->ids[index];
    u64 from_hash = IdHash(a);
    bool to_set = &g_unk_7101d47108->empty != to->ids[index].data;
    CStrId b = to->ids[index];
    u64 to_hash = IdHash(b);
    if (to_set && from_hash != to_hash) {
        switch (index) {
        case 0:
            if (UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608)
                if (enable)
                    fn_71011a2fd0(p, b, true);
            break;
        case 1:
            if (UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608) {
                if (enable)
                    fn_71011a3714(p, 2, a, b, false);
                else
                    fn_71011a3a5c(p, 2, b, a);
            }
            break;
        case 2:
            if (UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608) {
                if (enable)
                    fn_71011a3714(p, 1, a, b, false);
                else
                    fn_71011a3a5c(p, 1, b, a);
            }
            break;
        case 3:
            if (enable)
                fn_7100219c8c(g_unk_7101d47300, true, from_hash, to_hash, false);
            else
                fn_7100219c8c(g_unk_7101d47300, false, to_hash, from_hash, false);
            break;
        case 4:
            if (UnkGameManagerSub_0x608* p = g_unk_7101d47310->field_0x608) {
                if (enable)
                    fn_71011a3714(p, 0, a, b, false);
                else
                    fn_71011a3a5c(p, 0, b, a);
            }
            break;
        case 5:
            if (enable)
                fn_7100ca7740(g_unk_7101d47310->field_0x1af8, a, b, true);
            else
                fn_7100ca7740(g_unk_7101d47310->field_0x1af8, b, a, false);
            break;
        }
    }
    if (!enable && b.data != &g_unk_7101d47108->empty)
        self->current[index] = b;
}

// The id table (fn_7100031894 builds it; the same definition as src/game/CGameManager.cpp's and
// src/game/CSubAreaManager.cpp's). Its +0xd80 is the id of the default setup chain.
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// A string id as the CRntString constructor from one sees it (src/base/global/CRntString.cpp).
struct UnkPooledStrId;
void fn_710007eb20(CRntString* self, const UnkPooledStrId* id);

// A string built from a string id out of line (fn_710007eb20) and destroyed as one.
struct UnkSetupIdText {
    union {
        CRntString str;
    };

    explicit UnkSetupIdText(const void* id) { fn_710007eb20(&str, static_cast<const UnkPooledStrId*>(id)); }
    ~UnkSetupIdText() { fn_710007ecd4(&str); }
};

// A definition whose qualified id is kept as a string, with named sub-ids looked up in a map
// (the same definitions as src/game_s7/fn_7100fc4b34.cpp's).
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);
struct UnkSetupNamedDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
static inline UnkSubId_7100fc4b34 SubId(UnkSetupNamedDef& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}
extern UnkSetupNamedDef g_unk_71729ee018;  // the definition the setup chain is read under

// A string (header of 0x30 bytes) with 0x100 bytes of inline storage (as src/game/CGameManager.cpp's).
struct UnkSetupStrStorage {
    void* vtable;
    u8 pad_0x8[0x100];
    void* field_0x108;
    u32 field_0x110;
};
struct UnkSetupStr {
    u8 header[0x30];
    UnkSetupStrStorage storage;

    CRntString* Str() { return reinterpret_cast<CRntString*>(this); }

    UnkSetupStr() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage));
        storage.vtable = g_unk_7101a194b8 + 2;
        storage.field_0x108 = nullptr;
        storage.field_0x110 = 0;
    }
    ~UnkSetupStr() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// A list of strings (nodes of a CRntString and the next link at +0x30) with an embedded block
// pool (base/UnkNodeOwner.h), destroyed out of line (fn_71000732d4); the constructor is inlined.
#include "base/UnkNodeOwner.h"
using UnkSetupStrList = UnkStrList30;

// The list's constructor: no pool block, so the nodes come from the default allocator; 0x13 is
// the allocation flags (at +0x28), and +0x68 is cleared with the flag at +0x6c.
static inline void InitStrList(UnkSetupStrList* list) {
    list->field_0x8 = 0;
    list->field_0x10 = nullptr;
    list->field_0x18 = nullptr;
    list->field_0x20 = fn_7100083990();
    *reinterpret_cast<u32*>(list->pad_0x28) = 0x13;
    list->vtable = g_unk_7101a1a4a8 + 2;
    list->field_0x30 = g_unk_7101a1a4c8 + 0x10;
    list->field_0x38 = nullptr;
    fn_710008377c(&list->field_0x40);
    *reinterpret_cast<u32*>(list->pad_0x68) = 0;
    list->field_0x6c = 0;
}

// An id built from a string out of line, released like CStrId (as src/game/CSubAreaManager.cpp's).
struct UnkSetupPartId {
    union {
        CStrId id;
    };

    explicit UnkSetupPartId(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkSetupPartId() { id.~CStrId(); }
};

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608* self);
bool fn_7100395c38(CBlackboard::CSection* section, const void* name, CRntString* out);
void fn_71000730a0(const char* str, const char* separators, UnkSetupStrList* out);
void fn_7100fabd60(UnkSubareaSetupState* self, const CStrId& id, bool a, bool b);

// Reads the string under `key` in the section `section` names; false when there is none.
static inline bool ReadString(CBlackboard* blackboard, const CStrId& section, const UnkSubId_7100fc4b34& key,
                              CRntString* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_7100395c38(s, &key, out);
    return false;
}

// Applies the chain of setups written as "a>b>c": the one the blackboard keeps for the current
// setup under this state's name, or the default chain from the id table; each part goes to
// fn_7100fabd60 in turn. Nothing happens without the object at +0x608.
void fn_7100faba64(UnkSubareaSetupState* self, CBlackboard* blackboard) {
    UnkGameManagerSub_0x608* points = g_unk_7101d47310->field_0x608;
    if (!points)
        return;
    UnkSetupIdText chain(reinterpret_cast<char*>(&GameManagerIdTable()) + 0xd80);
    UnkSetupStr read;
    bool ok = ReadString(blackboard, fn_71011a2fbc(points), SubId(g_unk_71729ee018, self->name), read.Str());
    if (ok)
        fn_710007eaa0(&chain.str, read.Str());
    UnkSetupStrList parts;
    InitStrList(&parts);
    fn_71000730a0(chain.str.data, ">", &parts);
    for (UnkListNode<0x30>* node = parts.field_0x10; node; node = node->next) {
        UnkSetupPartId id(*reinterpret_cast<const CRntString*>(node));
        fn_7100fabd60(self, id.id, false, false);
    }
    fn_71000732d4(&parts);
}

// A list of setups by name: the first entry whose id (+0x18) is `*id`, or null; empty entries are
// skipped.
struct UnkSubareaNamedEntry {
    u8 pad_0x0[0x18];
    CStrId id;
};
struct UnkSubareaNamedList {
    u8 pad_0x0[0x10];
    UnkSubareaNamedEntry** data;
    u32 count;
};

UnkSubareaNamedEntry* fn_7100fab838(UnkSubareaNamedList* self, const CStrId* id) {
    UnkSubareaNamedEntry** it = self->data;
    UnkSubareaNamedEntry** end = it + self->count;
    const base::global::UnkStringData* want = id->data;
    for (; it != end; ++it) {
        UnkSubareaNamedEntry* entry = *it;
        if (entry && entry->id.data == want)
            return entry;
    }
    return nullptr;
}
