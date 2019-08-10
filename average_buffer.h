#define QUARTER 4


/*
The main idea is maintain correct averages at each entry of a new number, so no loops are needed,
therefore by minor Mathematical Calculations the performance and complexity are significantly better
for any size of buffer.
*/
template <typename T>
class AverageBuffer {
    T *buffer;
    int bufferSize, currentIndex, upperIndex, lowerIndex, numForever; 
    double bufferAvg, foreverAvg, upperAvg, lowerAvg; // I chose to keep the averages in double type to fit all types of Buffers


public:
    AverageBuffer(const int& bufferSize) : buffer(new T[bufferSize]), bufferSize(bufferSize) {
        initializeGlobal();
    }

    ~AverageBuffer() { delete[] buffer; }

    double getAverage() const { return bufferAvg; }

    double getAverageForever() const { return foreverAvg; }

    double getUpperQuarterAverage() const { return upperAvg; }

    double getLowerQuarterAverage() const { return lowerAvg; }

    // When a new number entered, we will update the averages accordingly
    void addSample(const T &newVal) {
        updateLowerAverage();
        updateUpperAverage(newVal);
        updateAverages(newVal);

        buffer[currentIndex] = newVal;   // Copy the new value to the right place in Buffer
        circularIndex(currentIndex);
    }

    void clear() {
        initializeGlobal();
        delete[] buffer; // The question require to clear the buffer, delete action seems be more effective
        buffer = new T[bufferSize];
    }

private:
    void updateLowerAverage() {
        if (numForever < bufferSize) {
            if ((numForever + 1) % QUARTER == 0) { // The quarter is growing, Need add new value to Lower Average
                lowerAvg = (lowerAvg * (numForever / QUARTER) + buffer[lowerIndex]) / ((numForever + 1) / QUARTER);
                circularIndex(lowerIndex);
            }
        } 
        else { // Need add and replace old value by new value to Lower Average
            lowerAvg += (buffer[lowerIndex] - buffer[currentIndex]) / (bufferSize / QUARTER);
            circularIndex(lowerIndex);
        }
    }

    void updateUpperAverage(const T &newVal) {
        if (numForever < bufferSize) {
            if ((numForever + 1) % QUARTER == 0) {   // The quarter is growing, Need add new value to Upper Average
                upperAvg = (upperAvg * (numForever / QUARTER) + newVal) / ((numForever + 1) / QUARTER);
            }
            else if ((numForever + 1) / QUARTER != 0) {   // Need add and replace old value by new value to Upper Average
                upperAvg += (newVal - buffer[upperIndex]) / ((numForever + 1) / QUARTER);
                circularIndex(upperIndex);
            }
            else {  // There is no quarter yet
                circularIndex(upperIndex);
            }
        }
        else {  // Need add and replace old value by new value to Upper Average
            upperAvg += (newVal - buffer[upperIndex]) / (bufferSize / QUARTER);
            circularIndex(upperIndex);
        }
    }

    // Update Buffer and forever averages
    void updateAverages(const T &newVal) {
        foreverAvg = (foreverAvg * numForever + newVal) / (numForever + 1);
        bufferAvg = numForever < bufferSize ? foreverAvg : bufferAvg + (newVal - buffer[currentIndex]) / bufferSize;
        numForever++;
    }

    // Move the index forward in a circular way
    void circularIndex(int &index) {
        index = (index + 1) % bufferSize;
    }

    void initializeGlobal() {
        foreverAvg = bufferAvg = upperAvg = lowerAvg = 0;
        currentIndex = upperIndex = lowerIndex = 0;
        numForever = 0;
    }
};