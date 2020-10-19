#include <vector>
#include <iostream>
#include <hls_vector.h>
#include <cmath>
#include <ctime>
#include "types.hpp"

extern "C"
void kernel(t_vecType*res, const t_vecType *lhs, const t_vecType *rhs, int n);

int main(int, char**)
{
      t_vecType *lhs = new t_vecType[DATA_SIZE];
      t_vecType *rhs = new t_vecType[DATA_SIZE];
      t_vecType *sw_res= new t_vecType[DATA_SIZE];
      t_vecType *res = new t_vecType[DATA_SIZE];

      srand(time(NULL));
      t_elem scale = 10;
      for (int i = 0; i < DATA_SIZE; ++i)
      {
        for (int j = 0; j < vecSize; ++j)
        {
            lhs[i][j] = scale * static_cast<t_elem>(rand())/RAND_MAX;
            rhs[i][j] = scale * static_cast<t_elem>(rand())/RAND_MAX;
        }
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
          if(diff > margin)
            {
            std::cout << "\n===========\nTest FAILED !!! \n==========\n" << std::endl;
            std::cout<<"Difference Between Expected and Produced Output =" <<diff<<std::endl;
            std::cout<<"Output Index i,j= " << i<<":"<<j<<std::endl;
            std::cout<<"Expected Result sw_res[i][j] = " <<sw_res[i][j]<<std::endl;
            std::cout<<"Produced Result res[i][j] = " <<res[i][j]<<std::endl;
            return -1;
            }
        }
      }

      std::cout << "\n===========\nTest PASSED !!! \n==========\n" << std::endl;

      return 0;
}
