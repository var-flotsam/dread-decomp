// Runs a script chunk once per name: strips the spaces from the name, skips names already run (by
// their hash), else loads the chunk and, when that fails, formats and reports the error message.
#include <stdarg.h>
#include "base/global/CRntString.h"
#include "types.h"

struct UnkAllocator {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual void vf20();
    virtual void vf28();
    virtual void vf30();
    virtual void vf38();
    virtual void* Alloc(u64 size, u64 align, u32 flags);
    virtual void Free(void* p);
};

extern void* g_unk_7101a1b750[];

struct UnkStrBuffer;
void fn_710007e6e4(UnkStrBuffer* self, void* storage);
void fn_710007e77c(UnkStrBuffer* self, const char* str);
void fn_710007eaa0(UnkStrBuffer* self, const UnkStrBuffer* other);
void fn_710007ed40(UnkStrBuffer* self);

struct UnkStrStorage {
    void* vtable;
    u8 pad_0x8[0x80];
    void* field_0x88;
    u32 field_0x90;
};

// A string (header of 0x30 bytes) with its own inline storage.
struct UnkStrBuffer {
    const char* data;
    u32 length;
    u8 pad_0xc[0x14];
    u64 hash;
    u8 pad_0x28[0x8];
    UnkStrStorage storage;

    UnkStrBuffer() {
        fn_710007e6e4(this, &storage);
        storage.vtable = g_unk_7101a1b750 + 2;
        storage.field_0x88 = nullptr;
        storage.field_0x90 = 0;
    }
    ~UnkStrBuffer() {
        fn_710007ed40(this);
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this));
    }
    const char* c_str() const { return data ? data : ""; }
};

struct UnkHashVector {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    u8 pad_0x14[0x4];
    u32 flags;
    u8 pad_0x1c[0x4];
    UnkAllocator* allocator;

    bool Contains(u64 v) const {
        for (u32 i = 0; i < count; ++i)
            if (data[i] == v)
                return true;
        return false;
    }

    void PushBack(u64 v) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        u64* p;
        if (old_capacity < new_capacity) {
            p = static_cast<u64*>(allocator->Alloc((u64)new_capacity * sizeof(u64), 8, flags));
            if (u64* old = data) {
                for (u32 i = 0; i < count; ++i)
                    __builtin_memcpy(&p[i], &old[i], sizeof(u64));
                allocator->Free(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        __builtin_memcpy(&p[n], &v, sizeof(u64));
        ++count;
    }
};

struct UnkScriptRunner {
    u8 pad_0x0[0x40];
    UnkHashVector field_0x40;
};

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* str);
s32 fn_71010bf400(lua_State* L, const char* fmt, va_list args);
s32 fn_71010c0d80(lua_State* L, s32 a, char* buf, u32 size, void* x, void* y);

// NON_MATCHING(minor): the hash vector's element-copy loop compares its index as 64-bit (32-bit in the
// original) and one move is scheduled differently; same behaviour.
bool fn_71009cde54(UnkScriptRunner* self, const char* name, const char* fmt, ...) {
    char buf[0x200];
    if (!name)
        return false;
    UnkStrBuffer str;
    fn_710007e77c(&str, name);
    s32 n = 0;
    const char* p = str.data;
    for (u64 left = str.length; left != 0; --left, ++p) {
        char c = *p;
        if (c != ' ')
            buf[n++] = c;
    }
    buf[n] = 0;
    {
        UnkStrBuffer stripped;
        fn_710007e77c(&stripped, buf);
        fn_710007eaa0(&str, &stripped);
    }
    u64 hash = str.hash;
    bool result;
    if (self->field_0x40.Contains(hash)) {
        result = false;
    } else if (fn_71010c0bb0(g_unk_7172a01d18, str.c_str()) == 0) {
        self->field_0x40.PushBack(hash);
        result = false;
    } else {
        va_list args;
        va_start(args, fmt);
        s32 r = fn_71010bf400(g_unk_7172a01d18, fmt, args);
        result = fn_71010c0d80(g_unk_7172a01d18, r, buf, 0x100, nullptr, nullptr) == 1;
        va_end(args);
    }
    return result;
}
