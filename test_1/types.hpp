#pragma once

#define BATCH_SIZE 16
#define ITERS 4
#define DATA_SIZE BATCH_SIZE *  ITERS
#define  vecSize  8

using t_elem= float;
using t_vecType = hls::vector<t_elem,vecSize>;

template <class t_elem>
struct MarginType
{
  static constexpr t_elem allowedMargin= 0;
};

template <>
struct MarginType<float>
{
  static constexpr t_elem allowedMargin = 1e-3;
};

template <>
struct MarginType<double>
{
  static constexpr t_elem allowedMargin = 1e-6;
};

