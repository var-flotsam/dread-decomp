// From unit 2577 of tools/tu_map.py (0x7100edf654-0x7100ef3494, CSubAreaManager and
// CSubAreaManager::CTunableSubAreaManager): the sub-area manager's scenario loading and its
// change of the current sub-area. The file reader's types repeat src/game/UnkObjectFileLoader.cpp's
// (the same names and layouts; the read target and the parse context are in
// base/reflection/UnkReflObjectRead.h); names and layouts are placeholders.
// Holds fn_7100ee30dc (reads the reflected global object from a scenario file), fn_7100ee327c
// (loads a scenario's files) and fn_7100ee4f84 (makes a sub-area the current one), and the two steps
// that hand the new sub-area's settings to the environment manager (fn_7100eea4c8) and the main
// camera (fn_7100eea618).
#include "base/UnkHashStream.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkCameraCtrl.h"
#include "base/input/UnkRumbleManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

// The id and path pair (base/UnkStrIdPathPair.h) the reader opens.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

// Builds the id and path pair from a file-path id.
void fn_710008d2ec(UnkStrPair_0x10* self, const base::global::CFilePathStrId* path);

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710000ccd8(UnkFileReader* self);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71729daf80 __attribute__((visibility("hidden")));

void fn_710000f750(UnkParseContext* self, void* root);

// The object read and its type.
extern base::reflection::CType g_unk_7172756e50;
extern u8 g_unk_7172757530[];

// Parses the file into the target, starting from its last entry's node.
static inline void Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71729daf80, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// The reader's first argument is an empty value (its caller leaves x0 as it is).
struct UnkFileReadTag {};

void fn_7100ee30dc(UnkFileReadTag, const CFilePathStrId* path) {
    UnkFileReader reader;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, path);
        fn_710007be8c(&reader.file, 0x16, nullptr);
        reader.field_0x38 = 3;
        reader.field_0x3c = 2;
        bool opened;
        {
            base::global::CRntString mode("rb");
            opened = fn_710007c1ac(&reader, &s, &mode);
        }
        if (opened)
            fn_710000ccd8(&reader);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    UnkReadTarget target;
    {
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, &g_unk_7172756e50, g_unk_7172757530, true);
        fn_71000f0568(&target, &value, 5);
        fn_71000a1c60(&value);
    }
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
        Parse(&reader, &target);
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
}

// The six id lists fn_7100ee7134 empties (src/game_hi/fn_7100ee7134.cpp: two string-id lists and
// four file-path-id lists); only the counts are read here.
struct UnkIdList {
    void* data;
    u32 count;
    u8 pad_0xc[0x1c];
};
struct UnkIdSet {
    UnkIdList field_0x0;
    UnkIdList field_0x28;
    UnkIdList field_0x50;
    UnkIdList field_0x78;
    UnkIdList field_0xa0;
    UnkIdList field_0xc8;

    bool IsEmpty() const {
        return field_0x28.count == 0 && field_0x0.count == 0 && field_0x50.count == 0 &&
               field_0x78.count == 0 && field_0xc8.count == 0 && field_0xa0.count == 0;
    }
};
void fn_7100ee7134(UnkIdSet* self);

// A sub-area (a placeholder for the fields used here): two ids and a flag.
struct UnkSubArea {
    CStrId field_0x0;
    u8 pad_0x8[0x48];
    CStrId field_0x50;
    u8 pad_0x58[0x129];
    // A union member (no type-based alias info): the original reloads the manager's sub-area
    // pointers after storing it, and the flag itself after every store.
    union {
        bool field_0x181;
    };
};

// Members the reset (fn_7100ee34f0) empties.
using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;  // a file-path id

// A queued sub-area request (0x10 bytes): an id and three flags (+0x8 picks the handler, +0x9 is
// passed on).
struct UnkSubAreaRequestFlags {
    u8 field_0x0;
    u8 field_0x1;
    u8 field_0x2;
};
struct UnkSubAreaRequest {
    base::UnkStrIdMember id;
    UnkSubAreaRequestFlags flags;
};
static_assert(sizeof(UnkSubAreaRequest) == 0x10, "UnkSubAreaRequest placeholder layout");

// A ring buffer of 64 requests.
struct UnkSubAreaRequestQueue {
    UnkSubAreaRequest* read;
    UnkSubAreaRequest* write;
    u64 count;
    UnkSubAreaRequest storage[64];
};

// An owned polymorphic object (slot 0x8 is its deleting destructor), deleted when released.
struct UnkSubAreaObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
};
struct UnkSubAreaObjectPtr {
    UnkSubAreaObject* ptr;
    ~UnkSubAreaObjectPtr() {
        UnkSubAreaObject* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x8();
    }
};

// A 0x188-byte record destroyed out of line.
struct UnkSubAreaRecord_0x188;
void fn_7100faa3d8(UnkSubAreaRecord_0x188* self);
struct UnkSubAreaRecord_0x188 {
    u8 data[0x188];
    ~UnkSubAreaRecord_0x188() { fn_7100faa3d8(this); }
};

