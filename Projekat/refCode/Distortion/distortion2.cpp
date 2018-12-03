/*

 This code accompanies the textbook:



 Digital Audio Effects: Theory, Implementation and Application

 Joshua D. Reiss and Andrew P. McPherson



 ---



 Distortion: distortion effect using different characteristic curves

 See textbook Chapter 7: Overdrive, Distortion and Fuzz



 Code by Brecht De Man, Joshua Reiss and Andrew McPherson



 When using this code (or a modified version thereof), please cite:



 Brecht De Man and Joshua D. Reiss, "Adaptive Control of Amplitude

 Distortion Effects," 53rd Conference of the Audio Engineering Society,

 2014.



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
typedef enum {
	FULL_WAVE_RECTIFIER, HALF_WAVE_RECTIFIER
} clipping_type_t;

typedef struct {
	int numChannels;
	int numSamples;
	int sampleRate;
	clipping_type_t type;
	float gain;
} distortion_state_t;

//-----------------------------------------------------------------------------

// P R O C E S S   B L O C K

void processSingleChannel(double* input, double* output,
		distortion_state_t state)

{
	// Apply gain
	for (int i = 0; i < state.numSamples; i++) {
		output[i] = input[i] * state.gain;
	}

	// Apply distortion (sample per sample)
	switch (state.type) {

	case FULL_WAVE_RECTIFIER:
	{
		for (int sample = 0; sample < state.numSamples; ++sample)

		{
			output[sample] = fabs(output[sample]);
		}

		break;
	}

	case HALF_WAVE_RECTIFIER:
	{
		for (int sample = 0; sample < state.numSamples; ++sample)
		{
			output[sample] = 0.5f * (fabs(output[sample]) + output[sample]);
		}
		break;
	}
	default:
		break;

	}

}
