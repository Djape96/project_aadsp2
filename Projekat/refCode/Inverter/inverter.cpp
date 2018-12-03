/**
 *
 * Swaps upper and lower half of audio samples. Mixing an inverted sample on top of
 * the original with a slight delay can produce effects that sound like resonance.
 * Creating a stereo sample from a mono source, with one channel inverted produces wide-stereo sounds.
 *
 */

typedef struct {
	float degree;
	float gain;
} inverter_data_t;

void audio_invert_init(inverter_data_t * data, float degree, float gain)
{
	data->degree = degree;
	data->gain = gain;
}

void
gst_audio_invert_transform(inverter_data_t * data,
float * input, float * output, unsigned int num_samples)
{
  int i;
  float dry = 1.0 - data->degree;
  float val;

  for (i = 0; i < num_samples; i++) {
	val = input[i] * dry - (1.0 + input[i]) * data->degree;
    output[i] = val * data->gain;
  }
}
