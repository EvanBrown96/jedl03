class Averager{
  /**
   * simple class for doing rolling averages
   */

private:

  int* read_vals;
  int size, count, offset;
  int32_t sum = 0;

public:

  Averager(int size): size(size), count(0), offset(0), read_vals(new int[size]){}

  void update(int val){
    if(count < size) count++;
    else sum -= read_vals[offset];
    read_vals[offset] = val;
    sum += val;
    offset = (offset+1)%size;
  }

  int get(){ return static_cast<int>(sum/count); }

  int getCount(){ return count; }

  void clear(){ count = 0; sum = 0; };

  ~Averager(){ delete[] read_vals; }

};