// An owned id set (src/game_s7/fn_7100ef1e1c.cpp destroys one), deleted when released.
struct UnkNamedIds;
void fn_7100ef1e1c(UnkNamedIds* self);
struct UnkNamedIdsPtr {
    UnkNamedIds* ptr;
    ~UnkNamedIdsPtr() {
        UnkNamedIds* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_7100ef1e1c(p);
            operator delete(p);
        }
    }
};

// A list of collision objects (+0x8 of a node holds one at +0x10).
struct UnkCollisionObject;
struct UnkSubAreaColliderRef {
    u8 pad_0x0[0x10];
    UnkCollisionObject* field_0x10;
};
struct UnkSubAreaColliderNode {
    u8 pad_0x0[0x8];
    UnkSubAreaColliderRef* field_0x8;
    UnkSubAreaColliderNode* next;
};

// The member at +0x8d8, emptied out of line (fn_7100d418c8): a container whose node list starts
// at +0x40.
struct UnkSubAreaColliders {
    u8 pad_0x0[0x40];
    UnkSubAreaColliderNode* head;
};

// The manager (a placeholder for the fields used here).
struct CSubAreaManager {
    u8 pad_0x0[0x108];
    u32 field_0x108;
    u32 field_0x10c;
    u32 field_0x110;
    u8 pad_0x114[0x44];
    UnkIdSet field_0x158;
    u8 pad_0x248[0xf0];
    base::UnkVector<base::UnkStrIdMember> field_0x338;
    base::UnkVector<UnkPathIdMember> field_0x360;
    base::UnkVector<UnkPathIdMember> field_0x388;
    u8 pad_0x3b0[0x40];
    UnkSubArea* field_0x3f0;  // the current sub-area
    UnkSubArea* field_0x3f8;  // the previous one
    u8 pad_0x400[0x18];
    UnkSubAreaRequestQueue field_0x418;
    base::UnkVector<UnkSubAreaObjectPtr> field_0x830;
    UnkSubArea* field_0x858;
    base::UnkVector<UnkSubAreaRecord_0x188> field_0x860;
    base::UnkVector<UnkNamedIdsPtr> field_0x888;
    u8 pad_0x8b0[0x28];
    UnkSubAreaColliders field_0x8d8;
    u8 pad_0x920[0x10];
    void* field_0x930;
    u8 pad_0x938[0x80];
    bool field_0x9b8;
    CStrId field_0x9c0;
    CStrId field_0x9c8;
    CStrId field_0x9d0;
    u8 pad_0x9d8[0x109];
    bool field_0xae1;
};
static_assert(__builtin_offsetof(CSubAreaManager, field_0x338) == 0x338, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0x418) == 0x418, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0x830) == 0x830, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0x8d8) == 0x8d8, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0x930) == 0x930, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0x9c0) == 0x9c0, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0x3f0) == 0x3f0, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0xae1) == 0xae1, "CSubAreaManager placeholder layout");

void fn_7100ee34f0(CSubAreaManager* self);
void fn_7100ee2ebc(CSubAreaManager* self, const char* path);
void fn_7100ee2768(CSubAreaManager* self);
void fn_7100ee400c(CSubAreaManager* self);

// The scenario file `name`.`ext` of the scenario `scenario` (the game manager's root folder first).
CFilePathStrId fn_7100e01d98(UnkGameManager* self, const CStrId& scenario, const CStrId& name, const CStrId& ext);
// The extension of the file read last.
extern CStrId g_unk_71729daef0 __attribute__((visibility("hidden")));

extern UnkRegistry_0x70* g_unk_7101d4b5a0;

// An id built from a string out of line, released like CStrId.
struct UnkBuiltId {
    union {
        CStrId id;
    };

    explicit UnkBuiltId(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltId() { id.~CStrId(); }
};

// The pooled data behind a file-path id as its text accessor sees it: the text at +0x10 and a
// count of the reads at +0x44.
struct UnkIdTextData {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    s32 reads;
};
static inline const char* PathText(const CFilePathStrId& id) {
    ++reinterpret_cast<UnkIdTextData*>(id.data)->reads;
    const char* s = reinterpret_cast<UnkIdTextData*>(id.data)->str.data;
    return s ? s : g_unk_710162e2ed;
}

// Loads the scenario's collision (`name`.bmscc) and then the file with g_unk_71729daef0's
// extension, when the path registry knows it.
void fn_7100ee327c(CSubAreaManager* self, const CRntString& scenario, const CStrId& name) {
    self->field_0xae1 = false;
    fn_7100ee34f0(self);
    CFilePathStrId collision =
        fn_7100e01d98(g_unk_7101d47310, UnkBuiltId(scenario).id, name, CStrId("bmscc", true));
    fn_7100ee2ebc(self, PathText(collision));
    fn_7100ee2768(self);
    CFilePathStrId path = fn_7100e01d98(g_unk_7101d47310, UnkBuiltId(scenario).id, name, g_unk_71729daef0);
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    bool known;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, &path);
        known = fn_7100087850(reg, &s);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    if (known)
        fn_7100ee30dc(UnkFileReadTag(), &path);
    fn_7100ee400c(self);
}

