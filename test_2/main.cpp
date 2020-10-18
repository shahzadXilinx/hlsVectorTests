#include <vector>
#include <iostream>
#include "include.h"

int main()
{
 std::vector<data_t> in1(1000);
 std::vector<data_t> in2(1000);
 std::vector<data_t> in3(1000);
 std::vector<data_t> out(1000);
 std::vector<data_t> cmp(1000);

 const data_t C = {1,1,1,1};

const int n = 4;
std::vector<hls::vector<bool,n> > temp(1000);
/*
 for(int i=0;i<n;i++)
 {
  for(int j=0;j<8;j++) {
   temp[i][j] = 0;
  }
 }
 */
 for(int i=0;i<n;i++)
 {
 for(int j=0;j<4;j++) {
  short x = 200;
  in1[i][j] = x + j*1;
  in2[i][j] =  x + j*2;
  in3[i][j] =  j*4;
  temp[i][j] = in1[i][j] <= in2[i][j];
  cmp[i][j] = temp[i][j] == 1 ? in1[i][j] : in2[i][j];
  }
 }


 top(in1.data(),in2.data(),out.data(),n);

 int fail = 0;


 for(int i=0;i<n;i++)
 {
  for(int j=0;j<4;j++) {
  if(out[i][j] != cmp[i][j])
  {
    fail = 1;
    std::cout << "Mismatch Exp val " << cmp[i][j] << " Actual " << out[i][j]  << "\n";
  }
  else
  {
    std::cout << "Exp val " << cmp[i][j] << " Actual " << out[i][j]  << "\n";
    
  }
  } 
 }

return fail;
}
