// The game manager (CGameManager): the lookup that replaces a string id by another.
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"

using base::global::CStrId;

CStrId fn_7100dfbda4(UnkGameManager* self, const CStrId& id) {
    CStrId* it = self->field_0x1000.Find(id);
    if (it != self->field_0x1000.values + self->field_0x1000.count)
        return *it;
    return id;
}

// The object behind +0x498 starts with a pointer-sized field, then a string.
bool fn_7100deabe4(UnkGameManager* self) {
    char* object = static_cast<char*>(self->field_0x498);
    if (object)
        return fn_710007f140(reinterpret_cast<const base::global::CRntString*>(object + 8), "EDITOR");
    return false;
}

void* fn_7100defcec(UnkGameManager* self) {
    UnkGameManagerTarget* target = fn_7100decab0(self);
    return target ? target->field_0x248 : nullptr;
}

__attribute__((always_inline)) static inline void* FindInMap(UnkGameManagerNode** buckets, u32 count, u32 capacity, u64 id) {
    if (count) {
        UnkGameManagerNode** bucket = buckets + (((u32)(id >> 32) ^ (u32)id) % capacity);
        if (bucket) {
            for (UnkGameManagerNode* n = *bucket; n; n = n->next) {
                if (n->key->id == id)
                    return n->value;
            }
        }
    }
    return nullptr;
}

// NON_MATCHING(minor): same behaviour; the original merges the two maps' hits before the checks of
// the flags and schedules the loads of each map's capacity ahead of the hash (64 of 79 instructions
// differ). The maps are read through byte offsets because a nested member access makes Clang compute
// the capacity's address separately.
void* fn_7100deba8c(UnkGameManager* self, const u64* id, const u32* flags) {
    char* base = reinterpret_cast<char*>(self);
    if (*id == (u64)-1)
        return nullptr;
    {
        void* found = FindInMap(*reinterpret_cast<UnkGameManagerNode***>(base + 0x2608), *reinterpret_cast<u32*>(base + 0x2610), *reinterpret_cast<u32*>(base + 0x2614), *id);
        if (!found)
            found = FindInMap(*reinterpret_cast<UnkGameManagerNode***>(base + 0x2650), *reinterpret_cast<u32*>(base + 0x2658), *reinterpret_cast<u32*>(base + 0x265c), *id);
        if (found) {
            if (!(*flags & 8)) {
                for (UnkGameManagerListNode* n = self->field_0x2778; n; n = n->next) {
                    if (n->object == found)
                        goto fallback;
                }
            }
            return found;
        }
        if (*flags & 4) {
            for (UnkGameManagerListNode* n = self->field_0x2698; n; n = n->next) {
                if (n->object) {
                    char* entry = static_cast<char*>(n->object);
                    if (*reinterpret_cast<u64*>(*reinterpret_cast<char**>(entry + 0x30) + 0x30) == *id)
                        return entry;
                }
            }
        }
    }
fallback:
    if (*flags & 2) {
        if (self->field_0x608)
            return fn_71011a3d74(self->field_0x608, id);
    }
    return nullptr;
}

// The rest of the unit: script calls, start-up steps and their helpers.
#include <stdarg.h>

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* str);
s32 fn_71010bf400(lua_State* L, const char* fmt, va_list args);
s32 fn_71010c0d80(lua_State* L, s32 a, char* buf, u32 size, void* x, void* y);

// Calls the script function `name` with the arguments `fmt` describes; whether it returned 1.
bool fn_7100e01f08(const char* name, const char* fmt, ...) {
    if (!name)
        return false;
    if (fn_71010c0bb0(g_unk_7172a01d18, name) == 0)
        return false;
    va_list args;
    va_start(args, fmt);
    s32 r = fn_71010bf400(g_unk_7172a01d18, fmt, args);
    char buf[0x100];
    bool result = fn_71010c0d80(g_unk_7172a01d18, r, buf, 0x100, nullptr, nullptr) == 1;
    va_end(args);
    return result;
}

using base::global::CRntString;

extern void* g_unk_7101a194b8[];

// A string (header of 0x30 bytes) with 0x100 bytes of inline storage.
struct UnkGameManagerStrStorage {
    void* vtable;
    u8 pad_0x8[0x100];
    void* field_0x108;
    u32 field_0x110;
};
struct UnkGameManagerStr {
    const char* data;
    u32 length;
    u8 pad_0xc[0x24];
    UnkGameManagerStrStorage storage;

    // The string part (its constructor and destructor run by hand).
    CRntString* Str() { return reinterpret_cast<CRntString*>(this); }

