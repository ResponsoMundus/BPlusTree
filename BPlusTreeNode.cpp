#include "BPlusTreeNode.h"

BPTNode :: BPTNode (int n) {
	
	order = n;
	
	parent = NULL;
	prev = NULL;
	next = NULL;
	
	keys = new int[n];
	keyNum = 0;
	
}

BPTNode :: ~BPTNode () {
	
	delete[] keys;
	
}

bool BPTNode :: IsLeaf() {
	
	return isLeaf;
	
}

int *BPTNode :: GetKeys() {
	
	return keys;
	
}

int BPTNode :: GetKeyNum () {
	
	return keyNum;
	
}

void BPTNode :: DecreaseKeyNum () {
	
	keyNum--;
	
}

int BPTNode :: GetKeyIndex (int key) {
	
	int left = 0;
	int right = keyNum - 1;
	int mid;
	
	while (left < right) {
		
		mid = (left + right) / 2;
		
		if (keys[mid] == key) {
			
			return mid;
			
		} else if (keys[mid] > key) {
			
			right = mid - 1;
			
		} else {
			
			left = mid + 1;
			
		}
		
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

BPTNode *BPTNode :: GetParent () {
	
	return parent;
	
}

void BPTNode :: SetParent (BPTNode *node) {
	
	parent = node;
	
}

BPTNode *BPTNode :: GetNext() {
	
	return next;
	
}

void BPTNode :: SetNext (BPTNode *node) {
	
	next = node;
	
}

BPTNode *BPTNode :: GetPrev () {
	
	return prev;
	
}

void BPTNode :: SetPrev (BPTNode *node) {
	
	prev = node;
	
}

InternalNode :: InternalNode (int n) : BPTNode (n) {
	
	isLeaf = false;
	children = new BPTNode*[n + 1];
	
}

InternalNode :: ~InternalNode () {
	
	delete[] children;
	
}

// Insert a new key and node into the current internal node
void InternalNode :: Insert (int key, BPTNode* rightChild) {
	
	int index = GetKeyIndex (key);
	
	// cout << "index : " << index << endl;
	
	// move every key and child after index
	for (int i = keyNum; i > index; i--) {
		
		keys[i] = keys[i - 1];
		children[i + 1] = children[i];
		
	}
	
	keys[index] = key;
	children[index + 1] = rightChild;
	
	rightChild->SetNext(children[index]->GetNext ());
	rightChild->SetPrev(children[index]);
	children[index]->SetNext (rightChild);
	if (rightChild->GetNext ()) {
		
		rightChild->GetNext ()->SetPrev(rightChild);
		
	}
	
	keyNum++;
	
}

// Used only when creating new InternalNode
void InternalNode :: Insert (int key, BPTNode* leftChild, BPTNode* rightChild) {
	
	keys[keyNum] = key;
	
	rightChild->SetPrev (leftChild);
	rightChild->SetNext (leftChild->GetNext ());
	leftChild->SetNext (rightChild);
	if (rightChild->GetNext ()) {
		
		rightChild->GetNext ()->SetPrev (rightChild);
		
	}
	
	
	children[keyNum++] = leftChild;
	children[keyNum] = rightChild;
	
}

void InternalNode :: Delete (int key) {
	
	int index = GetKeyIndex (key);
	
	DecreaseKeyNum ();
	
	if (keys[index] > key) {
		
		for (; index < keyNum; index++) {
			
			keys[index] = keys[index + 1];
			children[index] = children[index + 1];
			
		}
		
		children[index] = children[index + 1];
		
	} else {
		
		for (; index < keyNum; index++) {
			
			keys[index] = keys[index + 1];
			children[index + 1] = children[index + 2];
			
		}
		
	}
	
	if (keyNum < order / 2) {
	
		if (next && next->GetKeyNum () > order / 2) {
			
			next->DecreaseKeyNum ();
			
			int up = next->GetKeys ()[0];
			BPTNode *borrowed = next->GetChildren ()[0];
			borrowed->SetParent (this);
			
			BPTNode *leftParent = parent;
			BPTNode *rightParent = next->GetParent ();
			
			while (leftParent != rightParent) {
				
				leftParent = leftParent->GetParent ();
				rightParent = rightParent->GetParent ();
				
			}
			
			int pindex = leftParent->GetKeyIndex (up);
			if (leftParent->GetKeys ()[pindex] < up) {
				
				pindex--;
				
			}
			
			int down = leftParent->GetKeys ()[pindex];
			
			leftParent->GetKeys ()[pindex] = up;
			
			keys[keyNum++] = down;
			children[keyNum] = borrowed;
			
			for (int i = 0; i < next->GetKeyNum(); i++) {
				
				next->GetKeys ()[i] = next->GetKeys ()[i + 1];
				next->GetChildren ()[i] = next->GetChildren ()[i + 1];
				
			}
			
			next->GetChildren ()[next->GetKeyNum()] = next->GetChildren ()[next->GetKeyNum() + 1];
			
		} else if (prev && prev->GetKeyNum () > order / 2) {
			
			prev->DecreaseKeyNum ();
			
			int up = prev->GetKeys ()[prev->GetKeyNum ()];
			BPTNode *borrowed = prev->GetChildren ()[prev->GetKeyNum () + 1];
			borrowed->SetParent (this);
			
			BPTNode *leftParent = prev->GetParent ();
			BPTNode *rightParent = parent;
			
			while (leftParent != rightParent) {
				
				leftParent = leftParent->GetParent ();
				rightParent = rightParent->GetParent ();
				
			}
			
			int pindex = leftParent->GetKeyIndex (up);
			int down = leftParent->GetKeys ()[pindex];
			
			leftParent->GetKeys ()[pindex] = up;
			
			for (int i = keyNum; i > 0; i--) {
				
				keys[i] = next->GetKeys ()[i + 1];
				children[i + 1] = children[i];
				
			}
			
			children[1] = children[0];
			
			keys[0] = down;
			children[0] = borrowed;
			
		} else if (next) {
			
			Merge (next);
			
		} else if (prev) {
			
			prev->Merge (this);
			
		}
	
	}
	
}

void InternalNode :: Merge (BPTNode* rightNode) {
	
	int *rightKeys = rightNode->GetKeys ();
	BPTNode **rightChildren = rightNode->GetChildren ();
	BPTNode *rightParent = rightNode->GetParent ();
	BPTNode *leftParent = parent;
	
	while (leftParent != rightParent) {
		
		leftParent = leftParent->GetParent ();
		rightParent = rightParent->GetParent ();
		
	}
	
	int pindex = leftParent->GetKeyIndex (rightKeys[0]);
	
	if (pindex == leftParent->GetKeyNum () || leftParent->GetKeys ()[pindex] > rightKeys[0]) {
		
		pindex--;
		
	}
	
	int down = leftParent->GetKeys ()[pindex];
	
	if (keyNum == 0) {
		
		keys[keyNum] = down;
		
		for (int i = 0; i <= rightNode->GetKeyNum (); i++) {
		
			keys[keyNum + i + 1] = rightKeys[i];
			children[keyNum + i + 1] = rightChildren[i];
			children[keyNum + i + 1]->SetParent (this);
		
		}
		
	} else {
		
		keys[keyNum - 1] = down;
		
		for (int i = 0; i <= rightNode->GetKeyNum (); i++) {
		
			keys[keyNum + i] = rightKeys[i];
			children[keyNum + i + 1] = rightChildren[i];
			children[keyNum + i + 1]->SetParent (this);
		
		}
		
	}
	
	
	
	keyNum += 1 + rightNode->GetKeyNum ();
	
	next = rightNode->GetNext ();
	if (next) {
		
		next->SetPrev (this);
		
	}
	/*
	while (leftParent != rightParent) {
		
		leftParent = leftParent->GetParent ();
		rightParent = rightParent->GetParent ();
		
	}
	
	pindex = leftParent->GetKeyIndex (up) - 1;
	leftParent->GetKeys ()[pindex] = up;
	*/
	rightNode->GetParent ()->Delete (down);
	
	delete rightNode;
	
}

BPTNode *InternalNode :: Split (int &key) {
	
	// New right node
	InternalNode *rightNode = new InternalNode (order);
	
	// The key to be moved into the the parent
	key = keys[keyNum / 2];
	
	// Copy every key and child after keyNum / 2 to rightNode
	for (int i = keyNum / 2 + 1; i < keyNum; i++) {
		
		rightNode->keys[i - keyNum / 2 - 1] = keys[i];
		rightNode->children[i - keyNum / 2 - 1] = children[i];
		
	}
	
	rightNode->keyNum = keyNum - keyNum / 2 - 1;
	rightNode->children[rightNode->keyNum] = children[keyNum];
	
	keyNum = keyNum / 2;
	
	for (int i = 0; i <= rightNode->GetKeyNum (); i++) {
		
		rightNode->children[i]->SetParent (rightNode);
		
	}
	
	return rightNode;
	
}

BPTNode **InternalNode :: GetChildren () {
	
	return children;
	
}

LeafNode :: LeafNode (int n) : BPTNode (n) {
	
	isLeaf = true;
	
	values = new float[n];
	
}

LeafNode :: ~LeafNode () {
	
	delete[] values;
	
}

void LeafNode :: Insert (int key, float value) {
	
	int index = GetKeyIndex (key);
	
	// cout << "key : " << key << endl;
	// cout << "index : " << index << endl;
	// cout << "keyNum : " << keyNum << endl;
	
	// move every key and value after index
	for (int i = keyNum; i > index; i--) {
		
		keys[i] = keys[i - 1];
		values[i] = values[i - 1];
		
	}
	
	keys[index] = key;
	values[index] = value;
	
	keyNum++;
	
}

void LeafNode :: Delete (int key) {
	
	int index = GetKeyIndex (key);
	
	if (keys[index] == key) {
		
		// cout << key << " Found!" << endl;
		// cout << keyNum << endl;
		
		DecreaseKeyNum ();
		
		// cout << keyNum << endl;
		
		for (; index < keyNum; index++) {
			
			keys[index] = keys[index + 1];
			values[index] = values[index + 1];
			
		}
		
	} else {
		
		cout << key << " not found!" << endl;
		
		return;
		
	}
	
	if (keyNum < order / 2) {
		
		if (next && next->GetKeyNum () > order / 2) {
			
			next->DecreaseKeyNum ();
			
			keys[keyNum] = next->GetKeys ()[0];
			values[keyNum++] = next->GetValues ()[0];
			
			for (int i = 0; i < next->GetKeyNum (); i++) {
				
				next->GetKeys ()[i] = next->GetKeys ()[i + 1];
				next->GetValues ()[i] = next->GetValues ()[i + 1];
				
			}
			
			BPTNode *leftParent = parent;
			BPTNode *rightParent = next->GetParent ();
			
			while (leftParent != rightParent) {
				
				leftParent = leftParent->GetParent ();
				rightParent = rightParent->GetParent ();
				
			}
			
			int pindex = leftParent->GetKeyIndex (keys[keyNum - 1]);
			if (leftParent->GetKeys ()[pindex] != keys[keyNum - 1]) {
				
				pindex--;
				
			}
			
			leftParent->GetKeys ()[pindex] = next->GetKeys ()[0];
			
		} else if (prev && prev->GetKeyNum () > order / 2) {
			
			prev->DecreaseKeyNum ();
			
			for (int i = keyNum; i > 0; i--) {
				
				keys[i] = keys[i - 1];
				values[i] = values[i - 1];
				
			}
			
			keys[0] = prev->GetKeys ()[prev->GetKeyNum ()];
			values[0] = prev->GetValues ()[prev->GetKeyNum ()];
			
			keyNum++;
			
			BPTNode *leftParent = prev->GetParent ();
			BPTNode *rightParent = parent;
			
			while (leftParent != rightParent) {
				
				leftParent = leftParent->GetParent ();
				rightParent = rightParent->GetParent ();
				
			}
			
			int pindex = leftParent->GetKeyIndex (keys[0]);
			
			leftParent->GetKeys ()[pindex] = keys[0];
			
		} else if (next) {
			
			Merge(next);
			
		} else if (prev) {
			
			prev->Merge (this);
			
		}
		
	}
	
}

void LeafNode :: Merge (BPTNode* rightNode) {
	
	int *rightKeys = rightNode->GetKeys ();
	float *rightValues = rightNode->GetValues ();
	
	int down = rightKeys[0];
	
	for (int i = 0; i < rightNode->GetKeyNum (); i++) {
		
		keys[keyNum + i] = rightKeys[i];
		values[keyNum + i] = rightValues[i];
		
	}
	
	keyNum += rightNode->GetKeyNum ();
	
	next = rightNode->GetNext ();
	if (next) {
		
		next->SetPrev (this);
		
	}
	
	rightNode->GetParent ()->Delete (down);
	
	delete rightNode;
	
}

BPTNode *LeafNode :: Split (int &key) {
	
	// New right node
	LeafNode *rightNode = new LeafNode (order);
	
	// The key to be moved into the parent
	key = keys[keyNum / 2];
	
	// Copy every key and value after keyNum / 2 to rightNode
	for (int i = keyNum / 2; i < keyNum; i++) {
		
		rightNode->keys[i - keyNum / 2] = keys[i];
		rightNode->values[i - keyNum / 2] = values[i];
		
	}
	
	rightNode->keyNum = keyNum - keyNum / 2;
	
	keyNum = keyNum / 2;
	
	return rightNode;
	
}

float *LeafNode :: GetValues() {
	
	return values;
	
}
