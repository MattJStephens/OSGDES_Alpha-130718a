//////////////////////////////////////////////////////////////////////
//	MJim - Alpha
//	Stephens Atomics 
//	Copyright (C) 2013  Matthew James Stephens, B.Eng- Robotics(Hons)
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	<http://www.gnu.org/licenses/>   
//////////////////////////////////////////////////////////////////////

#define NOMINMAX
#include "windows.h"

#include "OSG_DES.h"
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"
#include "OSG_XML_Save_environment_tree.h"
#include "OSG_Tools.h"
#include <string>
#include "mjs_fourier_series.h"

using  namespace std;
using  namespace osg;

extern CEventHandler g_event_handler;
#define MAX_LENGTH	32700


enum eWaveForm { 	
	WaveForm_Unknown, WaveForm_Sine, WaveForm_Cosine, WaveForm_Fourier, WaveForm_SineSweep, WaveForm_Random
}; 


eWaveForm string_to_waveform(std::string str)
{
	if(str == "sin") { return WaveForm_Sine; }
	if(str == "Sin") { return WaveForm_Sine; }
	if(str == "Sine") { return WaveForm_Sine; }
	if(str == "cos") { return WaveForm_Cosine; }
	if(str == "Cos") { return WaveForm_Cosine; }
	if(str == "Cosine") { return WaveForm_Cosine; }
	if(str == "Chirp") { return WaveForm_SineSweep; }
	if(str == "SineSweep") { return WaveForm_SineSweep; }
	if(str == "Fourier") { return WaveForm_Fourier; }
	if(str == "Random") { return WaveForm_Random; }

	return WaveForm_Unknown;

}

eWaveForm get_random_wave_form(void)
{
	int i= rand()%100;

	if (i < 70) { return WaveForm_Sine; };
	if (i < 80) { return WaveForm_Cosine; };
	if (i < 90) { return WaveForm_SineSweep; };
	if (i < 100) { return WaveForm_Fourier; };
	
	return WaveForm_Sine;
}

enum eDynamics { 	
	Dynamics_Unknown, Dynamics_Constant, Dynamics_Stephens, Dynamics_FadeIn, Dynamics_FadeOut, Dynamics_FadeInOut, Dynamics_AttackSusDecay, Dynamics_AttackVibeDecay
}; 

eDynamics string_to_dynamics(std::string str)
{
	if(str == "Constant") { return Dynamics_Constant; }
	if(str == "Stephens") { return Dynamics_Stephens; }
	if(str == "FadeIn") { return Dynamics_FadeIn; }
	if(str == "FadeOut") { return Dynamics_FadeOut; }
	if(str == "FadeInOut") { return Dynamics_FadeInOut; }
	if(str == "AttackSusDecay") { return Dynamics_AttackSusDecay; }
	if(str == "AttackVibeDecay") { return Dynamics_AttackVibeDecay; }
	return Dynamics_Unknown;
}

double get_random_float(void)
{	
	int rand_max= rand()%RAND_MAX;	
	return (double)rand_max/RAND_MAX;
}


DWORD SWAP_32(DWORD dw) {	
	return (dw & 0x000000ff) << 24 | (dw & 0x0000ff00) << 8 | (dw & 0x00ff0000) >> 8 | (dw & 0xff000000) >> 24;
}

