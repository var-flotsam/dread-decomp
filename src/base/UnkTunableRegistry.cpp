// The tunables' registry unit (the one lua_bind::GenerateMissingTunables reaches): groups of
// tunables (0x80 bytes each) kept in two parallel arrays, the groups and their names (CStrId),
// that share a count, a capacity and a growth step (fn_710013ac14 builds them; fn_710013c2ac
// appends; src/game_lo/fn_710013c3c8.cpp looks a group up). Names and layouts are placeholders
// inferred from the code.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkVector;
using base::global::CStrId;

struct UnkTunableGroup;

// What a group lists first: objects whose first word points back at the group.
struct UnkTunableMember {
    UnkTunableGroup* owner;
};

struct UnkTunableGroup {
    UnkVector<UnkTunableMember*> members;  // flags 0xf, like the two other lists
    UnkVector<void*> field_0x28;
    UnkVector<void*> field_0x50;
    bool field_0x78;

    // The members are detached and the three lists emptied, then freed last to first.
    ~UnkTunableGroup() {
        for (UnkTunableMember **p = members.data, **end = members.data + members.count; p != end; ++p)
            (*p)->owner = nullptr;
        members.count = 0;
        field_0x28.count = 0;
        field_0x50.count = 0;
        field_0x50.Free();
        field_0x28.Free();
        members.Free();
    }
};
static_assert(sizeof(UnkTunableGroup) == 0x80, "");

// Builds a group as a copy of another, taking its members over (out of line).
void fn_710013ef1c(UnkTunableGroup* self, UnkTunableGroup* other);

struct UnkTunableGroupArrays {
    UnkTunableGroup* groups;
    CStrId* names;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;
};

// Grows both arrays to `new_capacity`: the groups are copied and destroyed, the names moved.
void fn_710013ec80(UnkTunableGroupArrays* self, u32 new_capacity) {
    if (self->capacity < new_capacity) {
        UnkTunableGroup* groups = static_cast<UnkTunableGroup*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkTunableGroup), 8, self->flags));
        CStrId* names = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
        if (UnkTunableGroup* old = self->groups) {
            UnkTunableGroup* dst = groups;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                fn_710013ef1c(dst, old);
                old->~UnkTunableGroup();
            }
            self->allocator->vf_0x48(self->groups);
        }
        if (CStrId* old = self->names) {
            CStrId* dst = names;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                // The moved-from id is cleared through a union, so its pool is reloaded.
                union Slot {
                    base::global::UnkStringData* p;
                };
                dst->data = old->data;
                reinterpret_cast<Slot*>(&old->data)->p = &g_unk_7101d47108->empty;
                old->Reset();
            }
            self->allocator->vf_0x48(self->names);
        }
        self->groups = groups;
        self->names = names;
        self->capacity = new_capacity;
    }
}

// ---- fn_710013b0e0: reading the tunables file ----
//
// The file (g_unk_710b372708) holds lines of `[section]` headers, `key=value` pairs and `;`
// comments. A header names the tunable object the following pairs set (looked up as written, then
// with "CTunable" and "CTunableCharClass" put in front of the name, after an optional `prefix|`).
// A key may walk into members with dots (each step looks the name up in the tunable's own class,
// as the original does) and, when it isn't found, is tried with the type prefixes b, i, f, s and v.
// Values are bools (true when they start with t or T), ints, floats, strings and 3D vectors
// (VEC3(x, y, z) or (x, y, z)). With `generate`, a pair without a value gets the current one and
// the file is written back.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntStringAssign.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"

using base::global::CRntString;
using base::reflection::UnkReflTypedPtr;

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkTunableLine {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    const CRntString* Str() const { return reinterpret_cast<const CRntString*>(str); }
    const char* Data() const { return Str()->data; }
    u32 Length() const { return Str()->length; }
    // The text, or the empty string for none.
    const char* CStr() const { return Data() ? Data() : g_unk_710162e2ed; }
    bool IsEmpty() const { return !Data() || !Data()[0]; }

    UnkTunableLine() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        field_0x138 = 0;
        field_0x140 = 0;
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
    }
    UnkTunableLine(const UnkTunableLine& other) : UnkTunableLine() { fn_710007eaa0(Str(), other.Str()); }
    ~UnkTunableLine() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
    UnkTunableLine& operator=(const UnkTunableLine& other) {
        fn_710007eaa0(Str(), other.Str());
        return *this;
    }
};
static_assert(sizeof(UnkTunableLine) == 0x148, "");

