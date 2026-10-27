// From unit 2577 of tools/tu_map.py (0x7100edf654-0x7100ef3494, CSubAreaManager and
// CSubAreaManager::CTunableSubAreaManager): the sub-area manager's scenario loading and its
// change of the current sub-area. The file reader's types repeat src/game/UnkObjectFileLoader.cpp's
// (the same names and layouts; the read target and the parse context are in
// base/reflection/UnkReflObjectRead.h); names and layouts are placeholders.
// Holds fn_7100ee30dc (reads the reflected global object from a scenario file), fn_7100ee327c
// (loads a scenario's files) and fn_7100ee4f84 (makes a sub-area the current one).
#include "base/UnkHashStream.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
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

// The manager (a placeholder for the fields used here).
struct CSubAreaManager {
    u8 pad_0x0[0x108];
    u32 field_0x108;
    u32 field_0x10c;
    u32 field_0x110;
    u8 pad_0x114[0x44];
    UnkIdSet field_0x158;
    u8 pad_0x248[0x1a8];
    UnkSubArea* field_0x3f0;  // the current sub-area
    UnkSubArea* field_0x3f8;  // the previous one
    u8 pad_0x400[0x6e1];
    bool field_0xae1;
};
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

bool fn_7100087850(UnkRegistry_0x70* r, const UnkStrPair_0x10* s);
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