double pitch_to_frequency(std::string pitch)
{
	if(pitch == "c0") return 16.35;	
	if(pitch == "c#0") return 17.32;
	if(pitch == "d0") return 18.35;
	if(pitch == "d#0") return 19.45;
	if(pitch == "e0") return 20.6;
	if(pitch == "f0") return 21.83;
	if(pitch == "f#0") return 23.12;
	if(pitch == "g0") return 24.5;
	if(pitch == "g#0") return 25.96;
	if(pitch == "a0") return 27.5;
	if(pitch == "a#0") return 29.14;
	if(pitch == "b0") return 30.87;

	if(pitch == "c1") return 32.7;
	if(pitch == "c#1") return 34.65;
	if(pitch == "d1") return 36.71;
	if(pitch == "d#1") return 38.89;
	if(pitch == "e1") return 41.2;
	if(pitch == "f1") return 43.65;
	if(pitch == "f#1") return 46.25;
	if(pitch == "g1") return 49;
	if(pitch == "g#1") return 51.91;
	if(pitch == "a1") return 55;
	if(pitch == "a#1") return 58.27;
	if(pitch == "b1") return 61.74;

	if(pitch == "c2") return 65.41;	
	if(pitch == "c#2") return 69.3;
	if(pitch == "d2") return 73.42;
	if(pitch == "d#2") return 77.78;
	if(pitch == "e2") return 82.41;
	if(pitch == "f2") return 87.31;
	if(pitch == "f#2") return 92.5;
	if(pitch == "g2") return 98;
	if(pitch == "g#2") return 103.83;
	if(pitch == "a2") return 110;
	if(pitch == "a#2") return 116.54;
	if(pitch == "b2") return 123.47;
	
	if(pitch == "c3") return 130.81;
	if(pitch == "c#3") return 138.59;
	if(pitch == "d3") return 146.83;
	if(pitch == "d#3") return 155.56;
	if(pitch == "e3") return 164.81;
	if(pitch == "f3") return 174.61;
	if(pitch == "f#3") return 185.0;
	if(pitch == "g3") return 196;
	if(pitch == "g#3") return 207;
	if(pitch == "a3") return 220;
	if(pitch == "a#3") return 233.08;
	if(pitch == "b3") return 246.94;

	if(pitch == "c4") return 261.63;
	if(pitch == "c#4") return 277.18;
	if(pitch == "d4") return 293.66;
	if(pitch == "d#4") return 311.13;
	if(pitch == "e4") return 329.63;
	if(pitch == "f4") return 349.23;
	if(pitch == "f#4") return 369.99;
	if(pitch == "g4") return 392;
	if(pitch == "g#4") return 415.3;
	if(pitch == "a4") return 440;
	if(pitch == "a#4") return 466.16;
	if(pitch == "b4") return 493.88;

	if(pitch == "c5") return 523.25;
	if(pitch == "c#5") return 554.37;
	if(pitch == "d5") return 587.33;
	if(pitch == "d#5") return 622.25;
	if(pitch == "e5") return 659.26;
	if(pitch == "f5") return 698.46;
	if(pitch == "f#5") return 739.99;
	if(pitch == "g5") return 783.99;
	if(pitch == "g#5") return 830.61;
	if(pitch == "a5") return 880;
	if(pitch == "a#5") return 932.33;
	if(pitch == "b5") return 987.77;
	return 440;
}


double stephens_alpha(double x, double a, double b, double c)
{	
	return pow(pow(x,a) - pow(x,b), c);
}

double stephens_beta(double x, double a1, double b1, double a2, double b2, double c)
{
	if(x<=0) return 0;
	if(x>=1) return 0;
	return stephens_alpha(x, a1, b1, c) - stephens_alpha(x, a2, b2, c);
}

//fade in
double sigmoid(double x)
{
	return 1.0/(1+pow(e,-x));
}

//fade out
double reverse_sigmoid(double x)
{
	return 1.0/(1+pow(e,x));
}

double stephens_approx_zero_grad(double a1, double b1, double a2, double b2, double c)
{	
	double x = 0;
	double inc = 1.0/1000.0;
	double lf_MAX= 0;
	for (x= 0; x< 1; x+= inc)
	{
		double result = stephens_beta(x, a1, b1, a2, b2, 0.5);
		if(result> lf_MAX)	{
			lf_MAX = result; 
		}
	}
	return lf_MAX;
}


double get_random_harmonic_fraction(void)
{
	int rand_int= rand()%106;

	if(rand_int > 85) { return 1.0/2.0; }
	if(rand_int > 64) { return 2.0/1.0; }//21

	if(rand_int > 51) { return 2.0/3.0; }
	if(rand_int > 38) { return 3.0/2.0; }//13

	if(rand_int > 30) { return 3.0/4.0; }
	if(rand_int > 22) { return 4.0/3.0; }//8

	if(rand_int > 17) { return 4.0/5.0; }
	if(rand_int > 12) { return 5.0/4.0; }//5

	if(rand_int > 9) { return 3.0/5.0; }
	if(rand_int > 6) { return 5.0/3.0; }//3

	if(rand_int > 4) { return 4.0/7.0; }
	if(rand_int > 2) { return 7.0/4.0; }//2

	if(rand_int > 1) { return 3.0/7.0; }//1
	if(rand_int >= 0) { return 7.0/3.0; }
	
	return 1;
}

