#include "BPlusTree.h"

#include <iostream>

using namespace std;
/*
int keys[] = {50000, 5000000};
int keyNum = 2;

int GetKeyIndex (int key) {
	
	int left = 0;
	int right = keyNum - 1;
	int mid;
	
	while (left < right) {
		
		mid = (left + right) / 2;
		
		cout << left << " " << mid << " " << right << endl;
		
		if (keys[mid] == key) {
			
			return mid;
			
		} else if (keys[mid] > key) {
			
			right = mid - 1;
			
		} else {
			
			left = mid + 1;
			
		}
		
		cout << left << " " << mid << " " << right << endl;
		
	}
	
	if (left == right) {
		
		if (keys[left] >= key) {
			
			return left;
		
		} else {
			
			return left + 1;
			
		}
		
	}
	
	return left;
	
}
*/
int main (int argc, char **argv) {
	
//	cout << GetKeyIndex (5000) << endl;
	
	BPlusTree tree;
	tree.Initialize (3);
	
	tree.Insert (21, 0.3534);
	tree.Insert (108, 31.907);
	tree.Insert (56089, 3.26);
	tree.Insert (234, 121.56);
	tree.Insert (4325, -109.23);
	tree.Insert (102, 39.56);
	tree.Insert (65, -3.95);
	tree.Insert (106, -3.91);
	tree.Insert (23, 3.55);
	
	cout << endl;
	cout << "current tree" << endl;
	tree.Display ();
	cout << endl;
	
	tree.Delete (65);
	
	cout << endl;
	cout << "current tree" << endl;
	tree.Display ();
	cout << endl;
	
	tree.Delete (102);
	
	cout << endl;
	cout << "current tree" << endl;
	tree.Display ();
	cout << endl;
	
	return 0;
	
}
