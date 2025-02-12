#pragma once

#include <Core/NamesAndAliases.h>
#include <Core/NamesAndTypes.h>
#include <Core/Settings.h>
#include <DataTypes/DataTypeArray.h>
#include <DataTypes/DataTypeNumberBase.h>
#include <DataTypes/DataTypeTuple.h>
#include <Interpreters/SystemLog.h>
#include <Interpreters/TransactionVersionMetadata.h>
#include <Common/logger_useful.h>

namespace DB
{
///
/// -------- Column --------- Type ------
/// |  event_date         |  DateTime   |
/// |  event_time         |  UInt64     |
/// |  query_id           |  String     |
/// |  remote_file_path   |  String     |
/// |  segment_range      |  Tuple      |
/// |  read_type          |  String     |
/// -------------------------------------
///
struct FilesystemCacheLogElement
{
    enum class ReadType
    {
        READ_FROM_CACHE,
        READ_FROM_FS_AND_DOWNLOADED_TO_CACHE,
        READ_FROM_FS_BYPASSING_CACHE,
    };

    time_t event_time{};

    String query_id;
    String remote_file_path;

    std::pair<size_t, size_t> file_segment_range{};
    ReadType read_type{};

    static std::string name() { return "FilesystemCacheLog"; }

    static NamesAndTypesList getNamesAndTypes();
    static NamesAndAliases getNamesAndAliases() { return {}; }

    void appendToBlock(MutableColumns & columns) const;
    static const char * getCustomColumnList() { return nullptr; }
};

class FilesystemCacheLog : public SystemLog<FilesystemCacheLogElement>
{
    using SystemLog<FilesystemCacheLogElement>::SystemLog;
};

};
