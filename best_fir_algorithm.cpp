#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

//global vectors
vector<string>jobName;
vector<int>partitionArray;
vector<int>jobSizeArray;


int getPartitionNumber() {
	int tempPartitionNumbers;
	int partitionCheck = 0;
	do {
		cout << "Enter Number of partition(s): ";
		cin >> tempPartitionNumbers;
		if (tempPartitionNumbers <= 5 && tempPartitionNumbers > 0) {
			partitionCheck = 1;
		}
		else
			cout << "Range for number of partition is 1 to 5." << endl;

	} while (partitionCheck == 0);
	return tempPartitionNumbers;
}

void getPartitionSize(int partitionNum, int memSize) {
	bool condition = true;

	int temp;


	for (int i = 0; i < partitionNum;i++) {
		cout << "Enter Partition size for P" << i + 1 << ":";
		cin >> temp;
		if (temp > memSize || temp < 0) {
			cout << "Size must be between 0 to " << memSize << endl;
			i--;
		}
		else {
			partitionArray.push_back(temp);
			memSize -= temp;
			cout << setw(50) << "Remaining memory: " << memSize << endl;
			temp = 0;
			if (memSize == 0) {
				for (int index = i + 1; index < partitionNum - 1; index++) {
					partitionArray.push_back(0);
				}
				i = partitionNum;
			}
		}
	}
}

void getJobSize(int jobCount) {

	cout << "\n\n******** Enter Job Details below ********\n" << endl;

	for (int i = 0; i < jobCount;i++) {
		string temp; int tempSize = 0;
		cout << "Enter name for job" << i + 1 << ": ";
		cin >> temp;
		jobName.push_back(temp);

		cout << "Enter job size: ";
		cin >> tempSize;
		jobSizeArray.push_back(tempSize);

		if (tempSize == 0) {
			cout << "Job size should to be at least 1" << endl;
			i--;
		}
	}
}


bool findElement(int element, int anArray[], int partSize) {


	for (int i = 0; i < partSize; i++) {

		if (anArray[i] == element) {
			cout << "looking for: " << element << "found at " << anArray[i] << endl;
			return true;
		}
	}
	return false;
}


void printData(int sizeHolder[], int sequenceHolder[], int partSize) {
	int waste = 0;
	int totalWaste = 0;

	for (int i = 0; i < partSize; i++) {

		if (sequenceHolder[i] > 0) {
			waste = partitionArray[sequenceHolder[i] - 1] - sizeHolder[i];
			totalWaste += waste;
			cout << setw(10) << "\nJob " << jobName[i] << " -> P" << sequenceHolder[i] << " Waste: " << setw(5) << waste << setw(10) << "  Run";
		}
		else {
			totalWaste += partitionArray[(-1)*(sequenceHolder[i] + 1)];
			cout << setw(10) << "\nJob " << jobName[i] << " -> -" << setw(25) << "Wait";
		}
	}
	cout << "\n\nTotal Waste: " << setw(11) << totalWaste;
}


void first_fit(int partSize) {

	//variables to keep track of indexes
	int empty_index = 0; int partIndex = 0; int jobIndex = 0;
	int sizeHolder[partSize];
	int sequenceHolder[partSize];
	int remJob[partSize];
	bool condition = true;
	int counter = 0;


	do {
		//loop stops when Job size is same as partition size
		if (jobIndex <= partSize) {

			//keeps in the range of partition size 0 to partSize 
			partIndex %= partSize;

			//finds if the partition in use or not, if not in use returns true
			if (!(findElement(partIndex + 1, sequenceHolder, partSize))) {

				//comparing the size partition with job size 
				if (partitionArray[partIndex] >= jobSizeArray[jobIndex]) {

					sizeHolder[jobIndex] = jobSizeArray[jobIndex];
					sequenceHolder[jobIndex] = partIndex + 1;
					jobIndex++;
					partIndex = 0;
					counter = 0;
					//counter will check if we need to fill the empty index 
					//or just need to increase partIndex.
					if (counter == 0 && partIndex != 0)
					{
						partIndex++;
					}
					else {
						partIndex = empty_index;
						counter = 0;
					}
				}
				else {

					if (counter < partSize) {
						counter++;
						if (counter == 1) {
							empty_index = partIndex;
						}
					}//if counter is same as toal number of partition 
					 //that means job can't fit in any partition
					else {
						//if job can't fit in any partition, adding index*(-1)
						sequenceHolder[jobIndex] = (partIndex + 1)*(-1);
						sizeHolder[jobIndex] = 0;
						jobIndex++;
						partIndex = 0;
						counter = 0;
					}
					partIndex++;
				}
			}
			else
				partIndex++;
		}
		else
			condition = false;

	} while (condition);


	printData(sizeHolder, sequenceHolder, partSize);

}

