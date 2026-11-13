// Best draft of fn_7100fc39b0 (97.6%: two loads swapped in the call arguments); the rest of this file is its unit.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkNoArg.h"
#include "base/UnkRegistry.h"
#include "base/UnkVector.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// ---- save registry


extern UnkRegistry_0x70* g_unk_7101d4b5a0;
extern CStrId g_unk_7101d592c8;

struct UnkRegistryCurrent_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CStrId& vf_0x18();
};
struct UnkRegistryIface_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual bool vf_0x18(const CStrId& id);
};

template <class T, u64 Size, void (*Ctor)(T*)>
static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

u32 fn_7100fc3828() {
    auto* current = reinterpret_cast<UnkRegistryCurrent_b0235*>(Singleton<UnkRegistry_0x70, 0x70, fn_7100087388>(g_unk_7101d4b5a0)->field_0x50);
    const CStrId& name = current ? current->vf_0x18() : g_unk_7101d592c8;
    bool ok = reinterpret_cast<UnkRegistryIface_b0235*>(Singleton<UnkRegistry_0x70, 0x70, fn_7100087388>(g_unk_7101d4b5a0))->vf_0x18(name);
    return ok ? 2 : 0;
}

// ---- 3510


namespace nn {
class Result;
namespace account {
struct Uid;
}
namespace diag::detail {
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line,
                            const Result* result, const char* format, ...);
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line);
}  // namespace diag::detail
namespace fs {
void Unmount(const char* name);
s32 EnsureSaveData(const account::Uid& user);
s32 MountSaveData(const char* name, const account::Uid& user);
}  // namespace fs
}  // namespace nn

extern const char g_unk_710162e2ed[] __attribute__((visibility("hidden")));
extern const char* g_unk_7101cab938;
extern u64 g_unk_71729efff0[2];

struct UnkUserId_b0235 {
    u64 words[2];
};
UnkUserId_b0235 fn_71001329f0();

u64 fn_7100fc3510() {
    if (g_unk_71729efff0[0] | g_unk_71729efff0[1]) {
        nn::fs::Unmount("save");
        g_unk_71729efff0[0] = 0;
        g_unk_71729efff0[1] = 0;
    }
    UnkUserId_b0235 user = fn_71001329f0();
    s32 ensure = nn::fs::EnsureSaveData(reinterpret_cast<const nn::account::Uid&>(user));
    if ((ensure & 0x1ff) == 2 && (u32)(((u32)ensure >> 9 & 0x1fff) - 0x1e) <= 0xf)
        nn::diag::detail::AbortImpl(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0);
    alignas(8) s32 result = nn::fs::MountSaveData(g_unk_7101cab938, reinterpret_cast<const nn::account::Uid&>(user));
    if (result != 0)
        nn::diag::detail::AbortImpl(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0,
                                    reinterpret_cast<const nn::Result*>(&result), g_unk_710162e2ed);
    g_unk_71729efff0[0] = user.words[0];
    g_unk_71729efff0[1] = user.words[1];
    // Hides the constant result from the callers in this file (they test it).
    u64 ok = 0;
    asm("" : "+r"(ok));
    return ok;
}

// ---- entry


extern char g_unk_7101aa9ac0[];

struct UnkEntryListNode_b0235 {
    void* value;
    UnkEntryListNode_b0235* next;
};

struct UnkEntryList_b0235 {
    void** vtable;
    union {
        u32 count;
    };
    UnkEntryListNode_b0235* head;
    union {
        UnkEntryListNode_b0235* tail;
    };
    union {
        base::UnkAllocator* allocator;
    };
    union {
        u32 flags;
    };