// `count` characters of `str`, as a string with inline storage.
static inline UnkTunableLine Mid(const char* str, u32 count) {
    UnkTunableLine s;
    fn_710007e7f8(s.Str(), str, count);
    return s;
}

// The tunables file reader (as in src/game/CSubAreaManager.cpp): the file held in memory and two
// words the opener sets.
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};
// The id and path pair (base/UnkStrIdPathPair.h) the file is opened with.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};
bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710007c144(UnkHashStream_0x38* s);  // closes the file
// Reads a line of at most `size` bytes from the position; false when none could be read.
bool fn_710007c9e8(base::global::CRntFile* file, char* buffer, u32 size);
// Rewinds the file opened for writing to its buffer (fn_710007c1a4).
void fn_710007c610(base::global::CRntFile* file, u64 buffer);
// Writes `count` blocks of `size` bytes.
u64 fn_710007c450(base::global::CRntFile* file, const void* data, u32 size, u32 count);
// Saves the file to the path.
bool fn_710007c388(base::global::CRntFile* file, const UnkStrPair_0x10* path);
extern UnkStrPair_0x10 g_unk_710b372708 __attribute__((visibility("hidden")));  // the tunables file

s32 fn_710007facc(const CRntString* self, char c, u32 start);  // finds c from start on; -1 when absent
void fn_710007f6dc(CRntString* self);                           // trims both ends
CRntString fn_710007ef90(const CRntString* a, const char* b);   // concatenation
CRntString fn_710007f050(const char* a, const CRntString* b);   // concatenation
void fn_710007f564(void* self, const char* str);                // appends a string
// The string's key (its CRC-64) into *out.
u64* fn_7100066630(u64* out, const CRntString* str);
// A copy of `str` with every `from` replaced by `to`.
UnkTunableLine fn_710013c834(const CRntString* str, const char* from, const char* to);

// The tunable object a header names: its class is vtable slot 0.
struct UnkTunableObject {
    virtual char* GetClass();
};
struct UnkTunableRegistry;
UnkTunableObject* fn_710013c5cc(UnkTunableRegistry* self, const char* name, bool qualified);

// A reflected field (0xb0 bytes): its type at +0x10.
struct UnkTunableField {
    u8 pad_0x0[0x10];
    const base::reflection::CType* type;
    u8 pad_0x18[0x10];
    void* field_0x28;  // the accessor; without one fn_71000e74bc decides whether it can be set
    u8 pad_0x30[0x80];
};
static_assert(sizeof(UnkTunableField) == 0xb0, "");

// A type descriptor as the lookups see it: its class (vtable slot 0) and its fields by name hash.
struct UnkTunableType {
    virtual const base::reflection::CType* GetClass();
    u8 pad_0x8[0xe0];
    UnkTunableField* fields;  // +0xe8
    u64* keys;                // +0xf0
    u32 count;                // +0xf8

    UnkTunableField* End() { return fields + count; }
    UnkTunableField* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
};

