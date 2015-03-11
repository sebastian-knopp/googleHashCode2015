#pragma once

#include <stdexcept>
#include <string>


namespace base {


/**
 * @brief Base class for all exceptions in this namespace.
 */
struct Exception : public std::runtime_error
{

public:

    explicit Exception(const char* const a_message);

    explicit Exception(const std::string& a_message);


public:

    /**
     * @brief Raises an exception of the given type and logs
     *        the exception as well as the current stacktrace.
     */
    template<typename ExceptionT = Exception, typename... Arguments>
    static void raise(Arguments... a_arguments)
    {
        ExceptionT e(a_arguments...);
        logException(e);
        throw e;
    }


private:

    //! Log exception to base logger
    static void logException(const std::exception& a_exception);

};


} // namespace base



