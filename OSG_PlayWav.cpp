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

#include "OSG_DES.h"
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"

#include <string>
#include <sstream>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include "mjs_wave.h"
#include "Framework.h"


//#include <Python.h>


///
// CEvent_DE_Logistics
///
bool CEvent_PlayWav::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "File")
		{
			m_file = iter->m_str_value;
		}		
			
	}		
	
	return true;
}

bool CEvent_PlayWav::Update(CEnvironment* p_env)
{

	ALuint      uiBuffer;
	ALuint      uiSource;  
	ALint       iState;

	/*ALFWInit();
	if (!ALFWInitOpenAL_default())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
	}*/

	alGenBuffers( 1, &uiBuffer );
	if (!ALFWLoadWaveToBuffer(m_file.c_str(), uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", m_file.c_str());
	}
	alGenSources( 1, &uiSource );
	alSourcei( uiSource, AL_BUFFER, uiBuffer );	
    alSourcePlay( uiSource );
	ALFWprintf("CEvent_PlayWav ");		
	do
	{
		Sleep(100);
		ALFWprintf(".");
		alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
	} while (iState == AL_PLAYING);	 


	ALFWprintf("\n");
	return true;
}