double get_random_harmonic_fraction2(void)
{
	int rand_int= rand()%106;
	
	//2
	if(rand_int > 176) { return 1.0/2.0; }
	if(rand_int > 167) { return 2.0/1.0; }

	//3
	if(rand_int > 158) { return 1.0/3.0; }
	if(rand_int > 150) { return 3.0/1.0; }

	if(rand_int > 142) { return 2.0/3.0; }
	if(rand_int > 135) { return 3.0/2.0; }

	//4
	if(rand_int > 128) { return 1.0/4.0; }
	if(rand_int > 122) { return 4.0/1.0; }	

	if(rand_int > 116) { return 3.0/4.0; }
	if(rand_int > 110) { return 4.0/3.0; }

	//5	
	if(rand_int > 104) { return 2.0/5.0; }
	if(rand_int > 89) { return 5.0/2.0; }

	if(rand_int > 84) { return 3.0/5.0; }
	if(rand_int > 79) { return 5.0/3.0; }

	if(rand_int > 74) { return 4.0/5.0; }
	if(rand_int > 69) { return 5.0/4.0; }

	//6		
	if(rand_int > 64) { return 5.0/6.0; }
	if(rand_int > 60) { return 6.0/5.0; }

	//7
	if(rand_int > 56) { return 1.0/7.0; }
	if(rand_int > 53) { return 7.0/1.0; }
	
	if(rand_int > 50) { return 2.0/7.0; }
	if(rand_int > 47) { return 7.0/2.0; }

	if(rand_int > 44) { return 3.0/7.0; }
	if(rand_int > 41) { return 7.0/3.0; }

	if(rand_int > 38) { return 4.0/7.0; }
	if(rand_int > 35) { return 7.0/4.0; }

	if(rand_int > 32) { return 5.0/7.0; }
	if(rand_int > 29) { return 7.0/5.0; }

	if(rand_int > 26) { return 6.0/7.0; }
	if(rand_int > 23) { return 7.0/6.0; }
	
	//8	
	if(rand_int > 20) { return 3.0/8.0; }
	if(rand_int > 18) { return 8.0/3.0; }

	if(rand_int > 16) { return 5.0/8.0; }
	if(rand_int > 14) { return 8.0/5.0; }

	if(rand_int > 12) { return 7.0/8.0; }
	if(rand_int > 10) { return 8.0/7.0; }

	//9	
	if(rand_int > 9) { return 2.0/9.0; }
	if(rand_int > 8) { return 9.0/2.0; }

	if(rand_int > 7) { return 4.0/9.0; }
	if(rand_int > 6) { return 9.0/4.0; }

	if(rand_int > 5) { return 5.0/9.0; }
	if(rand_int > 4) { return 9.0/5.0; }

	if(rand_int > 3) { return 7.0/9.0; }
	if(rand_int > 2) { return 9.0/7.0; }
	
	if(rand_int > 1) { return 8.0/9.0; }
	if(rand_int >= 0) { return 9.0/8.0; }
	
	return 1;
}

void generate_random_frequencies(std::vector<double>* pv_lf, int depth, double min, double max, double base_freq)
{
	double freq= base_freq;
	if(pv_lf->size()> 0) { freq= get_random_harmonic_fraction()*((*pv_lf)[0]);	}
	if((freq< min)||(freq> max)) { freq = base_freq; }
	pv_lf->clear();
	pv_lf->push_back(freq);
	int rand_int= rand()%depth;	
	int i=0;
	for(i= 0; i< rand_int; i++)	{
		double rand_freq = get_random_harmonic_fraction()*freq;
		pv_lf->push_back(rand_freq);
	}
}

