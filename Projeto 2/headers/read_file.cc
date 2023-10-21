#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#ifndef CPU_PARAMS
#define CPU_PARAMS

using namespace std;

class CpuParams
{	
public:
	CpuParams(int frames_input, map<int, vector<int>>* pagemap) { 
		frames = frames_input;
		pages = pagemap;
	}

	int get_frames() {
		return frames;
	}

	map<int, vector<int>>* get_pages() {
		return pages;
	}
	~CpuParams() {
		// nao sei oq botar aq
	}
	
private:	
	int frames;
	map<int, vector<int>>* pages;
};

class File
{

public:
	File(const string& name_file) {
		pagemap = new map<int, vector<int>>;
		input_refs = new vector<int>;
		current_line = 0;
		myfile.open(name_file);
		if (!myfile.is_open()) {
			cout << "Erro ao abrir o arquivo!\n";
		}
	}
	
	int read_file() {
        if (!myfile.is_open()) {
			cout << "Arquivo não está aberto!" << endl;
		}
		string line;
		if (getline(myfile, line)) {
			// Read one line
			int page = stoi(line);	// String to int
			if (!pagemap->count(page)) {
				// If virutal page doesn't exists as key
				pagemap->insert(pair<int, vector<int>>(page, vector<int>())); // Add key to virtual page
			}
			pagemap->at(page).push_back(current_line);	// Add to list of refs current line (time)
			input_refs->push_back(page);				// Add entry to input list 
            current_line++;
			return page;
		}
		return '-';
	}

	map<int, vector<int>>* get_pagemap() {
		return pagemap;
	}

	vector<int>* get_inputrefs() {
		return input_refs;
	}

	void print_pages() {
		for (auto i: *pagemap) {
			cout << i.first << ": ";
			for(auto a: i.second) {
				cout << a << ' ';
			}
			cout << endl;
		}
	}

	~File() {
		myfile.close();
            // nao sei oq botar aq
		delete pagemap;
		delete input_refs;
	}
private:
	ifstream myfile;
	map<int, vector<int>> *pagemap;
	vector<int> *input_refs;
	int current_line;
};


#endif