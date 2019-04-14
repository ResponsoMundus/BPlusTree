#include "BPlusTree.h"

BPlusTree :: BPlusTree () {
	
	order = 0;
	root = NULL;
	
}

BPlusTree :: ~BPlusTree () {
	
	if (root) {
		
		Destruction (root);
		
	}
	
}

BPTNode *BPlusTree :: RecursivelySearch (BPTNode *node, int key) {
	
	if (node->IsLeaf ()) {
		
		// Show (node, false);
		
		// if leafNode, return it
		return node;
		
	} else {
		
		// Show (node, false);
		
		int index = node->GetKeyIndex (key);
		
		// cout << index << endl;
		
		if (index < node->GetKeyNum () && key == node->GetKeys ()[index]) {
			
			// If key already found, move right
			return RecursivelySearch (node->GetChildren ()[index + 1], key);
			
		} else {
			
			// If not found yet, move left
			return RecursivelySearch (node->GetChildren ()[index], key);
			
		}
		
	}
	
}

void BPlusTree :: Destruction (BPTNode *node) {
	
	if (!node->IsLeaf ()) {
		
		for (int i = 0; i < node->GetKeyNum (); i++) {
			
			Destruction (node->GetChildren ()[i]);
			
		}
		
	}
	
	// cout << node->GetKeys ()[0] << endl;
	
	delete node;
	
}

void BPlusTree :: Show (BPTNode *node, bool showAll = false) {
	
	if (!node) {
		
		cout << "Dead End! Something went wrong!" << endl;
		
	} else if (!node->IsLeaf ()) {
		
		for (int i = 0; i < node->GetKeyNum (); i++) {
			
			cout << node->GetKeys ()[i] << "|";
			
		}
		
		cout << endl;
		
		if (showAll) {
			
			for (int i = 0; i <= node->GetKeyNum (); i++) {
				
				Show (node->GetChildren ()[i], showAll);
				
			}
		
		}		
		
		// cout << node->GetKeyNum () << " : ";
		
	} else {
		
		cout << node->GetKeyNum () << " : ";
		
		for (int i = 0; i < node->GetKeyNum (); i++) {
			
			cout << node->GetKeys ()[i] << ":" << node->GetValues ()[i] << " ";
			
		}
		
		cout << endl;
		// cout << "Parent :";
		// Show (node->GetParent ());
		
	}
	
}

void BPlusTree :: Display () {
	
	Show (root, true);
	
}

void BPlusTree :: Initialize (int m) {
	
	order = m;
	
}

void BPlusTree :: Insert (int key, float value) {
	
	if (!root) {
		
		root = new LeafNode (order);
		
	}
	
	int midKey;
	bool needNewRoot = false;
	
	BPTNode *rightNode;
	
	BPTNode *node = RecursivelySearch (root, key);
	
	// Used for test!
	/*
	cout << key << endl;
	Show (node);
	if (node->GetParent ()) {
		
		cout << "Parent of node :" << endl;
		Show (node->GetParent ());
		
	}
	cout << endl;
	*/
	
	node->Insert (key, value);
	
	while (node->GetKeyNum () == order) {
		
		BPTNode *parent = node->GetParent ();
		rightNode = node->Split (midKey);
		
		if (parent) {
			
			// If parent exists, insert
			parent->Insert (midKey, rightNode);
			
			rightNode->SetParent (parent);
			
			node = parent;
			
		} else {
			
			// Else break and deal with it outside
			needNewRoot = true;
			break;
			
		}
		
	}
	
	// Generate a new root and replace the old one
	if (needNewRoot) {
		
		InternalNode *newRoot = new InternalNode (order);
		
		newRoot->Insert (midKey, node, rightNode);
		node->SetParent (newRoot);
		rightNode->SetParent (newRoot);
		
		root = newRoot;
		
	}
	
}

bool BPlusTree :: Search (int key, float &value) {
	
	if (root->GetKeyNum () == 0) {
		
		// The tree is empty
		return false;
		
	}
	
	LeafNode *node = (LeafNode *)RecursivelySearch (root, key);
	int index = node->GetKeyIndex (key);
	
	if (key == node->GetKeys ()[index]) {
		
		value = node->GetValues ()[index];
		
		return true;
		
	} else {
		
		return false;
		
	}
	
}

bool *BPlusTree :: Search (int key1, int key2, float *&values, int &num) {
	
	num = 0;
	
	int buffSize = 1;
	
	bool *status = new bool[2];
	
	status[0] = false;
	status[1] = false;
	
	BPTNode *node1 = RecursivelySearch (root, key1);
	BPTNode *node2 = RecursivelySearch (root, key2);
	
	// cout << node1 << " " << node2 << endl;
	
	int index1 = node1->GetKeyIndex (key1);
	int index2 = node2->GetKeyIndex (key2);
	
	if (index1 < node1->GetKeyNum () && key1 == node1->GetKeys ()[index1]) {
		
		status[0] = true;
		
	}
	
	if (index2 < node2->GetKeyNum () && key2 == node2->GetKeys ()[index2]) {
		
		status[1] = true;
		
	}
	
	if (key1 < key2) {
		
		int index = index1;
		
		values = new float[buffSize];
		
		if (!index1 < node1->GetKeyNum ()) {
			
			index = 0;
			node1 = node1->GetNext ();
			
		}
		
		while (node1 != node2->GetNext ()) {
			
			for (; index < node1->GetKeyNum (); index++) {
				
				if (node1->GetKeys ()[index] <= key2) {
					
					if (num == buffSize) {
						
						buffSize *= 2;
						float *newValues = new float[buffSize];
						
						for (int i = 0; i < buffSize / 2; i++) {
							
							newValues[i] = values[i];
							
						}
						
						delete[] values;
						values = newValues;
						
					}
					
					values[num++] = node1->GetValues ()[index];
					// cout << values[num - 1] << endl;
					
				} else {
					
					break;
					
				}
				
			}
			
			node1 = node1->GetNext ();
			index = 0;
			
		}
	
	} else {
		
		values = new float[2];
		
		if (status[0]) {
			
			values[0] = node1->GetValues ()[index1];
			
		}		
		
		if (status[1]) {
			
			values[1] = node2->GetValues ()[index2];
			
		}
		
		num = 2;
		
	}
	
	return status;
	
}

void BPlusTree :: Delete (int key) {
	
	BPTNode *node = RecursivelySearch (root, key);
	
	node->Delete (key);
	
	if (root->GetKeyNum () == 0) {
		
		root = root->GetChildren ()[0];
		delete root->GetParent ();
		root->SetParent (NULL);
		
	}
	
}
