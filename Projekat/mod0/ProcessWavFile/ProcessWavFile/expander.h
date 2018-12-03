#ifndef EXPANDER_H
#define EXPANDER_H

typedef struct __AudioExpander {
	double threshold;
	double ratio;
} AudioExpander_t;


void gst_audio_dynamic_transform_expander_double(AudioExpander_t * expander,
	double * data, unsigned int num_samples);

void audio_expander_init(AudioExpander_t * expander);

#endif
