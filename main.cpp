#include "BPlusTree.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string INIT ("Initialize");
const string INSERT ("Insert");
const string DELETE ("Delete");
const string SEARCH ("Search");

const string OUTPUT_FILE ("output_file.txt");

int main (int argc, char **argv) {
	
	if (argc != 2) {
		
		cout << "Please enter the name of the input file!" << endl;
		exit (-1);
		
	}
	
	ifstream in (argv[1]);
	ofstream out (OUTPUT_FILE.c_str ());
	
	int i = 0;
	
	string line;
	string command;
	string data;
	
	BPlusTree tree;
	
	while (!in.eof ()) {
		
		i++;
		getline (in, line);
		
		// cout << line << endl;
		
		command = line.substr(0, line.find('('));
		data = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);
		
		// cout << command << endl;
		// cout << data << endl;
		
		if (command.compare (INIT) == 0) {
			
			int order = stoi(data);
			
			tree.Initialize (order);
			
		} else if (command.compare (INSERT) == 0) {
			
			int key = stoi (data.substr(0, data.find(',')));
			float val = stof (data.substr(data.find(',') + 1, data.length() - data.find(',')));
			
			tree.Insert (key, val);
			
		} else if (command.compare (DELETE) == 0) {
			
			int key = stoi (data);
			
			tree.Delete (key);
			
		} else if (command.compare (SEARCH) == 0) {
			
			if (data.find(',') != string::npos) {
				
				int num;
				bool *flags;
				float *vals;
				
				int key1 = stoi (data.substr(0, data.find(',')));
				int key2 = stoi (data.substr(data.find(',') + 1, data.length() - data.find(',')));
				
				flags = tree.Search (key1, key2, vals, num);
				
				if (!flags[0]) {
					
					out << "Null, ";
					
				}
				
				for (int i = 0; i < num; i++) {
					
					out << vals[i];
					if (i != num - 1) out << ", ";
					
				}
				
				if (!flags[1]) {
					
					out << ", Null";
					
				}
				
				out << endl;
			
			} else {
				
				float val;
				
				int key = stoi (data);
				
				if (tree.Search (key, val)) {
					
					out << val << endl;
					
				} else {
					
					out << "Null" << endl;
					
				}
				
			}
			
		} else {
			
			out << "Illegal command at line " << i << endl;
			
		}
		
	}
	
	in.close ();
	out.close ();
	
	return 0;
	
}
