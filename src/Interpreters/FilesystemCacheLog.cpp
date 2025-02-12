#include <DataTypes/DataTypeDate.h>
#include <DataTypes/DataTypeDateTime.h>
#include <DataTypes/DataTypeString.h>
#include <DataTypes/DataTypesNumber.h>
#include <Interpreters/FilesystemCacheLog.h>


namespace DB
{

static String typeToString(FilesystemCacheLogElement::ReadType type)
{
    switch (type)
    {
        case FilesystemCacheLogElement::ReadType::READ_FROM_CACHE:
            return "READ_FROM_CACHE";
        case FilesystemCacheLogElement::ReadType::READ_FROM_FS_AND_DOWNLOADED_TO_CACHE:
            return "READ_FROM_FS_AND_DOWNLOADED_TO_CACHE";
        case FilesystemCacheLogElement::ReadType::READ_FROM_FS_BYPASSING_CACHE:
            return "READ_FROM_FS_BYPASSING_CACHE";
    }
    __builtin_unreachable();
}

NamesAndTypesList FilesystemCacheLogElement::getNamesAndTypes()
{
    DataTypes types{
        std::make_shared<DataTypeNumber<UInt64>>(),
        std::make_shared<DataTypeNumber<UInt64>>(),
    };

    return {
        {"event_date", std::make_shared<DataTypeDate>()},
        {"event_time", std::make_shared<DataTypeDateTime>()},
        {"query_id", std::make_shared<DataTypeString>()},
        {"remote_file_path", std::make_shared<DataTypeString>()},
        {"file_segment_range", std::make_shared<DataTypeTuple>(std::move(types))},
        {"read_type", std::make_shared<DataTypeString>()},
    };
}

void FilesystemCacheLogElement::appendToBlock(MutableColumns & columns) const
{
    size_t i = 0;

    columns[i++]->insert(DateLUT::instance().toDayNum(event_time).toUnderType());
    columns[i++]->insert(event_time);

    columns[i++]->insert(query_id);

    columns[i++]->insert(remote_file_path);
    columns[i++]->insert(Tuple{file_segment_range.first, file_segment_range.second});
    columns[i++]->insert(typeToString(read_type));
}

};
