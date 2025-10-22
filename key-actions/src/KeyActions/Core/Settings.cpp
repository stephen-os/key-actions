#include "Settings.h"

#include "Lumina/Core/Assert.h"

#include <fstream>

#include <json.hpp>

using json = nlohmann::json;

namespace KeyActions
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
            s_Instance->m_SavedData = s_Instance->m_CurrentData;
        }

        return result;
    }

    bool Settings::Save()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");

        bool result = s_Instance->SaveToJson();
        if (result)
        {
            s_Instance->m_SavedData = s_Instance->m_CurrentData;
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
        return s_Instance->m_CurrentData;
    }

    bool Settings::IsModified()
    {
        LUMINA_ASSERT(s_Instance != nullptr, "Settings not initialized! Call Settings::Init() first.");
        return s_Instance->m_CurrentData != s_Instance->m_SavedData;
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
        m_SavedData = m_CurrentData;
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