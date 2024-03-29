#include <iostream>
#include "ArgumentManager.h"
#include <string>
#include <fstream>
//this program was made in DS/Algo Course for practicing linked lists the purpose is to have a library of books which you can add to,
// delete by position,author,title, or id. You can also sort the library by any of its attributes.
struct Node {
	string book_name;
	string book_author;
	string book_id;
	Node * next;
};

Node * head = NULL;     
int numOfNodes = 0;
void Print(string filename);
void Add(int pos,string ID,string author,string title);
void DeleteKeyword(string ID, string author, string title);
void DeletePos(int pos);
void bubbleSort(string str);
void swap(Node * a, Node * b);


//provide the program input/output files as command line arguments and another command file
int main(int argc, char * argv[])
{
	ArgumentManager am(argc, argv);
	string myinput = am.get("input");
	string myoutput = am.get("output");
	string commandLines = am.get("command");
	ifstream inputfile;
	inputfile.open(myinput);
	string line;
	string bName;
	string aName;
	string bID;
	//used to parse the inputfile and store data
	string sArr[4];
	Node * previous = NULL;
	Node * loop = NULL;
	bool duplicate;
	//read file
	while (!inputfile.eof()) {     
		getline(inputfile, line);

		if (line.length() > 0) {
			istringstream iss(line);
			int i = 0;
			while (getline(iss,line, ',')) {
				sArr[i] = line.substr(line.find(":") + 1);
				i++;
			}

			bName = sArr[1];
			aName = sArr[2];
			bID = sArr[0];

			Node * temp = new Node();
			temp->book_id = bID;
			temp->book_name = bName;
			temp->book_author = aName;
			temp->next = NULL;

			if (numOfNodes == 0) {
				head = temp;
				previous = head;
				numOfNodes++;
			}
			
			else {
				duplicate = false;
				loop = head;
				for (int i = 0; i < numOfNodes; i++) {
					if (bID == loop->book_id) {
						delete temp;
						duplicate = true;
					}
					loop = loop->next;

				}
				if (duplicate == false) {
					previous->next = temp;
					previous = temp;
					numOfNodes++;
				}

			}
		}
	}
	inputfile.close();

	string removeString= "";
	string commandText = "";
	string addPosition;
	string commandType;
	string temp;
	string keyword;
	ifstream commandFile;
	commandFile.open(commandLines);
	//read command file performs commands based on what is read one by one line
	while (!commandFile.eof()) {
		temp= "";
		getline(commandFile, temp);
		if (temp.length() > 0) {
			
			commandText = temp;
			istringstream stream(temp);
			if (getline(stream, temp, ' ')) {
				commandType = temp;
			}
			if (commandType == "add") {
				string arr[4];
				int i = 0;
				while (getline(stream, commandText, ',')) {
					arr[i] = commandText;
					i++;
				}
				addPosition = arr[0];
				bID = arr[0];
				istringstream s1(addPosition);
				(getline(s1, addPosition, ' '));
				bID = bID.substr(bID.find(" ") + 1);
				bName = arr[1];
				aName = arr[2];

				addPosition = addPosition.substr(addPosition.find(':') + 1);
				bName = bName.substr(bName.find(":") + 1);
				aName = aName.substr(aName.find(":") + 1);
				bID = bID.substr(bID.find(":") + 1);
				int addPos = stoi(addPosition);

				Add(addPos, bID, aName, bName);
			}
			if (commandType == "remove") {

				removeString = commandText.substr(commandText.find(" ") + 1);
				keyword = removeString.substr(0, removeString.find(":"));
				removeString = removeString.substr(removeString.find(":") + 1);


				if (keyword == "book_author") {
					DeleteKeyword("", removeString, "");
				}
				if (keyword == "book_name") {
					DeleteKeyword("", "", removeString);
				}
				if (keyword == "book_id") {
					DeleteKeyword(removeString, "", "");
				}
				if (keyword == "pos") {
					int removeInt = stoi(removeString);
					DeletePos(removeInt);
				}
			}
			if (commandType == "sort") {
				string typeOfSort = commandText.substr(commandText.find(" ") + 1);
				bubbleSort(typeOfSort);
			}
		}
	}
	commandFile.close();


	Print(myoutput);

}

