//////////////////////////////////////////////////////////////////////////
// VirtualDJ LFO Plugin
// Multiple LFOs and random number generator
// Values are stored in VDJ variables instead of processing audio
//////////////////////////////////////////////////////////////////////////

#include "./vdj_headers/vdjDsp8.h"
#include <math.h>
#include <vector>
#include <string>
#include <random>
#include <time.h>

// Constants for LFO types
#define LFO_SINE 0
#define LFO_SAW 1
#define LFO_SQUARE 2
#define LFO_TRIANGLE 3
#define LFO_RANDOM 4
#define M_PI 3.14159265358979323846

// Structure to store LFO parameters
struct LfoParams {
    float type;           // LFO type
    float frequency;    // Frequency in Hz
    float amplitude;    // Amplitude (0.0 - 1.0)
    float phase;        // Phase offset (0.0 - 1.0)
    float offset;       // Center offset (0.0 - 1.0)
    char variableName[64]; // VDJ variable name to store the value
    double currentPhase; // Current phase for this LFO
    int enabled;        // Whether this LFO is enabled
};

class VDJLfoPlugin : public IVdjPluginDsp8
{
public:
    VDJLfoPlugin() {
        // Initialize random number generator
        srand((unsigned int)time(NULL));
        std::random_device rd;
        rng = std::mt19937(rd());
        dist = std::uniform_real_distribution<float>(0.0f, 1.0f);

        // Default values
        randomUpdateInterval = 0.1f; // 100ms
        randomVariableName[0] = '\0';
        lastRandomUpdate = 0.0;

        // Initialize default LFOs
        maxLfos = 4;
        activeLfos = 0;
        lfos.resize(maxLfos);

        // Reset all LFOs
        for (int i = 0; i < maxLfos; i++) {
            lfos[i].type = LFO_SINE;
            lfos[i].frequency = 1.0f;
            lfos[i].amplitude = 0.5f;
            lfos[i].phase = 0.0f;
            lfos[i].offset = 0.5f;
            lfos[i].currentPhase = 0.0;
            lfos[i].enabled = 0;
            sprintf_s(lfos[i].variableName, "lfo_%d", i + 1);
        }
    }

    // Called when plugin is loaded
    HRESULT VDJ_API OnLoad() {
        // Plugin settings
        DeclareParameterSwitch(&pluginEnabled, 0, "Enable Plugin", "Enable", true);

        // LFO controls - for each LFO
        for (int i = 0; i < maxLfos; i++) {
            char name[64], shortName[64];

            // Enable switch for this LFO
            sprintf_s(name, "Enable LFO %d", i + 1);
            sprintf_s(shortName, "LFO%dEn", i + 1);
            DeclareParameterSwitch(&lfos[i].enabled, 10 + i * 10, name, shortName, false);

            // LFO type
            sprintf_s(name, "LFO %d Type", i + 1);
            sprintf_s(shortName, "LFO%dType", i + 1);
            DeclareParameterSlider(&lfos[i].type, 11 + i * 10, name, shortName, LFO_SINE);

            // Frequency
            sprintf_s(name, "LFO %d Freq", i + 1);
            sprintf_s(shortName, "LFO%dF", i + 1);
            DeclareParameterSlider(&lfos[i].frequency, 12 + i * 10, name, shortName, 1.0f);

            // Amplitude
            sprintf_s(name, "LFO %d Amp", i + 1);
            sprintf_s(shortName, "LFO%dA", i + 1);
            DeclareParameterSlider(&lfos[i].amplitude, 13 + i * 10, name, shortName, 0.5f);

            // Phase
            sprintf_s(name, "LFO %d Phase", i + 1);
            sprintf_s(shortName, "LFO%dP", i + 1);
            DeclareParameterSlider(&lfos[i].phase, 14 + i * 10, name, shortName, 0.0f);

            // Offset
            sprintf_s(name, "LFO %d Offset", i + 1);
            sprintf_s(shortName, "LFO%dO", i + 1);
            DeclareParameterSlider(&lfos[i].offset, 15 + i * 10, name, shortName, 0.5f);

            // Variable name
            sprintf_s(name, "LFO %d Variable", i + 1);
            sprintf_s(shortName, "LFO%dVar", i + 1);
            DeclareParameterString(lfos[i].variableName, 16 + i * 10, name, shortName, sizeof(lfos[i].variableName));
        }

        // Random generator settings
        DeclareParameterSwitch(&randomEnabled, 90, "Enable Random", "RndEn", false);
        DeclareParameterSlider(&randomUpdateInterval, 91, "Random Interval (s)", "RndInt", 0.1f);
        DeclareParameterString(randomVariableName, 92, "Random Variable", "RndVar", sizeof(randomVariableName));

        // Count active LFOs
        CountActiveLfos();

        strcpy_s(randomVariableName, "random_value");

        return S_OK;
    }

    // Called when parameter changes
    HRESULT VDJ_API OnParameter(int id) {
        // Recount active LFOs if any LFO was enabled/disabled
        if ((id >= 10 && id < 90 && (id % 10) == 0) || id == 0) {
            CountActiveLfos();
        }

        return S_OK;
    }

