#include <iostream>

template <typename T>
class AverageBuffer
{
    T *buffer;
    int bufferSize, currIndex, numForever; // The idea to keep the average and not the amount for overflow fear
    double avgBuffer, foreverAvg;

public:
    AverageBuffer(int bufferSize) : buffer(new T[bufferSize]), bufferSize(bufferSize)
    {
        foreverAvg = avgBuffer = currIndex = numForever = 0;
    }

    ~AverageBuffer() { delete[] buffer; }

    double getAverage()
    {
        return avgBuffer;
    }

    double getAverageForever()
    {
        return foreverAvg;
    }

    void addSample(T number)
    {
        updateAverages(buffer[currIndex], number);
        buffer[currIndex++] = number;
        currIndex %= bufferSize;
        for(int i=0; i< bufferSize; i++)
          std::cout <<  buffer[i] << " , ";

          std::cout << std::endl;
    }

private:
    void updateAverages(T oldVal, T newVal)
    {
        if (numForever < bufferSize) // Calculate first values only 
        {
            foreverAvg = avgBuffer = (avgBuffer * numForever + newVal) / (numForever + 1);
        } else  {
            avgBuffer += (newVal - oldVal) / bufferSize;
            foreverAvg = (foreverAvg * numForever + newVal) / (numForever + 1);
        }
        numForever++;
    }
};