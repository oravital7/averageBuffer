#define QUARTER 4

template <typename T>
class AverageBuffer
{
    T *buffer;
    int bufferSize, currIndex, upperIndex, lowerIndex, numForever;
    double BufferAvg, foreverAvg, upperAvg, lowerAvg;

public:
    AverageBuffer(int bufferSize) : buffer(new T[bufferSize]), bufferSize(bufferSize)
    {
        initializeGlobal();
    }

    ~AverageBuffer() { delete[] buffer; }

    double getAverage() const
    {
        return BufferAvg;
    }

    double getAverageForever() const
    {
        return foreverAvg;
    }

    double getUpperQuarterAverage() const
    {
        return upperAvg;
    }

    double getLowerQuarterAverage() const
    {
        return lowerAvg;
    }

    void addSample(const T &newVal)
    {
        updateLowerAverage();
        updateUpperAverage(newVal);
        updateAverages(newVal);

        buffer[currIndex] = newVal;
        circularIndex(currIndex);
    }

    void clear()
    {
        initializeGlobal();
        delete[] buffer; // The question require to clear the buffer, delete seems more effective
        buffer = new T[bufferSize];
    }

private:
    void updateUpperAverage(const T &newVal)
    {
        if (numForever < bufferSize)
        {
            if ((numForever + 1) % QUARTER == 0) // Need add new value to Upper Average
            {
                upperAvg = (upperAvg * (numForever / QUARTER) + newVal) / ((numForever + 1) / QUARTER);
            }
            else if ((numForever + 1) / QUARTER != 0) // Need add and replace old value by new value to Upper Average
            {
                upperAvg += (newVal - buffer[upperIndex]) / ((numForever + 1) / QUARTER);
                circularIndex(upperIndex);
            }
            else
            { // There is no quarter yet
                circularIndex(upperIndex);
            }
        }
        else // Need add and replace old value by new value to Upper Average
        {
            upperAvg += (newVal - buffer[upperIndex]) / (bufferSize / QUARTER);
            circularIndex(upperIndex);
        }
    }

    void updateLowerAverage()
    {
        if (numForever < bufferSize)
        {
            if ((numForever + 1) % QUARTER == 0) // Need add new value to Lower Average
            {
                lowerAvg = (lowerAvg * (numForever / QUARTER) + buffer[lowerIndex]) / ((numForever + 1) / QUARTER);
                circularIndex(lowerIndex);
            }
        }
        else // Need add and replace old value by new value to Lower Average
        {
            lowerAvg += (buffer[lowerIndex] - buffer[currIndex]) / (bufferSize / QUARTER);
            circularIndex(lowerIndex);
        }
    }

    // Update Buffer and forever averages
    void updateAverages(const T &newVal)
    {
        foreverAvg = (foreverAvg * numForever + newVal) / (numForever + 1);
        BufferAvg = numForever < bufferSize ? foreverAvg : BufferAvg + (newVal - buffer[currIndex]) / bufferSize;
        numForever++;
    }

    // Move the index in a circular way
    void circularIndex(int &index)
    {
        index = (index + 1) % bufferSize;
    }

    void initializeGlobal()
    {
        foreverAvg = BufferAvg = upperAvg = lowerAvg = 0;
        currIndex = upperIndex = lowerIndex = 0;
        numForever = 0;
    }
};