#define QUARTER 4

class InvalidSizeException {};

/*
The main idea is maintain correct averages at each entry of a new number, so no loops are needed,
therefore by minor Mathematical Calculations the performance and complexity are significantly better
for any size of buffer.
*/
template <typename T>
class AverageBuffer
{
    T *buffer;
    int bufferSize, currentIndex, upperIndex, lowerIndex, numForever; // numForever - count the samples since the buffer run
    double bufferAvg, foreverAvg, upperAvg, lowerAvg;                 // I chose to keep the averages in double type to fit all types of Buffers

public:
    AverageBuffer(const int &bufferSize) : bufferSize(bufferSize)
    {
        if (bufferSize < 1)
            throw InvalidSizeException();
        initializeGlobal();
    }

    ~AverageBuffer() { delete[] buffer; }

    double getAverage() const { return bufferAvg; }

    double getAverageForever() const { return foreverAvg; }

    double getUpperQuarterAverage() const { return upperAvg; }

    double getLowerQuarterAverage() const { return lowerAvg; }

    // When a new number entered, we will update the averages accordingly
    void addSample(const T &newVal)
    {
        updateLowerAverage();
        updateUpperAverage(newVal);
        updateAverages(newVal);

        buffer[currentIndex] = newVal; // Copy the new value to the right place in Buffer
        circularIndex(currentIndex);
    }

    void clear()
    {
        delete[] buffer; // The question require to clear the buffer, delete action seems be more effective
        initializeGlobal();
    }

private:
    void updateLowerAverage()
    {
        int currentQuarter = numForever < bufferSize ? (numForever + 1) / QUARTER : bufferSize / QUARTER;

        if (numForever < bufferSize)
        {
            if ((numForever + 1) % QUARTER == 0) // The quarter is growing, Need add new value to Lower Average
            {
                lowerAvg = (lowerAvg * (numForever / QUARTER) + buffer[lowerIndex]) / currentQuarter;
                circularIndex(lowerIndex);
            }
        }
        else if (currentQuarter != 0) // Need add and replace old value by new value to Lower Average
        {
            lowerAvg += (buffer[lowerIndex] - buffer[currentIndex]) / currentQuarter;
            circularIndex(lowerIndex);
        }
    }

    void updateUpperAverage(const T &newVal)
    {
        int currentQuarter = numForever < bufferSize ? (numForever + 1) / QUARTER : bufferSize / QUARTER;

        if (numForever < bufferSize && ((numForever + 1) % QUARTER == 0)) // The quarter is growing, Need add new value to Upper Average
        {
            upperAvg = (upperAvg * (numForever / QUARTER) + newVal) / currentQuarter;
            return; // There is no need to move the index
        }
        else if (currentQuarter != 0) // Need add and replace old value by new value to Upper Average except that case there is no quarter at all
        {
            upperAvg += (newVal - buffer[upperIndex]) / currentQuarter;
        }
        circularIndex(upperIndex);
    }

    // Update Buffer and forever averages
    void updateAverages(const T &newVal)
    {
        foreverAvg = (foreverAvg * numForever + newVal) / (numForever + 1);
        bufferAvg = numForever < bufferSize ? foreverAvg : bufferAvg + (newVal - buffer[currentIndex]) / bufferSize;
        numForever++;
    }

    // Move the index forward in a circular way
    void circularIndex(int &index)
    {
        index = (index + 1) % bufferSize;
    }

    void initializeGlobal()
    {
        buffer = new T[bufferSize];
        foreverAvg = bufferAvg = upperAvg = lowerAvg = 0;
        currentIndex = upperIndex = lowerIndex = 0;
        numForever = 0;
    }
};