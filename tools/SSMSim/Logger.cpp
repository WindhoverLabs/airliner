/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#include "Logger.h"

unsigned int Logger::referenceCount = 0;
LoggerInstance *Logger::instance = 0;



Logger::Logger()
{
    if(Logger::referenceCount == 0)
    {
        /* No instances have been created yet.  Create one. */
        Logger::instance = new LoggerInstance();

        /* Default verbosity to maximum */
        Logger::instance->setVerbosity(LOGGER_VERBOSITY_DEBUG);
    }

    /* Always increment the reference count so we know when to delete
     * the instance.
     */
    Logger::referenceCount++;
}



Logger::Logger(LoggerVerbosity_t verbosity) :
        Logger()
{
}



Logger::Logger(int verbosity) :
        Logger()
{
    Logger::instance->setVerbosity((LoggerVerbosity_t)verbosity);
}



bool Logger::setLogFile(const char *fileName)
{
    bool rc = false;

    if(Logger::instance != 0)
    {
        rc = Logger::instance->openLogFile(fileName);
    }

    return rc;
}



void Logger::logError(const char *spec, ...)
{
    if(Logger::instance != 0)
    {
        std::string result;

        va_list args, args_copy;

        va_start(args, spec);
        va_copy(args_copy, args);

        int len = vsnprintf(nullptr, 0, spec, args);
        if (len < 0)
        {
            va_end(args_copy);
            va_end(args);
            throw std::runtime_error("vsnprintf error");
        }

        if (len > 0)
        {
            result.resize(len);
            // note: &result[0] is *guaranteed* only in C++11 and later
            // to point to a buffer of contiguous memory with room for a
            // null-terminator, but this "works" in earlier versions
            // in *most* common implementations as well...
            vsnprintf(&result[0], len+1, spec, args_copy); // or result.data() in C++17 and later...
        }

        va_end(args_copy);
        va_end(args);

        Logger::instance->LogEvent(LOGGER_EVENT_ERROR, result);
    }
}



void Logger::logWarning(const char *spec, ...)
{
    if(Logger::instance != 0)
    {
        std::string result;

        va_list args, args_copy;

        va_start(args, spec);
        va_copy(args_copy, args);

        int len = vsnprintf(nullptr, 0, spec, args);
        if (len < 0)
        {
            va_end(args_copy);
            va_end(args);
            throw std::runtime_error("vsnprintf error");
        }

        if (len > 0)
        {
            result.resize(len);
            // note: &result[0] is *guaranteed* only in C++11 and later
            // to point to a buffer of contiguous memory with room for a
            // null-terminator, but this "works" in earlier versions
            // in *most* common implementations as well...
            vsnprintf(&result[0], len+1, spec, args_copy); // or result.data() in C++17 and later...
        }

        va_end(args_copy);
        va_end(args);

        Logger::instance->LogEvent(LOGGER_EVENT_WARNING, result);
    }
}



void Logger::logInfo(const char *spec, ...)
{
    if(Logger::instance != 0)
    {
        std::string result;

        va_list args, args_copy;

        va_start(args, spec);
        va_copy(args_copy, args);

        int len = vsnprintf(nullptr, 0, spec, args);
        if (len < 0)
        {
            va_end(args_copy);
            va_end(args);
            throw std::runtime_error("vsnprintf error");
        }

        if (len > 0)
        {
            result.resize(len);
            // note: &result[0] is *guaranteed* only in C++11 and later
            // to point to a buffer of contiguous memory with room for a
            // null-terminator, but this "works" in earlier versions
            // in *most* common implementations as well...
            vsnprintf(&result[0], len+1, spec, args_copy); // or result.data() in C++17 and later...
        }

        va_end(args_copy);
        va_end(args);

        Logger::instance->LogEvent(LOGGER_EVENT_INFO, result);
    }
}



void Logger::logDebug(const char *spec, ...)
{
    if(Logger::instance != 0)
    {
        std::string result;

        va_list args, args_copy;

        va_start(args, spec);
        va_copy(args_copy, args);

        int len = vsnprintf(nullptr, 0, spec, args);
        if (len < 0)
        {
            va_end(args_copy);
            va_end(args);
            throw std::runtime_error("vsnprintf error");
        }

        if (len > 0)
        {
            result.resize(len);
            // note: &result[0] is *guaranteed* only in C++11 and later
            // to point to a buffer of contiguous memory with room for a
            // null-terminator, but this "works" in earlier versions
            // in *most* common implementations as well...
            vsnprintf(&result[0], len+1, spec, args_copy); // or result.data() in C++17 and later...
        }

        va_end(args_copy);
        va_end(args);

        Logger::instance->LogEvent(LOGGER_EVENT_DEBUG, result);
    }
}



Logger::~Logger()
{
    /* Always decrement the reference count so we know when to delete
     * the instance.
     */
    Logger::referenceCount--;

    if(Logger::referenceCount == 0)
    {
        /* This is the last reference.  Delete the instance. */
        delete Logger::instance;
        Logger::instance = 0;
    }
}
