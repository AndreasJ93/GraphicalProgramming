#ifndef FILE_READER_H
#define FILE_READER_H

#include <vector>
#include <fstream>

namespace GPFileReader {

	inline std::streampos FileSize(std::ifstream &file) {

		std::streampos fsize = 0;

		fsize = file.tellg();
		file.seekg(0, std::ios::end);
		fsize = file.tellg() - fsize;

		return fsize;
	}

	std::vector<unsigned char> ReadFromFile(const char* filename)
	{
		std::vector<unsigned char> readData;

		std::ifstream file;

		file.open(filename, std::ifstream::in | std::ifstream::binary);
		if (file.is_open())
		{
			auto size = FileSize(file);
			
			readData.resize(static_cast<size_t>(size));
			file.seekg(0, std::ios::beg);
			file.read(reinterpret_cast<char*>(&readData[0]), size);
			file.close();
		}

		

		return readData;
	}
	


}





#endif
