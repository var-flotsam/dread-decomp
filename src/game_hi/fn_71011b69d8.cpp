// Resolves a GUI script name to a path, loads it once through the lazily created registry, and returns a handle to the loaded object.
#include "base/global/CRntString.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTag_71011b69d8 {};

// A string (base::global::CRntString's layout), moved by fn_710007eab4.
struct UnkEntry_0x30 {
    u8 data[0x30];
};

struct UnkScriptPath_0x148 {
    UnkEntry_0x30 path;
    u8 unk_0x30[0x118];
};

struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

struct UnkRefCounted_71011b69d8 {
    virtual void vf_0x0();
    virtual ~UnkRefCounted_71011b69d8();
    s32 ref_count;
};

struct UnkRefHandle_71011b69d8 {
    UnkRefHandle_71011b69d8() {}
    UnkRefHandle_71011b69d8(const UnkRefHandle_71011b69d8& other);
    union {
        UnkRefCounted_71011b69d8* ptr;
    };
    void Set(UnkRefCounted_71011b69d8* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_71011b69d8* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};

struct UnkRefSource_71011b69d8 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20();
    virtual UnkRefHandle_71011b69d8* vf_0x28();
};

struct UnkRegistry_0x70 {
    u8 data[0x70];
};

UnkScriptPath_0x148 fn_71011b66fc(UnkTag_71011b69d8 tag, const char* const* name);
void fn_7100087388(UnkRegistry_0x70* r);
void fn_710008d4c0(UnkStrPair_0x10* s, const UnkEntry_0x30* e);
void fn_710008d8e8(UnkStrPair_0x10* s);
bool fn_7100087850(UnkRegistry_0x70* r, const UnkStrPair_0x10* s);
UnkRefSource_71011b69d8* fn_71011b6208(UnkTag_71011b69d8 tag, const UnkStrPair_0x10* s);
extern UnkRegistry_0x70* g_unk_7101d4b5a0;

UnkRefHandle_71011b69d8 fn_71011b69d8(UnkTag_71011b69d8 tag, const char* const* name) {
    UnkEntry_0x30 path;
    {
        UnkScriptPath_0x148 sp = fn_71011b66fc(tag, name);
        auto* sp_str = reinterpret_cast<base::global::CRntString*>(&sp.path);
        fn_710007eab4(reinterpret_cast<base::global::CRntString*>(&path), sp_str);
        fn_710007ed40(sp_str);
        fn_710007ecd4(sp_str);
    }
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    UnkRefHandle_71011b69d8 result;
    bool ok;
    {
        UnkStrPair_0x10 s;
        fn_710008d4c0(&s, &path);
        ok = fn_7100087850(reg, &s);
        fn_710008d8e8(&s);
    }
    UnkRefCounted_71011b69d8* h = nullptr;
    if (ok) {
        UnkRefSource_71011b69d8* src;
        {
            UnkStrPair_0x10 s;
            fn_710008d4c0(&s, &path);
            src = fn_71011b6208(UnkTag_71011b69d8(), &s);
            fn_710008d8e8(&s);
        }
        if (src) {
            if (UnkRefHandle_71011b69d8* slot = src->vf_0x28()) {
                h = slot->ptr;
                if (!h) {
                    char* n = static_cast<char*>(fn_71000839a0(0x18, 0x3e));
                    *reinterpret_cast<s32*>(n + 8) = 0;
                    *reinterpret_cast<void***>(n) = &g_unk_7101a209e0[2];
                    *reinterpret_cast<UnkRefSource_71011b69d8**>(n + 0x10) = src;
                    h = reinterpret_cast<UnkRefCounted_71011b69d8*>(n);
                    slot->Set(h);
                }
            }
        }
    }
    result.ptr = nullptr;
    if (h) {
        ++h->ref_count;
        result.ptr = h;
    }
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&path));
    return result;
}
