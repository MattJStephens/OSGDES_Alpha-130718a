#ifndef _LogFile_
#define _LogFile_

#include <fstream>
#include <string>

class CLogFile
{
public:
	bool m_active;
	bool m_close_after_append;
	std::string m_file_name;
	std::ofstream m_ofs_alpha;
	void create(bool active, bool close_after_append, std::string file_name);
	void write(std::string message);
	void close();

};





#endif