// The field calls' argument: a value's type and where it is.
struct UnkTunableValue {
    const void* type;
    void* value;
};
struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
struct UnkEnumType;
bool fn_71000e7a80(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // sets
bool fn_71000e79a8(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // gets
bool fn_71000e74bc(UnkReflField* field);  // whether a field without an accessor can be set
// The member object a field of `obj` holds.
UnkReflTypedPtr fn_71000e755c(UnkReflField* field, const UnkReflTypedPtr* obj);
s32 fn_7100024140(const base::reflection::CType* type);  // the type's letter (b, f, i, s, v)
UnkEnumType* fn_710009ffd4();  // base::global::TRntString256's type

static inline void SetField(UnkTunableField* field, UnkReflTypedPtr* obj, UnkTunableValue* value) {
    fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(obj),
                  reinterpret_cast<UnkReflValueArg*>(value));
}
static inline void GetField(UnkTunableField* field, UnkReflTypedPtr* obj, UnkTunableValue* value) {
    fn_71000e79a8(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(obj),
                  reinterpret_cast<UnkReflValueArg*>(value));
}
static inline UnkReflTypedPtr Typed(char* type, void* obj) {
    UnkReflTypedPtr p;
    p.ptr = obj;
    p.type = fn_710009dcd8(type, obj);
    return p;
}

// The pieces a vector value is split into (a list of strings with inline storage, its nodes from a
// block pool; fn_71000fc570 fills it, fn_71000fc7cc destroys it).
struct UnkTunableToken {
    UnkTunableLine text;
    UnkTunableToken* next;  // +0x148
};
struct UnkTunableTokens {
    char* vtable;
    u32 count;
    UnkTunableToken* head;
    UnkTunableToken* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    char* pool_vtable;  // +0x30: the node pool (base::UnkBlockPool's layout)
    base::UnkAllocator* pool_parent;
    UnkBlockFreeList pool_blocks;
    u32 field_0x68;
    bool field_0x6c;
};
static_assert(sizeof(UnkTunableTokens) == 0x70, "");
void fn_71000fc570(const char* text, const char* separator, UnkTunableTokens* out);
void fn_71000fc7cc(UnkTunableTokens* self);

// Looks up the tunable a header names: prefix, then `middle`, then the name.
static inline UnkTunableObject* FindTunable(UnkTunableRegistry* self, const UnkTunableLine& prefix,
                                            const char* middle, const UnkTunableLine& name, bool qualified) {
    CRntString a = fn_710007ef90(prefix.Str(), middle);
    CRntString b = fn_710007eee0(&a, name.Str());
    return fn_710013c5cc(self, b.data, qualified);
}

static inline u64 HashOf(const UnkTunableLine& s) {
    const char* str = s.CStr();
    return fn_7100001570(str, strlen(str));
}

// Reads the tunables file into the tunables (see above); false when it can't be opened or a header
// isn't closed.
// NON_MATCHING(minor): written for the behaviour, block by block from the original's code (the same
// calls with the same arguments in the same order); the stack slots, register allocation, block order
// and the temporaries' placement differ, and the final NUL is written from another empty string.
bool fn_710013b0e0(UnkTunableRegistry* self, bool generate) {
    UnkFileReader reader;
    fn_710007be8c(&reader.file, 0x16, nullptr);
    bool opened;
    {
        CRntString mode("rb");
        opened = fn_710007c1ac(&reader, &g_unk_710b372708, &mode);
    }
    bool result = false;
    if (opened) {
        UnkTunableLine line;
        base::UnkVector<UnkTunableLine> lines(0xf);
        UnkTunableObject* tunable = nullptr;
        result = true;
        while (reader.file.position < reader.file.size) {
            char buffer[0x100];
            if (!fn_710007c9e8(&reader.file, buffer, 0x100))
                continue;
            fn_710007e77c(line.Str(), buffer);
            if (generate)
                lines.PushBackCopy(line);
            s32 comment = fn_710007facc(line.Str(), ';', 0);
            if (comment >= 0)
                line = Mid(line.Data(), comment);
            fn_710007f6dc(line.Str());

            s32 open = fn_710007facc(line.Str(), '[', 0);
            if (open >= 0) {
                s32 close = fn_710007facc(line.Str(), ']', open);
                if (close < 0) {
                    result = false;
                    break;
                }
                u32 start = open + 1;
                line = Mid(line.Data() + start, close - start);
                fn_710007f6dc(line.Str());
                s32 bar = fn_710007facc(line.Str(), '|', 0);
                UnkTunableLine prefix;
                fn_710007e77c(prefix.Str(), g_unk_710162e2ed);
                if (bar >= 0) {
                    u32 n = bar + 1;
                    prefix = Mid(line.Data(), n);
                    line = Mid(line.Data() + n, line.Length() - n);
                }
                bool qualified = bar >= 0;
                tunable = FindTunable(self, prefix, g_unk_710162e2ed, line, qualified);
                if (!tunable)
                    tunable = FindTunable(self, prefix, "CTunable", line, qualified);
                if (!tunable)
                    tunable = FindTunable(self, prefix, "CTunableCharClass", line, qualified);
                continue;
            }

            s32 equals = fn_710007facc(line.Str(), '=', 0);
            if (equals < 0)
                continue;
            UnkTunableLine key = Mid(line.Data(), equals);
            u32 after = equals + 1;
            UnkTunableLine value = Mid(line.Data() + after, line.Length() - after);
            fn_710007f6dc(key.Str());
            fn_710007f6dc(value.Str());
            if (!tunable)
                continue;

            // Walks the dotted members down to the last name.
            char* type = fn_710009dcd8(tunable->GetClass(), tunable);
            void* object = tunable;
            for (s32 dot = fn_710007facc(key.Str(), '.', 0); dot >= 0; dot = fn_710007facc(key.Str(), '.', 0)) {
                UnkTunableLine member = Mid(key.Data(), dot);
                u64 hash = HashOf(member);
                UnkTunableType* cls = reinterpret_cast<UnkTunableType*>(tunable->GetClass());
                UnkTunableField* field = cls->Find(hash);
                if (field == cls->End() || !field)
                    break;
                UnkReflTypedPtr outer = Typed(type, object);
                UnkReflTypedPtr inner = fn_71000e755c(reinterpret_cast<UnkReflField*>(field), &outer);
                type = inner.type;
                object = inner.ptr;
                key = Mid(key.Data() + (dot + 1), key.Length() - (dot + 1));
            }
            UnkTunableType* owner = reinterpret_cast<UnkTunableType*>(type);
            if (!owner)
                continue;
            if (!fn_710009d50c(owner->GetClass(), fn_7100096100()))
                continue;
            UnkTunableField* field = owner->Find(HashOf(key));
            if (field == owner->End())
                field = nullptr;
            if (!field) {
                static const char kPrefixes[] = "bifsv";
                u64 i = 0;
                do {
                    char prefix[2] = {kPrefixes[i], 0};
                    CRntString name = fn_710007f050(prefix, key.Str());
                    u64 hash;
                    fn_7100066630(&hash, &name);
                    field = owner->Find(hash);
                    if (field == owner->End())
                        field = nullptr;
                    ++i;
                } while (i < 5 && !field);
                if (!field)
                    continue;
            }

            if (generate && value.IsEmpty()) {
                // The line gets the current value.
                UnkTunableLine& last = lines.GrowAt(lines.count - 1);
                u32 n = fn_710007facc(last.Str(), '=', 0) + 1;
                if (n == 0xffffffff)
                    n = last.Length();
                last = Mid(last.Data(), n);
                char text[0x100];
                switch (fn_7100024140(field->type)) {
                case 'b': {
                    u8 v = 0;  // a bool, tested as a byte
                    UnkReflTypedPtr obj = Typed(type, object);
                    UnkTunableValue arg = {fn_710000d228(), &v};
                    GetField(field, &obj, &arg);
                    fn_710007e77c(value.Str(), v != 0 ? "true" : "false");
                    break;
                }
                case 'f': {
                    f32 v = 0.0f;
                    UnkReflTypedPtr obj = Typed(type, object);
                    UnkTunableValue arg = {fn_710000abbc(), &v};
                    GetField(field, &obj, &arg);
                    sprintf(text, "%f", (f64)v);
                    fn_710007e77c(value.Str(), text);
                    break;
                }
                case 'i': {
                    s32 v = 0;
                    UnkReflTypedPtr obj = Typed(type, object);
                    UnkTunableValue arg = {fn_7100014768(), &v};
                    GetField(field, &obj, &arg);
                    sprintf(text, "%i", v);
                    fn_710007e77c(value.Str(), text);
                    break;
                }
                case 's': {
                    UnkTunableLine v;
                    UnkReflTypedPtr obj = Typed(type, object);
                    UnkTunableValue arg = {fn_710009ffd4(), &v};
                    GetField(field, &obj, &arg);
                    value = v;
                    break;
                }
                case 'v': {
                    base::math::CVector3D v = {0.0f, 0.0f, 0.0f};
                    UnkReflTypedPtr obj = Typed(type, object);
                    UnkTunableValue arg = {fn_71000246e4(), &v};
                    GetField(field, &obj, &arg);
                    sprintf(text, "V3D(%f,%f,%f)", (f64)v.x, (f64)v.y, (f64)v.z);
                    fn_710007e77c(value.Str(), text);
                    break;
                }
                default:
                    break;
                }
                fn_710007f3b4(last.Str(), value.Str());
                fn_710007f564(last.Str(), "\n");
                if (value.IsEmpty())
                    continue;
            } else if (!generate && value.IsEmpty()) {
                continue;
            }

            // Sets the field from the value.
            if (!field->field_0x28 && !fn_71000e74bc(reinterpret_cast<UnkReflField*>(field)))
                continue;
            switch (fn_7100024140(field->type)) {
            case 'b': {
                UnkReflTypedPtr obj = Typed(type, object);
                bool v = (value.Data()[0] | 0x20) == 't';
                UnkTunableValue arg = {fn_710000d228(), &v};
                SetField(field, &obj, &arg);
                break;
            }
            case 'f': {
                UnkReflTypedPtr obj = Typed(type, object);
                f32 v = (f32)atof(value.Data());
                UnkTunableValue arg = {fn_710000abbc(), &v};
                SetField(field, &obj, &arg);
                break;
            }
            case 'i': {
                UnkReflTypedPtr obj = Typed(type, object);
                s32 v = atoi(value.Data());
                UnkTunableValue arg = {fn_7100014768(), &v};
                SetField(field, &obj, &arg);
                break;
            }
            case 's': {
                UnkReflTypedPtr obj = Typed(type, object);
                UnkTunableValue arg = {fn_710009ffd4(), &value};
                SetField(field, &obj, &arg);
                break;
            }
            case 'v': {
                {
                    UnkTunableLine a = fn_710013c834(value.Str(), "(", g_unk_710162e2ed);
                    UnkTunableLine b = fn_710013c834(a.Str(), ")", g_unk_710162e2ed);
                    UnkTunableLine c = fn_710013c834(b.Str(), "VEC3", g_unk_710162e2ed);
                    value = c;
                }
                UnkTunableTokens tokens;
                tokens.vtable = reinterpret_cast<char*>(g_unk_7101a1bcf0 + 2);
                tokens.count = 0;
                tokens.head = nullptr;
                tokens.tail = nullptr;
                tokens.allocator = fn_7100083990();
                tokens.flags = 0x13;
                tokens.vtable = reinterpret_cast<char*>(g_unk_7101a1bcd0 + 2);
                tokens.pool_vtable = g_unk_7101a1a4c8 + 0x10;
                tokens.pool_parent = nullptr;
                fn_710008377c(&tokens.pool_blocks);
                tokens.field_0x68 = 0;
                tokens.field_0x6c = false;
                fn_71000fc570(value.Data(), ",", &tokens);
                UnkReflTypedPtr obj = Typed(type, object);
                base::math::CVector3D v = {0.0f, 0.0f, 0.0f};
                u32 count = tokens.count;
                if (count) {
                    UnkTunableToken* t = tokens.head;
                    v.x = (f32)atof(t->text.Data());
                    if (count != 1) {
                        t = t->next;
                        v.y = (f32)atof(t->text.Data());
                        if (count >= 3)
                            v.z = (f32)atof(t->next->text.Data());
                    }
                }
                UnkTunableValue arg = {fn_71000246e4(), &v};
                SetField(field, &obj, &arg);
                fn_71000fc7cc(&tokens);
                break;
            }
            default:
                break;
            }
        }
        if (result) {
            fn_710007c144(&reader.file);
            if (generate) {
                {
                    CRntString mode("wt");
                    fn_710007c1ac(&reader, &g_unk_710b372708, &mode);
                }
                fn_710007c610(&reader.file, fn_710007c1a4(&reader.file));
                for (UnkTunableLine *p = lines.data, *end = lines.data + lines.count; p != end; ++p)
                    fn_710007c450(&reader.file, p->CStr(), p->Length(), 1);
                fn_710007c450(&reader.file, g_unk_710162e2ed, 1, 1);
                fn_710007c388(&reader.file, &g_unk_710b372708);
                fn_710007c144(&reader.file);
            }
        }
        lines.Free();
    }
    fn_710007c0e4(&reader.file);
    return result;
}
