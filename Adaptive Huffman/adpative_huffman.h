//2021.11.09 정철현
//Adaptive_huffman coding using binary tree

#include <iostream> 
#include <vector>
#include <queue>
using namespace std;

struct node
{
	string characters;
	unsigned int frequency;
	string code;
	node* leftChild;
	node* rightChild;
	node* parent;
};


void SwapNode(node* a, node* b) {
	bool AisLeft = false;
	bool BisLeft = false;
	if (a->parent->leftChild->characters == a->characters) AisLeft = true;
	if (b->parent->leftChild->characters == b->characters) BisLeft = true;

	if (AisLeft) a->parent->leftChild = b;
	else a->parent->rightChild = b;

	if (BisLeft) b->parent->leftChild = a;
	else b->parent->rightChild = a;

	node* tmpparent = a->parent;
	a->parent = b->parent;
	b->parent = tmpparent;


	node* parentA = a->parent;
	while (true) {
		parentA->frequency = parentA->leftChild->frequency + parentA->rightChild->frequency;
		parentA = parentA->parent;
		if (parentA == NULL) break;
	}

	node* parentB = b->parent;

	while (true) {
		parentB->frequency = parentB->leftChild->frequency + parentB->rightChild->frequency;
		parentB = parentB->parent;
		if (parentB == NULL) break;
	}
}

node * findnode(node *root, string target = "NEW") {
	queue<node> NodeQ;
	NodeQ.push(*root);
	if (root->characters == target) return root;
	while (!NodeQ.empty()) {
		node *current_node = &NodeQ.front();
		if (current_node->leftChild != NULL) {
			if (current_node->leftChild->characters == target) return current_node->leftChild;

			NodeQ.push(*current_node->leftChild);
		}
		if (current_node->rightChild != NULL) {
			if (current_node->rightChild->characters == target) return current_node->rightChild;
			NodeQ.push(*current_node->rightChild);

		}
		NodeQ.pop();

	}
	
	return NULL;
}

int SearchDepth(node& root, string target) {
	int depth = 0;
	queue<node> NodeQ;
	NodeQ.push(root);

	while (!NodeQ.empty()) {
		int node_size = NodeQ.size();
		for (int i = 0; i < node_size; i++) {
			node* current_node = &NodeQ.front();
			if (current_node->characters == target) {
				return depth;
			}
			else {
				if (current_node->leftChild != NULL) {
					NodeQ.push(*current_node->leftChild);
				}
				if (current_node->rightChild != NULL) {
					NodeQ.push(*current_node->rightChild);
				}
			}
			NodeQ.pop();

		}
		depth++;
	}
	return -1; //new가 없을경우
}

void UpdateFrequency(node* target) {
	node *tmp = target->parent;
	if (tmp == NULL) return;
	tmp->frequency = tmp->leftChild->frequency + tmp->rightChild->frequency;
	UpdateFrequency(tmp);
}

void CheckTree(node& root, node * changed_node, vector<string> SArr, vector<int> IArr) {
	int freq = changed_node->frequency;
	int depth = SearchDepth(root, changed_node->characters);

	for (int i = 0; i < SArr.size(); i++) {
		if (freq > IArr[i]) {
			int tmp_dep = SearchDepth(root, SArr[i]);
			if (tmp_dep < depth && SArr[i] != changed_node->characters) {
				node* tmp = findnode(&root, SArr[i]);
				SwapNode(changed_node, tmp);
				break;
			}
		}
	}
}


void AssignCode(node * root) {
	if (root->leftChild == NULL && root->rightChild == NULL) return;
	else {
		if (root->leftChild != NULL) {
			root->leftChild->code = root->code + "0";
			AssignCode(root->leftChild);
		}
		if (root->rightChild != NULL) {
			root->rightChild->code = root->code + "1";
			AssignCode(root->rightChild);
		}
	}
}


void CheckTree2(node *root) {
	

	if (root->leftChild != NULL && root->rightChild != NULL) {
		if (root->leftChild->frequency > root->rightChild->frequency) {
			SwapNode(root->leftChild, root->rightChild);
		}
		CheckTree2(root->leftChild);
		CheckTree2(root->rightChild);
	}
	else return;
	
}


void Print(node *root, const vector<string> string_arr,const vector<int> cnt_arr) {
	cout << endl;
	cout << "\tdata\tcount" << endl;
	cout << "\t-------------" << endl;
	for (int i = 0; i < string_arr.size(); i++) {
		cout << "\t" << string_arr[i] << "\t" << cnt_arr[i] << endl;
	}
	cout << endl;
	cout << "\tdata\tcode" << endl;
	cout << "\t-------------" << endl;
	for (int i = 0; i < string_arr.size(); i++) {
		node* tmp = findnode(root, string_arr[i]);
		cout << "\t" << string_arr[i] << "\t" << tmp->code << endl;
	}
	cout << endl;
}

void get_adaptive_huffman_code() {
	//필요한 변수 준비
	cout << "Adaptive huffman coding." << endl << endl;
	vector<string> charset;
	vector<int> cnt;

	charset.push_back("NEW");
	cnt.push_back(0);

	//Init New node for Adaptive huffman coding
	node NEW;
	NEW.characters = "NEW";
	NEW.code = "";
	NEW.frequency = 0;
	NEW.leftChild = NULL;
	NEW.rightChild = NULL;
	NEW.parent = NULL;

	//Root Node 준비
	node * root = &NEW;

	string userinput;
	while (true){ 
		cout << "input : ";
		cin >> userinput;
		if (userinput == "quit") break;
		

		auto search_it = find(charset.begin(), charset.end(), userinput);
		if (search_it == charset.end()) { //기존 트리에 없는 char가 입력으로 들어왔을 경우
			charset.push_back(userinput);
			node* Newadd = findnode(root);
			Newadd->characters = "PARENT";
			node * AppendNew;
			AppendNew = new node;
			AppendNew->characters = "NEW";
			AppendNew->code = "";
			AppendNew->frequency = 0;
			AppendNew->leftChild = NULL;
			AppendNew->rightChild = NULL;
			Newadd->leftChild = AppendNew;
			AppendNew->parent = Newadd;
			node *Appendchar = new node;
			Appendchar->characters = userinput;
			Appendchar->code = "";
			Appendchar->frequency = 1;
			Appendchar->leftChild = NULL;
			Appendchar->rightChild = NULL;
			Newadd->rightChild = Appendchar;
			Appendchar->parent = Newadd;
			cnt.push_back(1);

			UpdateFrequency(Appendchar);
			CheckTree(*root, Appendchar,charset,cnt);
			CheckTree2(root);
		}
		else {
			node *tmp = findnode(root, userinput);
			tmp->frequency++;
			auto it = find(charset.begin(), charset.end(), userinput);
			cnt[it - charset.begin()]++;
			UpdateFrequency(tmp);
			CheckTree(*root, tmp,charset,cnt);
			CheckTree2(root);

		}
		AssignCode(root);

		Print(root, charset, cnt);

	}
		


}

