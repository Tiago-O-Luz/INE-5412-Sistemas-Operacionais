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
			int page = stoi(line);
			if (pagemap->count(page)) {
				pagemap->at(page).push_back(current_line);
			} else {
				pagemap->insert(pair<int, vector<int>>(page, vector<int>()));
				pagemap->at(page).push_back(current_line);
			}
			input_refs->push_back(page);
            current_line++;
			return page;
		}
		return '-';
        // while (getline(myfile, line)) {
		// 	if (line[0] != '\n') {
		// 	}
        // }

        // for (int i = 0; i < pagemap.size(); i++) {
        // cout << pagemap[i] << ",";
        // }

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