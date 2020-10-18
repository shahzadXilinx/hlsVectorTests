#include <vector>
#include <iostream>
#include <hls_vector.h>
#include <cmath>
#include "types.hpp"



extern "C"
void kernel(t_vecType*res, const t_vecType *lhs, const t_vecType *rhs, int n);

int main(int, char**) {
  t_vecType *lhs = new t_vecType[DATA_SIZE];
  t_vecType *rhs = new t_vecType[DATA_SIZE];
  t_vecType *res = new t_vecType[DATA_SIZE];
  t_vecType *sw_res= new t_vecType[DATA_SIZE];

  t_vecType temp = {3.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1};
 
  for (int i = 0; i < DATA_SIZE; ++i)
  {
    lhs[i]                   = temp;
    rhs[i]                   = temp;
  }

  for (int i = 0; i < DATA_SIZE; ++i)
  {
    for (int j = 0; j < vecSize; ++j)
    {
      sw_res[i][j] = lhs[i][j] + rhs[i][j];
    }
  }

  kernel(res, lhs, rhs, ITERS);

  t_elem margin = MarginType<t_elem>::allowedMargin;
  
  //verify results
  for (int i = 0; i < DATA_SIZE; ++i)
  {
    for (int j = 0; j < vecSize; ++j)
    {
      t_elem diff = std::abs(sw_res[i][j] - res[i][j]);
      //std::cout<<"i,j= " << i<<":"<<j<<std::endl;

      if(diff > margin)
      {
        std::cout << "Failed" << std::endl;
        std::cout<<"Diff =" <<diff<<std::endl;
        std::cout<<"i,j= " << i<<":"<<j<<std::endl;
        std::cout<<"sw_res[i][j] = " <<sw_res[i][j]<<std::endl;
        std::cout<<"res[i][j] = " <<res[i][j]<<std::endl;
        return -1;
      }
    }
  }

  std::cout << "PASSED" << std::endl;

  return 0;
}
