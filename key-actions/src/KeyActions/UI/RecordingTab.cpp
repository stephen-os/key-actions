#include "RecordingTab.h"

#include "Lumina/Core/Log.h"

#include "KeyActions/Core/Settings.h"

#include "KeyActions/UI/Styles/Theme.h"

#include "KeyActions/UI/Components/Buttons.h"
#include "KeyActions/UI/Components/Text.h"
#include "KeyActions/UI/Components/Headings.h"
#include "KeyActions/UI/Components/Layouts.h"
#include "KeyActions/UI/Components/Keybinds.h"
#include "KeyActions/UI/Components/Inputs.h"

#include <cstring>

namespace KeyActions
{
    RecordingTab::RecordingTab() : Tab("Recording"), m_EventPanel(1000) {}
    
    void RecordingTab::OnAttach()
    {
        m_RecordingSession.SetEventRecordedCallback([this](const RecordedEvent& event) {
            m_EventPanel.AddEvent(event);
            });

        m_RecordingSession.SetRecordingStartedCallback([this]() {
            LUMINA_LOG_INFO("Recording started callback");
            });

        m_RecordingSession.SetRecordingStoppedCallback([this]() {
            const Recording& recording = m_RecordingSession.GetRecording();
            if (Serialization::SaveRecording(recording))
            {
                LUMINA_LOG_INFO("Recording saved successfully!");
            }
            else
            {
                LUMINA_LOG_ERROR("Failed to save recording!");
            }
            });

        LUMINA_LOG_INFO("Recording tab initialized");
    }

    void RecordingTab::OnDetach()
    {
        if (m_RecordingSession.IsRecording())
        {
            m_RecordingSession.Stop();
        }
    }

    void RecordingTab::OnUpdate(float timestep)
    {
        m_RecordingSession.Update(timestep);
    }

    void RecordingTab::OnEvent(Event& e)
    {
        Lumina::EventDispatcher dispatcher(e);

        // Forward  events to recording session
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) {

            const auto& settings = Settings::Data();

            if (!m_CapturedKeyCombo.Contains(event.GetKeyCode()))
			{
				m_CapturedKeyCombo.Add(event.GetKeyCode());
			}

            if (m_CapturedKeyCombo == settings.StartRecording)
            {
                if (!m_RecordingSession.IsRecording() &&
                    !m_RecordingSession.IsWaitingForDelay() &&
                    strnlen(m_RecordingName, sizeof(m_RecordingName)) > 0)
                {
                    m_RecordingSession.Start({ m_RecordingName, m_RecordMouseMovement, m_InitialDelay });
                    return true;
                }
            }

            if (m_CapturedKeyCombo == settings.StopRecording)
			{
                if (m_RecordingSession.IsRecording() || m_RecordingSession.IsWaitingForDelay())
				{
				    m_RecordingSession.Stop();
				    return true;
				}
			}

            m_RecordingSession.OnKeyPressed(event);
            return false;
            });

        dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& event) {
            
            const auto& settings = Settings::Data();

            bool wasPartOfStartCombo = settings.StartRecording.Contains(event.GetKeyCode()) && m_CapturedKeyCombo == settings.StartRecording;
            bool wasPartOfStopCombo = settings.StopRecording.Contains(event.GetKeyCode()) && m_CapturedKeyCombo == settings.StopRecording;

            m_CapturedKeyCombo.Remove(event.GetKeyCode());

            if (wasPartOfStartCombo || wasPartOfStopCombo)
            {
                return true;
            }

            m_RecordingSession.OnKeyReleased(event);
            return false;
            });

        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& event) {
            m_RecordingSession.OnMouseButtonPressed(event);
            return false;
            });

        dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& event) {
            m_RecordingSession.OnMouseButtonReleased(event);
            return false;
            });

        dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& event) {
            m_RecordingSession.OnMouseMoved(event);
            return false;
            });

        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) {
            m_RecordingSession.OnMouseScrolled(event);
            return false;
            });
    }

    void RecordingTab::OnRender()
    {
        UI::BeginPanel("RecordControls", ImVec2(320, 0));

        UI::HeadingTwo("Recording Settings");

        UI::Label("Name of Recording:");
        
        UI::InputText("##RecordingName", m_RecordingName, sizeof(m_RecordingName));

        if (m_ShowNameError)
        {
            UI::TextDanger("Name cannot be blank!");
        }

        UI::Spacing();

        UI::Checkbox("Record Mouse Movement", &m_RecordMouseMovement);

        UI::Spacing();

        UI::Label("Delay (seconds):");

        UI::SliderInt("##InitialDelay", &m_InitialDelay, 0, 10);

        UI::SectionSeparator();

        bool isRecording = m_RecordingSession.IsRecording();
        bool isWaiting = m_RecordingSession.IsWaitingForDelay();

        if (UI::ButtonSuccess("Start Recording", UI::Sizes::ButtonFull, (!isRecording && !isWaiting)))
        {
            StartRecording();
        }

        if (UI::ButtonDanger("Stop Recording", UI::Sizes::ButtonFull, (isRecording || isWaiting)))
        {
            StopRecording();
        }

        UI::Spacing();

        if (isWaiting)
        {
            UI::TextWarning("Recording starts in %.1f seconds...", m_RecordingSession.GetDelayTimeRemaining());
        }
        else if (isRecording)
        {
            UI::TextHighlight("Recording... %.2fs", m_RecordingSession.GetElapsedTime());
            UI::TextMuted("Events recorded: %zu", m_RecordingSession.GetEventCount());
        }

        UI::EndPanel();

        UI::SameLine();
        
        UI::BeginPanel("RecordEventPanel", ImVec2(0, 0));

        UI::HeadingTwo("Recorded Events");

        UI::Spacing();

        m_EventPanel.Render(ImVec2(0, 0));

        UI::EndPanel();
    }

    void RecordingTab::StartRecording()
    {
        m_ShowNameError = false;

        if (strlen(m_RecordingName) == 0)
        {
            m_ShowNameError = true;
            return;
        }

        // Clear event panel
        m_EventPanel.Clear();

        // Create settings
        RecordingSettings settings;
        settings.Name = m_RecordingName;
        settings.RecordMouseMovement = m_RecordMouseMovement;
        settings.InitialDelaySeconds = m_InitialDelay;

        // Start recording
        m_RecordingSession.Start(settings);
    }

    void RecordingTab::StopRecording()
    {
        m_RecordingSession.Stop();
    }
}