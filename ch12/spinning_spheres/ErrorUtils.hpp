#ifndef ERROR_UTILS_H_
#define ERROR_UTILS_H_

#include "Context.hpp"
 
void ck(vk::Result result);

void setupValidation(std::vector<const char*>& layerNames,
  std::vector<const char*>& extNames);
      
void setupReporting(Context& ctx, std::vector<const char*>& extNames);

void destroyReporting(Context& ctx);

#endif /* ERROR_UTILS_H_ */