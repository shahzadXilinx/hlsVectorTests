#include "hls_vector.h"
#include "ap_int.h"
#include "hls_half.h"
typedef hls::vector<ap_fixed<32, 8>,4> data_t;

template<int N, typename T>
void gt(const T (&data_in1)[N], const T (&data_in2)[N], T (&data_out)[N])
{
const float C = 1;
hls::vector<bool,4> temp[N];
hls::vector<bool,4> zero = {true,true,true,true};

#pragma HLS inline off
  for(int i=0;i<N;i++) {
#pragma HLS pipeline II=1
	  temp[i] = data_in1[i] <= data_in2[i];
    data_out[i] = (temp[i] == zero ? data_in1[i] : data_in2[i]);
  }
}


template<int N, typename T>
void load_data(const T *data_in, T (&data_out)[N], int n)
{
#pragma HLS inline off
  for(int i=0;i<N;i++) {
#pragma HLS pipeline II=1
	data_out[i] = data_in[i + N*n];

  }
}

template<int N, typename T>
void store_data(const T (&data_in)[N], T *data_out, int n)
{
#pragma HLS inline off
  for(int i=0;i<N;i++) {
#pragma HLS pipeline II=1
	data_out[i + N*n] = data_in[i];
  }
}

template<int N, typename T>
void compute(const T (&data_in1)[N], const T (&data_in2)[N], T (&data_out)[N])
{
 gt(data_in1,data_in2,data_out);
}

#define N 1 

extern "C"
void top(const data_t *in1, const data_t *in2, data_t *out, int n);

