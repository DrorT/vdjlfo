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

1. Download the latest release from the [Releases](https://github.com/yourusername/vdj-lfo-plugin/releases) page
2. Copy the plugin file to your VirtualDJ plugins folder:
   - Windows: `C:\Program Files\VirtualDJ\Plugins\` or `C:\Users\[YourUsername]\Documents\VirtualDJ\Plugins\`
   - macOS: `/Applications/VirtualDJ/Plugins/` or `~/Documents/VirtualDJ/Plugins/`
3. Restart VirtualDJ
4. The plugin should now appear in the Effects/Plugins list as "Multi LFO Generator"

## Usage

### Basic Setup

1. Load the plugin in VirtualDJ
2. Enable the plugin using the main "Enable Plugin" switch
3. Configure each LFO you want to use:
   - Enable the LFO with its own enable switch
   - Set the LFO type, frequency, amplitude, etc.
   - Set a variable name for the LFO (e.g., "lfo_1", "filter_mod", etc.)
4. If desired, enable the random generator and set its update interval and variable name

### Using LFO Values in VirtualDJ

Once configured, the plugin continuously updates the variables you've specified. These variables can be used in:

- **VDJScript**: Access variables in your scripts with `%variable_name%`
- **Mappings**: Use variables in MIDI/controller mappings
- **Actions**: Create custom actions that respond to variable changes

### LFO Types

1. **Sine**: Smooth oscillation between min and max values
2. **Saw**: Linear ramp from min to max, then instant reset
3. **Square**: Alternates between min and max values
4. **Triangle**: Linear ramp up, then linear ramp down
5. **Random**: Random values that change at LFO cycle boundaries

### Example Use Cases

- **Automated Filter Modulation**: Map an LFO to a filter cutoff parameter
- **Tempo-Synced Effects**: Set LFO frequency to match song BPM divisions
- **Dynamic EQ Changes**: Use an LFO to slowly modulate EQ parameters
- **Random Track Selection**: Use the random generator to select tracks
- **Visual Effects**: Drive video effects with LFO values

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
- **LFO x Variable**: VirtualDJ variable name where the value is stored

### Random Generator Settings

- **Enable Random**: Turns the random generator on/off
- **Random Interval**: How often (in seconds) to generate a new random value
- **Random Variable**: VirtualDJ variable name where the random value is stored

## Building from Source

### Prerequisites

- C++ compiler (Visual Studio for Windows, Xcode for Mac)
- VirtualDJ SDK

### Windows (Visual Studio)

1. Create a new DLL project
2. Add the source files and VirtualDJ SDK headers
3. Configure for x64 (or x86 if needed)
4. Build the solution

### macOS (Xcode)

1. Create a new Bundle project
2. Add the source files and VirtualDJ SDK headers
3. Set the bundle extension to ".bundle"
4. Build the project

## License

[MIT License](LICENSE)

## Acknowledgments

- Built using the VirtualDJ Plugin SDK
- Inspired by the need for modulation sources in DJ software
