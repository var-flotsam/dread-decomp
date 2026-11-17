// Unit 112 (the tunables' registry): reads a tunables file and writes each of its lines again, a
// `key=value` pair whose tunable field is found becoming "key = <its current value>".
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkFileReader.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrPair.h"
#include "base/UnkTunableLine.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU112.h"
#include "game/UnkTU56.h"
#include "game/UnkTU57.h"
#include "game/UnkTunableFieldHandle.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::UnkReflTypedPtr;

// Reads a line of at most `size` bytes from the position; false when none could be read.
bool fn_710007c9e8(base::global::CRntFile* file, char* buffer, u32 size);
void fn_710007f6dc(CRntString* self);  // trims both ends
UnkTunableLine fn_710013ab54(UnkTunableFieldHandle* field, const UnkReflTypedPtr* obj);  // the value as text

struct UnkReflFieldInfo;
struct UnkFieldOwner;
// The member object a field of `owner` holds.
UnkReflTypedPtr fn_71000e755c(const UnkReflFieldInfo* field, UnkFieldOwner* owner);

extern const char g_unk_71015abdba[] __attribute__((visibility("hidden")));  // the line end

// The tunable object a header names: its class is vtable slot 0.
struct UnkTunableObject {
    virtual char* GetClass();
};
struct UnkTunableRegistry;

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

// The input file and its reader's words, closed when it goes out of scope.
struct UnkTunablesInput_l13caac : UnkFileReader {
    UnkTunablesInput_l13caac() { fn_710007be8c(&file, 0x16, nullptr); }
    ~UnkTunablesInput_l13caac() { fn_710007c0e4(&file); }
};

static inline UnkReflTypedPtr Typed(char* type, void* obj) {
    UnkReflTypedPtr p;
    p.ptr = obj;
    p.type = fn_710009dcd8(type, obj);
    return p;
}

static inline u64 HashOf(const UnkTunableLine& s) {
    const char* str = s.CStr();
    return fn_7100001570(str, strlen(str));
}

// Looks up the tunable a header names: prefix, then `middle`, then the name.
static inline UnkTunableObject* FindTunable(UnkTunableRegistry* self, const UnkTunableLine& prefix,
                                            const char* middle, const UnkTunableLine& name, bool qualified) {
    CRntString a = fn_710007ef90(prefix.Str(), middle);
    CRntString b = fn_710007eee0(&a, name.Str());
    return fn_710013c5cc(self, b.data, qualified);
}

bool fn_710013caac(UnkTunableRegistry* self, const UnkStrPair_0x10* path) {
    UnkTunablesInput_l13caac in;
    bool opened;
    {
        CRntString mode("rb");
        opened = fn_710007c1ac(&in, path, &mode);
    }
    if (!opened)
        return false;

    UnkScopedHashStream out(0x16, nullptr);
    char buffer[0x100];
    UnkTunableLine line;
    UnkTunableObject* tunable = nullptr;
    bool result = true;
    while (in.file.position < in.file.size) {
        if (!fn_710007c9e8(&in.file, buffer, 0x100))
            continue;
        fn_710007e77c(line.Str(), buffer);
        fn_710007f6dc(line.Str());

        if (line.Length() != 0 && line.Data()[0] != ';') {
            s32 open = fn_710007facc(line.Str(), '[', 0);
            if (open != -1) {
                // A header: the tunable the following pairs belong to.
                s32 close = fn_710007facc(line.Str(), ']', 0);
                if (close == -1) {
                    result = false;
                    break;
                }
                u32 start = open + 1;
                UnkTunableLine name(line.Data() + start, close - start);
                fn_710007f6dc(name.Str());
                s32 bar = fn_710007facc(name.Str(), '|', 0);
                UnkTunableLine prefix(g_unk_710162e2ed);
                if (bar >= 0) {
                    u32 n = bar + 1;
                    prefix = UnkTunableLine(name.Data(), n);
                    name = UnkTunableLine(name.Data() + n, name.Length() - n);
                }
                tunable = FindTunable(self, prefix, g_unk_710162e2ed, name, bar >= 0);
                if (!tunable)
                    tunable = FindTunable(self, prefix, "CTunable", name, bar >= 0);
                if (!tunable)
                    tunable = FindTunable(self, prefix, "CTunableCharClass", name, bar >= 0);
            } else {
                s32 equals = fn_710007facc(line.Str(), '=', 0);
                if (equals != -1) {
                    UnkTunableLine key(line.Data(), equals);
                    fn_710007f6dc(key.Str());
                    UnkTunableLine written(key);
                    if (tunable) {
                        // Walks the dotted members down to the last name.
                        char* type = fn_710009dcd8(tunable->GetClass(), tunable);
                        void* object = tunable;
                        UnkReflTypedPtr obj;
                        obj.type = type;
                        obj.ptr = object;
                        s32 dot = fn_710007facc(key.Str(), '.', 0);
                        while (dot >= 0) {
                            UnkTunableLine member(key.Data(), dot);
                            u64 hash = HashOf(member);
                            UnkTunableType* cls = reinterpret_cast<UnkTunableType*>(tunable->GetClass());
                            UnkTunableField* field = cls->Find(hash);
                            if (!field || field == cls->End())
                                break;
                            UnkReflTypedPtr outer = Typed(type, object);
                            obj = fn_71000e755c(reinterpret_cast<const UnkReflFieldInfo*>(field),
                                                reinterpret_cast<UnkFieldOwner*>(&outer));
                            type = obj.type;
                            object = obj.ptr;
                            u32 after = dot + 1;
                            key = UnkTunableLine(key.Data() + after, key.Length() - after);
                            dot = fn_710007facc(key.Str(), '.', 0);
                        }

                        UnkTunableType* owner = reinterpret_cast<UnkTunableType*>(type);
                        if (owner && fn_710009d50c(owner->GetClass(), fn_7100096100())) {
                            UnkTunableField* field = owner->Find(HashOf(key));
                            if (field == owner->End())
                                field = nullptr;
                            if (!field) {
                                // Tried again with each type prefix.
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
                            }
                            if (field) {
                                UnkTunableLine value =
                                    fn_710013ab54(reinterpret_cast<UnkTunableFieldHandle*>(field), &obj);
                                CRntString a = fn_710007ef90(written.Str(), " = ");
                                CRntString b = fn_710007eee0(&a, value.Str());
                                fn_710007eaa0(line.Str(), &b);
                            }
                        }
                    }
                }
            }
        }
        fn_710007f564(line.Str(), g_unk_71015abdba);
        fn_710007c450(&out, line.CStr(), line.Length(), 1);
    }
    fn_710007c144(&in.file);
    // The original closes the input a second time where the output would be expected.
    if (result)
        fn_710007c144(&in.file);
    else
        fn_710007c144(&out);
    return result;
}
