
#ifndef HW1_EXCEPTIONS_H
#define HW1_EXCEPTIONS_H
#include <memory>

class AlreadyExist : public std::exception{};
class NotExist : public std::exception{};
class InvalidInput : public std::exception{};
class AllocationError : public std::exception{};
#endif //HW1_EXCEPTIONS_H