    UnkGameManagerStr() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage));
        storage.vtable = g_unk_7101a194b8 + 2;
        storage.field_0x108 = nullptr;
        storage.field_0x110 = 0;
    }
    ~UnkGameManagerStr() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
    const char* c_str() const { return data ? data : ""; }
};

CRntString fn_710007ef90(const CRntString* a, const char* b);  // concatenation

// The script environment's name: the string id at +0x18 of the object at +0x608, seen through its
// pooled data (the text as a CRntString at +0x10, a count of reads at +0x44; UnkIdTextData below is
// the same view). Its text prefixes the functions fn_7100def678 calls.
struct UnkGameManagerScriptEnv {
    u8 pad_0x0[0x10];
    CRntString name;
    u8 pad_0x40[0x4];
    // A union member: the original reloads the id's data pointer after the increment.
    union {
        s32 field_0x44;
    };
};
struct UnkGameManagerSub_0x608Env {
    u8 pad_0x0[0x18];
    UnkGameManagerScriptEnv* env;
};

// A copy of `s` without its spaces.
static inline UnkGameManagerStr StripSpaces(const UnkGameManagerStr& s) {
    char buf[0x200];
    s32 n = 0;
    const char* p = s.data;
    for (u64 left = s.length; left != 0; --left, ++p) {
        char c = *p;
        if (c != ' ')
            buf[n++] = c;
    }
    buf[n] = 0;
    UnkGameManagerStr out;
    fn_710007e77c(out.Str(), buf);
    return out;
}

static inline CRntString Concat(const CRntString& a, const CRntString& b) {
    return fn_710007eee0(&a, &b);
}
static inline void Assign(CRntString* self, const CRntString& other) {
    fn_710007eaa0(self, &other);
}

// Calls the script function `name` (spaces removed) of the environment behind +0x608, with the
// arguments `fmt` describes; nothing happens when there is no such object.
void fn_7100def678(UnkGameManager* self, const CRntString& name, const char* fmt, ...) {
    if (!self->field_0x608)
        return;
    UnkGameManagerStr str;
    fn_710007eaa0(str.Str(), &name);
    UnkGameManagerSub_0x608Env* owner = reinterpret_cast<UnkGameManagerSub_0x608Env*>(self->field_0x608);
    ++owner->env->field_0x44;
    Assign(str.Str(), Concat(fn_710007ef90(&owner->env->name, "."), *StripSpaces(str).Str()));
    if (fn_71010c0bb0(g_unk_7172a01d18, str.c_str()) != 0) {
        va_list args;
        va_start(args, fmt);
        s32 r = fn_71010bf400(g_unk_7172a01d18, fmt, args);
        char buf[0x100];
        fn_71010c0d80(g_unk_7172a01d18, r, buf, 0x100, nullptr, nullptr);
        va_end(args);
    }
}

#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The file device singleton (created on first use) and the sound manager.
extern base::file::UnkFileDevice* g_unk_7101d4d828;
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;

void fn_7100089894(base::file::UnkFileDevice* self);  // locks the device
void fn_71000898c4(base::file::UnkFileDevice* self);  // unlocks it
struct UnkFilePathList;
// Lists the files under `path` into `out`.
void fn_7100089eec(base::file::UnkFileDevice* self, const CFilePathStrId* path, UnkFilePathList* out);
struct UnkFileRequestArgs {
    u64 field_0x0;
    u64 field_0x8;
};
// Requests the file `path` (with 0 and 9; the arguments are zeroed).
void fn_7100089754(base::file::UnkFileDevice* self, const CFilePathStrId* path, u32 a, u32 b,
                   const UnkFileRequestArgs* args);
void fn_7100107b74(UnkSoundMgr* self, const CFilePathStrId* path);

