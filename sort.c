#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t *ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void *ptr)
{
	size_t *pSz = (size_t *)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t *)ptr - 1);
}

size_t Size(void *ptr)
{
	return ((size_t *)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated

void printArray(int pData[], int dataSz);
void merge(int pData[], int *L, int *R, int n1, int n2, int l);



void mergeSort(int pData[], int l, int r)
{
	int m = (l+r)/2;
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	
	if(r<=l)
	{
		return;
	}

	
	/* create temp arrays */
	int *L = (int *)Alloc(n1 * sizeof(int));
	int *R = (int *)Alloc(n2 * sizeof(int));
	
	
	

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
	{
		L[i] = pData[l + i];
	}

	//printArray(L, n1);
		
	for (j = 0; j < n2; j++)
	{
		R[j] = pData[m + 1 + j];
	}

	//printArray(R, n2);
		
	mergeSort(L, 0, n1-1);
	mergeSort(R, 0, n2-1);
	
	merge(pData, L, R, n1, n2, l);
	
}
/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void merge(int pData[], int *L, int *R, int n1, int n2, int l)
{
	
	int i = 0; // Initial index of first subarray
	int j = 0; // Initial index of second subarray
	int k = l; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			pData[k] = L[i];
			i++;
		}
		else
		{
			pData[k] = R[j];
			j++;
		}
		k++;
	}
	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1)
	{
		pData[k] = L[i];
		i++;
		k++;
	}
	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2)
	{
		pData[k] = R[j];
		j++;
		k++;
	}
	DeAlloc(L);
	DeAlloc(R);
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE *inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d ", &n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
int firstHundred = (dataSz < 100 ? dataSz : 100);
printf("\tData:\n\t");
for (i=0;i<firstHundred;++i)
{
printf("%d ",pData[i]);
}
printf("\n\t");
for (i=sz;i<dataSz;++i)
{
printf("%d ",pData[i]);
}
printf("\n\n");
}



int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char *fileNames[] = {"input1.txt", "input2.txt", "input3.txt", "input4.txt"};


	// change 3 back to 4 when finished
	for (i = 0; i < 4; ++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int *)malloc(sizeof(int) * dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n", dataSz);
		printf("---------------------------\n");

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		free(pDataCopy);
		free(pDataSrc);
	}
}