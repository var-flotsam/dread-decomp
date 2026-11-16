#pragma once

// A string with 0x100 bytes of inline storage (0x148 bytes): a CRntString whose allocator is the
// inline buffer after it. The tunables' registry and its text helpers (fn_710013c834 replaces text in
// one) build these on the stack; the runtime asset path keeps one as a guarded static. Members by
// their uses; the names and the layout are placeholders.

#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkTunableLine {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    base::global::CRntString* Str() { return reinterpret_cast<base::global::CRntString*>(str); }
    const base::global::CRntString* Str() const { return reinterpret_cast<const base::global::CRntString*>(str); }
    const char* Data() const { return Str()->data; }
    u32 Length() const { return Str()->length; }
    // The text, or the empty string for none.
    const char* CStr() const { return Data() ? Data() : g_unk_710162e2ed; }
    bool IsEmpty() const { return !Data() || !Data()[0]; }

    // An empty string over the inline buffer.
    void Init() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }

    UnkTunableLine() { Init(); }
    explicit UnkTunableLine(const char* text) {
        Init();
        fn_710007e77c(Str(), text);
    }
    // The first `length` characters of `text`.
    UnkTunableLine(const char* text, u32 length) {
        Init();
        fn_710007e7f8(Str(), text, length);
    }
    explicit UnkTunableLine(const base::global::CRntString& text) {
        Init();
        fn_710007eaa0(Str(), &text);
    }
    UnkTunableLine(const UnkTunableLine& other) : UnkTunableLine() { fn_710007eaa0(Str(), other.Str()); }
    // Inlined where it runs; the copy the runtime asset path's static registers at exit is fn_7100de9f54.
    ~UnkTunableLine() asm("fn_7100de9f54");

    UnkTunableLine& operator=(const UnkTunableLine& other) {
        fn_710007eaa0(Str(), other.Str());
        return *this;
    }
    UnkTunableLine& operator=(const base::global::CRntString& other) {
        fn_710007eaa0(Str(), &other);
        return *this;
    }
};
inline UnkTunableLine::~UnkTunableLine() {
    fn_710007ed40(Str());
    fn_710007ecd4(Str());
}
static_assert(sizeof(UnkTunableLine) == 0x148, "UnkTunableLine placeholder layout");
static_assert(__builtin_offsetof(UnkTunableLine, storage_vtable) == 0x30, "UnkTunableLine placeholder layout");
static_assert(__builtin_offsetof(UnkTunableLine, field_0x138) == 0x138, "UnkTunableLine placeholder layout");