static inline base::file::UnkFileDevice* FileDevice() {
    if (!g_unk_7101d4d828) {
        auto* device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    return g_unk_7101d4d828;
}

// The pooled data behind a string id as its text accessor sees it: the text as a CRntString at
// +0x10 and a count of the reads at +0x44.
struct UnkIdTextData {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    // A union member: the original reloads the id's data pointer after the increment.
    union {
        s32 reads;
    };
};
template <class Id>
static inline const CRntString& IdText(const Id& id) {
    ++reinterpret_cast<UnkIdTextData*>(id.data)->reads;
    return reinterpret_cast<UnkIdTextData*>(id.data)->str;
}
static inline const char* CStr(const CRntString& s) {
    return s.data ? s.data : g_unk_710162e2ed;
}

// A path id whose release decrements and tests the count in two steps.
struct UnkListedPathId {
    UnkStringData* data;

    ~UnkListedPathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};
struct UnkListedPath {
    UnkListedPathId id;
    UnkListedPath* next;
};
extern void* g_unk_7101a1b030[];
// A list of path ids (its destructor is fn_7100087cf8).
struct UnkFilePathList {
    void** vtable;
    u32 count;
    UnkListedPath* head;
    UnkListedPath* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        for (UnkListedPath* n = head; n;) {
            UnkListedPath* next = n->next;
            n->id.~UnkListedPathId();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

// The copy-assignment of a path id (CStrId's, on the file-path pool).
static inline void AssignPath(CFilePathStrId* self, const CFilePathStrId& other) {
    if (self->data != other.data) {
        UnkStringData* d = other.data;
        UnkStringPool* pool = g_unk_7101d47200;
        // The empty id's address computed as an integer (the original compares `d` with it in
        // this operand order).
        UnkStringData* empty = reinterpret_cast<UnkStringData*>(reinterpret_cast<u64>(pool) + 0x10);
        if (d != empty)
            ++d->ref_count;
        if (self->data != empty) {
            --self->data->ref_count;
            if (self->data->ref_count == 0)
                fn_71000803a4(pool, self->data);
        }
        self->data = d;
    }
}

struct UnkGameManagerPaths {
    u8 pad_0x0[0x610];
    CFilePathStrId field_0x610;
};

// Keeps `path` at +0x610 and, with the file device locked, requests every file listed under it and
// passes each to the sound manager.
// NON_MATCHING(minor): in the path assignment the original loads the old id before the new one (the
// two independent loads are scheduled the other way round; 2 instructions).
void fn_7100df026c(UnkGameManager* self, const CFilePathStrId& path) {
    CFilePathStrId* own = &reinterpret_cast<UnkGameManagerPaths*>(self)->field_0x610;
    AssignPath(own, path);
    base::file::UnkFileDevice* device = FileDevice();
    fn_7100089894(device);
    UnkFilePathList files;
    fn_7100089eec(device, own, &files);
    for (UnkListedPath* n = files.head; n; n = n->next) {
        {
            CFilePathStrId id(CStr(IdText(n->id)), true);
            UnkFileRequestArgs args = {};
            fn_7100089754(device, &id, 0, 9, &args);
        }
        UnkSoundMgr* sound = g_unk_7101d4d820;
        CFilePathStrId id(CStr(IdText(n->id)), true);
        fn_7100107b74(sound, &id);
    }
    fn_71000898c4(FileDevice());
}

#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkTUProperty.h"

using base::global::UnkStrIdBase;

extern UnkStrIdBase g_unk_71727c8018;     // "s070_basesanc" (src/game_s7/fn_7100e72680.cpp)
extern CStrId g_unk_7172a0bc98;           // a blackboard section's name
extern UnkPropertyEx<> g_unk_71727591b8;  // a property of the "GAME_PROGRESS" section

// A property seen as a definition whose qualified id (game/UnkGameBlackboard.h) is built on first
// use (as in src/game_s7/fn_7100fc4b34.cpp).
struct UnkGameManagerDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};
static inline const UnkQualifiedId& Qid(UnkGameManagerDef& def) {
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}

// A definition whose qualified id is kept as a string, with named sub-ids looked up in a map
// (fn_71003327a4; the declaration is src/game_s7/fn_7100fc4b34.cpp's).
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);
struct UnkGameManagerNamedDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
static inline UnkSubId_7100fc4b34 SubId(UnkGameManagerNamedDef& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}
extern UnkGameManagerNamedDef g_unk_7172a0bb10;  // the definition the cutscene's sub-id is read under

bool fn_7100a68fb0(CBlackboard::CSection* section, const void* name, u32* out);
// Reads the u32 under `def`'s sub-id `name`.
static inline bool ReadU32(CBlackboard::CSection* section, UnkGameManagerNamedDef& def, const CStrId& name, u32* out) {
    UnkSubId_7100fc4b34 id = SubId(def, name);
    return fn_7100a68fb0(section, &id, out);
}

// A typed value as the section setter takes it (as in src/game_aca57c/fn_7100dc8828.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// The world's object reference at +0xd63f40 (as in src/game_aca57c/fn_7100b2de48.cpp).
struct UnkGameManagerObjRef {
    u8 pad_0x0[0x10];
    void* obj;
};
struct UnkGameManagerWorld {
    u8 pad_0x0[0xd63f40];
    UnkGameManagerObjRef* field_0xd63f40;
};
void fn_7100b42ff0(void* obj, u32 a, u32 b);

// Whether the presentation cutscene is counted in the section g_unk_7172a0bc98 names (the count
// is read into `*count`).
static inline bool CutsceneSeen(CBlackboard* blackboard, u32* count) {
    CBlackboard::CSection* section = fn_7101047484(blackboard, g_unk_7172a0bc98);
    if (!section)
        return false;
    static CStrId s_cutscene("cutscenes/0040sancemmypresentation_part2a/0040sancemmypresentation_part2a.bmscu", true);
    *count = 0;
    ReadU32(section, g_unk_7172a0bb10, s_cutscene, count);
    return *count != 0;
}

// Sets the GAME_PROGRESS flag g_unk_71727591b8 names, unless the blackboard is locked.
static inline void SetProgressFlag(CBlackboard* blackboard) {
    static CStrId s_progress("GAME_PROGRESS", true);
    const UnkQualifiedId& name = Qid(reinterpret_cast<UnkGameManagerDef&>(g_unk_71727591b8));
    bool value = true;
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, s_progress)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

// Unless the current scenario's spawn group "SG_ChozoRobotSoldier" is enabled and the
// presentation cutscene has not been counted yet, calls fn_7100b42ff0 on the world's object and
// sets a GAME_PROGRESS flag (unless the blackboard is locked).
// NON_MATCHING(minor): the original loads the game manager's GOT slot through x9 instead of x8
// (register choice only; 3 instructions).
void fn_7100e01978(UnkGameManager* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    bool enabled = true;
    CBlackboard* blackboard = &self->field_0x968;
    CStrId scenario = fn_7100dfbda4(manager, static_cast<const CStrId&>(g_unk_71727c8018));
    if (CBlackboard::CSection* section = fn_7101047484(blackboard, scenario)) {
        static CStrId s_enabled("SG_ChozoRobotSoldier:SPAWNGROUP:Enabled", true);
        fn_710006d11c(section, &s_enabled, &enabled);
    }
    {
        u32 count;
        if (!CutsceneSeen(blackboard, &count) && enabled)
            return;
    }
    UnkGameManagerObjRef* ref = static_cast<UnkGameManagerWorld*>(self->field_0x2228)->field_0xd63f40;
    if (ref && ref->obj)
        fn_7100b42ff0(ref->obj, 1, 0);
    SetProgressFlag(blackboard);
}

// The start-up step after loading (fn_7100df19c0) and what it calls.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/sound/CMusicManager.h"

// The singletons it creates on first use.
struct UnkLoadingScreen;  // 0x220 bytes
extern UnkLoadingScreen* g_unk_7101d47380;
void fn_7100e0f89c(UnkLoadingScreen* self);  // constructor
void fn_7100e100f8(UnkLoadingScreen* self, bool shown);
struct UnkSoundHolder;  // 0x48 bytes (src/game_aca57c/fn_7100b2de48.cpp)
extern UnkSoundHolder* g_unk_7101d47340;
void fn_710006cd9c(UnkSoundHolder* holder);  // constructor
void fn_710006cea8(UnkSoundHolder* holder, bool value);
extern void* g_unk_71726bc560;  // the music manager (game/sound/CMusicManager.h)
extern void* g_unk_7101d47330;
extern UnkTunable g_unk_7101d68580;  // base::snd::CSoundSystemATK's tunable; a float at +0x28

void fn_710109d0a0();
void fn_71011a2770(UnkGameManagerSub_0x608* points, void* object);
void fn_7100df31b8(UnkGameManager* self, u32 a, bool b, bool c, CStrId id, u32 d);
struct UnkTreeNode;  // src/game_s1/fn_710014be78.cpp
void fn_710023298c(UnkTreeNode* self);
struct UnkObj_7100e01680;  // src/game_aca57c/fn_7100e01680.cpp
void* fn_7100e01680(UnkObj_7100e01680* self);
void* fn_7100e0189c(void* start_point);
void fn_710116c5dc(void* target, void* object);
// A mode word passed by address (the word after it is not set).
struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};
void fn_7100fd90d8(f32 a, f32 b, void* music, const CRntString* name, u32 c, const UnkMusicMode* d, u32 e, u32 f, void* g);
bool fn_7101196fd8(void* object);
struct UnkStruct_71002334fc;  // src/game_lo/fn_71002334fc.cpp
void fn_71002334fc(UnkStruct_71002334fc* self, bool value);
struct UnkCameraCtrl;  // src/game_s2/fn_71003ad014.cpp
UnkCameraCtrl* fn_71010e389c(void* holder, void* key);
void fn_7100935d40(UnkCameraCtrl* ctrl);
void fn_7100e0203c(UnkGameManager* self);
void fn_71010e4798(void* target);
void fn_7100000790(const char* name, const char* format, ...);  // calls a script function
void fn_71000fbe1c(void* self, u32 a, f32 b, f32 c);
void fn_7101048af8(CBlackboard* self);
void fn_7100dfc638(UnkGameManager* self, CBlackboard* blackboard, const CRntString* name, const CRntString* tag);
void fn_7101046e78(CBlackboard* self, CBlackboard* other, bool a, bool b);
void fn_7101047178(CBlackboard* self);
struct UnkHandleList {
    UnkGameManagerObjRef** data;
    u32 count;
};
UnkHandleList* fn_7100a5e1dc();

