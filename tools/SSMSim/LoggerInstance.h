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

#ifndef LOGGER_INSTANCE_H_
#define LOGGER_INSTANCE_H_

#include <fstream>
#include <string>
#include <iostream>


typedef enum {
  LOGGER_VERBOSITY_SILENT   = 0,
  LOGGER_VERBOSITY_ERRORS   = 1,
  LOGGER_VERBOSITY_WARNINGS = 2,
  LOGGER_VERBOSITY_INFO     = 3,
  LOGGER_VERBOSITY_DEBUG    = 4
} LoggerVerbosity_t;


typedef enum {
  LOGGER_EVENT_ERROR    = 0,
  LOGGER_EVENT_WARNING  = 1,
  LOGGER_EVENT_INFO     = 2,
  LOGGER_EVENT_DEBUG    = 3
} LoggerCriticality_t;

class LoggerInstance
{
public:
    LoggerInstance();
    ~LoggerInstance();
    void setVerbosity(LoggerVerbosity_t verbosity);
    void LogEvent(LoggerCriticality_t criticality, const std::string &message);

private:
    LoggerInstance(LoggerInstance const &){}; // copy constructor is private
    LoggerInstance &operator=(LoggerInstance const &)
    {
        return *this;
    }; // assignment operator is private
    static LoggerInstance *m_pInstance;
    std::ofstream logFile;
    std::string logFileName;
    LoggerVerbosity_t Verbosity;
    bool isCriticalityEnabled(LoggerCriticality_t criticality);
    bool isCriticalityValid(LoggerCriticality_t criticality);
    bool isVerbosityValid(LoggerVerbosity_t verbosity);
    bool openLogFile(std::string &logFile);
    bool openLogFile(const char *logFile);
    void writeLogFileToStdOutput();
    bool closeLogFile();
    std::string getCriticalityLabel(LoggerCriticality_t criticality);
    bool isLogFileOpen();

    friend class Logger;
};

#endif /* LOGGER_H_ */
