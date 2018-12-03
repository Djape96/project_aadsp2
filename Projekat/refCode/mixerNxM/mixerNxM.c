
/*
* NxM Mixer 
*
*
*  The algorithm inplements generic NxM mixer
*
*          N
*         ---
*         \
*  y[i] =  >  a[i][j] * x[j]
*         /
*         ---
*         j=1
*
*  i = 1,2,3,...,M
*/

#define BLOCK_SIZE 16
#define N 16   // max number of input channels
#define M 16   // max number of output channels

typedef struct {
	int enable; 	//non-zero to enable this function
	int n_input_channels;
	int m_output_channels;
	float output_gain;		//amount of gain to apply to all output channels
    float gain_matrix[N][M];  // gain_controls
} mixerNxM_data;


void mixerNxM_processing(float** input, float** output, mixerNxM_data data)
{
	int i = 0;
	int in = 0;
	int out = 0;
	int gain_index = 0;
	float temp = 0;
	float current_gain;
	float* brick_in;
	float* brick_out;
	
	/* bypass */
	if(!(data.enable))
	{
		for(out = 0; out < data.m_output_channels; out++)
		{
			brick_out = output[out];
			brick_in = input[out];

			for(i = 0; i < BLOCK_SIZE; i++)
			{
				/* if N < M, bypass N channels to output, other outputs set to zero*/
				if(out >= data.n_input_channels)
				{
					brick_out[i] = 0;
				}
				else
				{
					brick_out[i] = brick_in[i];
				}
			}
		}
	}
	/* processing */
	else
	{
		for(out = 0; out < data.m_output_channels; out++)
		{
			brick_out = output[out];
			/* clear old out values */
			for(i = 0; i < BLOCK_SIZE; i++)
			{
				brick_out[i] = 0;
			}

			for(in = 0; in < data.n_input_channels; in++)
			{
				brick_in = input[in];
				for(i = 0; i < BLOCK_SIZE; i++)
				{
					brick_out[i] += data.gain_matrix[in][out] * brick_in[i];
				}
			}

			for(i = 0; i < BLOCK_SIZE; i++)
			{
				brick_out[i] = brick_out[i] * data.output_gain;
			}
		}
	}
}


