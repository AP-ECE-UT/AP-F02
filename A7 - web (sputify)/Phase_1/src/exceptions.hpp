#ifndef EXCEPTIONS_HPP_INCLUDE
#define EXCEPTIONS_HPP_INCLUDE

#include <stdexcept>

#include "consts.hpp"

namespace exceptions {

class NotFound : public std::exception {
public:
    virtual char const* what() const throw() {
        return consts::responses::NOT_FOUND.c_str();
    }
};

class BadRequest : public std::exception {
public:
    virtual char const* what() const throw() {
        return consts::responses::BAD_REQUEST.c_str();
    }
};

class PermissionDenied : public std::exception {
public:
    virtual char const* what() const throw() {
        return consts::responses::PERMISSION_DENIED.c_str();
    }
};

class Empty : public std::exception {
public:
    virtual char const* what() const throw() {
        return consts::responses::EMPTY.c_str();
    }
};

} // namespace exceptions

#endif // EXCEPTIONS_HPP_INCLUDE