    // Get parameter string representation
    HRESULT VDJ_API OnGetParameterString(int id, char* outParam, int outParamSize) {
        for (int i = 0; i < maxLfos; i++) {
            // LFO type names
            if (id == 11 + i * 10) {
                switch ((int)(lfos[i].type * 4)) {
                case LFO_SINE:
                    strcpy_s(outParam, outParamSize, "Sine");
                    return S_OK;
                case LFO_SAW:
                    strcpy_s(outParam, outParamSize, "Saw");
                    return S_OK;
                case LFO_SQUARE:
                    strcpy_s(outParam, outParamSize, "Square");
                    return S_OK;
                case LFO_TRIANGLE:
                    strcpy_s(outParam, outParamSize, "Triangle");
                    return S_OK;
                case LFO_RANDOM:
                    strcpy_s(outParam, outParamSize, "Random");
                    return S_OK;
                }
            }

            // Frequency display
            if (id == 12 + i * 10) {
                sprintf_s(outParam, outParamSize, "%.2f Hz", lfos[i].frequency);
                return S_OK;
            }
        }

        // Random update interval
        if (id == 91) {
            sprintf_s(outParam, outParamSize, "%.2f s", randomUpdateInterval);
            return S_OK;
        }

        return E_NOTIMPL;
    }

    // Called when plugin is started
    HRESULT VDJ_API OnStart() {
        // Reset all LFO phases
        for (int i = 0; i < maxLfos; i++) {
            lfos[i].currentPhase = lfos[i].phase;
        }
        lastRandomUpdate = 0.0;

        return S_OK;
    }

    // Called when plugin is stopped
    HRESULT VDJ_API OnStop() {
        return S_OK;
    }

    // Process audio buffer - used here just for timing
    HRESULT VDJ_API OnProcessSamples(float* buffer, int nb) {
        if (!pluginEnabled) {
            return S_OK;
        }

        // Calculate time increment for this buffer
        double timeIncrement = (double)nb / SampleRate;

        // Process all active LFOs
        for (int i = 0; i < maxLfos; i++) {
            if (!lfos[i].enabled) continue;

            // Update LFO phase
            lfos[i].currentPhase += timeIncrement * lfos[i].frequency;

            // Keep phase in 0-1 range
            while (lfos[i].currentPhase >= 1.0) {
                lfos[i].currentPhase -= 1.0;
            }

            // Calculate LFO value based on type
            float value = CalculateLfoValue(i);

            // Set value to VDJ variable
            if (lfos[i].variableName[0] != '\0') {
                char command[256];
                sprintf_s(command, "set_var $%s %f", lfos[i].variableName, value);
                SendCommand(command);
            }
        }

        // Process random generator
        if (randomEnabled && randomVariableName[0] != '\0') {
            lastRandomUpdate += timeIncrement;

            if (lastRandomUpdate >= randomUpdateInterval) {
                // Generate new random value
                float randomValue = dist(rng);

                // Set value to VDJ variable
                char command[256];
                sprintf_s(command, "set_var $%s %f", randomVariableName, randomValue);
                SendCommand(command);

                lastRandomUpdate = 0.0;
            }
        }

        return S_OK;
    }

    // Plugin info
    HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8* info) {
        info->PluginName = "Multi LFO Generator";
        info->Author = "VDJ Plugin SDK Example";
        info->Description = "Generates multiple LFO values and random numbers to VDJ variables";
        info->Version = "1.0";
        info->Flags = VDJFLAG_PROCESSFIRST;  // Process early in the chain

        return S_OK;
    }

private:
    void CountActiveLfos() {
        activeLfos = 0;
        for (int i = 0; i < maxLfos; i++) {
            if (lfos[i].enabled) {
                activeLfos++;
            }
        }
    }

    float CalculateLfoValue(int index) {
        double phase = lfos[index].currentPhase;
        float result = 0.0f;

        // Calculate based on LFO type
        switch ((int)(lfos[index].type*4)) {
        case LFO_SINE:
            result = (float)(sin(phase * 2.0 * M_PI) * 0.5 + 0.5);
            break;

        case LFO_SAW:
            result = (float)phase;
            break;

        case LFO_SQUARE:
            result = (phase < 0.5) ? 1.0f : 0.0f;
            break;

        case LFO_TRIANGLE:
            if (phase < 0.5) {
                result = (float)(phase * 2.0);
            }
            else {
                result = (float)((1.0 - phase) * 2.0);
            }
            break;

        case LFO_RANDOM:
            // Use random values that change at zero-crossings
            if (lfos[index].currentPhase < lfos[index].phase) {
                // We've wrapped around
                randomValues[index] = dist(rng);
            }
            result = randomValues[index];
            break;
        }

        // Apply amplitude and offset
        result = result * lfos[index].amplitude + (lfos[index].offset - lfos[index].amplitude / 2.0f);

        // Clamp to 0-1 range
        if (result < 0.0f) result = 0.0f;
        if (result > 1.0f) result = 1.0f;

        return result;
    }

    // Plugin parameters
    int pluginEnabled;

    // LFO parameters
    std::vector<LfoParams> lfos;
    int maxLfos;
    int activeLfos;
    float randomValues[16]; // For random type LFOs

    // Random generator
    int randomEnabled;
    float randomUpdateInterval;
    char randomVariableName[64];
    double lastRandomUpdate;
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;
};

// DLL export function
VDJ_EXPORT HRESULT VDJ_API DllGetClassObject(const GUID& rclsid, const GUID& riid, void** ppObject) {
    if (memcmp(&rclsid, &CLSID_VdjPlugin8, sizeof(GUID)) == 0 &&
        memcmp(&riid, &IID_IVdjPluginDsp8, sizeof(GUID)) == 0) {
        *ppObject = new VDJLfoPlugin();
        return S_OK;
    }

    return CLASS_E_CLASSNOTAVAILABLE;
}
