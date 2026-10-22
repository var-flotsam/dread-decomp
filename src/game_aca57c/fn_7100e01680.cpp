// Looks up the start point to use: the object's own start point id, unless the current scenario
// overrides it (the script call fills a property bag with the result, read back as a string),
// then the start point registered under the id's hash. Layouts are placeholders.
#include "../game_s7/UnkPropBagS7.h"

using namespace unk_prop_bag;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string with 0x80 bytes of inline storage (as in fn_7100ae9a10).
struct UnkInlineString {
    const char* str;
    u8 unk_0x8[0x28];
    void** vtable;
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
};
extern void* g_unk_7101a1b750[];
void fn_710007e6e4(UnkInlineString* self, void* allocator);  // string constructor
void fn_710007ed40(UnkInlineString* self);
void fn_710007ecd4(UnkInlineString* self);
void fn_7100000850(UnkStringData** out, const UnkInlineString* str, bool flag);  // id from a string

bool fn_7100e03d78(UnkPropBag* bag, const char* function, const char* args);  // runs a script call
bool fn_7100e03f50(UnkPropBag* bag, const u64* key, UnkInlineString* out, bool flag);  // reads a string

struct UnkStartPoints;
void* fn_71011a3d74(UnkStartPoints* points, const u64* key);

struct UnkObj_7100e01680 {
    u8 unk_0x0[0x518];
    UnkInlineString field_0x518;
    u8 unk_0x5e0[0x28];
    UnkStartPoints* field_0x608;
};

// A string id built from a string out of line and released like CStrId.
struct UnkBuiltId {
    union {
        CStrId id;
    };

    explicit UnkBuiltId(const UnkInlineString& str) { fn_7100000850(&id.data, &str, true); }
    ~UnkBuiltId() { id.~CStrId(); }
};

struct UnkHashedName {
    u8 unk_0x0[0x30];
    u64 hash;
};

// NON_MATCHING(minor): the original keeps the store of the empty string into the moved-from
// temporary id (and the pool reload after a release); ours drops both as dead.
void* fn_7100e01680(UnkObj_7100e01680* self) {
    UnkBuiltId built(self->field_0x518);
    CStrId& id = built.id;
    UnkPropBag bag;
    if (fn_7100e03d78(&bag, "CurrentScenario.GetStartPointOverride", "")) {
        UnkInlineString s;
        fn_710007e6e4(&s, &s.vtable);
        s.vtable = &g_unk_7101a1b750[2];
        s.field_0xb8 = 0;
        s.field_0xc0 = 0;
        u64 key = fn_7100001570("StartPoint", 10);
        if (fn_7100e03f50(&bag, &key, &s, false) && s.str && *s.str) {
            UnkBuiltId built_tmp(s);
            CStrId& tmp = built_tmp.id;
            // Move-assign: release the old id, then take over the new one.
            id.~CStrId();
            id.data = tmp.data;
            tmp.data = &g_unk_7101d47108->empty;
        }
        fn_710007ed40(&s);
        fn_710007ecd4(&s);
    }
    if (id.data == &g_unk_7101d47108->empty)
        return nullptr;
    u64 hash = reinterpret_cast<UnkHashedName*>(id.data)->hash;
    return fn_71011a3d74(self->field_0x608, &hash);
}
