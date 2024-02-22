#ifndef ERROR_UTILS_H_
#define ERROR_UTILS_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "Context.hpp"
 
template<typename T>
T ck(vk::ResultValueType<T> result);

void setupValidation(std::vector<const char*>& layerNames,
  std::vector<const char*>& extNames);
      
void setupReporting(Context& ctx, std::vector<const char*>& extNames);

void destroyReporting(Context& ctx);

#endif /* ERROR_UTILS_H_ */