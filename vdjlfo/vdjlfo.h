//////////////////////////////////////////////////////////////////////////
//
// VirtualDJ LFO Plugin
// Multiple LFOs and random number generator
// Values are stored in VDJ variables instead of processing audio
//
//////////////////////////////////////////////////////////////////////////

#ifndef VDJLFOPLUGIN_H
#define VDJLFOPLUGIN_H

#include "./vdj_headers/vdjDsp8.h"
#include <vector>
#include <random>

// Constants for LFO types
#define LFO_SINE 0
#define LFO_SAW 1
#define LFO_SQUARE 2
#define LFO_TRIANGLE 3
#define LFO_RANDOM 4

// Structure to store LFO parameters
struct LfoParams {
    int type;           // LFO type
    float frequency;    // Frequency in Hz
    float amplitude;    // Amplitude (0.0 - 1.0)
    float phase;        // Phase offset (0.0 - 1.0)
    float offset;       // Center offset (0.0 - 1.0)
    char variableName[64]; // VDJ variable name to store the value
    double currentPhase; // Current phase for this LFO
    bool enabled;        // Whether this LFO is enabled
};

class VDJLfoPlugin : public IVdjPluginDsp8
{
public:
    VDJLfoPlugin();

    virtual HRESULT VDJ_API OnLoad();
    virtual HRESULT VDJ_API OnParameter(int id);
    virtual HRESULT VDJ_API OnGetParameterString(int id, char* outParam, int outParamSize);
    virtual HRESULT VDJ_API OnStart();
    virtual HRESULT VDJ_API OnStop();
    virtual HRESULT VDJ_API OnProcessSamples(float* buffer, int nb);
    virtual HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8* info);

private:
    void CountActiveLfos();
    float CalculateLfoValue(int index);

    // Plugin parameters
    int pluginEnabled;

    // LFO parameters
    std::vector<LfoParams> lfos;
    int maxLfos;
    int activeLfos;
    float randomValues[16]; // For random type LFOs

    // Random generator
    bool randomEnabled;
    float randomUpdateInterval;
    char randomVariableName[64];
    double lastRandomUpdate;
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;
};

#endif // VDJLFOPLUGIN_H