/*
 * Filename   : Sorts.cc
 * Author     : Ryan Haas
 * Course     : CSCI 362-01
 * Assignment : Program 7
 * Description: Write a program that compares merge sort, quicksort, and shell sort
 * 		by the time it takes to execute as well as how many comparisons each
 * 		sort performs with various sample sizes
 */

/****************************************************/
// System includes

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <math.h>
#include <iomanip>

/****************************************************/
// Local includes

#include "Timer.hpp"

/****************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::sort;
using std::mt19937;
using std::uniform_int_distribution;
using std::random_device;
using std::setprecision;
using std::swap;

/****************************************************/
// Function prototypes/global vars/type definitions

size_t
promptSize();

vector<int>
generateVector(size_t size);

size_t
mergeSort(vector<int>& v);

size_t
mergeHelper(vector<int>& v, size_t begin, size_t end, vector<int>& temp);

size_t
merge(vector<int>& v, size_t begin, size_t mid, size_t end, vector<int>& temp);

size_t
quickSort(vector<int>& v);

size_t
quickHelper(vector<int>& v, size_t first, size_t last);

size_t
quickPartition(vector<int>& v, size_t first, size_t last, size_t& compares);

size_t
median3(vector<int>& v, size_t left, size_t right);

size_t
insertionSort(vector<int>& v, size_t begin, size_t end);

size_t
shellSort(vector<int>& v);

void
printVector(vector<int>& v);

/****************************************************/

int
main(int argc, char* argv[])
{
	// Get vector size
	size_t size = promptSize();

	// Create vector and copy it 3 additional times for each sort
	vector<int> mergeVec = generateVector(size);
	vector<int> quickVec (mergeVec);
	vector<int> shellVec (mergeVec);
	vector<int> stdVec (mergeVec);
	
	// Create timer
	Timer<> timer;

	// Perform and time MergeSort algorithm storing necessary information 
	timer.start();
	size_t mergeCompares = mergeSort(mergeVec);
	timer.stop();
	double mergeTime = timer.getElapsedMs();

	// Print MergeSort information
	cout << "Merge time: " /*<< setprecision(3)*/ << mergeTime << " ms" << endl;
	cout << "Merge compares: " << mergeCompares << endl << endl;

	// Perform and time QuickSort algorithm storing necessary information
	timer.start();
	size_t quickCompares = quickSort(quickVec);
	timer.stop();
	double quickTime = timer.getElapsedMs();

	// Print QuickSort information
	cout << "Quick time: " /*<< setprecision(3)*/ << quickTime << " ms" << endl;
	cout << "Quick compares: "<< quickCompares << endl << endl;

	// Perform and time ShellSort algorithm storing necessary information
	timer.start();
	size_t shellCompares = shellSort(shellVec);
	timer.stop();
	double shellTime = timer.getElapsedMs();

	// Print ShellSort information
	cout << "Shell time: " << /*setprecision(3) << */shellTime << " ms" << endl;
	cout << "Shell compares: " << shellCompares << endl << endl;

	// Perform and time std::sort algorithm storing necessary information
	timer.start();
	sort(stdVec.begin(), stdVec.end());
	timer.stop();
	double stdTime = timer.getElapsedMs();

	// Print std::sort information
	cout << "std  time: " /*<< setprecision(3)*/ << stdTime << " ms" << endl << endl;

	// Check all custom written algorithms are properly sorted
	cout << "Merge ok? " << std::boolalpha << (stdVec == mergeVec) << endl;
	cout << "Quick ok? " << std::boolalpha << (stdVec == quickVec) << endl;
	cout << "Shell ok? " << std::boolalpha << (stdVec == shellVec) << endl;

	return EXIT_SUCCESS;
}

/****************************************************/

// Prompt the user for the size of the vector
size_t
promptSize()
{
	size_t size;
	cout << "N ==> ";
	cin >> size;
	cout << endl;
	return size;
}

/****************************************************/

// Create a vector with the given size filled with random
// 	ints between 0 and 1000000000 inclusive using the
// 	mt19937 engine and random_device for the seed
vector<int>
generateVector(size_t size)
{
	// Allocate the vector
	vector<int> v (size);

	// Set random generation
	random_device device;
	mt19937 random (device());
	uniform_int_distribution<int> distro (0, pow(10, 9));

	// Populate vector
	for(size_t i = 0; i < size; ++i)
		v[i] = distro(random);

	return v;
}

/***************************************************/

// Print the given vector in '[ ... ]' format
// 	FOR DEBUGGING
void
printVector(vector<int>& v)
{
	cout << "[ ";
	for(const auto& elem : v)
		cout << elem << " ";
	cout << "]" << endl;
}

/***************************************************/

// Sort given array using merge sort
size_t
mergeSort(vector<int>& v)
{	
	// Allocate temporary vector for storing sorted value
	vector<int> temp (v.size());
	// Call recursive method
	return mergeHelper(v, 0, v.size(), temp);
}

/***************************************************/

// Merge sort helper for recursion
size_t
mergeHelper(vector<int>& v, size_t begin, size_t end, vector<int>& temp)
{
	// Make sure there is more than 1 element to sort
	if(end - begin > 1)
	{
		// Calculate the middle of the vector
		size_t mid = begin + (end-begin) / 2;
		
		// Recursively split the range in half
		size_t compares = mergeHelper(v, begin, mid, temp) + mergeHelper(v, mid, end, temp);
		// Mere the 2 halves together
		return compares + merge(v, begin, mid, end, temp);
	}
	return 1;
}

/***************************************************/

