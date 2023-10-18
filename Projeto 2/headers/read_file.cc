#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifndef CPU_PARAMS
#define CPU_PARAMS

using namespace std;

class CpuParams
{	
public:
	CpuParams(int frames_input, vector<int> pages_input) { 
		frames = frames_input;
		pages = pages_input;
	}

	int get_frames() {
		return frames;
	}

	vector<int> get_pages() {
		return pages;
	}
	~CpuParams() {
		// nao sei oq botar aq
	}
	
private:	
	int frames;
	vector<int> pages;
};

class File
{

public:
	File(const std::string& name_file) {
		myfile.open(name_file);
		if (!myfile.is_open()) {
			cout << "Erro ao abrir o arquivo!\n";
		}
	}
	
	int read_file() {
        if (!myfile.is_open()) {
			cout << "Arquivo não está aberto!" << endl;
		}
		std::string line;
		std::getline(myfile, line);
		if (line[0] != '\n') {
			int page = line[0] - '0';
            pages_input.push_back(page);
			return page;
		}
		return '-';
        // while (std::getline(myfile, line)) {
		// 	if (line[0] != '\n') {
		// 	}
        // }

        // for (int i = 0; i < pages_input.size(); i++) {
        // std::cout << pages_input[i] << ",";
        // }

	}

	vector<int> get_pages_input() {
		return pages_input;
	}

	~File() {
		myfile.close();
            // nao sei oq botar aq
	}
private:
	std::ifstream myfile;
	std::vector<int> pages_input;
};


#endif