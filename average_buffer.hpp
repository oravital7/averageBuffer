#include <iostream>

#define QUARTER 4

template <typename T>
class AverageBuffer
{
    T *buffer;
    int bufferSize, currIndex, upperIndex, lowerIndex, numForever; // The idea to keep the average and not the whole amount prevent overflow
    double BufferAvg, foreverAvg, upperAvg, lowerAvg;

public:
    AverageBuffer(int bufferSize) : buffer(new T[bufferSize]), bufferSize(bufferSize)
    {
        foreverAvg = BufferAvg = currIndex = numForever = 0;
        upperAvg = lowerAvg = 0;
        upperIndex = lowerIndex = 0;
        // Init method
    }

    ~AverageBuffer() { delete[] buffer; }

    double getAverage()
    {
        return BufferAvg;
    }

    double getAverageForever()
    {
        return foreverAvg;
    }

    double getUpperQuarterAverage() {
        return upperAvg;
    }
    
    double getLowerQuarterAverage() {
        return lowerAvg;
    }

    void addSample(T number)
    {
        updateLowerAverage();
        updateUpperAverage(number);
        updateAverages(number);
 
        buffer[currIndex] = number;
        circleIndex(currIndex);
        for(int i=0; i< bufferSize; i++)
          std::cout <<  buffer[i] << " , ";

          std::cout << std::endl;
    }
// Try change ifs
private:
    void updateUpperAverage(T newVal) {
        if(numForever < bufferSize){
            if((numForever+1) % QUARTER == 0) {  // We need to add new value to avearge
                upperAvg =  (upperAvg * (numForever / QUARTER) + newVal) / ((numForever + 1) / QUARTER);
            } else if((numForever+1) /  QUARTER != 0) {  
              upperAvg += (newVal - buffer[upperIndex]) / ((numForever+1) /  QUARTER);
                circleIndex(upperIndex);
            }
            else { // stay on 0 while there is no quarter yet
                circleIndex(upperIndex);
            }

        } else {
            upperAvg += (newVal - buffer[upperIndex]) / (bufferSize /  QUARTER);
            circleIndex(upperIndex);
        }

    }

    void updateLowerAverage() {
         if(numForever < bufferSize){
            if((numForever+1) % QUARTER == 0) {
                lowerAvg =  (lowerAvg * (numForever / QUARTER) + buffer[lowerIndex]) / ((numForever + 1) / QUARTER);
                circleIndex(lowerIndex);
            }

        } else {
            std::cout << "yes" << std::endl;
            lowerAvg += (buffer[lowerIndex] - buffer[currIndex]) / (bufferSize /  QUARTER);
            circleIndex(lowerIndex);
        }


    }


    void updateAverages(T newVal)
    {
        if (numForever < bufferSize) // Calculate first values only 
        {
            foreverAvg = BufferAvg = (foreverAvg * numForever + newVal) / (numForever + 1);
        } else  {
            BufferAvg += (newVal - buffer[currIndex]) / bufferSize;
            foreverAvg = (foreverAvg * numForever + newVal) / (numForever + 1);
        }
        numForever++;
    }

    void circleIndex(int &index) {
        index = (index+1) % bufferSize;
    }

    double nextAverage(T newVal) {
        return (foreverAvg * numForever + newVal) / (numForever + 1);
    }
};