// The sub-area's ids, the global empty id without one.
static inline const CStrId& FirstId(const UnkSubArea* area) { return area ? area->field_0x0 : g_unk_7101d470e0; }
static inline const CStrId& SecondId(const UnkSubArea* area) { return area ? area->field_0x50 : g_unk_7101d470e0; }

// A bit mask passed by value (copied into a temporary).
struct UnkMask {
    u32 bits;
    UnkMask() : bits(0) {}
    UnkMask(u32 b) : bits(b) {}
    UnkMask(const UnkMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkMaskOf {
    static const UnkMask& None() {
        static UnkMask s;
        return s;
    }
    static const UnkMask& All() {
        static UnkMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkSubAreaMaskTag;

// The id table (fn_7100031894 builds it; src/game/CGameManager.cpp has the same definition).
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

// The camera holder fn_7100defcec gives and its current camera controller (game/UnkCameraCtrl.h).
void fn_7100723120(void* holder);
bool fn_71010e4168(void* holder, const CStrId* id, bool flag);
bool fn_7100023d00(const UnkCameraCtrl* object, const base::reflection::CClass* cls);

struct UnkPlayerSub {
    u8 pad_0x0[0x6c];
    bool field_0x6c;
};
struct UnkPlayer {
    u8 pad_0x0[0x1c0];
    UnkPlayerSub* field_0x1c0;
};
UnkPlayer* fn_7100dec788(UnkGameManager* manager);

struct UnkScenarioSub {
    u8 pad_0x0[0x58];
    bool field_0x58;
};
struct UnkScenario {
    u8 pad_0x0[0x820];
    UnkScenarioSub* field_0x820;
};

// Helpers that take an empty value first (their callers leave x0 as it is).
struct UnkSubAreaTag {};
void* fn_7100eea4c8(UnkSubAreaTag, UnkSubArea* area, bool a, bool b, UnkMask mask);
void* fn_7100eea618(UnkSubAreaTag, UnkSubArea* area, bool flag);

void fn_7100eead58(CSubAreaManager* self, const CStrId& prev_a, const CStrId& prev_b, const CStrId& cur_a,
                   const CStrId& cur_b);
void fn_7100ee6d00(CSubAreaManager* self, UnkSubArea* prev, UnkSubArea* cur, UnkIdSet* set, bool flag);
void fn_7100ee8ea4(CSubAreaManager* self, UnkIdSet* set);
void fn_7100fac120(UnkSubArea* area, UnkMask mask, bool a, bool b, void* c, bool d);
void fn_7100d59dd8(UnkScenarioSub* self);
void fn_7100eeaa48(CSubAreaManager* self, const CStrId& prev_a, const CStrId& prev_b, const CStrId& cur_a,
                   const CStrId& cur_b, bool flag);
void fn_7100ee6884(CSubAreaManager* self);
void fn_7100eeaed0(CSubAreaManager* self);
void fn_7100ee62bc(CSubAreaManager* self, UnkSubArea* area);

// Makes `area` the current sub-area (unless it already is and `force` is unset).
// NON_MATCHING(minor): scheduling only: the flag's `and w, #1` is placed before the store of the
// previous sub-area (another register), and the two csels picking the current sub-area's ids for
// fn_7100eeaa48 are swapped.
void fn_7100ee4f84(CSubAreaManager* self, UnkSubArea* area, bool force, bool flag) {
    if (self->field_0x3f0 == area && !force)
        return;
    self->field_0x3f8 = self->field_0x3f0;
    self->field_0x3f0 = area;
    area->field_0x181 = flag;
    CStrId prev_a(FirstId(self->field_0x3f8));
    CStrId prev_b(SecondId(self->field_0x3f8));
    CStrId cur_a(FirstId(self->field_0x3f0));
    CStrId cur_b(SecondId(self->field_0x3f0));
    fn_7100eead58(self, prev_a, prev_b, cur_a, cur_b);
    bool cleared = false;
    if (void* holder = fn_7100defcec(g_unk_7101d47310)) {
        if (UnkCameraCtrl* camera = fn_71010e2080(holder)) {
            const CStrId& id = *reinterpret_cast<CStrId*>(GameManagerIdTable().pad_0x0 + 0x868);
            if (camera->field_0x20.data == id.data) {
                fn_7100723120(holder);
                camera = fn_71010e2080(holder);
                if (!camera)
                    goto update;
                fn_71010e4168(holder, &camera->field_0x20, false);
            }
            if (fn_7100023d00(camera, fn_71010e7f90()) && !force) {
                fn_7100ee7134(&self->field_0x158);
                cleared = true;
            }
        }
    }
update:
    if (!cleared)
        fn_7100ee6d00(self, self->field_0x3f8, self->field_0x3f0, &self->field_0x158, false);
    UnkSubArea* prev = self->field_0x3f8;
    UnkSubArea* next = self->field_0x3f0;
    if (!prev) {
        fn_7100eea4c8(UnkSubAreaTag(), next, false, true, UnkMask(0x1c0));
        fn_7100eea618(UnkSubAreaTag(), self->field_0x3f0, true);
        fn_7100ee8ea4(self, &self->field_0x158);
        UnkScenario* scenario = reinterpret_cast<UnkScenario*>(g_unk_7101d47310->field_0x608);
        UnkScenarioSub* sub = scenario ? scenario->field_0x820 : nullptr;
        if (sub)
            sub->field_0x58 = false;
        fn_7100fac120(self->field_0x3f0, UnkMaskOf<UnkSubAreaMaskTag>::All(), true, false, nullptr, false);
        if (sub) {
            sub->field_0x58 = true;
            fn_7100d59dd8(sub);
        }
        UnkSubArea* cur = self->field_0x3f0;
        const CStrId& cur_b = SecondId(cur);
        fn_7100eeaa48(self, FirstId(prev), SecondId(prev), FirstId(cur), cur_b, true);
    } else if (!self->field_0x158.IsEmpty() || self->field_0x108 == 2) {
        fn_7100ee6884(self);
    } else if (next != prev) {
        fn_7100eeaed0(self);
    }
    if (self->field_0x3f8)
        self->field_0x3f8->field_0x181 = false;
    UnkSubArea* cur = self->field_0x3f0;
    if (cur->field_0x181) {
        if (self->field_0x110 == 2) {
            UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
            if (player && player->field_0x1c0)
                player->field_0x1c0->field_0x6c = false;
            self->field_0x110 = 0;
        }
        self->field_0x110 = 1;
        u32 old = self->field_0x108;
        if (self->field_0x3f0->field_0x181)
            self->field_0x108 = 1;
        if (self->field_0x108 == 1) {
            self->field_0x10c = 0;
            if (self->field_0x3f0->field_0x181)
                self->field_0x108 = old;
            fn_7100ee62bc(self, self->field_0x3f0);
        }
    }
    self->field_0x3f0->field_0x181 = false;
}

// What the reset (fn_7100ee34f0) calls. Names are placeholders.
extern u32 g_unk_71727bebb8;  // the kind the main camera is looked up with
base::reflection::UnkReflObject* fn_7100d786c0(void* entity);  // its "CAMERA" component
UnkCameraCtrl* fn_71010e389c(void* holder, void* key);
void fn_71010ea4fc(UnkCameraCtrl* ctrl);

// A request taken off the queue: the id moved out of the queued one (released as CStrId is), and
// the flags.
struct UnkPoppedRequest {
    union {
        CStrId id;
    };
    UnkSubAreaRequestFlags flags;

    // The moved-from id is cleared through an untyped store: the original reloads the queue's read
    // pointer after it.
    explicit UnkPoppedRequest(UnkSubAreaRequest& r) {
        typedef base::global::UnkStringData* __attribute__((may_alias)) AnyData;
        id.data = r.id.data;
        *reinterpret_cast<AnyData*>(&r.id.data) = &g_unk_7101d47108->empty;
        flags = r.flags;
    }
    ~UnkPoppedRequest() { id.~CStrId(); }
};
void fn_7100ee4550(CSubAreaManager* self, const UnkPoppedRequest* request, bool a, bool b);
void fn_7100ee4704(CSubAreaManager* self, const UnkPoppedRequest* request, bool a, bool b);

// The GUI root (src/game/CCutscene.cpp) and a 0x48-byte singleton created on first use.
extern void* g_unk_7101d47330;
#include "game/UnkSubAreaGuiCache.h"
extern UnkSubAreaGuiCache* g_unk_71727254f8;
void fn_710119a110(void* root, const UnkPathIdMember* id);
void fn_710119e684(UnkSubAreaGuiCache* self, const base::UnkStrIdMember* id, bool flag);
void fn_710119d9b8(UnkSubAreaGuiCache* self, const base::UnkStrIdMember* id, bool flag);
void fn_710119d910(UnkSubAreaGuiCache* self);
void fn_7100de6ee8(UnkGameManager* self);
void fn_71011aa820(UnkModelManager* self);

// The entities whose component (+0x10) of class fn_7101117478 is told about each id.
struct UnkSubAreaEntity {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};
struct UnkSubAreaEntityList {
    UnkSubAreaEntity** data;
    u32 count;
};
UnkSubAreaEntityList* fn_7101119514();
void fn_710111b138(base::reflection::UnkReflObject* component, const base::UnkStrIdMember* id);

// The manager at g_unk_7101d47318 as far as it is called here.
struct UnkDropTarget;
extern UnkDropTarget* g_unk_7101d47318;
struct UnkSubAreaDropTargetView {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(u32 a, u32 b);
};

using base::input::UnkRumbleManager;
extern UnkRumbleManager* g_unk_7101d47348;
void fn_7100073f48(UnkRumbleManager* self, u32 a, u32 b);
void fn_71002191a4(UnkRenderManager* self, u32 value);

// The collision manager singleton (src/game/CCollisionComponent/fn_7100863a8c.cpp).
struct UnkManager_7100863a8c;
extern UnkManager_7100863a8c* g_unk_71726a4788;
void fn_7100d46804(UnkManager_7100863a8c* self);  // constructor
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* obj);
void fn_7100d476e4(UnkManager_7100863a8c* self, void* obj);

// The file device singleton and the sound manager (src/game/CGameManager.cpp).
extern base::file::UnkFileDevice* g_unk_7101d4d828;
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_710008987c(base::file::UnkFileDevice* self, const CFilePathStrId* path, u32 flags);
void fn_7100089894(base::file::UnkFileDevice* self);
void fn_7100108c94(UnkSoundMgr* self, const CFilePathStrId* path);
// The file-path id array's element accessor (UnkVector::GrowAt, out of line).
UnkPathIdMember* fn_710008c174(base::UnkVector<UnkPathIdMember>* self, u32 index);

void fn_7100ee48ec(CSubAreaManager* self);
void fn_710109d0c0();
void fn_7100d418c8(UnkSubAreaColliders* self);

static inline UnkManager_7100863a8c* CollisionManager() {
    if (!g_unk_71726a4788) {
        UnkManager_7100863a8c* m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return g_unk_71726a4788;
}

static inline UnkCollisionObject* ColliderOf(const UnkSubAreaColliderNode* n) {
    return n->field_0x8 ? n->field_0x8->field_0x10 : nullptr;
}

// Resets the main camera's controller when it is the CAnimatedCameraCtrl one.
static inline void ResetMainCameraCtrl() {
    UnkGameManager* gm = g_unk_7101d47310;
    u32 kind_value = g_unk_71727bebb8;
    u64 name = fn_7100001570("MainCamera", 10);
    alignas(8) u32 kind = kind_value;  // stored after the hash, in an 8-byte slot
    void* camera = fn_7100deba8c(gm, &name, &kind);
    bool skip = fn_7100deac04(g_unk_7101d47310);
    if (camera && !skip) {
        if (base::reflection::UnkReflObject* component = fn_7100d786c0(camera)) {
            UnkCameraCtrl* ctrl = fn_71010e389c(component, GameManagerIdTable().pad_0x0 + 0x7c8);
            if (ctrl && ctrl->vf_0x0() == fn_71010e7f90()) {
                fn_71010ea4fc(ctrl);
                reinterpret_cast<void (*)(UnkCameraCtrl*)>(ctrl->vtable[0x38 / 8])(ctrl);
            }
        }
    }
}

// Resets the manager: releases its ids, leaves the current and previous sub-areas (and the one at
// +0x858), resets the main camera's controller when it is the CAnimatedCameraCtrl one, handles the
// queued requests, tells the GUI, the entities and the file device to drop the loaded ids, and
// empties the arrays.
// NON_MATCHING(minor): register allocation and block layout only: the collision manager's GOT
// address is loaded inside the +0x930 branch and again before the collider loop instead of once
// ahead of that branch (so the sound manager's and the file-path pool's GOT registers differ), the
// collider loop loads the collider into a scratch register and reloads it for the call instead of
// keeping it in x1 (8 bytes longer), and the clears of the two owned-pointer arrays (+0x830, +0x888)
// reload the count at the end of the body instead of at the loop head.
void fn_7100ee34f0(CSubAreaManager* self) {
    self->field_0x9b8 = false;
    self->field_0x9c0.Reset();
    self->field_0x9c8.Reset();
    self->field_0x9d0.Reset();
    if (UnkSubArea* area = self->field_0x858) {
        fn_7100fac120(area, UnkMaskOf<UnkSubAreaMaskTag>::All(), false, true, nullptr, false);
        self->field_0x858 = nullptr;
    }

    ResetMainCameraCtrl();

    if (UnkSubArea* area = self->field_0x3f8) {
        fn_7100fac120(area, UnkMaskOf<UnkSubAreaMaskTag>::All(), false, true, nullptr, false);
        self->field_0x3f8 = nullptr;
    }
    if (UnkSubArea* area = self->field_0x3f0) {
        fn_7100fac120(area, UnkMaskOf<UnkSubAreaMaskTag>::All(), false, true, nullptr, false);
        self->field_0x3f0 = nullptr;
    }

    UnkSubAreaRequestQueue& queue = self->field_0x418;
    while (queue.count != 0) {
        UnkPoppedRequest request(*queue.read);
        queue.read->~UnkSubAreaRequest();
        UnkSubAreaRequest* begin = queue.storage;
        UnkSubAreaRequest* next = queue.read + 1;
        queue.read = next == begin + 64 ? begin : next;
        --queue.count;
        if (request.flags.field_0x0)
            fn_7100ee4550(self, &request, request.flags.field_0x1 != 0, false);
        else
            fn_7100ee4704(self, &request, request.flags.field_0x1 != 0, false);
    }

    void* gui = g_unk_7101d47330;
    UnkSubAreaGuiCache* cache = g_unk_71727254f8;
    if (!cache) {
        cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    UnkGameManager* gm = g_unk_7101d47310;
    UnkModelManager* models = g_unk_71726b0cd8;
    if (!models) {
        models = static_cast<UnkModelManager*>(fn_71000839a0(sizeof(UnkModelManager), 0x3f));
        fn_71011a870c(models);
        g_unk_71726b0cd8 = models;
    }
    for (UnkPathIdMember* it = self->field_0x360.data; it != self->field_0x360.data + self->field_0x360.count; ++it)
        fn_710119a110(gui, it);
    for (base::UnkStrIdMember *it = self->field_0x338.data, *end = it + self->field_0x338.count; it != end; ++it)
        fn_710119e684(cache, it, true);
    fn_7100de6ee8(gm);
    for (base::UnkStrIdMember *it = self->field_0x338.data, *end = it + self->field_0x338.count; it != end; ++it)
        fn_710119d9b8(cache, it, true);
    fn_7100de6ee8(gm);
    UnkSubAreaEntityList* entities = fn_7101119514();
    for (u32 i = 0; i < entities->count; ++i) {
        UnkSubAreaEntity* e = entities->data[i];
        base::reflection::UnkReflObject* component = e ? e->field_0x10 : nullptr;
        if (component && !fn_710009d50c(component->vf_0x0(), fn_7101117478()))
            component = nullptr;
        for (base::UnkStrIdMember *it = self->field_0x338.data, *end = it + self->field_0x338.count; it != end;
             ++it)
            fn_710111b138(component, it);
    }
    fn_71011aa820(models);
    fn_710119d910(cache);
    fn_7100de6ee8(gm);

    reinterpret_cast<UnkSubAreaDropTargetView*>(g_unk_7101d47318)->vf_0x38(0, 0);
    UnkRumbleManager* rumble = g_unk_7101d47348;
    if (!rumble) {
        rumble = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(rumble);
        g_unk_7101d47348 = rumble;
    }
    fn_7100073f48(rumble, 0, 0);
    fn_71002191a4(g_unk_7101d47300, 4);
    if (self->field_0x930) {
        fn_7100d476e4(CollisionManager(), self->field_0x930);
        self->field_0x930 = nullptr;
    }

    base::file::UnkFileDevice* device = g_unk_7101d4d828;
    if (!device) {
        device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    base::UnkVector<UnkPathIdMember>* paths = &self->field_0x388;
    for (s32 i = (s32)paths->count - 1; i != -1; --i) {
        fn_710008987c(device, reinterpret_cast<const CFilePathStrId*>(fn_710008c174(paths, i)), 0);
        fn_7100108c94(g_unk_7101d4d820, reinterpret_cast<const CFilePathStrId*>(fn_710008c174(paths, i)));
    }
    fn_7100ee48ec(self);
    fn_7100089894(device);
    paths->Clear();

    fn_710109d0c0();
    UnkSubAreaColliders& colliders = self->field_0x8d8;
    for (UnkSubAreaColliderNode* n = colliders.head; n; n = n->next) {
        if (ColliderOf(n))
            fn_7100d46c60(CollisionManager(), ColliderOf(n));
    }
    fn_7100d418c8(&colliders);
    self->field_0x830.Clear();
    self->field_0x860.Clear();
    self->field_0x360.Clear();
    self->field_0x338.Clear();
    self->field_0x888.Clear();
    self->field_0x110 = 0;
}

// The first argument of the two settings steps below: their caller (fn_7100ee5dcc) leaves x0 as
// the previous call returned it, and neither reads it.
struct UnkLeftoverArg {};

// The environment manager singleton (src/engine/UnkStartupManagers.cpp creates it the same way)
// and the three settings it takes from a sub-area, each from one slot of the sub-area's table.
// The singleton and its constructor keep the untyped declarations the other files share.
struct UnkEnvironmentManager;
extern void* g_unk_7101d47368;
void fn_7100d8ced8(void* self);  // constructor
void fn_7100d8e32c(UnkEnvironmentManager* self, const CStrId* id, bool a, bool b);  // slot 6
void fn_7100d8dac0(UnkEnvironmentManager* self, const CStrId* id, bool a, bool b);  // slot 7
void fn_7100d8df84(UnkEnvironmentManager* self, const CStrId* id, bool a, bool b);  // slot 8
// The sub-area's slot `index`: an id in its table at +0x68.
const CStrId* fn_7100fac818(UnkSubArea* area, u32 index);
// The id the three settings are reset with when there is no sub-area.
extern CStrId g_unk_71729daf08 __attribute__((visibility("hidden")));

static inline UnkEnvironmentManager* EnvironmentManager() {
    if (!g_unk_7101d47368) {
        void* m = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(m);
        g_unk_7101d47368 = m;
    }
    return static_cast<UnkEnvironmentManager*>(g_unk_7101d47368);
}

// Hands the sub-area's settings for the slots set in `mask` (6, 8, then 7) to the environment
// manager; without a sub-area, resets all three (`b` is then false).
void fn_7100eea4c8(UnkLeftoverArg, UnkSubArea* area, bool a, bool b, const UnkMask* mask) {
    UnkEnvironmentManager* env = EnvironmentManager();
    if (!area) {
        fn_7100d8e32c(env, &g_unk_71729daf08, a, false);
        fn_7100d8dac0(env, &g_unk_71729daf08, a, false);
        fn_7100d8df84(env, &g_unk_71729daf08, a, false);
        return;
    }
    u32 bits = mask->bits;
    if (bits >> 6 & 1) {
        fn_7100d8e32c(env, fn_7100fac818(area, 6), a, b);
        bits = mask->bits;
    }
    if (bits >> 8 & 1) {
        fn_7100d8df84(env, fn_7100fac818(area, 8), a, b);
        bits = mask->bits;
    }
    if (bits >> 7 & 1)
        fn_7100d8dac0(env, fn_7100fac818(area, 7), a, b);
}

// The sub-area's camera settings as fn_7100eea618 reads them.
struct UnkSubAreaCameraSettings {
    u8 pad_0x0[0x100];
    f32 field_0x100;
    bool field_0x104;
};

// What the camera step calls: the CMetroidCameraCtrl controller takes a value and a flag, and a
// flag with the camera's id.
void fn_7100935cb8(f32 value, UnkCameraCtrl* ctrl, bool flag);
void fn_71009349b0(UnkCameraCtrl* ctrl, bool flag, const CStrId* id);

// Hands the sub-area's camera settings to the main camera's controller when it is the
// CMetroidCameraCtrl one.
void fn_7100eea618(UnkLeftoverArg, UnkSubArea* area, bool flag) {
    UnkGameManager* gm = g_unk_7101d47310;
    u32 kind_value = g_unk_71727bebb8;
    u64 name = fn_7100001570("MainCamera", 10);
    alignas(8) u32 kind = kind_value;  // stored after the hash, in an 8-byte slot
    void* camera = fn_7100deba8c(gm, &name, &kind);
    if (!camera)
        return;
    base::reflection::UnkReflObject* component = fn_7100d786c0(camera);
    if (!component)
        return;
    UnkCameraCtrl* ctrl = fn_71010e389c(component, GameManagerIdTable().field_0x17e0);
    if (ctrl && ctrl->vf_0x0() == fn_7100932fa4()) {
        const UnkSubAreaCameraSettings* settings = reinterpret_cast<const UnkSubAreaCameraSettings*>(area);
        fn_7100935cb8(settings->field_0x100, ctrl, flag);
        static CStrId s_id("SubareaCamera", true);
        fn_71009349b0(ctrl, settings->field_0x104, &s_id);
    }
}

// ---- fn_7100ee2ebc: loading the scenario's collision file ----

#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"

// The collision file's reader (0x18 bytes; its constructor and destructor), and the read that
// fills the records (src/game/CCollisionComponent's unit).
struct UnkCollisionFileReader {
    u8 unk_0x0[0x18];
};
void fn_7100d45fc4(UnkCollisionFileReader* self, void* arg);
void fn_7100d45fd4(UnkCollisionFileReader* self);
void fn_7100860830(UnkCollisionFileReader* self, const char* path, base::UnkVector<UnkCollisionRecord>* out,
                   bool a, void* b);

// A collision object as the sub-area manager keeps it: weakly referenced, owned by the manager's
// object at +0x920, and named by the id at +0x20.
struct UnkSubAreaCollision : UnkWeakTarget {
    void* field_0x8;
    void* owner;  // 0x10
    void* field_0x18;
    CStrId name;  // 0x20
};

// The colliders' map insert (src/game/logic/collision/CCollider/fn_7100d419c0.cpp).
template <class Node>
struct UnkKeyedMap;
struct UnkKeyedNode;
UnkKeyedNode* fn_7100d41b00(UnkKeyedMap<UnkKeyedNode>* self, const CStrId& key, const UnkRefPtr& value);

// Reads the collision objects of the file at `path` and adds each one, weakly referenced, to the
// colliders under its name.
// NON_MATCHING(minor): the weak handle's branches are laid out differently (the original stores the
// empty reference in each of them and keeps a separate exit for each).
void fn_7100ee2ebc(CSubAreaManager* self, const char* path) {
    UnkCollisionFileReader reader;
    fn_7100d45fc4(&reader, nullptr);
    base::UnkVector<UnkCollisionRecord> records(0xf);
    fn_7100860830(&reader, path, &records, true, nullptr);
    for (u64 i = 0; i < records.count; ++i) {
        auto* object = reinterpret_cast<UnkSubAreaCollision*>(records.data[i].field_0x28);
        object->owner = reinterpret_cast<u8*>(self) + 0x920;
        const CStrId& name = object->name;
        UnkRefPtr ref;
        UnkWeakHandle* handle = GetWeakHandle(object);
        if (handle)
            ++handle->ref_count;
        ref.field_0x0 = handle;
        fn_7100d41b00(reinterpret_cast<UnkKeyedMap<UnkKeyedNode>*>(&self->field_0x8d8), name, ref);
    }
    records.Free();
    fn_7100d45fd4(&reader);
}

// ---- fn_7100ee6d00 and fn_7100ee8044: what a change of sub-area loads and unloads ----

// A list of file-path ids (base/UnkResPathVec.h), assigned by fn_71000d0958.
#include "base/UnkResPathVec.h"
void fn_71000d0958(UnkResPathVec* self, const UnkResPathVec* other);

// The file-path ids of a sub-area's resources (or of the same list in another object).
void fn_7100ee7e74(void* source, UnkResPathVec* out);

// The two steps before the files: one fills the set's first two lists and says whether everything
// is to be reloaded, the other the next two.
bool fn_7100ee7438(CSubAreaManager* self, UnkSubArea* prev, UnkSubArea* cur, UnkIdList* list, UnkIdSet* set,
                   bool flag);
void fn_7100ee7808(CSubAreaManager* self, UnkSubArea* prev, UnkSubArea* cur, UnkIdList* a, UnkIdList* b,
                   bool all);
bool fn_7100ee8470(CSubAreaManager* self, void* prev, void* cur, UnkIdList* list, UnkIdSet* set, bool flag);
void fn_7100ee8838(CSubAreaManager* self, void* prev, void* cur, UnkIdList* a, UnkIdList* b, bool all);

static inline UnkResPathVec* AsPaths(UnkIdList* list) { return reinterpret_cast<UnkResPathVec*>(list); }

// Appends a counted copy of `id` (PushBack's growth, including its hang).
static inline void Append(UnkResPathVec* v, const CFilePathStrId& id) {
    u32 n = v->count;
    u32 c = v->capacity;
    if (c <= n && v->step != 0)
        c = (n / v->step + 1) * v->step;
    if ((s32)c <= (s32)n)
        for (;;) {
        }
    fn_7100018450(v, c);
    new (&v->data[v->count]) CFilePathStrId(id);
    ++v->count;
}

static inline bool Contains(const UnkResPathVec& v, const CFilePathStrId& id) {
    for (u32 i = 0; i < v.count; ++i)
        if (v.data[i].data == id.data)
            return true;
    return false;
}

static inline void Release(UnkResPathVec* v) {
    if (v->data) {
        for (u64 i = 0; i < v->count; ++i)
            reinterpret_cast<CFilePathStrId*>(&v->data[i])->~CFilePathStrId();
        v->count = 0;
        v->allocator->vf_0x48(v->data);
        v->capacity = 0;
    }
}

// The files the previous sub-area had and the current one does not go to the set's list at 0xa0
// (to unload); those the current one has and the previous did not go to 0xc8 (to load). When the
// current one has none, or everything is reloaded, the two lists are the whole of each.
__attribute__((always_inline)) static inline void DiffFiles(UnkIdSet* set, UnkResPathVec& prev, UnkResPathVec& cur, bool all) {
    if (cur.count != 0 && !all) {
        for (u32 i = 0; i < prev.count; ++i)
            if (!Contains(cur, prev.data[i]))
                Append(AsPaths(&set->field_0xa0), prev.data[i]);
        for (u32 i = 0; i < cur.count; ++i)
            if (!Contains(prev, cur.data[i]))
                Append(AsPaths(&set->field_0xc8), cur.data[i]);
    } else {
        fn_71000d0958(AsPaths(&set->field_0xa0), &prev);
        fn_71000d0958(AsPaths(&set->field_0xc8), &cur);
    }
    Release(&prev);
    Release(&cur);
}

// What changes from sub-area `prev` to `cur` (either may be null): the id lists and the files.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_7100ee6d00(CSubAreaManager* self, UnkSubArea* prev, UnkSubArea* cur, UnkIdSet* set, bool flag) {
    fn_7100ee7134(set);
    bool all = fn_7100ee7438(self, prev, cur, &set->field_0x28, set, false);
    fn_7100ee7808(self, prev, cur, &set->field_0x50, &set->field_0x78, all);
    UnkResPathVec cur_files(0xf);
    if (cur)
        fn_7100ee7e74(reinterpret_cast<u8*>(cur) + 0x38, &cur_files);
    UnkResPathVec prev_files(0xf);
    if (prev)
        fn_7100ee7e74(reinterpret_cast<u8*>(prev) + 0x38, &prev_files);
    DiffFiles(set, prev_files, cur_files, all);
}

// The same for two other file lists (`prev` and `cur` are the lists' owners themselves).
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_7100ee8044(CSubAreaManager* self, void* prev, void* cur, UnkIdSet* set) {
    fn_7100ee7134(set);
    bool all = fn_7100ee8470(self, prev, cur, &set->field_0x28, set, false);
    fn_7100ee8838(self, prev, cur, &set->field_0x50, &set->field_0x78, all);
    UnkResPathVec cur_files(0xf);
    fn_7100ee7e74(cur, &cur_files);
    UnkResPathVec prev_files(0xf);
    fn_7100ee7e74(prev, &prev_files);
    DiffFiles(set, prev_files, cur_files, all);
}
