// Looks up the start point to use: the object's own start point id, unless the current scenario
// overrides it (the script call fills a property bag with the result, read back as a string),
// then the start point registered under the id's hash. Layouts are placeholders.
#include "../game_s7/UnkPropBagS7.h"
#include "base/global/CRntString.h"

using namespace unk_prop_bag;

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

    explicit UnkBuiltId(const UnkInlineString& str) {
        fn_7100000850(&id, *reinterpret_cast<const base::global::CRntString*>(&str), true);
    }
    ~UnkBuiltId() { id.~CStrId(); }
};

struct UnkHashedName {
    u8 unk_0x0[0x30];
    u64 hash;
};

// NON_MATCHING(minor): the id's stack slot (8 bytes higher in the original) and the order of the
// final hash load against the start-point lookup's argument setup.
void* fn_7100e01680(UnkObj_7100e01680* self) {
    UnkBuiltId built(self->field_0x518);
    CStrId& id = built.id;
    UnkPropBag bag;
    if (fn_7100e03d78(&bag, "CurrentScenario.GetStartPointOverride", "")) {
        UnkInlineString s;
        auto* str = reinterpret_cast<base::global::CRntString*>(&s);
        fn_710007e6e4(str, reinterpret_cast<base::UnkAllocator*>(&s.vtable));
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
        fn_710007ed40(str);
        fn_710007ecd4(str);
    }
    if (id.data == &g_unk_7101d47108->empty)
        return nullptr;
    u64 hash = reinterpret_cast<UnkHashedName*>(id.data)->hash;
    return fn_71011a3d74(self->field_0x608, &hash);
}
