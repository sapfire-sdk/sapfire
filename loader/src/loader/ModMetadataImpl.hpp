#pragma once

#include <Sapfire/loader/Loader.hpp>
#include <Sapfire/loader/Mod.hpp>
#include <Sapfire/utils/JsonValidation.hpp>
#include <Sapfire/utils/VersionInfo.hpp>

using namespace sapfire::prelude;

namespace sapfire {
    class ModMetadataLinks::Impl final {
    public:
        std::optional<std::string> m_homepage;
        std::optional<std::string> m_source;
        std::optional<std::string> m_community;
    };

    class ModMetadata::Impl {
    public:
        std::filesystem::path m_path;
        std::string m_binaryName;
        VersionInfo m_version{1, 0, 0};
        std::string m_id;
        std::string m_name;
        std::vector<std::string> m_developers;
        // TODO: remove once #895 is fixed
        std::optional<std::string> m_softInvalidReason;
        std::string m_gdVersion;
        VersionInfo m_sapfireVersion;
        std::optional<std::string> m_description;
        std::optional<std::string> m_details;
        std::optional<std::string> m_changelog;
        std::optional<std::string> m_supportInfo;
        ModMetadataLinks m_links;
        std::optional<IssuesInfo> m_issues;
        std::vector<Dependency> m_dependencies;
        std::vector<Incompatibility> m_incompatibilities;
        std::vector<std::string> m_spritesheets;
        std::vector<std::pair<std::string, Setting>> m_settings;
        std::unordered_set<std::string> m_tags;
        bool m_needsEarlyLoad = false;
        bool m_isAPI = false;

        ModJson m_rawJSON;

        static Result<ModMetadata> createFromSapfireZip(utils::file::Unzip& zip);
        static Result<ModMetadata> createFromSapfireFile(std::filesystem::path const& path);
        static Result<ModMetadata> createFromFile(std::filesystem::path const& path);
        static Result<ModMetadata> create(ModJson const& json);

        ModJson toJSON() const;
        ModJson getRawJSON() const;

        bool operator==(ModMetadata::Impl const& other) const;

        static bool validateID(std::string const& id);
        static bool validateOldID(std::string const& id);
        static bool isDeprecatedIDForm(std::string const& id);

        static Result<ModMetadata> createFromSchemaV010(ModJson const& rawJson);

        Result<> addSpecialFiles(std::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };

    class ModMetadataImpl : public ModMetadata::Impl {
    public:
        static ModMetadata::Impl& getImpl(ModMetadata& info);
    };
}

template <>
struct matjson::Serialize<sapfire::ModMetadata::Dependency::Importance> {
    static matjson::Value SAPFIRE_DLL to_json(sapfire::ModMetadata::Dependency::Importance const& importance) {
        switch (importance) {
            case sapfire::ModMetadata::Dependency::Importance::Required: return {"required"};
            case sapfire::ModMetadata::Dependency::Importance::Recommended: return {"recommended"};
            case sapfire::ModMetadata::Dependency::Importance::Suggested: return {"suggested"};
            default: return {"unknown"};
        }
    }
    static sapfire::ModMetadata::Dependency::Importance SAPFIRE_DLL from_json(matjson::Value const& importance) {
        auto impStr = importance.as_string();
        if (impStr == "required")
            return sapfire::ModMetadata::Dependency::Importance::Required;
        if (impStr == "recommended")
            return sapfire::ModMetadata::Dependency::Importance::Recommended;
        if (impStr == "suggested")
            return sapfire::ModMetadata::Dependency::Importance::Suggested;
        throw matjson::JsonException(R"(Expected importance to be "required", "recommended" or "suggested")");
    }

    static bool is_json(matjson::Value const& value) {
        return value.is_string();
    }
};

template <>
struct matjson::Serialize<sapfire::ModMetadata::Incompatibility::Importance> {
    static matjson::Value SAPFIRE_DLL to_json(sapfire::ModMetadata::Incompatibility::Importance const& importance) {
        switch (importance) {
            case sapfire::ModMetadata::Incompatibility::Importance::Breaking: return {"breaking"};
            case sapfire::ModMetadata::Incompatibility::Importance::Conflicting: return {"conflicting"};
            case sapfire::ModMetadata::Incompatibility::Importance::Superseded: return {"superseded"};
            default: return {"unknown"};
        }
    }
    static sapfire::ModMetadata::Incompatibility::Importance SAPFIRE_DLL from_json(matjson::Value const& importance) {
        auto impStr = importance.as_string();
        if (impStr == "breaking")
            return sapfire::ModMetadata::Incompatibility::Importance::Breaking;
        if (impStr == "conflicting")
            return sapfire::ModMetadata::Incompatibility::Importance::Conflicting;
        if (impStr == "superseded")
            return sapfire::ModMetadata::Incompatibility::Importance::Superseded;
        throw matjson::JsonException(R"(Expected importance to be "breaking", "conflicting", or "superseded")");
    }

    static bool is_json(matjson::Value const& value) {
        return value.is_string();
    }
};
