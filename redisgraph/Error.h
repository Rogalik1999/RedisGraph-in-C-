#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>
#include <typeinfo>

class Error : public std::exception{

    std::string _msg;

public:
    explicit Error(const std::string &msg) : _msg(msg) {}

    Error(const Error &) = default;
    Error& operator=(const Error &) = default;

    Error(Error &&) = default;
    Error& operator=(Error &&) = default;


    virtual ~Error() override = default;

    virtual const char* what() const noexcept override {
        return _msg.data();
    }
};

class SocketError : public Error {
public:
    explicit SocketError(const std::string &msg) : Error(msg) {}

    SocketError(const SocketError &) = default;
    SocketError& operator=(const SocketError &) = default;

    SocketError(SocketError &&) = default;
    SocketError& operator=(SocketError &&) = default;

    virtual ~SocketError() override = default;
};

class ResponseError : public Error{
public:
    explicit ResponseError(const std::string &msg): Error(msg){}

    ResponseError(const ResponseError &) = default;
    ResponseError& operator=(const ResponseError &) = default;

    ResponseError(ResponseError &&) = default;
    ResponseError& operator=(ResponseError &&) = default;

    virtual ~ResponseError() override = default;
};

class BadCast : public std::bad_cast{

public:
    const char* what() const noexcept override
    {
        return "bad redis value cast";
    }

};

class OutOfRangeProperties : public std::out_of_range{

public:
    explicit OutOfRangeProperties(const std::string &msg): std::out_of_range(msg){}

    virtual ~OutOfRangeProperties() override = default;

};

#endif // ERROR_H