    __attribute__((always_inline)) ~UnkEntryList_b0235() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        for (UnkEntryListNode_b0235* n = head; n;) {
            UnkEntryListNode_b0235* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct alignas(8) UnkEntryData_b0235 {
    u8 bytes[0x80];
    CRntString name;  // 0x80
};
void fn_7100afa4c0(UnkEntryData_b0235* self, const UnkEntryData_b0235* other);

struct UnkEntryFields_b0235 {
    CRntString scenario;             // 0x0
    CRntString name;                 // 0x30
    CRntString project;              // 0x60
    CRntString s2;                   // 0x90
    CRntString s3;                   // 0xc0
    UnkEntryList_b0235 blackboards;  // 0xf0
    CRntString c;                    // 0x120
    bool valid;                      // 0x150
    UnkEntryData_b0235 data;         // 0x158

};
static_assert(sizeof(UnkEntryFields_b0235) == 0x208, "UnkEntryFields_b0235 layout");

// The element: raw storage, so no member is default-constructed ahead of the copy.
struct alignas(8) UnkEntry_b0235 {
    u8 raw[0x208];

    UnkEntry_b0235(const UnkEntry_b0235& other);
    __attribute__((always_inline)) ~UnkEntry_b0235() {
        auto* f = reinterpret_cast<UnkEntryFields_b0235*>(this);
        f->data.name.~CRntString();
        f->c.~CRntString();
        f->blackboards.~UnkEntryList_b0235();
        f->s3.~CRntString();
        f->s2.~CRntString();
        f->project.~CRntString();
        f->name.~CRntString();
        f->scenario.~CRntString();
    }
};

void fn_7100fc6fc0(UnkEntry_b0235* self, const UnkEntry_b0235* other);
inline UnkEntry_b0235::UnkEntry_b0235(const UnkEntry_b0235& other) {
    fn_7100fc6fc0(this, &other);
}

__attribute__((noinline)) void fn_7100fc6fc0(UnkEntry_b0235* self_, const UnkEntry_b0235* o_) {
    auto* self = reinterpret_cast<UnkEntryFields_b0235*>(self_);
    auto* o = reinterpret_cast<const UnkEntryFields_b0235*>(o_);
    new (&self->scenario) CRntString(o->scenario);
    new (&self->name) CRntString(o->name);
    new (&self->project) CRntString(o->project);
    new (&self->s2) CRntString(o->s2);
    new (&self->s3) CRntString(o->s3);
    self->blackboards.vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
    self->blackboards.count = 0;
    self->blackboards.head = nullptr;
    self->blackboards.tail = nullptr;
    self->blackboards.allocator = o->blackboards.allocator;
    self->blackboards.flags = o->blackboards.flags;
    for (const UnkEntryListNode_b0235* n = o->blackboards.head; n; n = n->next) {
        auto* node = static_cast<UnkEntryListNode_b0235*>(
            self->blackboards.allocator->vf_0x40(sizeof(UnkEntryListNode_b0235), 8, self->blackboards.flags));
        if (!node)
            continue;
        node->value = n->value;
        node->next = nullptr;
        if (self->blackboards.count == 0) {
            self->blackboards.head = node;
            self->blackboards.tail = node;
            self->blackboards.count = 1;
        } else {
            self->blackboards.tail->next = node;
            node->next = nullptr;
            self->blackboards.tail = node;
            ++self->blackboards.count;
        }
    }
    new (&self->c) CRntString(o->c);
    self->valid = o->valid;
    fn_7100afa4c0(&self->data, &o->data);
}

void fn_7100fc6e20(base::UnkVector<UnkEntry_b0235>* self, const UnkEntry_b0235* value) {
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (old_capacity < new_capacity) {
        auto* p = static_cast<UnkEntry_b0235*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry_b0235), 8, self->flags));
        if (self->data) {
            UnkEntry_b0235* src = self->data;
            UnkEntry_b0235* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkEntry_b0235(static_cast<UnkEntry_b0235&&>(*src));
                src->~UnkEntry_b0235();
            }
            self->allocator->vf_0x48(self->data);
        }
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    }
    new (&self->data[n]) UnkEntry_b0235(*value);
    ++self->count;
}

// ---- 36d4
struct UnkUserDataOwner;
extern UnkUserDataOwner* g_unk_7101d47308;
void fn_710120320c(UnkUserDataOwner* self);
struct UnkUserDataOwnerFlag_b48a3;
void fn_71012037fc(UnkUserDataOwnerFlag_b48a3* self, bool flag);
bool fn_7100132f60();

struct UnkSaveOperation_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual s32 vf_0x30();
};

struct UnkRegistrySelect_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(const CStrId& id);
};

