#pragma once

#include <functional>

// Generic publisher interface
template <class T>
class Publisher {
public:
  virtual int subscribe(std::function<void(T)> callback) = 0;
  virtual void unsubscribe(int subscriberId) = 0;
};