// Merge parts of vector
size_t
merge(vector<int>& v, size_t begin, size_t mid, size_t end, vector<int>& temp)
{
	// Values for keeping track of indexes
	size_t indexA = begin;
	size_t indexB = mid;
	size_t compares = 0;
	size_t tempIndex = -1;
	while(true)
	{
		// If neither index has reached their end point then compare
		// 	the values at both indexes
		if(indexA != mid && indexB != end)
		{
			if(v[indexA] < v[indexB])
				temp[++tempIndex] = v[indexA++];
			else
				temp[++tempIndex] = v[indexB++];
			++compares;
		}
		// If indexB reached its end but indexA hasn't then push all the
		// 	values on that side into the temp vector
	       	else if(indexA != mid && indexB == end)
		{
			while(indexA != mid)
				temp[++tempIndex] = v[indexA++];
			break;
		}
		// If indexA reached its end but indexB hasn't then push all the
		// 	values on that side into the temp vector
		else if(indexA == mid && indexB != end)
		{
			while(indexB != end)
				temp[++tempIndex] = v[indexB++];
			break;
		}
	}

	// Start temp vector counter at -1 to allow pre-increment
	tempIndex = -1;
	// Take the sorted values from the temp vector and place
	// 	them back in the original vector
	for(size_t i = begin; i < end; ++i)
		v[i] = temp[++tempIndex];

	// Return the number of comparisons made
	return compares;
}

/***************************************************/

// Sort the given vector using a QuickSort algorithm
size_t
quickSort(vector<int>& v)
{
	// Call recursive method
	return quickHelper(v, 0, v.size());
}

/***************************************************/

// Helper function for quickSort to allow for recursion
size_t
quickHelper(vector<int>& v, size_t first, size_t last)
{
	// If there are less than 20 elements just perform an insertion sort
	if(first + 20 > last)
		return 1 + insertionSort(v, first, last);
	else if(last - first > 1)
	{
		size_t compares = 0;
		// Partiton the range
		size_t i = quickPartition(v, first, last, compares);
		// Recursively call method for each partition
		return compares + quickHelper(v, first, i) + quickHelper(v, i, last);;
	}
	return 1;
}

/***************************************************/

// Partition the range using median-of-three
size_t
quickPartition(vector<int>& v, size_t first, size_t last, size_t& compares)
{
	// Perform median-of-three
	compares += median3(v, first, last);
	// Grab and store the pivot
	int pivot = v[last-2];
	// Create variables for keeping track of the indexes
	size_t up = first, down = last-2;
	while(true)
	{
		// Position the left index
		do { ++compares; } while (v[++up] < pivot);
		// Position the right index
		do { ++compares; } while (v[--down] > pivot);
		// If the indexes crossed each other break out of the loop
		if (up >= down)
			break;
		// Otherwise swap the values at the indexes and continue
		swap(v[up], v[down]);
	}
	// Swap the pivot with the value at the point the indexes crossed
	swap(v[last-2], pivot);

	// Return the index of the pivot (now up)
	return up;
}

/***************************************************/

// Make sure that left, right, and center ((right-left) / 2) are in
// 	the correct order so that left < center < right.
// 	Return the number of comparisons done
size_t
median3(vector<int>& v, size_t left, size_t right)
{
	size_t center = left + (right - 1 - left) / 2;
	if (v[center] < v[left])
		swap(v[left], v[center]);
	if (v[right-1] < v[left])
		swap(v[left], v[right-1]);
	if (v[right-1] < v[center])
		swap(v[center], v[right-1]);
	
	// Position the pivot
	swap(v[center], v[right-2]);

	return 3;
}

/***************************************************/

// Sort the given vector from [begin, end) with an insertion sort
size_t
insertionSort(vector<int>& v, size_t begin, size_t end)
{
	size_t compares = 0;
	
	for (size_t i = begin; i < end; ++i)
	{
		int temp = v[i];
		size_t j = i;
		while (j >= begin)
		{
			++compares;
			if (temp < v[j-1])
				v[j] = v[j-1];
			else
				break;
			--j;
		}
		v[j] = temp;
	}

	return compares;
}

/***************************************************/

// Sort the vector using a shell sort
size_t
shellSort(vector<int>& v)
{
	size_t compares = 0;

	// Get initital h value
	size_t h;
	for(h = 1; h <= v.size() / 4; h = h*3 + 1);

	while (h > 0)
	{
		// Perform h sort
		for (size_t i = 0; i < v.size(); ++i)
		{
			int temp = v[i];
			size_t j = i;
			while (j >= h)
			{
				++compares;
				if (temp < v[j-h])
					v[j] = v[j-h];
				else
					break;
				j -= h;
			}
			v[j] = temp;
		}
		h /= 2;
	}

	return compares;
}

/***************************************************/

/* Time and comparison data for all sorting algorithms
 *
 * N:       20,000,000              40,000,000               80,000,000
 * ============================================================================
 * Merge    2260.24 (480201145)     4664.04 (1000413748)     9647.21  (2080813026)
 * Quick    1644.53 (583440297)     3428.76 (1207579628)     7138.64  (2504321829)
 * Shell    3940.21 (2260487131)    9329.10 (6647909551)     20042.70 (14522814196)
 * std      1502.61                 3113.40                  6532.27
 * 
 * Discussion:
 * ============================================================================
 * The sorting algorithms performed approximately as expected with quick sort
 * being the fastest compared to merge and shell sort. Across the board my
 * quick sort algorithm performed about as well as std::sort showing that
 * std::sort does use a quick sort behind the scenes. My merge sort did much better
 * than my shell sort in time and comparisons. This makes sense because merge
 * sort has a constant complexity of O(NlogN) while shell sorts complexity
 * is quite variable and is depended on the data and the h values used.
 * 
 */

/***************************************************/