// The id table (fn_7100031894 builds it, src/tables/strid_table_7100031894.cpp: 0x2578 bytes of
// string ids; a weak function-local static reached through the GOT). Only the ids used here are
// named; the rest is reached by offset.
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
static_assert(sizeof(UnkGameManagerIdTable) == 0x2578, "the table's whole size (its constructor fills it)");
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkLoadedObject {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x1aa];
    bool field_0x1b2;
};
struct UnkLoadedComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    base::math::CVector3D field_0x40;
    void vf_0x1e8(bool value) { reinterpret_cast<void (*)(UnkLoadedComponent*, bool)>(vtable[0x1e8 / 8])(this, value); }
    void vf_0x208(const char* name) { reinterpret_cast<void (*)(UnkLoadedComponent*, const char*)>(vtable[0x208 / 8])(this, name); }
};
struct UnkPlayerObject {
    void** vtable;
    u8 pad_0x8[0x1f8];
    UnkLoadedComponent* field_0x200;
    void vf_0xd0(const base::math::CVector3D* v, bool b) {
        reinterpret_cast<void (*)(UnkPlayerObject*, const base::math::CVector3D*, bool)>(vtable[0xd0 / 8])(this, v, b);
    }
    void vf_0xd8(const base::math::CVector3D* v, bool b) {
        reinterpret_cast<void (*)(UnkPlayerObject*, const base::math::CVector3D*, bool)>(vtable[0xd8 / 8])(this, v, b);
    }
};
struct UnkTriggerView {
    u8 pad_0x0[0x240];
    bool field_0x240;
};
struct UnkCameraCtrlView {
    virtual const base::reflection::CType* vf_0x0();
};
struct UnkBossMusicFlag {
    u8 pad_0x0[0x7c];
    bool field_0x7c;
};

