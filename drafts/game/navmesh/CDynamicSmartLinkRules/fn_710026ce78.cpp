// navmesh::CDynamicSmartLinkRules's unit: the file read. The name taken from the path is opened "rb", the
// old rules are deleted, then the flag byte, the kinds text and the rules are read; each rule is made from
// its class name, named, owned by the set and reads its own data. Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/UnkHashStream.h"
#include "game/UnkPathPairTemp_b45a1.h"
#include "game/UnkTU25.h"
#include "game/UnkTU56.h"
#include "game/UnkTU83.h"
#include "game/navmesh/CDynamicSmartLinkRules.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::global::CStrId;

struct UnkFileReader;
extern "C" u64 strlen(const char*);
void fn_710007ca94(CRntFile* file, char* buf);

// What a registered class's descriptor gives: the object's size and its in-place constructor.
struct UnkRuleTypeView {
    u8 pad_0x0[0x18];
    u64 size;
    u8 pad_0x20[0x8];
    void (*construct)(void* object);
};

struct UnkSmartLinkRules_l26ce78;

// A string id built from a string out of line (fn_7100000850), released like CStrId.
struct UnkBuiltStrId_l26ce78 {
    union {
        CStrId id;
    };

    explicit UnkBuiltStrId_l26ce78(const CRntString& str) { CStrId_ConstructFromRntString(&id, str, true); }
    ~UnkBuiltStrId_l26ce78() { id.~CStrId(); }
};

// A smart-link rule: its name, its set, and its own read at slot 0x48.
struct UnkSmartLinkRule_l26ce78 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();  // 0x10
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void Read(CRntFile* file);  // 0x48
    u8 pad_0x8[0x8];
    CStrId name;                    // 0x10
    UnkSmartLinkRules_l26ce78* set;  // 0x18
};

extern template UnkSmartLinkRule_l26ce78*& base::global::CRntVector<UnkSmartLinkRule_l26ce78*>::GrowAt(u32);

struct UnkSmartLinkRules_l26ce78 {
    void** vtable;
    u8 path[0x50];  // 0x8
    CStrId name;  // 0x58
    u8 field_0x60;
    u8 pad_0x61[3];
    u32 kinds;  // 0x64
    base::global::CRntVector<UnkSmartLinkRule_l26ce78*> rules;  // 0x68
};

bool fn_710026ce78(UnkSmartLinkRules_l26ce78* self) {
    {
        UnkPathPairTemp_b45a1 s = MakePathPairFromRegistryCallback(self->path);
        base::global::UnkStringData* data = static_cast<base::global::UnkStringData*>(s.path);
        ++data->reads;
        self->name = static_cast<CStrId&&>(UnkBuiltStrId_l26ce78(data->String()).id);
    }
    CRntFile file;
    fn_710007be8c(&file, 4, nullptr);
    bool opened;
    {
        UnkStrPair_0x10 s;
        fn_710008d680(&s, self->name.Text());
        {
            CRntString mode("rb");
            opened = fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&file), &s, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    bool ok;
    if (opened) {
        CRntFile_Seek(&file, 8, 0);
        for (u32 i = 0; i < self->rules.count; ++i) {
            if (self->rules.GrowAt(i))
                if (UnkSmartLinkRule_l26ce78* rule = self->rules.GrowAt(i))
                    rule->Destroy();
            self->rules.GrowAt(i) = nullptr;
        }
        self->rules.count = 0;
        CRntFile_Read(&file, &self->field_0x60, 1, 1);
        char text[0x200];
        fn_710007ca94(&file, text);
        self->kinds = CDynamicSmartLinkRules_ParseSmartLinkKindMask(text);
        s32 count;
        CRntFile_Read(&file, &count, 4, 1);
        for (s32 i = 0; i < count; ++i) {
            const char* rule_name;
            const char* class_name;
            CRntFile_ReadCString(&file, &rule_name);
            CRntFile_ReadCString(&file, &class_name);
            UnkVersionRegistry* registry = g_unk_7101d4c8d0;
            UnkVersionEntry* entry;
            {
                u64 id = StringCrc64(class_name, strlen(class_name));
                entry = fn_71000a5118(registry, &id);
            }
            if (!entry)
                continue;
            auto* type = reinterpret_cast<const base::reflection::CType*>(entry);
            UnkSmartLinkRule_l26ce78* rule = nullptr;
            if (fn_710009d50c(type, fn_71002696e4())) {
                const auto* view = reinterpret_cast<const UnkRuleTypeView*>(entry);
                if (view->construct) {
                    void* p = GetDefaultAllocator()->vf_0x40(view->size, 8, 4);
                    if (p)
                        view->construct(p);
                    rule = static_cast<UnkSmartLinkRule_l26ce78*>(p);
                }
            }
            {
                CStrId id_name(rule_name, true);
                rule->name = id_name;
            }
            rule->set = self;
            self->rules.PushBack(rule);
            rule->Read(&file);
        }
        ok = true;
    } else {
        ok = false;
    }
    fn_710007c0e4(&file);
    return ok;
}
