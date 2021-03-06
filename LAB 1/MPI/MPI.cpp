// MPI.cpp : Defines the entry point for the console application.
//
#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>

#define SEND_DATA_TAG 0
#define END_SENDING_TAG 1

void GenerateData(int segments, int dataLenght, int* outSegmentLimits, int* outData)
{
	std::cout << "Generate limits: " << std::endl;

	outSegmentLimits[0] = 0;
	for (int i = 1; i <= segments; i++)
	{
		outSegmentLimits[i] = i;

		std::cout << outSegmentLimits[i] << ", ";
	}
	outSegmentLimits[segments + 1] = 0;

	std::cout << std::endl;
	std::cout << "Generate data: " << std::endl;

	for (int i = 0; i < dataLenght; i++)
	{
		outData[i] = rand() % segments;

		std::cout << outData[i] << ", ";
	}

	std::cout << std::endl;
	std::cout << "Data generated" << std::endl;
}

void initializeHistogram(int segments, int* outHistogram)
{
	for (int i = 0; i <= segments; i++)
	{
		outHistogram[i] = 0;
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	// Number of total threads
	int size;
	// Thread index - 0 is master thread. Last index is statistic thread. Rest threads are segmentation threads
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);		

	int amountOfSegments, dataLength;
	amountOfSegments = size - 2;
	dataLength = 50;

	std::cout << "rank " << rank << std::endl;

	if (amountOfSegments > 0)
	{
		int* segmentLimitsArray = nullptr;
		int* histogramArray = nullptr;
		int* dataArray = nullptr;

		int localLimit = 0;
		segmentLimitsArray = new int[size];

		if (rank == 0)								// Master thread (P)
		{
			std::cout << "rank p" << rank << std::endl;
			dataArray = new int[dataLength];

			GenerateData(amountOfSegments, dataLength, segmentLimitsArray, dataArray);

			std::cout << "print segments array" << std::endl;
			// Gives every thread a limit value
			for (int i = 0; i < size; i++)
			{
				std::cout << segmentLimitsArray[i] << ", ";
			}
			std::cout << std::endl;
		}

		MPI_Scatter(segmentLimitsArray, 1, MPI_INT, &localLimit, 1, MPI_INT, SEND_DATA_TAG, MPI_COMM_WORLD);

		if (rank == 0)
		{
			histogramArray = new int[amountOfSegments];

			// Sending every value to first SEG
			for (int i = 0; i < dataLength; i++)
			{
				MPI_Send(&dataArray[i], 1, MPI_INT, rank + 1, SEND_DATA_TAG, MPI_COMM_WORLD);
				std::cout << "Send " << dataArray[i] << " to rank " << rank + 1 << std::endl;
			}

			MPI_Send(&rank, 1, MPI_INT, rank + 1, END_SENDING_TAG, MPI_COMM_WORLD);
			std::cout << "Sended end" << std::endl;

			MPI_Recv(histogramArray, amountOfSegments, MPI_INT, size - 1, SEND_DATA_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			std::cout << "Print histogram: " << std::endl;
			for (int i = 0; i < amountOfSegments; i++)
			{
				std::cout << histogramArray[i] << ", ";
			}
			std::cout << std::endl;
			std::cout << "Histogram printed" << std::endl;
		}
		else if (rank != (amountOfSegments + 1))	// Segmentation thread (SEG[i])
		{
			int recievingData = 0;
			int matchingData = 0;
			int buf;
			MPI_Request recieveRequest;

			MPI_Irecv(&buf, 1, MPI_INT, rank - 1, END_SENDING_TAG, MPI_COMM_WORLD, &recieveRequest);

			while (!recievingData)
			{
				std::cout << "rank seg" << rank << std::endl;
				int currentValue = 0;

				MPI_Test(&recieveRequest, &recievingData, MPI_STATUS_IGNORE);

				if (!recievingData)
				{
					MPI_Recv(&currentValue, 1, MPI_INT, rank - 1, SEND_DATA_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					std::cout << "seg" << rank << " received " << currentValue << std::endl;

					if (currentValue < localLimit)
					{
						matchingData++;
					}
					else
					{
						MPI_Send(&currentValue, 1, MPI_INT, rank + 1, SEND_DATA_TAG, MPI_COMM_WORLD);
					}
				}
				else
				{
					std::cout << "end rank seg" << rank << std::endl;
					if (rank + 1 != amountOfSegments + 1)
					{
						MPI_Send(&rank, 1, MPI_INT, rank + 1, END_SENDING_TAG, MPI_COMM_WORLD);
					}
					MPI_Send(&matchingData, 1, MPI_INT, amountOfSegments + 1, SEND_DATA_TAG, MPI_COMM_WORLD);
					std::cout << "Sended end" << std::endl;
				}
			}
		}
		else										// Stat thread
		{
			std::cout << "rank stat" << rank << std::endl;
			bool bWatiForResults = true;
			int histogramValue = 0;
			histogramArray = new int[amountOfSegments];

			for (int i = 1; i < (size - 1); i++)
			{
				MPI_Recv(&histogramValue, 1, MPI_INT, i, SEND_DATA_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				std::cout << "rank stat " << rank << " recieve from " << i <<  std::endl;
				histogramArray[i - 1] = histogramValue;
			}

			std::cout << "Print histogram: " << std::endl;
			for (int i = 0; i < amountOfSegments; i++)
			{
				std::cout << histogramArray[i] << ", ";
			}
			std::cout << std::endl;
			std::cout << "Histogram printed" << std::endl;

			MPI_Send(histogramArray, amountOfSegments, MPI_INT, 0, SEND_DATA_TAG, MPI_COMM_WORLD);
		}

		if (rank == 0)								// Master thread (P)
		{
			std::cout << "Deleting" << std::endl;
			delete segmentLimitsArray;
			delete dataArray;
			delete histogramArray;
		}
		else if (rank == (amountOfSegments + 1))		// Stat thread
		{
			std::cout << "Deleting" << std::endl;
			delete histogramArray;
		}
	}
	else
	{
		if (rank == 0)
		{
			std::cout << "You need minimum 3 threads to run application" << std::endl;
		}
	}

	MPI_Finalize();
	return 0;
}

