#include <vector>
#include "include.h"
extern "C"
void top(const data_t *in1, const data_t *in2,  data_t *out, int n)
{
#pragma HLS INTERFACE m_axi offset=slave depth=1000 port=in2 bundle=gmem1
#pragma HLS INTERFACE m_axi offset=slave depth=1000 port=in1 bundle=gmem0
#pragma HLS INTERFACE m_axi offset=slave depth=1000 port=out bundle=gmem2
 for(int i=0;i<n;i++)
 {
#pragma HLS DATAFLOW
   // define Temporary storage
   data_t in1_buf[N];
   data_t in2_buf[N];
   data_t res_buf[N];

   load_data(in1,in1_buf,i);
   load_data(in2,in2_buf,i);
   compute(in1_buf,in2_buf,res_buf);
   store_data(res_buf,out,i);

 }
}