// The game manager's members this step reads (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerSteps {
    u8 pad_0x0[0x578];
    bool field_0x578;
    u8 pad_0x579[0x537];
    CBlackboard field_0xab0;  // another blackboard (fn_7101046e78)
    u8 pad_0xba0[0x1df8];
    UnkGameManagerObjRef* field_0x2998;
    UnkGameManagerObjRef* field_0x29a0;
    UnkGameManagerObjRef* field_0x29a8;
    UnkGameManagerObjRef* field_0x29b0;
};
static_assert(__builtin_offsetof(UnkGameManagerSteps, field_0x2998) == 0x2998, "UnkGameManagerSteps placeholder layout");
static inline UnkGameManagerSteps* Steps(UnkGameManager* self) {
    return reinterpret_cast<UnkGameManagerSteps*>(self);
}
static inline void* RefObject(UnkGameManagerObjRef* ref) {
    return ref ? ref->obj : nullptr;
}

template <class T, u64 Size, void (*Ctor)(T*)>
static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

static inline bool IsMode(UnkGameManager* self, const char* name) {
    char* object = static_cast<char*>(self->field_0x498);
    if (object)
        return fn_710007f140(reinterpret_cast<const CRntString*>(object + 8), name);
    return false;
}

// Hides the loading screen and places the four objects at +0x2998.. (the first one also at the
// start point, or at the origin when there is none, with its triggers reset); then the cameras, the
// sounds and, unless the flag at +0x578 is set, the blackboard's "[MANTAIN]" pass.
// NON_MATCHING(minor): the copy of g_unk_7101d470e0 passed to fn_7100df31b8: the original keeps its
// data pointer in a register across the call (the callee, in this unit, is known not to change its
// argument) where this reloads it after the call, and the registers of that part differ (21
// instructions).
void fn_7100df19c0(UnkGameManager* self) {
    UnkGameManagerSteps* steps = Steps(self);
    fn_7100e100f8(Singleton<UnkLoadingScreen, 0x220, fn_7100e0f89c>(g_unk_7101d47380), false);
    fn_7100e01f08("Scenario.HideLoadingScreen", "");
    fn_710109d0a0();
    if (steps->field_0x2998) {
        if (UnkPlayerObject* player = static_cast<UnkPlayerObject*>(steps->field_0x2998->obj)) {
            fn_71011a2770(self->field_0x608, player);
            fn_71011a2770(self->field_0x608, RefObject(steps->field_0x29a0));
            fn_71011a2770(self->field_0x608, RefObject(steps->field_0x29a8));
            fn_71011a2770(self->field_0x608, RefObject(steps->field_0x29b0));
            fn_7100df31b8(self, 0, true, true, g_unk_7101d470e0, 0);
            auto* first = static_cast<UnkLoadedObject*>(RefObject(steps->field_0x2998));
            auto* second = static_cast<UnkLoadedObject*>(RefObject(steps->field_0x29a0));
            if (first && !first->field_0x1b2)
                fn_710023298c(reinterpret_cast<UnkTreeNode*>(first));
            if (second && !second->field_0x1b2)
                fn_710023298c(reinterpret_cast<UnkTreeNode*>(second));
            if (void* start = fn_7100e01680(reinterpret_cast<UnkObj_7100e01680*>(self))) {
                if (void* target = fn_7100e0189c(start)) {
                    fn_710116c5dc(target, player);
                    if (static_cast<UnkBossMusicFlag*>(target)->field_0x7c) {
                        void* music = Singleton<void, 0xf18, fn_7100fd3804>(g_unk_71726bc560);
                        CRntString name("s_boss_defeated");
                        UnkMusicMode mode;
                        mode.kind = 2;
                        fn_7100fd90d8(-1.0f, -1.0f, music, &name, 2, &mode, 0, 0, nullptr);
                    }
                }
            } else if (UnkLoadedComponent* c = player->field_0x200) {
                c->field_0x40 = {1.0f, 0.0f, 0.0f};
                c->vf_0x208("");
                {
                    base::math::CVector3D v;
                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = 0.0f;
                    player->vf_0xd0(&v, true);
                }
                {
                    base::math::CVector3D v;
                    v.x = 0.0f;
                    v.y = -1.57f;
                    v.z = 0.0f;
                    player->vf_0xd8(&v, true);
                }
                UnkHandleList* triggers = fn_7100a5e1dc();
                for (UnkGameManagerObjRef **it = triggers->data, **end = it + triggers->count; it != end; ++it) {
                    auto* o = static_cast<UnkLoadedObject*>((*it)->obj);
                    auto* t = fn_710009d50c(o->vf_0x0(), fn_7100a5bab0()) ? reinterpret_cast<UnkTriggerView*>(o) : nullptr;
                    t->field_0x240 = false;
                }
                c->vf_0x1e8(true);
            }
            if (steps->field_0x2998 && steps->field_0x2998->obj && !fn_7101196fd8(g_unk_7101d47330))
                fn_71002334fc(static_cast<UnkStruct_71002334fc*>(RefObject(steps->field_0x2998)), true);
            if (steps->field_0x29a0 && steps->field_0x29a0->obj && !fn_7101196fd8(g_unk_7101d47330))
                fn_71002334fc(static_cast<UnkStruct_71002334fc*>(RefObject(steps->field_0x29a0)), true);
            if (void* holder = fn_7100defcec(g_unk_7101d47310)) {
                if (UnkCameraCtrl* ctrl = fn_71010e389c(holder, GameManagerIdTable().field_0x17e0)) {
                    if (reinterpret_cast<UnkCameraCtrlView*>(ctrl)->vf_0x0() == fn_7100932fa4())
                        fn_7100935d40(ctrl);
                }
            }
        }
    }
    fn_710109d0a0();
    fn_7100e0203c(self);
    if (!fn_7100deabe4(g_unk_7101d47310) && !IsMode(g_unk_7101d47310, "VIEWER")) {
        fn_71010e4798(fn_7100defcec(self));
        fn_7100000790("Scenario.ActivateDefaultCamera", "b", true);
    }
    fn_710006cea8(Singleton<UnkSoundHolder, 0x48, fn_710006cd9c>(g_unk_7101d47340), true);
    UnkSoundMgr* sound = g_unk_7101d4d820;
    f32 volume = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_7101d68580) + 0x28);
    fn_71000fbe1c(reinterpret_cast<char*>(sound) + 8, 0, volume, 0.0f);
    if (!steps->field_0x578) {
        CBlackboard* blackboard = &self->field_0x968;
        fn_7101048af8(blackboard);
        fn_71010472e0(&g_unk_7101d47310->field_0x968, g_unk_7172a0bc98)->data.field_0x38 = false;
        const CStrId& env = self->field_0x608 ? *reinterpret_cast<CStrId*>(reinterpret_cast<char*>(self->field_0x608) + 0x18)
                                               : g_unk_7101d470e0;
        const CRntString& name = IdText(env);
        CRntString tag("[MANTAIN]");
        if (self->field_0x608) {
            fn_7100dfc638(self, blackboard, &name, &tag);
            fn_7101046e78(&steps->field_0xab0, blackboard, true, false);
            fn_7101047178(blackboard);
        }
    }
}

