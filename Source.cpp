#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
using namespace std;

struct runInfo {
	int left;
	int size;
};

int GetMinrun(int n) //get minrun by size of arr
{
	int r = 0;
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

void insertionSort(int arr[], int n) 
{
	for (int i = 1; i < n; i++) 
	{
		int temp = arr[i];
		int j = i - 1;
		while (arr[j] > temp && j >= 0)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;
	}
}

void print(int arr[], int n)
{
	for (int i = 0; i < n; i++)
		cout << arr[i] << " ";
	cout << endl;
}

void merge(int arr[], runInfo object1, runInfo object2)
{
	int* temp1, * temp2;                     //mergin arrays 
	temp1 = new int[object1.size];
	temp2 = new int[object2.size];
	int index1 = 0, index2 = 0, indexRes = 0;  // indexes of elements in first, second and result arrays respectively
	int* resArr = new int[object1.size + object2.size];
	int amount = 0, arrID = 0;                 // amount --- count of sequance going elements // arrID --- number of array
	for (int i = 0; i < object1.size; i++)     //                                                         (temp1 or temp2)
		temp1[i] = arr[object1.left + i];      //filling
	for (int i = 0; i < object2.size; i++)     //temporary
		temp2[i] = arr[object2.left + i];      //arrays
	while (index1 < object1.size && index2 < object2.size)
	{
		if (temp1[index1] <= temp2[index2]) //compare first elements in both arrays
		{
			if (amount >= 6 && arrID == 1)
			{
				int jump = 1;               //amount of elements that would be added to main arr without comparing
				while (index1 + jump < object1.size && temp1[index1+jump] < temp2[index2])  //
				{                                                                           //Galloping
					jump *= 2;                                                              //
				}                                                                           //Mode
				jump /= 2;                                                                  //
				for (int i = index1; i < index1 + jump; i++)
				{
					resArr[indexRes] = temp1[i];
					indexRes++;
				}
				index1 += jump;
				indexRes--;
				amount = -1;
			}
			else
			{
				resArr[indexRes] = temp1[index1];
				index1++;
				if (arrID == 2 || arrID == 0)
				{
					arrID = 1;
					amount = 0;
				}
				amount++;
			}
		}
		else
		{
			if (amount >= 6 && arrID == 2)
			{
				int jump = 1;
				while (jump + index2 < object2.size && temp2[index2 + jump] < temp1[index1])
				{
					jump *= 2;
				}
				jump /= 2;
				for (int i = index2; i < index2+jump; i++)
				{
					resArr[indexRes] = temp2[i];
					indexRes++;
				}
				index2 += jump;
				indexRes--;
				amount = -1;
			}
			else
			{
				resArr[indexRes] = temp2[index2];
				index2++;
				if (arrID == 1 || arrID == 0)
				{
					arrID = 2;
					amount = 0;
				}
				amount++;
			}
		}
		indexRes++;
		if (index1 >= object1.size) {                                       
			for (int i = index2; i < object2.size; i++)                  //add temp2 and temp1 to result array
			{                                                               
				resArr[indexRes] = temp2[i];                                
				indexRes++;                                                 
			}
		}
		else if (index2 >= object2.size) {
			for (int i = index1; i < object1.size; i++)
			{
				resArr[indexRes] = temp1[i];
				indexRes++;
			}
		}
	}

	for (int i = 0; i < object1.size + object2.size; i++)
	{
		arr[object1.left + i] = resArr[i];                                 //add result array to main array
	}                                 
	delete[] temp1;
	delete[] temp2;
	delete[] resArr;
}

void TimSort(int arr[], int n)
{
	int minrun = n;
	if (n > 64)                              //if minrun < 64 then our array will be sorted by InsertionSort function only
	{                                        //else will use TimSort
		minrun = GetMinrun(n);
	}
	runInfo *info = new runInfo[n/minrun];
	int infosize = 0;
	int pointer = 0;
	while (pointer + 1 < n)
	{
		int first = pointer;                 //initial element of main array
		int now = pointer+1;                 //current element of main array
		int* temp = new int[n];              //temporary array to save part of main array (run)
		temp[0] = arr[first];                //in run it always exist the first element
		int size = 1;                        //counter for save size of run
		int i = 1;                           //current element of run 
		
		if (arr[first] >= arr[first - 1])    //define of increasing of first elements
		{
			while (arr[now] >= arr[now - 1])
			{
				temp[i] = arr[now];         //adding elements in temp array from main array while they increase
				size++;                   
				now++;
				i++;
			}
			while (size < minrun)          //adding rest elements from main array to run
			{
				temp[size] = arr[now];
				size++;
				now++;
				i++;
			}
		}
		else                              //define of decreasing of first elements
		{
			while (arr[now] < arr[now - 1])
			{
				temp[i] = arr[now];
				size++;
				now++;
				i++;
			}
			for (int i = 0; i < size / 2; i++)
			{
				int val = temp[i];
				temp[i] = temp[size - 1 - i];
				temp[size - i - 1] = val;
			}
			while (size < minrun)
			{
				temp[size] = arr[now];
				size++;
				now++;
			}
		}
		pointer = now;
		insertionSort(temp, size);                    //sorting run array (temp) by InsertionSort                     
		for (int i = 0; i < size; i++)
		{
			arr[first + i] = temp[i];
		}
		runInfo a;                                   //varuable for info about run array 
		a.left = first;                              //pos of first element in main arr
		a.size = size;                               //size of run array
		info[infosize] = a;                          //array with info about all run arrays
		infosize++;                                  //counter of runs
		delete[] temp;
	}

	vector <runInfo> stack;                          //stack with all information about run arrays 

	for (int i = 0; i <= infosize; i++)
	{
		if(i < infosize)
			stack.push_back(info[i]);                //adding new element (runInfo) in vector from array (info)
		if (stack.size() == 2 && i >= infosize - 1)  //if there is no more items to add into stack 
		{ 
			merge(arr, stack.at(stack.size() - 2), stack.at(stack.size() - 1));        //merging last two elements in stack
			stack.at(stack.size() - 2).size += stack.at(stack.size() - 1).size;
			stack.erase(stack.begin() + stack.size() - 1);
		}
		if (stack.size() > 2) {                                                         //merge anyway
			if (stack.at(stack.size() - 1).size > stack.at(stack.size() - 2).size + stack.at(stack.size() - 3).size
				&& stack.at(stack.size() - 2).size > stack.at(stack.size() - 3).size) {             //if X > Y + Z && Y > Z
				//merge(Y and X);
				merge(arr, stack.at(stack.size() - 2), stack.at(stack.size() - 1));
				stack.at(stack.size() - 2).size += stack.at(stack.size() - 1).size;
				stack.erase(stack.begin() + stack.size() - 1);
			}
			else {
				if (stack.at(stack.size() - 1).size < stack.at(stack.size() - 3).size)             //if X < Z 
				{
					//merge(Y and X);
					merge(arr, stack.at(stack.size() - 2), stack.at(stack.size() - 1));
					stack.at(stack.size() - 2).size += stack.at(stack.size() - 1).size;
					stack.erase(stack.begin() + stack.size() - 1);
				}
				else {                                                                             //in else cases
					//merge(Z and Y);
					merge(arr, stack.at(stack.size() - 3), stack.at(stack.size() - 2));
					stack.at(stack.size() - 3).size += stack.at(stack.size() - 2).size;
					stack.erase(stack.begin() + stack.size() - 2);
				}
			
			}
		}
	}
}
int main()
{
	int* arr;
	cout << "Enter amount of elements\n->";
	int n = 0;
	cin >> n;
	arr = new int[n];

	srand(time(NULL));

	for (int i = 0; i < n; i++)
	{
		arr[i] = rand() % 100;
	}

	print(arr, n);
	TimSort(arr, n);
	cout << endl << endl << endl;
	print(arr, n);
	system("pause");
	return 0;
}