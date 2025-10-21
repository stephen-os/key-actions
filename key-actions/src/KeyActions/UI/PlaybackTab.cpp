#include "PlaybackTab.h"
#include "Lumina/Core/Log.h"

namespace Lumina
{
    PlaybackTab::PlaybackTab()
        : Tab("Playback")
    {
    }

    void PlaybackTab::OnAttach()
    {
        // Set up callbacks
        m_PlaybackSession.SetProgressCallback([this](float progress, size_t eventIndex) {
            m_CurrentProgress = progress;
            m_CurrentEventIndex = eventIndex;
            });

        m_PlaybackSession.SetCompleteCallback([this]() {
            LUMINA_LOG_INFO("Playback completed!");
            m_TotalPlays++;
            });

        // Load available recordings
        LoadRecordingsList();

        LUMINA_LOG_INFO("Playback tab initialized");
    }

    void PlaybackTab::OnDetach()
    {
        if (m_PlaybackSession.IsPlaying())
        {
            m_PlaybackSession.Stop();
        }
    }

    void PlaybackTab::OnUpdate(float timestep)
    {
    }

    void PlaybackTab::OnEvent(Event& e)
    {
    }

    void PlaybackTab::OnRender()
    {
        // Recording selection
        ImGui::Text("Available Recordings:");

        if (ImGui::Button("Refresh List"))
        {
            LoadRecordingsList();
        }

        ImGui::BeginChild("RecordingsList", ImVec2(0, 150), true);
        for (int i = 0; i < m_AvailableRecordings.size(); i++)
        {
            bool isSelected = (m_SelectedRecordingIndex == i);
            if (ImGui::Selectable(m_AvailableRecordings[i].c_str(), isSelected))
            {
                m_SelectedRecordingIndex = i;
                LoadSelectedRecording();
            }
        }
        ImGui::EndChild();

        ImGui::Separator();

        // Loaded recording info
        if (m_HasLoadedRecording)
        {
            ImGui::Text("Loaded: %s", m_LoadedRecording.Name.c_str());
            ImGui::Text("Events: %zu", m_LoadedRecording.Events.size());
            ImGui::Text("Duration: %.2fs", m_LoadedRecording.TotalDuration);
        }
        else
        {
            ImGui::Text("No recording loaded");
        }

        ImGui::Separator();

        // Playback settings
        ImGui::Text("Playback Settings:");
        ImGui::SliderFloat("Speed", &m_Settings.Speed, 0.1f, 5.0f);
        ImGui::Checkbox("Loop", &m_Settings.Loop);
        ImGui::Checkbox("Ignore Mouse Movement", &m_Settings.IgnoreMouseMove);

        ImGui::Separator();

        // Playback controls
        ImGui::BeginDisabled(!m_HasLoadedRecording);

        if (!m_PlaybackSession.IsPlaying())
        {
            if (ImGui::Button("Play", ImVec2(100, 40)))
            {
                m_PlaybackSession.Play(m_LoadedRecording, m_Settings);
            }
        }
        else
        {
            ImGui::BeginDisabled();
            ImGui::Button("Play", ImVec2(100, 40));
            ImGui::EndDisabled();
        }

        ImGui::SameLine();

        if (m_PlaybackSession.IsPlaying())
        {
            if (ImGui::Button("Stop", ImVec2(100, 40)))
            {
                m_PlaybackSession.Stop();
            }
        }
        else
        {
            ImGui::BeginDisabled();
            ImGui::Button("Stop", ImVec2(100, 40));
            ImGui::EndDisabled();
        }

        ImGui::SameLine();

        if (m_PlaybackSession.IsPlaying() && !m_PlaybackSession.IsPaused())
        {
            if (ImGui::Button("Pause", ImVec2(100, 40)))
            {
                m_PlaybackSession.Pause();
            }
        }
        else if (m_PlaybackSession.IsPlaying() && m_PlaybackSession.IsPaused())
        {
            if (ImGui::Button("Resume", ImVec2(100, 40)))
            {
                m_PlaybackSession.Resume();
            }
        }
        else
        {
            ImGui::BeginDisabled();
            ImGui::Button("Pause", ImVec2(100, 40));
            ImGui::EndDisabled();
        }

        ImGui::EndDisabled();

        ImGui::Separator();

        // Playback status
        ImGui::Text("Status: %s",
            m_PlaybackSession.IsPlaying() ?
            (m_PlaybackSession.IsPaused() ? "PAUSED" : "PLAYING") :
            "STOPPED");

        if (m_PlaybackSession.IsPlaying())
        {
            ImGui::Text("Current Event: %zu / %zu",
                m_CurrentEventIndex,
                m_PlaybackSession.GetTotalEvents());

            ImGui::Text("Time: %.2fs / %.2fs",
                m_PlaybackSession.GetElapsedTime(),
                m_PlaybackSession.GetTotalDuration());

            ImGui::ProgressBar(m_CurrentProgress, ImVec2(-1, 0));
        }

        ImGui::Separator();

        // Stats
        ImGui::Text("Total Plays: %d", m_TotalPlays);
    }

    void PlaybackTab::LoadRecordingsList()
    {
        m_AvailableRecordings = Serialization::GetAvailableRecordings();
        LUMINA_LOG_INFO("Found {} recordings", m_AvailableRecordings.size());
    }

    void PlaybackTab::LoadSelectedRecording()
    {
        if (m_SelectedRecordingIndex < 0 || m_SelectedRecordingIndex >= m_AvailableRecordings.size())
            return;

        std::string filepath = "recordings/" + m_AvailableRecordings[m_SelectedRecordingIndex] + ".rec";

        if (Serialization::LoadRecording(m_LoadedRecording, filepath))
        {
            m_HasLoadedRecording = true;
            LUMINA_LOG_INFO("Loaded recording: {}", m_LoadedRecording.Name);
        }
        else
        {
            m_HasLoadedRecording = false;
            LUMINA_LOG_ERROR("Failed to load recording: {}", filepath);
        }
    }
}