// The start of a scenario load (fn_7100df99f8) and what it calls.
#include "base/UnkDefaultAllocator.h"

// The user data's owner (created on first use; src/game_aca57c/fn_7100dc8828.cpp).
struct UnkUserDataOwner;
extern UnkUserDataOwner* g_unk_7101d47308;
void fn_710120320c(UnkUserDataOwner* self);  // constructor
void fn_7101203768(UnkUserDataOwner* self);

void fn_7100dee46c(UnkGameManager* self);

// The result a load reports to its callback.
struct UnkLoadResult {
    u8 pad_0x0[0xa8];
    UnkLoadResult(u32 code) asm("fn_7100dfb050");
    ~UnkLoadResult() asm("fn_7100de8ac4");
};
struct UnkLoadCallback {
    void* context;
    void (*fn)(void* context, UnkLoadResult* result);
};

// A list of pointers (vtable g_unk_7101aa9ac0), its nodes from `allocator` with `flags`.
extern char g_unk_7101aa9ac0[];
struct UnkPtrListNode {
    void* value;
    UnkPtrListNode* next;
};
struct UnkPtrList {
    void** vtable;
    // Union members: the original reloads the count, the tail and the allocator after storing a new
    // node's links.
    union {
        u32 count;
    };
    UnkPtrListNode* head;
    union {
        UnkPtrListNode* tail;
    };
    union {
        base::UnkAllocator* allocator;
    };
    union {
        u32 flags;
    };

