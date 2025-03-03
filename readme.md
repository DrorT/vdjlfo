# VirtualDJ LFO Plugin

A plugin for VirtualDJ that generates multiple LFO (Low-Frequency Oscillator) signals and random values. Instead of processing audio, this plugin stores values in VirtualDJ variables that can be used in scripts, mappings, and custom interfaces.

## Features

- **Four independent LFOs** - Each with configurable:

  - Type (Sine, Saw, Square, Triangle, Random)
  - Frequency (Hz)
  - Amplitude (0.0-1.0)
  - Phase offset (0.0-1.0)
  - Center offset (0.0-1.0)
  - Custom variable name

- **Random Number Generator**

  - Configurable update interval
  - Custom variable name for storing random values
  - Generates random values between 0.0 and 1.0

- **Efficient Design**
  - No audio processing (passes audio through unchanged)
  - Only active LFOs are calculated
  - Uses VirtualDJ's audio engine timing for precise synchronization

## Installation

1. Download the latest release from the [Releases](https://github.com/DrorT/vdjlfo/releases) page
2. Copy the plugin file to your VirtualDJ plugins folder:
   - Windows: `C:\Users\[USERNAME]\AppData\Local\VirtualDJ\Plugins64\SoundEffect`
   - macOS: ?
3. Restart VirtualDJ
4. The plugin should now appear in the Effects/Plugins list as "vdjlfo"

## Usage

### Basic Setup

1. Load the plugin in VirtualDJ, on a specific deck or preferably on master
2. Open the plugin setup
3. Enable the LFOs you want to use (same for random value)
4. Configure each LFO you want to use:
   - Enable the LFO with its own enable switch
   - Set the LFO type, frequency, amplitude, etc.
   - Variable name are set to "$lfo_1", "$lfo_2", "$lfo_3", "$lfo_4", "$random_value" (can be changed in ini file)
5. If desired, enable the random generator and set its update interval and variable name

### Using LFO Values in VirtualDJ

Once configured, the plugin continuously updates the variables you've specified. These variables can be used in scripts -

`repeat_start 'lfo_example' 25ms 1000 & get_var '$random_value' & param_cast float & deck 1 level`

`repeat_start 'lfo_example' 25ms 1000 & get_var '$lfo_1' & param_cast float & deck 1 pitch`

### LFO Types

1. **Sine**: Smooth oscillation between min and max values
2. **Saw**: Linear ramp from min to max, then instant reset
3. **Square**: Alternates between min and max values
4. **Triangle**: Linear ramp up, then linear ramp down
5. **Random**: Random values that change at LFO cycle boundaries

## Parameter Details

### Global Settings

- **Enable Plugin**: Master switch for the entire plugin

### Per-LFO Settings

For each of the four LFOs:

- **Enable LFO x**: Turns this specific LFO on/off
- **LFO x Type**: The waveform type (Sine, Saw, Square, Triangle, Random)
- **LFO x Freq**: Frequency in Hz (how fast the LFO cycles)
- **LFO x Amp**: Amplitude (controls the range of values generated)
- **LFO x Phase**: Starting phase/offset in the cycle (0.0-1.0)
- **LFO x Offset**: Center value around which the LFO oscillates
- **LFO x Variable**: $lfo_1, $lfo_2, $lfo_3, $lfo_4 (can be changed in the ini file)

### Random Generator Settings

- **Enable Random**: Turns the random generator on/off
- **Random Interval**: How often (in seconds) to generate a new random value
- **Random Variable**: $random_value (can be changed in the ini file)

## Building from Source

### Prerequisites

- C++ compiler - Visual Studio for Windows

### Windows (Visual Studio)

- 1st time compiling on windows (only did c++ on linux before), hopefully you can just use the same sln file in visual studio code
- just build and you will get the DLL

### macOS (Xcode)

I do not have mac to try to build for mac, so if you can check if it works and create a PR with updates

## License

[MIT License](LICENSE) - fork it, use it, change it, you can mention this original repo or not (I think that what MIT means)

## Acknowledgments

- Built using the VirtualDJ Plugin SDK
- Written with the help of Clause ai, and github co-pilot to figure out how to build it
