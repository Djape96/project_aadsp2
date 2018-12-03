/*
 *
 * This element can act as a dynamic range compressor. A compressor changes the
 * amplitude of all samples above a specific threshold with a specific ratio. If
 * soft-knee mode is selected the ratio is applied smoothly.
 *
 * parameters: ratio [0.0, 1.0)
 *             threshold [0.0, 1.0)
 */

typedef struct __AudioCompressor {
	float threshold;
	float ratio;
} AudioCompressor_t;

void audio_compressor_init(AudioCompressor_t * compressor) {
	compressor->ratio = 1.0;
	compressor->threshold = 0.0;
}

void gst_audio_dynamic_transform_compressor_float(
		AudioCompressor_t * compressor, float * data, unsigned int num_samples) {
	double val, threshold = compressor->threshold;
	int i;

	/* Nothing to do for us if ratio == 1.0. */
	if (compressor->ratio == 1.0)
		return;

	for (i = 0; i < num_samples; i++) {
		val = data[i];

		if (val > threshold) {
			val = threshold + (val - threshold) * compressor->ratio;
		} else if (val < -threshold) {
			val = -threshold + (val + threshold) * compressor->ratio;
		}
		data[i] = (float) val;
	}

}
