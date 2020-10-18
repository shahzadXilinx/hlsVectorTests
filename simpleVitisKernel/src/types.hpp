#include <hls_vector.h>

using element_type= unsigned int;
constexpr auto elemSize = sizeof(element_type);
constexpr auto memWidth =  512/8;
constexpr auto elementsPerVec = memWidth / elemSize;
using uintVec = hls::vector<element_type,elementsPerVec>;