s32 fn_7100fc36d4(UnkSaveOperation_b0235* self, void (*callback)(s32, s32)) {
    s32 result;
    if (fn_7100132f60()) {
        if (!g_unk_7101d47308) {
            auto* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
            fn_710120320c(owner);
            g_unk_7101d47308 = owner;
        }
        fn_71012037fc(reinterpret_cast<UnkUserDataOwnerFlag_b48a3*>(g_unk_7101d47308), true);
        result = 2;
    } else {
        auto* current = reinterpret_cast<UnkRegistryCurrent_b0235*>(
            Singleton<UnkRegistry_0x70, 0x70, fn_7100087388>(g_unk_7101d4b5a0)->field_0x50);
        const CStrId& name = current ? current->vf_0x18() : g_unk_7101d592c8;
        if (name.data != g_unk_7101d592c8.data)
            reinterpret_cast<UnkRegistrySelect_b0235*>(Singleton<UnkRegistry_0x70, 0x70, fn_7100087388>(g_unk_7101d4b5a0))
                ->vf_0x10(name);
        result = self->vf_0x30();
    }
    if (callback)
        callback(result, 0);
    return result;
}

// ---- directory walk helpers
struct UnkSaveTarget_b0235 {
    u8 pad_0x0[0x30];
    CRntString save_path;  // 0x30
    u8 pad_0x60[0x238 - 0x60];
    CRntString dest_path;  // 0x238
};

struct UnkSaveCommand_b0235 {
    u32 kind;  // 0x0
    u8 flag;   // 0x4
    u8 pad_0x5[0xb];
    UnkSaveTarget_b0235* target;  // 0x10
};

struct UnkSaveWriter_b0235 {
    u8 pad_0x0[8];
    base::global::CRntFile* file;  // 0x8
};

struct UnkDirItem_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};

struct UnkDirIter_b0235 {
    u8 pad_0x0[0x10];
    UnkDirItem_b0235** node;  // 0x10
    u8 pad_0x18[8];
};

// A string with inline storage (0x148 bytes), returned by the iterator.
struct UnkInlineString_b0235 {
    // A union member: the destructor below clears and destroys it itself.
    union {
        CRntString str;
    };
    u8 storage[0x118];
    __attribute__((always_inline)) ~UnkInlineString_b0235() {
        fn_710007ed40(&str);
        fn_710007ecd4(&str);
    }
};
static_assert(sizeof(UnkInlineString_b0235) == 0x148, "UnkInlineString_b0235 layout");

UnkDirIter_b0235 fn_7100fc7b3c(UnkSaveCommand_b0235* command);
bool fn_7100fc7c5c(UnkDirIter_b0235* it);
void fn_7100fc7b78(UnkDirIter_b0235* it);
UnkInlineString_b0235 fn_7100fc7c7c(UnkDirIter_b0235* it);
void fn_7100fc7d88(UnkDirIter_b0235* it, base::global::CRntFile* file);
void fn_710007c080(base::global::CRntFile* file, void* data, u32 flags, u64 size);
void fn_710007c144(UnkHashStream_0x38* s);
s32 fn_710002ed98(void* table, CRntString* path, void* out, u32 b, void* allocator, u32 align, u32 flags);
bool fn_7100fc7d58(UnkDirIter_b0235* it);
void fn_7100fc7cfc(UnkDirIter_b0235* it, base::global::CRntFile* file);
s32 fn_710002eec8(void* table, void* path, void* data, u64 size, bool commit, bool create);
extern const char g_unk_710164784b;  // the path separator
extern u8 g_unk_71729f0020[] __attribute__((visibility("hidden")));
u32 fn_710002ec40(void* table, CRntString* path);
s32 fn_710002ecdc(void* table, CRntString* path, void* dest, bool commit);
u32 fn_710002f2b8();

// A path: a copy of `root`, with the iterator's current name appended after a separator.
static inline __attribute__((always_inline)) CRntString JoinPath(const CRntString& root,
                                                                 UnkInlineString_b0235&& name) {
    char sep = g_unk_710164784b;
    CRntString path(root);
    if (name.str.data && name.str.data[0]) {
        fn_710007f5ac(&path, sep);
        fn_710007f3b4(&path, &name.str);
    }
    return path;
}

