/*

 This code accompanies the textbook:



 Digital Audio Effects: Theory, Implementation and Application

 Joshua D. Reiss and Andrew P. McPherson



 ---



 Tremolo: amplitude modulation using a low-frequency oscillator

 See textbook Chapter 5: Amplitude Modulation



 Code by Andrew McPherson, Brecht De Man and Joshua Reiss



 ---


 This program is free software: you can redistribute it and/or modify

 it under the terms of the GNU General Public License as published by

 the Free Software Foundation, either version 3 of the License, or

 (at your option) any later version.



 This program is distributed in the hope that it will be useful,

 but WITHOUT ANY WARRANTY; without even the implied warranty of

 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

 GNU General Public License for more details.


 You should have received a copy of the GNU General Public License

 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <math.h>

//==============================================================================

#define SAMPLE_RATE 48000
#define BLOCK_SIZE 16
#define PI 3.14159265358979323846

typedef enum

{
	kWaveformTriangle, kWaveformSquare
} wave_forms_t;

// Adjustable parameters:
typedef struct {
	int numChannels;

	float LFO_frequency;  // LFO frequency (Hz)

	float depth;      // Depth of effect (0-1)

	wave_forms_t waveform;      // What shape should be used for the LFO

	float lfoPhase;

	float inverseSampleRate;
} tremolo_struct_t;

float lfo(float phase, wave_forms_t waveform);

void init(tremolo_struct_t * data) {

	// Set default values:
	data->LFO_frequency = 2.0;
	data->depth = 1.0;
	data->waveform = kWaveformSquare;
	data->lfoPhase = 0.0;
	data->inverseSampleRate = 1.0 / SAMPLE_RATE;
}

void processBlock(double* input, double* output, tremolo_struct_t* data,
		int numSamples) {

	float ph;

	// Make a temporary copy of any state variables which need to be
	// maintained between calls to processBlock(). Each channel needs to be processed identically
	// which means that the activity of processing one channel can't affect the state variable for
	// the next channel.
	ph = data->lfoPhase;

	for (int i = 0; i < numSamples; ++i)
	{
		const float in = input[i];

		// Ring modulation is easy! Just multiply the waveform by a periodic carrier
		output[i] = in * (1.0f - data->depth * lfo(ph, data->waveform));

		// Update the carrier and LFO phases, keeping them in the range 0-1
		ph += data->LFO_frequency * data->inverseSampleRate;
		if (ph >= 1.0)
			ph -= 1.0;
	}

	// Having made a local copy of the state variables for each channel, now transfer the result
	// back to the main state variable so they will be preserved for the next call of processBlock()
	data->lfoPhase = ph;
}

//==============================================================================

float lfo(float phase, wave_forms_t waveform)
{
	switch (waveform)
	{
	case kWaveformTriangle:
		if (phase < 0.25f)
			return 0.5f + 2.0f * phase;
		else if (phase < 0.75f)
			return 1.0f - 2.0f * (phase - 0.25f);
		else
			return 2.0f * (phase - 0.75f);

	case kWaveformSquare:
		if (phase < 0.5f)
			return 1.0f;
		else
			return 0.0f;
	default:
		return 1.0;
	}
}
