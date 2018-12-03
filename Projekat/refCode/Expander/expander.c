/*
 *
 * This element can act as a dynamic range expander. A expander changes the
 * amplitude of all samples below a specific threshold with a specific ratio
 * If soft-knee mode is selected the ratio is applied smoothly.
 *
 * parameters: ratio [0.0, 8.0)
 *             threshold [0.0, 1.0)
 */

typedef struct __AudioExpander {
	float threshold;
	float ratio;
} AudioExpander_t;

void audio_expander_init(AudioExpander_t * expander) {
	expander->ratio = 1.0;
	expander->threshold = 0.0;
}

void gst_audio_dynamic_transform_expander_float(AudioExpander_t * expander,
		float * data, unsigned int num_samples) {
	double val, threshold = expander->threshold, zero;
	int i;

	/* Nothing to do for us here if threshold equals 0.0
	 * or ratio equals 1.0 */
	if (expander->threshold == 0.0 || expander->ratio == 1.0)
		return;

	/* zero crossing of our function */
	if (expander->ratio != 0.0)
		zero = threshold - threshold / expander->ratio;
	else
		zero = 0.0;

	if (zero < 0.0)
		zero = 0.0;

	for (i = 0; i < num_samples; i++) {
		val = data[i];

		if (val < threshold && val > zero) {
			val = expander->ratio * val + threshold * (1.0 - expander->ratio);
		} else if ((val <= zero && val > 0.0) || (val >= -zero && val < 0.0)) {
			val = 0.0;
		} else if (val > -threshold && val < -zero) {
			val = expander->ratio * val - threshold * (1.0 - expander->ratio);
		}
		data[i] = (float) val;
	}
}