s32 fn_7100fc44b4(UnkNoArg, UnkSaveCommand_b0235* command) {
    UnkDirIter_b0235 it = fn_7100fc7b3c(command);
    s32 result = 0;
    while (fn_7100fc7c5c(&it)) {
        {
            CRntString path = JoinPath(command->target->save_path, fn_7100fc7c7c(&it));
            if (fn_710002ec40(g_unk_71729f0020, &path)) {
                result = 1;
            } else {
                CRntString dest = JoinPath(command->target->dest_path, fn_7100fc7c7c(&it));
                if (fn_710002ecdc(g_unk_71729f0020, &path, &dest, false))
                    result = 2;
                else
                    result = fn_710002f2b8() ? 5 : 0;
            }
        }
        fn_7100fc7b78(&it);
        if (result)
            return result;
    }
    return 0;
}

// Writes the file's contents under a path.
static inline __attribute__((always_inline)) s32 WriteFile(CRntString&& path, UnkSaveWriter_b0235* self) {
    base::global::CRntFile* file = self->file;
    u8* data = file->data;
    u64 size = file->position;
    return fn_710002eec8(g_unk_71729f0020, &path, data, size, false, true);
}

s32 fn_7100fc39b0(UnkSaveWriter_b0235* self, UnkSaveCommand_b0235* command) {
    UnkDirIter_b0235 it = fn_7100fc7b3c(command);
    while (fn_7100fc7c5c(&it)) {
        if (command->flag || fn_7100fc7d58(&it)) {
            fn_7100fc7cfc(&it, self->file);
            if (WriteFile(JoinPath(command->target->save_path, fn_7100fc7c7c(&it)), self)) {
                fn_7100fc7b78(&it);
                return 5;
            }
        }
        fn_7100fc7b78(&it);
    }
    return fn_710002f2b8() ? 5 : 0;
}

// A file read whole into a buffer that is deleted through its vtable.
struct UnkSaveBuffer_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

static inline __attribute__((always_inline)) s32 ReadFile(CRntString&& path, UnkSaveBuffer_b0235** buffer) {
    return fn_710002ed98(g_unk_71729f0020, &path, buffer, 1, nullptr, 8, 0x16);
}

s32 fn_7100fc3b00(UnkNoArg, UnkSaveCommand_b0235* command) {
    {
        UnkDirIter_b0235 it = fn_7100fc7b3c(command);
        while (fn_7100fc7c5c(&it)) {
            (*it.node)->vf_0x18();
            fn_7100fc7b78(&it);
        }
    }
    UnkDirIter_b0235 it = fn_7100fc7b3c(command);
    bool failed = false;
    while (!failed && fn_7100fc7c5c(&it)) {
        UnkSaveBuffer_b0235* buffer = nullptr;
        failed = true;
        if (ReadFile(JoinPath(command->target->save_path, fn_7100fc7c7c(&it)), &buffer) == 0) {
            {
                base::global::CRntFile file;
                fn_710007c080(&file, buffer, 0x16, 0);
                fn_7100fc7d88(&it, &file);
                fn_710007c144(&file);
                fn_710007c0e4(&file);
            }
            failed = false;
        }
        if (buffer) {
            buffer->vf_0x8();
            buffer = nullptr;
        }
        fn_7100fc7b78(&it);
    }
    return failed;
}

// ---- 38e0
struct UnkSaveSystem_b0235 {
    u8 pad_0x0[0x10];
    u8 mounted;  // 0x10
};
s32 fn_7100fc3cc0(UnkNoArg, UnkSaveCommand_b0235* command);
void fn_7100fc41e4(UnkSaveSystem_b0235* self, UnkSaveCommand_b0235* command);

UnkUserId_b0235 fn_71001329f0();

s32 fn_7100fc38e0(UnkSaveSystem_b0235* self, UnkSaveCommand_b0235* command) {
    if (self->mounted) {
        fn_71001329f0();
        if ((u32)fn_7100fc3510())
            return 5;
        self->mounted = 0;
    }
    switch (command->kind) {
    case 0:
    case 2:
        return fn_7100fc3b00(UnkNoArg(), command);
    case 1:
    case 4:
        return fn_7100fc39b0(reinterpret_cast<UnkSaveWriter_b0235*>(self), command);
    default:
        return 0x7fffffff;
    case 3:
        return fn_7100fc3cc0(UnkNoArg(), command);
    case 5:
        fn_7100fc41e4(self, command);
        return 0;
    case 6:
        return fn_7100fc44b4(UnkNoArg(), command);
    }
}
