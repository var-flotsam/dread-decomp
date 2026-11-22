// Unit 112 (the tunables' registry): writes a list of tunable lines to a file, grouped: a commented
// header for each new group between two separator lines, then for each line a comment with its name
// and "<id> = <value>". Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU112.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStrIdInline;

// A line to write (0x160 bytes): its full id, its group, its name and its value.
struct UnkTunableWriteLine_l13d35c {
    CStrId id;
    CStrId group;
    CStrId name;
    CRntString value;
    u8 pad_0x48[0x118];
};
static_assert(sizeof(UnkTunableWriteLine_l13d35c) == 0x160, "");

struct UnkTunableWriteList_l13d35c;
UnkTunableWriteLine_l13d35c* fn_710013318c(UnkTunableWriteList_l13d35c* list);  // begin
UnkTunableWriteLine_l13d35c* fn_7100133194(UnkTunableWriteList_l13d35c* list);  // end

// This unit's two fixed ids: the line end and the separator line.
extern CStrId g_unk_710b3726f8[2] __attribute__((visibility("hidden")));

// An id's text as a string (fn_710007eb20), destroyed as one.
struct UnkIdText_l13d35c {
    alignas(8) u8 storage[sizeof(CRntString)];

    CRntString* Str() { return reinterpret_cast<CRntString*>(storage); }
    explicit UnkIdText_l13d35c(const CStrId& id) {
        CRntString_ConstructFromStrId(Str(), reinterpret_cast<const UnkPooledStrId*>(&id));
    }
    ~UnkIdText_l13d35c() { CRntString_Destroy(Str()); }
};

// Appends an id's text, counting the read.
static inline void AppendId(CRntString* line, const CStrId& id) {
    UnkStringData* d = id.data;
    ++d->reads;
    CRntString_Append(line, &d->String());
}

static inline void Write(CRntFile* file, const CRntString& s) {
    fn_710007c450(file, s.data ? s.data : g_unk_710162e2ed, s.length, 1);
}

// Writes an id's text, counting the read.
static inline void WriteId(CRntFile* file, const CStrId& id) {
    UnkStringData* d = id.data;
    ++d->reads;
    Write(file, d->String());
}

void fn_710013d35c(UnkTunableRegistry* self, UnkTunableWriteList_l13d35c* list, CRntFile* file) {
    const CStrId& newline = g_unk_710b3726f8[0];
    const CStrId& separator = g_unk_710b3726f8[1];
    CStrId group(UnkStrIdInline(), nullptr, false);
    for (UnkTunableWriteLine_l13d35c *it = fn_710013318c(list), *end = fn_7100133194(list); it != end; ++it) {
        if (group.data != it->group.data) {
            group = it->group;
            CRntString header(";");
            AppendId(&header, group);
            AppendId(&header, newline);
            WriteId(file, separator);
            Write(file, header);
            WriteId(file, separator);
        }
        {
            CRntString comment(";     ");
            ++it->name.data->reads;
            CRntString_Append(&comment, &it->name.data->String());
            AppendId(&comment, newline);
            Write(file, comment);
        }
        UnkIdText_l13d35c line(it->id);
        CRntString_AppendCString(line.Str(), " = ");
        CRntString_Append(line.Str(), &it->value);
        AppendId(line.Str(), newline);
        Write(file, *line.Str());
    }
}