void DeleteKeyword(string ID, string author, string title) {
	Node * temp1 = head;
	Node * temp2 = head;
	Node * deleter = NULL;
	int size = numOfNodes;
	for (int i = 0; i < size; i++) {
		if ((temp1->book_author == author) || (temp1->book_id == ID) || (temp1->book_name == title)) {
			if (i==0) {
				head = temp1->next;
				deleter = temp1;
				temp1 = temp1->next;
				temp2 = temp1;
				delete deleter;
				numOfNodes--;
				
			}
			else {
				temp2->next = temp1->next;
				deleter = temp1;
				temp1 = temp1->next;
				delete deleter;
				numOfNodes--;
			}
		}
		else {
			temp2 = temp1;
			temp1 = temp1->next;
		}
	}
}
void DeletePos(int pos) {
	Node * temp1 = head;
	if (pos < 0 || pos >= numOfNodes) {
		return;
	}
	if (pos == 0) {
		head = temp1->next;
		delete temp1;
		numOfNodes--;
		return;
	}
	for (int i = 0; i < pos - 1; i++) {
		temp1=temp1->next;
	}
	Node * temp2 = temp1->next;
	temp1->next = temp2->next;
	delete temp2;
	numOfNodes--;
}

void Add(int pos,string ID, string author, string title) {
	Node * temp1 = new Node();
	Node * looper = head;
	temp1->book_id = ID;
	temp1->book_author = author;
	temp1->book_name = title;
	for (int i = 0; i < numOfNodes; i++) {
		if (temp1->book_id == looper->book_id) {
			delete temp1;
			return;
		}
		looper = looper->next;
	}

	if ((pos > numOfNodes - 1)||pos < 0) {
		pos = numOfNodes;
	}

	if (pos == 0) {
		temp1->next = head;
		head = temp1;
	}
	else {
		Node * temp2 = head;
		for (int i = 0; i < pos - 1; i++) {
			temp2 = temp2->next;
		}
		temp1->next = temp2->next;
		temp2->next = temp1;
	}
	numOfNodes++;
}

void Print(string filename) {
	
	ofstream outputfile;
	outputfile.open(filename);
	Node * temp = head;
	while (temp != NULL) {
		outputfile << "book_id:" << temp->book_id << ", book_name:" << temp->book_name << ", book_author:" << temp->book_author << "\n";
		temp = temp->next;
	}
	outputfile.close();
}

void bubbleSort(string str) {
	Node * temp1 = head;
	Node * temp2 = head;
	if (temp1 == NULL) {
		return;
	}

	if (str == "book_author") {
		for (int i = 0; i <= numOfNodes-1; i++) {
			while (temp1->next != NULL) {
				if (temp1->book_author > temp1->next->book_author) {
					temp2 = temp1->next;
					swap(temp1, temp2);
				}
				temp1 = temp1->next;
			}
			temp1 = head;
		}
	}
	if (str == "book_name") {
		for (int i = 0; i <= numOfNodes - 1; i++) {
			while (temp1->next != NULL) {
				if (temp1->book_name > temp1->next->book_name) {
					temp2 = temp1->next;
					swap(temp1, temp2);
				}
				temp1 = temp1->next;
			}
			temp1 = head;
		}
	}
	if (str == "book_id") {
		for (int i = 0; i <= numOfNodes - 1; i++) {
			while (temp1->next != NULL) {
				if (temp1->book_id > temp1->next->book_id) {
					temp2 = temp1->next;
					swap(temp1, temp2);
				}
				temp1 = temp1->next;
			}
			temp1 = head;
		}
	}
}


//exchange data between two books
void swap(Node * a, Node * b) {
	string bID = a->book_id;
	string bName = a->book_name;
	string aName = a->book_author;
	a->book_author = b->book_author;
	a->book_id = b->book_id;
	a->book_name = b->book_name;
	b->book_name = bName;
	b->book_author = aName;
	b->book_id = bID;
}