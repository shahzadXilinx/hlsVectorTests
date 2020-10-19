#include <ap_int.h>
#include <hls_vector.h>
#include "types.hpp"
template<int N, typename T>
void load(T out[N], const T *in, int o) {
#pragma HLS INLINE off
  for (int i = 0; i < N; ++i) {
#pragma HLS pipeline
    out[i] = in[i+N*o];
  }
}

template<int N, typename T>
void store(T *out, const T in[N] , int o) {
#pragma HLS INLINE off
  for (int i = 0; i < N; ++i) {
#pragma HLS pipeline
    out[i+o*N] = in[i];
  }
}

template<int N, typename T, typename S>
void compute(T res[N], const S lhs[N], const S rhs[N]) {
#pragma HLS INLINE off
  for (int i = 0; i < N; ++i) {
#pragma HLS pipeline
    res[i] = lhs[i] + rhs[i];
  }
}


extern "C"
void kernel(t_vecType* res, const t_vecType *lhs, const t_vecType *rhs, int n) {
#pragma HLS INTERFACE m_axi port=lhs offset=slave bundle=gmem0 depth=64
#pragma HLS INTERFACE m_axi port=rhs offset=slave bundle=gmem1 depth=64
#pragma HLS INTERFACE m_axi port=res offset=slave bundle=gmem2 depth=64

  for(int i = 0; i < n; ++i) {
    t_vecType lhs_buf[BATCH_SIZE];
    t_vecType rhs_buf[BATCH_SIZE];
    t_vecType res_buf[BATCH_SIZE];

#pragma HLS DATAFLOW
    load<BATCH_SIZE>(lhs_buf, lhs, i);
    load<BATCH_SIZE>(rhs_buf, rhs, i);
    compute<BATCH_SIZE>(res_buf, lhs_buf, rhs_buf);
    store<BATCH_SIZE>(res, res_buf, i);
  }
}
