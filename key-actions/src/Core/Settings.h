// Settings.h
#pragma once
#include <string>
#include <filesystem>
#include <functional>
#include <vector>
#include "Lumina/Core/Input.h"
#include "Lumina/File/Paths.h"

namespace Lumina
{
    struct SettingsData
    {
        KeyCombo StartRecording = { {KeyCode::LeftControl, KeyCode::LeftShift, KeyCode::R} };
        KeyCombo StopRecording = { {KeyCode::LeftControl, KeyCode::LeftShift, KeyCode::S} };
        KeyCombo PlayRecording = { {KeyCode::LeftControl, KeyCode::LeftShift, KeyCode::P} };
        KeyCombo StopPlayback = { {KeyCode::LeftControl, KeyCode::LeftShift, KeyCode::X} };
        bool AutoSaveEnabled = true;
        int AutoSaveIntervalSeconds = 300;
        std::filesystem::path RecordingsFolder = Path::AppData() / "KeyActions" / "Recordings";

        bool operator==(const SettingsData& other) const = default;
    };

    struct ValidationResult
    {
        bool IsValid = true;
        std::string ErrorMessage;
    };

    class Settings
    {
    public:
        static void Init();
        static void Shutdown();
        static bool Load();
        static bool Save();

        static const SettingsData& Data();
        static SettingsData& GetMutable();

        static bool IsModified();
        static bool IsValid();
        static ValidationResult Validate();

        static ValidationResult ValidateRecordingsFolder(const std::filesystem::path& path);
        static ValidationResult ValidateAutoSaveInterval(int seconds);

        static void RevertRecordingsFolder();
        static void RevertAutoSaveInterval();
        static void RevertAll();

        using SettingsChangedCallback = std::function<void()>;
        static void SubscribeToChanges(SettingsChangedCallback callback);
        static void NotifyChanged();

    private:
        Settings();
        ~Settings() = default;

        Settings(const Settings&) = delete;
        Settings& operator=(const Settings&) = delete;
        Settings(Settings&&) = delete;
        Settings& operator=(Settings&&) = delete;

        static inline const std::string s_ApplicationName = "KeyActions";
        static inline const std::string s_SettingsFileName = "settings.json";
        static inline const std::filesystem::path s_ApplicationFolder = Path::AppData() / s_ApplicationName;
        static inline const std::filesystem::path s_SettingsFilePath = s_ApplicationFolder / s_SettingsFileName;

        bool LoadFromJson();
        bool SaveToJson();
        ValidationResult ValidateInternal();

        SettingsData m_CurrentData;
        SettingsData m_ValidData;
        SettingsData m_SavedData;
        bool m_Modified = false;
        std::vector<SettingsChangedCallback> m_Callbacks;

        static Settings* s_Instance;
    };
}