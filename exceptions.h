
#ifndef HW1_EXCEPTIONS_H
#define HW1_EXCEPTIONS_H
#include <memory>
#include <exception>


class AlreadyExist : public std::exception  {
public:
//    const char exception::what() const noexcept override;

};
class NotExist : public std::exception {
public:
//    const char *what() const noexcept override;

};
class InvalidInput : std::exception {
public:
//    const char *what() const noexcept override;

};
class AllocationError : std::exception {
public:
//    const char *what() const noexcept override;

};
class NotEnoughGroups: std::exception {
public:
//    const char *what() const noexcept override;

};
#endif //HW1_EXCEPTIONS_H