void generate_random_frequencies2(std::vector<double>* pv_lf, int depth, double min, double max, double base_freq)
{
	double freq= base_freq;
	
	if(pv_lf->size()> 0) { freq= get_random_harmonic_fraction2()*((*pv_lf)[0]);	}
	if((freq< min)||(freq> max)) { base_freq= 440; }
	pv_lf->clear();
	pv_lf->push_back(freq);
	int rand_int= rand()%depth;	
	int i=0;
	for(i= 0; i< rand_int; i++)	{
		double rand_freq = get_random_harmonic_fraction2()*freq;
		pv_lf->push_back(rand_freq);
	}
}


class WaveElement
{
public:
	eWaveForm m_WaveForm;
	eDynamics m_Dynamics;
	double m_phase;
	double m_p0;
	double m_p1;
	double m_frequency;
	double m_f0;
	double m_f1;
};

// CEventType_CreateWave
///
bool CEvent_CreateWave::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	//8-bit samples are stored as unsigned bytes, ranging from 0 to 255. 
	//16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767. 
	DWORD ChunkID = SWAP_32(0x52494646);          
	DWORD Format = SWAP_32(0x57415645);        	
	DWORD Subchunk1ID = SWAP_32(0x666d7420);  
	DWORD Subchunk1Size = 16;
	WORD AudioFormat = 1;
	WORD NumChannels= 1; //ie mono or stereo
	DWORD SampleRate = 8000; // samples per second
	WORD BitsPerSample = 8; //16 etc.
	DWORD ByteRate  = SampleRate * NumChannels * (BitsPerSample/8);	
	WORD BlockAlign = NumChannels * (BitsPerSample/8);
	DWORD Subchunk2ID = SWAP_32(0x64617461);   
	DWORD NumSamples= 16384;
	DWORD Subchunk2Size= NumSamples * NumChannels * (BitsPerSample/8);
	DWORD ChunkSize = 4 + (8 + Subchunk1Size) + (8 + Subchunk2Size); 
	double frequency = 440;//cycles per second
	double angular_frequency = 2* osg::PI* frequency;
	double period = 1/frequency;		
	double sample_period = 1/(double)SampleRate;
	double sample_time_elapsed= 0;
	double amplitude = 256/2;
	double amplitude_coefficient= 1.0;
	std::string type= "Sine";
    std::string file_name = "wave.wav";
	std::vector<double> v_frequency;
	std::vector<CParsedVariable>::iterator iter;	
	DWORD dw_a = 0;
	DWORD dw_b = 0;
	double min_freq= 20.0;
	double max_freq= 1500.0;
	double start_freq= 440.0;
	double end_freq= 880.0;
	bool frequency_sweep= false;
	double freq_inc= 0;
	double duration=0;
	double chirp_rate =0;
	long N= 10;
	double phase=0;
	eWaveForm WaveForm= WaveForm_Sine;
	eDynamics Dynamics= Dynamics_Constant;
	DWORD i=0;
	sample_time_elapsed=0;
	double ft=0;
	double stephens_find_max=0;
	double recip_sampl_accumulator= 0;
	double stephens_max = 0;
	double stephens_amplitude_coefficient= 0;
	double stephens=0;
	double ParamC= 0.5;
	double num_samp_recip=0;
	int n_A = 0;
	double lf_A = 0;
	std::vector<double>::iterator lf_iter;
	std::vector<double> v_dynamics;
	std::size_t wave_form_index= 0;
	std::vector<eWaveForm> v_wave_form;
	std::size_t num_freq;
	double theta=0;
	double MinRandNumSamples = 44100;
	double base_freq= 440;
	double sigmoid_coeff1= 0;
	double sigmoid_coeff2= 0;
	double sigmoid_acum1= 0;
	
	double attack= 0.1;
	double sustain= 0.7;
	double decay= 1.0;
	bool b_attack= false;
	bool b_sustain= false;
	bool b_decay= false;

	double vibe_period=0;
	double vibe_freq=0;
	double vibe_ang_freq=0;
	double vibe_coeff= 0;
	double vibe_acum= 0;
	double vibe_inc= 0;
	double vibe_cycles= 1.0;
	double vibe_A= 0;

	bool b_noise= false;
	double noise=0;

	bool b_distort= false;
	double distortion= 1.5;

	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "FileName") {
			file_name = iter->m_str_value;
		}			
		else if(iter->m_identifier == "Base_freq") {
			std::istringstream iss(iter->m_str_value);
			iss >> base_freq;
		}	
		else if(iter->m_identifier == "Channels") {
			std::istringstream iss(iter->m_str_value);
			iss >> NumChannels;
		}	
		else if(iter->m_identifier == "SampleRate") {
			std::istringstream iss(iter->m_str_value);
			iss >> SampleRate;
		}	
		else if(iter->m_identifier == "BitsPerSample") {
			std::istringstream iss(iter->m_str_value);
			iss >> BitsPerSample;
		}	
		else if(iter->m_identifier == "Noise") {
			std::istringstream iss(iter->m_str_value);			
			iss >> noise;
			b_noise= true;
		}	
		else if(iter->m_identifier == "Distortion") {
			std::istringstream iss(iter->m_str_value);			
			iss >> distortion;
			b_distort= true;
		}	
		else if(iter->m_identifier == "NumSamples") {
			std::istringstream iss(iter->m_str_value);
			iss >> NumSamples;
		}	
		else if(iter->m_identifier == "Amplitude") {
			std::istringstream iss(iter->m_str_value);
			iss >> amplitude_coefficient;
		}	
		else if(iter->m_identifier == "Phase") {
			std::istringstream iss(iter->m_str_value);
			iss >> phase;
		}		
		else if(iter->m_identifier == "Attack") {
			std::istringstream iss(iter->m_str_value);
			iss >> attack;
		}		
		else if(iter->m_identifier == "Sustain") {
			std::istringstream iss(iter->m_str_value);
			iss >> sustain;
		}		
		else if(iter->m_identifier == "Decay") {
			std::istringstream iss(iter->m_str_value);
			iss >> decay;
		}		
		else if(iter->m_identifier == "MinRandNumSamples") {			
			std::istringstream iss(iter->m_str_value);					
			iss >> MinRandNumSamples;			
		}	
		else if(iter->m_identifier == "RandNumSamples") {			
			std::istringstream iss(iter->m_str_value);					
			iss >> dw_a;
			dw_b = MinRandNumSamples+ rand()%dw_a;			
			NumSamples = dw_b;
		}	
		else if((iter->m_identifier == "Type")||(iter->m_identifier == "WaveForm")) {
			type = iter->m_str_value;
			WaveForm = string_to_waveform(type);
			v_wave_form.push_back(WaveForm);
		}		
		else if(iter->m_identifier == "RandomForm") {
			std::istringstream iss(iter->m_str_value);
		
			int num_rand;
			iss >> num_rand;
			int i=0;
			for(i = 0; i < num_rand; i++) {					
				v_wave_form.push_back(get_random_wave_form());
			}
		}	
		else if(iter->m_identifier == "Dynamics") {
			Dynamics = string_to_dynamics(iter->m_str_value);
		}				
		else if(iter->m_identifier == "Frequency") {			
			std::istringstream iss(iter->m_str_value);
			iss >> frequency;
			v_frequency.push_back(frequency);
		}	
		else if(iter->m_identifier == "Pitch") {
			frequency = pitch_to_frequency(iter->m_str_value);
			v_frequency.push_back(frequency);
		}	
		else if(iter->m_identifier == "StartFrequency") {
			std::istringstream iss(iter->m_str_value);
			iss >> start_freq;
			frequency = start_freq;
		}	
		else if(iter->m_identifier == "RandomStartFrequency") {
			std::istringstream iss(iter->m_str_value);
			iss >> start_freq;
			double grf_max= 440;
			double grf_min= 1;
			start_freq+= get_random_float()* (grf_max- grf_min);
		}		
		else if(iter->m_identifier == "EndFrequency") {
			std::istringstream iss(iter->m_str_value);
			iss >> end_freq;
		}		
		else if(iter->m_identifier == "RandomEndFrequency") {
			std::istringstream iss(iter->m_str_value);
			iss >> end_freq;
			double grf_max= 440;
			double grf_min= 1;
			end_freq+= get_random_float()* (grf_max- grf_min);			
		}		
		else if(iter->m_identifier == "Min_freq") {
			std::istringstream iss(iter->m_str_value);
			iss >> min_freq;
		}		
		else if(iter->m_identifier == "Max_freq") {
			std::istringstream iss(iter->m_str_value);
			iss >> max_freq;
		}		
		else if(iter->m_identifier == "SustainVibeFrequency") {
			std::istringstream iss(iter->m_str_value);
			iss >> vibe_cycles;
		}	
		else if(iter->m_identifier == "RandomSustainVibeFrequency") {
			int rand_vibe_cycles;
			std::istringstream iss(iter->m_str_value);			
			iss >> rand_vibe_cycles;
			vibe_cycles= rand()%rand_vibe_cycles+1;
		}	
		else if(iter->m_identifier == "N") {
			std::istringstream iss(iter->m_str_value);
			iss >> N;
		}		
		else if(iter->m_identifier == "RandomN") {
			std::istringstream iss(iter->m_str_value);
			iss >> N;

			N= rand()%N;
		}		
		else if(iter->m_identifier == "RandomChords") {
			std::istringstream iss(iter->m_str_value);
			int r;
			iss >> r;
			generate_random_frequencies(&p_env->mv_frequency, r, min_freq, max_freq, base_freq);
			std::vector<double>::iterator lf_iter001;
			for(lf_iter001 = p_env->mv_frequency.begin(); lf_iter001 < p_env->mv_frequency.end(); ++lf_iter001) {					
				v_frequency.push_back(*lf_iter001);
			}
		}	
		else if(iter->m_identifier == "RandomChords2") {
			std::istringstream iss(iter->m_str_value);
			int r;
			iss >> r;
			generate_random_frequencies2(&p_env->mv_frequency, r, min_freq, max_freq, base_freq);
			std::vector<double>::iterator lf_iter001;
			for(lf_iter001 = p_env->mv_frequency.begin(); lf_iter001 < p_env->mv_frequency.end(); ++lf_iter001) {					
				v_frequency.push_back(*lf_iter001);
			}
		}	
		else if(iter->m_identifier == "ParamC") {
			std::istringstream iss(iter->m_str_value);
			iss >> ParamC;		
		}			
	}		

	
	ByteRate= SampleRate * NumChannels * (BitsPerSample/8);	
	BlockAlign = NumChannels * (BitsPerSample/8);
	Subchunk2Size= NumSamples * NumChannels * (BitsPerSample/8);
	ChunkSize = 4 + (8 + Subchunk1Size) + (8 + Subchunk2Size);  
	HANDLE hFile;
	DWORD dwFileSize;
	hFile = CreateFile(file_name.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
		
	period = 1.0/frequency;	
	sample_period = 1.0/(double)SampleRate;
	num_samp_recip = 1.0/ (double)NumSamples;
	sigmoid_coeff1 = 12.0 /(double)NumSamples;
	duration= (double)NumSamples* sample_period;

	freq_inc =  (end_freq- start_freq)/ (double)NumSamples;
	chirp_rate = (end_freq- start_freq)/duration;
	
	angular_frequency = 2.0* osg::PI* frequency;	
	num_freq = v_frequency.size();

	if (hFile != NULL)
	{
		//header
		WriteFile(hFile, &ChunkID, 4, &dwFileSize, NULL);
		WriteFile(hFile, &ChunkSize, 4, &dwFileSize, NULL);
		WriteFile(hFile, &Format, 4, &dwFileSize, NULL);
		//chunck 1
		WriteFile(hFile, &Subchunk1ID, 4, &dwFileSize, NULL);	
		WriteFile(hFile, &Subchunk1Size, 4, &dwFileSize, NULL);
		WriteFile(hFile, &AudioFormat, 2, &dwFileSize, NULL);
		WriteFile(hFile, &NumChannels, 2, &dwFileSize, NULL);	
		WriteFile(hFile, &SampleRate, 4, &dwFileSize, NULL);
		WriteFile(hFile, &ByteRate, 4, &dwFileSize, NULL);
		WriteFile(hFile, &BlockAlign, 2, &dwFileSize, NULL);
		WriteFile(hFile, &BitsPerSample, 2, &dwFileSize, NULL);	
		//chunck 2
		WriteFile(hFile, &Subchunk2ID, 4, &dwFileSize, NULL);
		WriteFile(hFile, &Subchunk2Size, 4, &dwFileSize, NULL);
		
		num_samp_recip = 1.0/(double)NumSamples;
		recip_sampl_accumulator= 0;
		stephens_max = stephens_approx_zero_grad(3,4,5,6,ParamC);
		stephens_amplitude_coefficient= 1.0/stephens_max;
		
		sigmoid_acum1= -6.0;

		if(BitsPerSample== 16) {
			amplitude = 32767 * amplitude_coefficient;	
			for(i=0; i< NumSamples; i++) {	
				switch (Dynamics)
				{
					case Dynamics_Constant:
					{			
						v_dynamics.push_back(amplitude);
						break;
					}
					case Dynamics_FadeIn:
					{			
						sigmoid_coeff1 = 12.0 /(double)NumSamples;
						v_dynamics.push_back(amplitude*sigmoid(sigmoid_acum1));
						sigmoid_acum1+= sigmoid_coeff1;
						break;
					}
					case Dynamics_FadeOut:
					{			
						sigmoid_coeff1 = 12.0 /(double)NumSamples;
						v_dynamics.push_back(amplitude*reverse_sigmoid(sigmoid_acum1));
						sigmoid_acum1+= sigmoid_coeff1;
					
						break;
					}
					case Dynamics_FadeInOut:
					{		
						sigmoid_coeff1 = 24.0 /(double)NumSamples;
						if(recip_sampl_accumulator <0.5)
						{			
							if(!b_attack) {
								sigmoid_acum1= -6.0;
								b_attack= true;								
							}
							v_dynamics.push_back(amplitude*sigmoid(sigmoid_acum1));
						}
						else
						{
							if(b_attack) {
								sigmoid_acum1= -6.0;
								b_attack= false;										
								b_decay= true;								
							}
							v_dynamics.push_back(amplitude*reverse_sigmoid(sigmoid_acum1));
						}

						sigmoid_acum1+= sigmoid_coeff1;
						break;
					}
					case Dynamics_AttackSusDecay:
					{						
						if(recip_sampl_accumulator< attack)
						{
							if(!b_attack) {
								sigmoid_coeff2 = (1.0/attack)*12.0;
								sigmoid_coeff1 = sigmoid_coeff2 /(double)NumSamples;
								sigmoid_acum1= -6.0;
								b_attack= true;								
							}
							v_dynamics.push_back(amplitude*sigmoid(sigmoid_acum1));
						}
						else if(recip_sampl_accumulator< sustain)
						{
							if(b_attack) {
								sigmoid_coeff2 = (1.0/(sustain-attack))*12.0;
								sigmoid_coeff1 = sigmoid_coeff2 /(double)NumSamples;
								sigmoid_acum1= -6.0;
								b_attack= false;								
								b_sustain= true;
								b_decay= false;								
							}
							v_dynamics.push_back(amplitude);
						}
						else if(recip_sampl_accumulator< decay)
						{
							if(b_sustain) {
								sigmoid_coeff2 = (1.0/(decay-sustain))*12.0;
								sigmoid_coeff1 = sigmoid_coeff2 /(double)NumSamples;
								sigmoid_acum1= -6.0;
								b_attack= false;								
								b_sustain= false;
								b_decay= true;								
							}
							v_dynamics.push_back(amplitude*reverse_sigmoid(sigmoid_acum1));
						}
						else
						{
							v_dynamics.push_back(amplitude);
						}
						sigmoid_acum1+= sigmoid_coeff1;
						break;
					}
					case Dynamics_AttackVibeDecay:
					{						
						if(recip_sampl_accumulator< attack)
						{
							if(!b_attack) {
								sigmoid_coeff2 = (1.0/attack)*12.0;
								sigmoid_coeff1 = sigmoid_coeff2 /(double)NumSamples;
								sigmoid_acum1= -6.0;
								b_attack= true;								
							}
							v_dynamics.push_back(amplitude*sigmoid(sigmoid_acum1));
						}
						else if(recip_sampl_accumulator< sustain)
						{
							if(b_attack) {
								b_attack= false;								
								b_sustain= true;
								b_decay= false;									
								vibe_period = (sustain-attack);
								vibe_inc = vibe_period/(vibe_period*((double)NumSamples));
								vibe_freq= vibe_cycles/vibe_period;
								vibe_ang_freq = 2.0 * 3.1415926535897932384626433832795 * vibe_freq;
								vibe_acum=0;
							}						

							vibe_A = 0.5+ (cos(vibe_ang_freq* vibe_acum)/2.0);
							v_dynamics.push_back(amplitude* vibe_A);
							vibe_acum+= vibe_inc;
						}
						else if(recip_sampl_accumulator< decay)
						{
							if(b_sustain) {
								sigmoid_coeff2 = (1.0/(decay-sustain))*12.0;
								sigmoid_coeff1 = sigmoid_coeff2 /(double)NumSamples;
								sigmoid_acum1= -6.0;
								b_attack= false;								
								b_sustain= false;
								b_decay= true;								
							}
							v_dynamics.push_back(amplitude*reverse_sigmoid(sigmoid_acum1));
						}
						else
						{
							v_dynamics.push_back(amplitude);
						}
						sigmoid_acum1+= sigmoid_coeff1;
						break;
					}			
					case Dynamics_Stephens:
					{			
						v_dynamics.push_back(amplitude*(stephens_amplitude_coefficient* stephens_beta(recip_sampl_accumulator, 3,4,5,6,ParamC)));
						
						break;
					}
					default:
					{
						v_dynamics.push_back(amplitude);
						break;
					}
				}
				recip_sampl_accumulator+= num_samp_recip;
			}

			for(i=0; i< NumSamples; i++) {	
				lf_A=0;
				
				for(lf_iter = v_frequency.begin(); lf_iter < v_frequency.end(); ++lf_iter) {	
					angular_frequency= 2.0* osg::PI* (*lf_iter);
					theta = (angular_frequency* sample_time_elapsed) + phase;			

					
					if(wave_form_index>= v_wave_form.size())
					{
						if(v_wave_form.size()>0) {
							WaveForm = v_wave_form[0];
							wave_form_index=0;
						}
					}
					if(wave_form_index< v_wave_form.size())
					{
						WaveForm = v_wave_form[wave_form_index];
						wave_form_index++;
					}
					switch (WaveForm)
					{
						case WaveForm_Sine:
						{
							lf_A += sin(theta);								
							break;
						}
						case WaveForm_Cosine:
						{
							lf_A += cos(theta);		
							break;
						}
						case WaveForm_SineSweep:
						{
							lf_A += sin(2.0* osg::PI* ((start_freq* sample_time_elapsed)+((chirp_rate/2)*powf(sample_time_elapsed,2))));
							break;
						}
						case WaveForm_Fourier:
						{
							lf_A += square_wave_fourier_series(theta, N);		
							break;
						}								
					}
					
				}
				lf_A/= (double)num_freq;
				lf_A*= v_dynamics[i];

				if(b_noise)
				{
					lf_A*= 0.9;
					int rand_coin= rand()%2;
					if(rand_coin>0)
					{
						lf_A+= get_random_float()* (noise);
					}
					else
					{
						lf_A-= get_random_float()* (noise);
					}
				}

				if(b_distort)
				{
					lf_A*= distortion;
				}

				if(lf_A >32766) { lf_A= 32766; }
				if(lf_A <-32766) { lf_A= -32766; }

				n_A = static_cast<int>(lf_A);
				WriteFile(hFile, &n_A, 2, &dwFileSize, NULL);
				sample_time_elapsed+= sample_period;
			}
		}		
		CloseHandle(hFile);
	}


	return true;
}

bool CEvent_CreateWave::Update(CEnvironment* p_env)
{
	
	return true;
}