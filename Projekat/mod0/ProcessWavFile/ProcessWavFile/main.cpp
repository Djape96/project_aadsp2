#include <stdlib.h>
#include <string.h>
#include "WAVheader.h"
#include "expander.h"
#include <cmath>

#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8
#define NUM_OF_CHANNELS 5

enum output_mode {
					mode1, // (2_0_0) Samo LS i RS
					mode2, // (3_2_0) Svi, nemamo bass (to je treci broj)
};

double sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

AudioExpander_t expander;

void processing(double pInbuff[][BLOCK_SIZE], double pOutbuff[][BLOCK_SIZE])
{
	const double tap_gain[4] = { 0.025,0.32,0.25,0.5 };
	output_mode o_mode;
	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		pInbuff[0][i] = pInbuff[0][i] * tap_gain[2]; // L -> input gain = -6db
		pInbuff[1][i] = pInbuff[1][i] * tap_gain[2]; //R -> input gain = -6db

	}

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		pInbuff[2][i] = pInbuff[0][i] * tap_gain[0]; //L[0]-> input gain = -16db
	}

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		pInbuff[3][i] = pInbuff[0][i] * tap_gain[2];  //L[1]-> input gain = -6db
	}

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		pInbuff[4][i] = pInbuff[0][i] * tap_gain[1];  //L[2]-> input gain = -5db
	}

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		pInbuff[5][i] = pInbuff[0][i] * tap_gain[3]; //L[3]-> input gain = -3db
	}

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		pInbuff[6][i] = pInbuff[1][i] * (-1); //R[0]-> (-1)
	}

	gst_audio_dynamic_transform_expander_double(&expander, pInbuff[0], BLOCK_SIZE);
	gst_audio_dynamic_transform_expander_double(&expander, pInbuff[1], BLOCK_SIZE);

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		if (o_mode == mode1)
		{
			pOutbuff[0][i] = pInbuff[2][i];
		}
		else
		{
			pOutbuff[0][i] = pInbuff[3][i];
		}

		pOutbuff[1][i] = pInbuff[0][i];

		if (o_mode == mode1)
		{
			pOutbuff[2][i] = pInbuff[5][i];
		}
		else
		{
			pOutbuff[2][i] = pInbuff[4][i];
		}

		pOutbuff[3][i] = pInbuff[6][i];
		pOutbuff[4][i] = pInbuff[1][i];
	}
}

int main(int argc, char* argv[])
{
	FILE *wav_in=NULL;
	FILE *wav_out=NULL;
	char WavInputName[256];
	char WavOutputName[256];
	WAV_HEADER inputWAVhdr,outputWAVhdr;	
	int enable_processing = 0;
	output_mode mode_o = mode1;

	AudioExpander_t expander;



	if (argc == 5)
	{
		enable_processing = atoi(argv[3]);
		mode_o = (output_mode)atoi([argv[4]);

	}

	// Init channel buffers
	for (int i = 0; i < MAX_NUM_CHANNEL; i++)
		memset(&sampleBuffer[i], 0, BLOCK_SIZE);

	// Open input and output wav files
	//-------------------------------------------------
	strcpy(WavInputName,argv[1]);
	wav_in = OpenWavFileForRead (WavInputName,"rb");
	strcpy(WavOutputName,argv[2]);
	wav_out = OpenWavFileForRead (WavOutputName,"wb");
	//-------------------------------------------------

	// Read input wav header
	//-------------------------------------------------
	ReadWavHeader(wav_in,inputWAVhdr);
	//-------------------------------------------------
	
	// Set up output WAV header
	//-------------------------------------------------	
	outputWAVhdr = inputWAVhdr;
	outputWAVhdr.fmt.NumChannels = inputWAVhdr.fmt.NumChannels; // change number of channels

	int oneChannelSubChunk2Size = inputWAVhdr.data.SubChunk2Size/inputWAVhdr.fmt.NumChannels;
	int oneChannelByteRate = inputWAVhdr.fmt.ByteRate/inputWAVhdr.fmt.NumChannels;
	int oneChannelBlockAlign = inputWAVhdr.fmt.BlockAlign/inputWAVhdr.fmt.NumChannels;
	
	outputWAVhdr.data.SubChunk2Size = oneChannelSubChunk2Size*outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.ByteRate = oneChannelByteRate*outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.BlockAlign = oneChannelBlockAlign*outputWAVhdr.fmt.NumChannels;


	// Write output WAV header to file
	//-------------------------------------------------
	WriteWavHeader(wav_out,outputWAVhdr);

	//Init audio expander
	audio_expander_init(&expander);


	// Processing loop
	//-------------------------------------------------	
	{
		int sample;
		int BytesPerSample = inputWAVhdr.fmt.BitsPerSample/8;
		const double SAMPLE_SCALE = -(double)(1 << 31);		//2^31
		int iNumSamples = inputWAVhdr.data.SubChunk2Size/(inputWAVhdr.fmt.NumChannels*inputWAVhdr.fmt.BitsPerSample/8);
		
		// exact file length should be handled correctly...
		for(int i=0; i<iNumSamples/BLOCK_SIZE; i++)
		{	
			for(int j=0; j<BLOCK_SIZE; j++)
			{
				for(int k=0; k<inputWAVhdr.fmt.NumChannels; k++)
				{	
					sample = 0; //debug
					fread(&sample, BytesPerSample, 1, wav_in);
					sample = sample << (32 - inputWAVhdr.fmt.BitsPerSample); // force signextend
					sampleBuffer[k][j] = sample / SAMPLE_SCALE;				// scale sample to 1.0/-1.0 range		
				}
			}
			if (enable_processing)
				processing(sampleBuffer, sampleBuffer);

			for(int j=0; j<BLOCK_SIZE; j++)
			{
				for(int k=0; k<outputWAVhdr.fmt.NumChannels; k++)
				{	
					sample = sampleBuffer[k][j] * SAMPLE_SCALE ;	// crude, non-rounding 			
					sample = sample >> (32 - inputWAVhdr.fmt.BitsPerSample);
					
					if ((o_mode == mode1) && (k != 1 && k != 3)) {
						sample = 0;
					}

					if ((o_mode == mode2) && (k != 0 && k != 4)) {
						sample = 0;
					}
					
					fwrite(&sample, outputWAVhdr.fmt.BitsPerSample/8, 1, wav_out);		
				}
			}		
		}
	}
	
	// Close files
	//-------------------------------------------------	
	fclose(wav_in);
	fclose(wav_out);
	//-------------------------------------------------	

	return 0;
}