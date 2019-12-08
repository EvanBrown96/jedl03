/**
 * averager.h
 * a simple class for doing rolling averages
 */

class Averager{
private:
  int* read_vals;
  int size, count, offset;
  int32_t sum = 0;
public:
  Averager(int size): size(size), count(0), offset(0), read_vals(new int[size]){
    /**
     * initializes averager with given size and no stored elements
     */
  }

  void update(int val){
    /**
     * updates the average with a new value
     * @param val the new value
     */
    if(count < size) count++;
    else sum -= read_vals[offset];
    read_vals[offset] = val;
    sum += val;
    offset = (offset+1)%size;
  }

  int get() {
    /**
     * get the average
     * @return the average
     */
    return static_cast<int>(sum/count);
  }

  int getCount() {
    /**
     * get the number of samples being average
     * @return the number of samples in the average
     */
    return count;
  }

  void clear() {
    /**
     * clear the average
     */
    count = 0; sum = 0;
  }

  ~Averager() {
    /**
     * safely destroy averager instance
     */
    delete[] read_vals;
  }
};
