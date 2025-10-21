#include "Settings.h"

#include "Lumina/Core/Assert.h"

#include <fstream>

#include <json.hpp>

using json = nlohmann::json;

namespace Lumina
{
    Settings* Settings::s_Instance = nullptr;

    void Settings::Init()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = new Settings();
        }
    }

    void Settings::Shutdown()
    {
        if (s_Instance != nullptr)
        {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    bool Settings::Load()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");

        bool result = s_Instance->LoadFromJson();
        if (result)
        {
            s_Instance->m_ValidData = s_Instance->m_CurrentData;
            s_Instance->m_SavedData = s_Instance->m_CurrentData;
            s_Instance->m_Modified = false;
        }

        return result;
    }

    bool Settings::Save()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");

        ValidationResult validation = s_Instance->ValidateInternal();
        if (!validation.IsValid)
            return false;

        bool result = s_Instance->SaveToJson();
        if (result)
        {
            s_Instance->m_ValidData = s_Instance->m_CurrentData;
            s_Instance->m_SavedData = s_Instance->m_CurrentData;
            s_Instance->m_Modified = false;
        }

        return result;
    }

    const SettingsData& Settings::Data()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        return s_Instance->m_CurrentData;
    }

    SettingsData& Settings::GetMutable()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        s_Instance->m_Modified = true;
        return s_Instance->m_CurrentData;
    }

    bool Settings::IsModified()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        return s_Instance->m_Modified && (s_Instance->m_CurrentData != s_Instance->m_SavedData);
    }

    bool Settings::IsValid()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        return s_Instance->ValidateInternal().IsValid;
    }

    ValidationResult Settings::Validate()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        return s_Instance->ValidateInternal();
    }

    ValidationResult Settings::ValidateRecordingsFolder(const std::filesystem::path& path)
    {
        ValidationResult result;

        if (path.empty())
        {
            result.IsValid = false;
            result.ErrorMessage = "Recordings folder path cannot be empty";
            return result;
        }

        std::string pathString = path.string();
        const std::string invalidChars = "<>:\"|?*";

        for (char c : pathString)
        {
            if (invalidChars.find(c) != std::string::npos)
            {
                result.IsValid = false;
                result.ErrorMessage = "Path contains invalid characters: " + std::string(1, c);
                return result;
            }
        }

        try
        {
            std::filesystem::path testPath = path;
            if (!testPath.is_absolute() && !testPath.is_relative())
            {
                result.IsValid = false;
                result.ErrorMessage = "Invalid path format";
                return result;
            }
        }
        catch (...)
        {
            result.IsValid = false;
            result.ErrorMessage = "Invalid path format";
            return result;
        }

        return result;
    }

    ValidationResult Settings::ValidateAutoSaveInterval(int seconds)
    {
        ValidationResult result;

        if (seconds < 10)
        {
            result.IsValid = false;
            result.ErrorMessage = "Auto-save interval must be at least 10 seconds";
            return result;
        }

        if (seconds > 3600)
        {
            result.IsValid = false;
            result.ErrorMessage = "Auto-save interval cannot exceed 3600 seconds (1 hour)";
            return result;
        }

        return result;
    }

    void Settings::RevertRecordingsFolder()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        s_Instance->m_CurrentData.RecordingsFolder = s_Instance->m_ValidData.RecordingsFolder;
    }

    void Settings::RevertAutoSaveInterval()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        s_Instance->m_CurrentData.AutoSaveIntervalSeconds = s_Instance->m_ValidData.AutoSaveIntervalSeconds;
    }

    void Settings::RevertAll()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        s_Instance->m_CurrentData = s_Instance->m_ValidData;
        s_Instance->m_Modified = (s_Instance->m_CurrentData != s_Instance->m_SavedData);
    }

    void Settings::SubscribeToChanges(SettingsChangedCallback callback)
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        s_Instance->m_Callbacks.push_back(callback);
    }

    void Settings::NotifyChanged()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        for (auto& callback : s_Instance->m_Callbacks)
            callback();
    }

    Settings::Settings()
    {
        LoadFromJson();
        m_ValidData = m_CurrentData;
        m_SavedData = m_CurrentData;
    }

    ValidationResult Settings::ValidateInternal()
    {
        ValidationResult folderValidation = ValidateRecordingsFolder(m_CurrentData.RecordingsFolder);
        if (!folderValidation.IsValid)
            return folderValidation;

        ValidationResult intervalValidation = ValidateAutoSaveInterval(m_CurrentData.AutoSaveIntervalSeconds);
        if (!intervalValidation.IsValid)
            return intervalValidation;

        return ValidationResult{ true, "" };
    }

    bool Settings::LoadFromJson()
    {
        try
        {
            if (!std::filesystem::exists(s_ApplicationFolder))
            {
                std::filesystem::create_directories(s_ApplicationFolder);
            }
            if (!std::filesystem::exists(m_CurrentData.RecordingsFolder))
            {
                std::filesystem::create_directories(m_CurrentData.RecordingsFolder);
            }

            if (!std::filesystem::exists(s_SettingsFilePath))
                return false;

            std::ifstream file(s_SettingsFilePath);
            if (!file.is_open())
                return false;

            json jsonData = json::parse(file);

            if (jsonData.contains("recording"))
            {
                auto& recording = jsonData["recording"];

                if (recording.contains("startRecording"))
                {
                    m_CurrentData.StartRecording.Keys.clear();
                    for (const auto& key : recording["startRecording"])
                        m_CurrentData.StartRecording.Keys.push_back(static_cast<KeyCode>(key.get<int>()));
                }

                if (recording.contains("stopRecording"))
                {
                    m_CurrentData.StopRecording.Keys.clear();
                    for (const auto& key : recording["stopRecording"])
                        m_CurrentData.StopRecording.Keys.push_back(static_cast<KeyCode>(key.get<int>()));
                }
            }

            if (jsonData.contains("playback"))
            {
                auto& playback = jsonData["playback"];

                if (playback.contains("playRecording"))
                {
                    m_CurrentData.PlayRecording.Keys.clear();
                    for (const auto& key : playback["playRecording"])
                        m_CurrentData.PlayRecording.Keys.push_back(static_cast<KeyCode>(key.get<int>()));
                }

                if (playback.contains("stopPlayback"))
                {
                    m_CurrentData.StopPlayback.Keys.clear();
                    for (const auto& key : playback["stopPlayback"])
                        m_CurrentData.StopPlayback.Keys.push_back(static_cast<KeyCode>(key.get<int>()));
                }
            }

            if (jsonData.contains("application"))
            {
                auto& application = jsonData["application"];

                if (application.contains("recordingsFolder"))
                {
                    m_CurrentData.RecordingsFolder = application["recordingsFolder"].get<std::string>();
                }

                if (application.contains("autoSaveEnabled"))
                    m_CurrentData.AutoSaveEnabled = application["autoSaveEnabled"].get<bool>();

                if (application.contains("autoSaveIntervalSeconds"))
                    m_CurrentData.AutoSaveIntervalSeconds = application["autoSaveIntervalSeconds"].get<int>();
            }

            return true;
        }
        catch (const std::exception& e)
        {
            return false;
        }
    }

    bool Settings::SaveToJson()
    {
        try
        {
            if (!std::filesystem::exists(s_ApplicationFolder))
            {
                std::filesystem::create_directories(s_ApplicationFolder);
            }
            if (!std::filesystem::exists(m_CurrentData.RecordingsFolder))
            {
                std::filesystem::create_directories(m_CurrentData.RecordingsFolder);
            }

            json jsonData;

            json recording;
            json startRecordingKeys = json::array();
            for (const auto& key : m_CurrentData.StartRecording.Keys)
                startRecordingKeys.push_back(static_cast<int>(key));
            recording["startRecording"] = startRecordingKeys;

            json stopRecordingKeys = json::array();
            for (const auto& key : m_CurrentData.StopRecording.Keys)
                stopRecordingKeys.push_back(static_cast<int>(key));
            recording["stopRecording"] = stopRecordingKeys;
            jsonData["recording"] = recording;

            json playback;
            json playRecordingKeys = json::array();
            for (const auto& key : m_CurrentData.PlayRecording.Keys)
                playRecordingKeys.push_back(static_cast<int>(key));
            playback["playRecording"] = playRecordingKeys;

            json stopPlaybackKeys = json::array();
            for (const auto& key : m_CurrentData.StopPlayback.Keys)
                stopPlaybackKeys.push_back(static_cast<int>(key));
            playback["stopPlayback"] = stopPlaybackKeys;
            jsonData["playback"] = playback;

            json application;
            application["recordingsFolder"] = m_CurrentData.RecordingsFolder.string();
            application["autoSaveEnabled"] = m_CurrentData.AutoSaveEnabled;
            application["autoSaveIntervalSeconds"] = m_CurrentData.AutoSaveIntervalSeconds;
            jsonData["application"] = application;

            std::ofstream file(s_SettingsFilePath);
            if (!file.is_open())
                return false;

            file << jsonData.dump(4);
            return true;
        }
        catch (const std::exception& e)
        {
            return false;
        }
    }
}