void best_fit(int partSize) {

	cout << "inside" << endl;
	//variables to keep track of indexes
	int empty_index = 0; int partIndex = 0; int jobIndex = 0;
	int sizeHolder[partSize];
	int sequenceHolder[partSize];
	int remJob[partSize];
	bool condition = true;

	for (jobIndex = 0; jobIndex < partSize; jobIndex++) {
		for (partIndex = 0; partIndex < partSize; partIndex++) {
			cout << "jobIndex: " << jobIndex << "  partIndex: " << partIndex << endl;
			if ((partitionArray[partIndex] >= jobSizeArray[jobIndex]) && (!(findElement(partIndex + 1, sequenceHolder, partSize)))) {

				if (partitionArray[partIndex] == jobSizeArray[jobIndex]) {
					cout << "2jobIndex: " << jobIndex << "  2partIndex: " << partIndex << endl;
					sizeHolder[jobIndex] = jobSizeArray[jobIndex];
					sequenceHolder[jobIndex] = partIndex + 1;
					//jobIndex++;
					partIndex = partSize;
					if (jobIndex == partSize) {
						cout << "oneoneone" << endl;
						partIndex = partSize;
					}
					cout << "its in     2jobIndex: " << jobIndex << "  2partIndex: " << partIndex << endl;

				}
				else {
					int tempSize = partitionArray[partIndex] - jobSizeArray[jobIndex];
					int tempIndex = -1;
					if (jobIndex == partSize - 1) {
						tempIndex = partIndex;
					}

					for (int i = 0; i < partSize; i++) {
						int curSize = partitionArray[i] - jobSizeArray[jobIndex];
						cout << "For.......i= " << i << endl;
						if ((partitionArray[i] > jobSizeArray[jobIndex]) && (!(findElement(i + 1, sequenceHolder, partSize))) && (tempSize > curSize)) {
							cout << "inside most insidefor loop" << endl;
							cout << "Another one     3jobIndex: " << jobIndex << "  3partIndex: " << i << endl;
							tempSize = curSize;
							tempIndex = i;
						}
					}

					if (tempIndex != -1) {
						cout << "Last if" << endl;
						cout << "Last one     3jobIndex: " << jobIndex << "  3partIndex: " << tempIndex << endl;
						sizeHolder[jobIndex] = jobSizeArray[jobIndex];
						sequenceHolder[jobIndex] = tempIndex + 1;
						partIndex = partSize;
					}
				}
			}
		}
	}

	/**do{
		partIndex %= partSize -1;

		if((partitionArray[partIndex] >= jobSizeArray[jobIndex]) && (!(findElement(partIndex+1, sequenceHolder)))){

			if(partitionArray[partIndex] == jobSizeArray[jobIndex]){
				sizeHolder[jobIndex] = jobSizeArray[jobIndex];
				sequenceHolder[jobIndex] = partIndex+1;
				jobIndex++;
				partIndex = 0;
			}else{
				int tempSize = partitionArray[partIndex] - jobSizeArray[jobIndex];
				int tempIndex = -1;

				for(int i = 0; i<partSize; i++){
					curSize = partitionArray[i] - jobSizeArray[jobIndex];
					if((partitionArray[i] > jobSizeArray[jobIndex]) && (!(findElement(i+1, sequenceHolder)))&&(tempSize>curSize)){
						tempSize = curSize;
						tempIndex = i;
					}
				}

				if(tempIndex != -1){
					sizeHolder[jobIndex] = jobSizeArray[jobIndex];
					sequenceHolder[jobIndex] = tempIndex+1;
					jobIndex++;
					partIndex = 0;
				}
			}
		}else{
			//checks the condition for do..while loop
			if(jobIndex >= partSize-1){
				condition = false;
			}else{
				partIndex++;

				//increase the job index if partition index is
				//at the last partition
				if(partIndex >= partSize){
					jobIndex++;
					partIndex=0;
				}
			}
		}

	}while(condition);**/

	printData(sizeHolder, sequenceHolder, partSize);

}

int main() {
	int memorySize;
	int partitionSize;
	int partitionCheck = 0;
	int userInput = 0;

	cout << "Enter 0 to menually enter data or press any number and ENTER." << endl;
	cin >> userInput;

	//giving option to do quik check or by menual data entry
	if (userInput == 0) {
		cout << "Enter Memory Size: ";
		cin >> memorySize;

		partitionSize = getPartitionNumber();

		getPartitionSize(partitionSize, memorySize);

		getJobSize(partitionSize);
	}

	//to do quik check user can input any number (except 0)
	else {
		memorySize = 1000;
		partitionSize = 4;
		string jobName1[] = { "1","2","3","4" };
		int partitionArray1[] = { 300,200,400,100 };
		int jobSizeArray1[] = { 200,300,100,200 };

		//copying arrays into global variable
		copy(begin(jobName1), end(jobName1), back_inserter(jobName));
		copy(begin(partitionArray1), end(partitionArray1), back_inserter(partitionArray));
		copy(begin(jobSizeArray1), end(jobSizeArray1), back_inserter(jobSizeArray));

		int index[] = { 0,1,2,3 };
		for (int i : index) {
			cout << "Partition " << i + 1 << ": " << partitionArray[i] << "   " << "Job "
				<< i + 1 << ": " << jobSizeArray[i] << endl;
		}

	}
	first_fit(partitionSize);

	best_fit(partitionSize);

	return 0;
}


