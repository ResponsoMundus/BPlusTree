#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "BPlusTreeNode.h"

class BPlusTree {
	
private:
	
	int order;
	BPTNode *root;
	
	BPTNode *RecursivelySearch (BPTNode *node, int key);
	void Destruction (BPTNode *node);
	
	void Show (BPTNode *node, bool showAll);
	
public:
	
	BPlusTree ();
	~BPlusTree ();
	
	void Display ();
	
	void Initialize (int m);
	void Insert (int key, float value);
	bool Search (int key, float &value);
	bool *Search (int key1, int key2, float *&values, int &num);
	void Delete (int key);
	
};

#endif
