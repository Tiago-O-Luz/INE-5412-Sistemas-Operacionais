#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


using namespace std;

class File
{

public:
	File(const string& name_file) {
		pagemap = new map<int, vector<int>>;
		input_refs = new vector<int>;
		current_line = 0;
		flag_error = true;
		myfile.open(name_file);
		if (!myfile.is_open()) {
			cout << "Erro ao abrir o arquivo!\n";
			flag_error = false;
		}
	}
	
	int ReadFile() {
        if (!myfile.is_open()) {
			cout << "Arquivo não está aberto!" << endl;
			flag_error = false;
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

	map<int, vector<int>>* GetPagemap() {
		return pagemap;
	}

	vector<int>* GetInputRefs() {
		return input_refs;
	}

	int GetLenghtRefs() {
		return input_refs->size();
	}

	bool GetFlagError() {
		return flag_error;
	}

	void PrintPages() {
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

		delete pagemap;
		delete input_refs;
	}
private:
	ifstream myfile;
	map<int, vector<int>> *pagemap;
	vector<int> *input_refs;
	int current_line;
	bool flag_error;
};