    UnkPtrList() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkPtrList() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        for (UnkPtrListNode* n = head; n;) {
            UnkPtrListNode* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }
    void PushBack(void* value) {
        auto* node = static_cast<UnkPtrListNode*>(allocator->vf_0x40(sizeof(UnkPtrListNode), 8, flags));
        if (!node)
            return;
        node->value = value;
        node->next = nullptr;
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
            ++count;
        }
    }
};

// A vector of strings (base::UnkVector's layout) whose growth moves the strings across.
struct UnkStrVector {
    CRntString* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkStrVector(u32 step_, u32 flags_)
        : data(nullptr), count(0), capacity(0), step(step_), field_0x14(true), flags(flags_),
          allocator(fn_7100083990()) {}
    void Reserve(u32 n) {
        if (capacity < n) {
            CRntString* p = static_cast<CRntString*>(allocator->vf_0x40((u64)n * sizeof(CRntString), 8, flags));
            if (CRntString* old = data) {
                CRntString* to = p;
                for (u32 i = 0; i < count; ++i, ++to, ++old) {
                    fn_710007eab4(to, old);
                    fn_710007ecd4(old);
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
    void Free() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                fn_710007ecd4(&data[i]);
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
static_assert(sizeof(UnkStrVector) == 0x28, "UnkStrVector layout");
void fn_71000e0524(UnkStrVector* self, UnkStrVector* other);

// Callbacks run before the blackboard pass.
struct UnkStepCallback {
    void* context;
    void (*fn)(void* context);
};
struct UnkStepCallbacks {
    u8 pad_0x0[0x8];
    u32 count;
};
UnkStepCallback* fn_7100dfc428(UnkStepCallbacks* self, u32 index);

// Named objects (a CRntString at +8), in a list at +0x478.
struct UnkNamedObject {
    u8 pad_0x0[0x8];
    CRntString name;
};
struct UnkNamedNode {
    UnkNamedObject* value;
    UnkNamedNode* next;
};
struct UnkNamedUser {
    u8 pad_0x0[0x88];
    bool field_0x88;
};
// String equality (the declaration is src/game_s1/fn_71000d4d50.cpp's, over a record of CRntString's size).
struct UnkCompared_0x30;
bool fn_710007f120(const UnkCompared_0x30* a, const UnkCompared_0x30* b);
static inline bool Equal(const CRntString* a, const CRntString* b) {
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(a), reinterpret_cast<const UnkCompared_0x30*>(b));
}
UnkNamedUser* fn_7101190890(UnkNamedObject* self, CRntString name);

// A flag holder (as src/game_aca57c/fn_7100b2de48.cpp sees it): set, the step callbacks and the
// blackboard pass are skipped.
struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};
extern UnkModeFlags* g_unk_71726bc570;
extern UnkPropertyEx<> g_unk_7172758c30;  // an int property

void fn_7100df7468();
s32 fn_71012034b0(UnkUserDataOwner* self, u32 kind, const CRntString* scenario, const CRntString* name,
                  UnkPtrList* blackboards, bool a, void (*fn)(), bool b, const CRntString* c);

// The game manager's members the load reads (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerLoad {
    u8 pad_0x0[0x478];
    UnkNamedNode* field_0x478;
    u8 pad_0x480[0x2c8];
    UnkLoadCallback field_0x748;
    UnkStrVector field_0x758;
    u8 pad_0x780[0x48];
    nn::os::MutexType field_0x7c8;
    u8 pad_0x7e8[0x8];
    CRntString field_0x7f0;
    u8 field_0x820[1];
    u8 pad_0x821[0x147];
    CBlackboard field_0x968;
    u8 pad_0xa58[0x58];
    CBlackboard field_0xab0;
    u8 pad_0xba0[0x58];
    CBlackboard field_0xbf8;
    u8 pad_0xce8[0x1c60];
    UnkStepCallbacks field_0x2948;
};
static_assert(__builtin_offsetof(UnkGameManagerLoad, field_0x7f0) == 0x7f0, "UnkGameManagerLoad placeholder layout");
static_assert(__builtin_offsetof(UnkGameManagerLoad, field_0xbf8) == 0xbf8, "UnkGameManagerLoad placeholder layout");
static_assert(__builtin_offsetof(UnkGameManagerLoad, field_0x2948) == 0x2948, "UnkGameManagerLoad placeholder layout");

// Starts loading the scenario named at +0x7f0 (mode 2 first resets an int GAME_PROGRESS-style
// property, runs the step callbacks and the blackboard's "profilesave" pass); without a name the
// callback is told 5 and 0x7fffffff is returned.
s32 fn_7100df99f8(UnkGameManager* self, u32 mode, const UnkLoadCallback* callback, bool flag) {
    auto* load = reinterpret_cast<UnkGameManagerLoad*>(self);
    fn_7101203768(Singleton<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308));
    fn_7100dee46c(self);
    if (load->field_0x7f0.data && load->field_0x7f0.data[0]) {
        UnkPtrList blackboards;
        blackboards.PushBack(load->field_0x820);
        switch (mode) {
        case 2: {
            CBlackboard* blackboard = &load->field_0x968;
            {
                UnkGameManagerIdTable& table = GameManagerIdTable();
                const UnkQualifiedId& name = Qid(reinterpret_cast<UnkGameManagerDef&>(g_unk_7172758c30));
                s32 value = 0;
                if (CBlackboard::CSection* section = fn_71010472e0(blackboard, *reinterpret_cast<CStrId*>(table.pad_0x0 + 0x1148))) {
                    if (!blackboard->field_0xe8) {
                        UnkTypedValue v;
                        v.type = fn_7100014768();
                        v.value = &value;
                        fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
                    }
                }
            }
            {
                CRntString save("profilesave");
                CRntString tag("[MANTAIN]");
                if (!g_unk_71726bc570->field_0xb8) {
                    for (u32 i = 0; i < load->field_0x2948.count; ++i) {
                        UnkStepCallback* c = fn_7100dfc428(&load->field_0x2948, i);
                        c->fn(c->context);
                    }
                    if (self->field_0x608) {
                        fn_7100dfc638(self, blackboard, &save, &tag);
                        fn_7101046e78(&load->field_0xab0, blackboard, true, false);
                        fn_7101047178(blackboard);
                    }
                }
            }
            fn_7101046e78(&load->field_0xbf8, &load->field_0xab0, false, false);
            fn_7101047178(&load->field_0xab0);
        }
            [[fallthrough]];
        case 1:
            blackboards.PushBack(&load->field_0xbf8);
            break;
        }
        CRntString* scenario = &load->field_0x7f0;
        const CRntString& text = IdText(*reinterpret_cast<CStrId*>(GameManagerIdTable().pad_0x0 + 0x16a8));
        for (UnkNamedNode* n = load->field_0x478; n; n = n->next) {
            if (Equal(&n->value->name, &text)) {
                if (UnkNamedObject* object = n->value) {
                    UnkNamedUser* user = fn_7101190890(object, *scenario);
                    user->field_0x88 = true;
                }
                break;
            }
        }
        CRntString name(*scenario);
        {
            UnkStrVector empty(1, 0xf);
            empty.Reserve(1);
            fn_710006691c(&load->field_0x7c8);
            fn_71000e0524(&load->field_0x758, &empty);
            UnkLoadCallback copy = *callback;
        load->field_0x748.context = copy.context;
        load->field_0x748.fn = copy.fn;
            fn_7100066920(&load->field_0x7c8);
            empty.Free();
        }
        UnkUserDataOwner* owner = Singleton<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308);
        CRntString none("");
        return fn_71012034b0(owner, 4, scenario, &name, &blackboards, true, fn_7100df7468, flag, &none);
    }
    if (callback->fn) {
        UnkLoadResult result(5);
        callback->fn(callback->context, &result);
    }
    return 0x7fffffff;
}
