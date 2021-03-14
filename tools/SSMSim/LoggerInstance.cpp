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

#include "LoggerInstance.h"

#include "LoggerInstance.h"
#include <fstream>
#include <iostream>
#include <array>
#include <stddef.h> // defines NULL
#include <stdarg.h>

#define LOGGER_DEBUG_LABEL   "DEBUG:   "
#define LOGGER_INFO_LABEL    "INFO:    "
#define LOGGER_WARNING_LABEL "WARNING: "
#define LOGGER_ERROR_LABEL   "ERROR:   "
#define LOGGER_UNKNOWN_LABEL "UNKNOWN: "



LoggerInstance::LoggerInstance()
{

}


LoggerInstance::~LoggerInstance()
{
    if(isLogFileOpen() == true)
    {
        closeLogFile();
    }
}


/**
 * @brief opens _logFile for logging. If _logFile does not exist, it is created.
 * @param _logFile the path to the file where logging will be written to.
 * @return true if the file was open successfully, otherwise it returns false.
 */
bool LoggerInstance::openLogFile(std::string &inFileName)
{
    bool rc;

    if(isLogFileOpen() == true)
    {
        /* Log file is already open.  Return false. */
        LogEvent(LOGGER_EVENT_WARNING, "Failed to open log file.  Log already opened.");
        rc = false;
    }
    else
    {
        logFileName = inFileName;
        logFile.open(inFileName.data(), std::ios::out);
        if (logFile.is_open())
        {
            LogEvent(LOGGER_EVENT_DEBUG, "Opened log file.");
            rc = true;
        }
        else
        {
            LogEvent(LOGGER_EVENT_WARNING, "Failed to open log file.");
            rc = false;
        }
    }

    return rc;
}


/**
 * @brief opens _logFile for logging. If _logFile does not exist, it is created.
 * @param _logFile the path to the file where logging will be written to.
 * @return true if the file was open successfully, otherwise it returns false.
 */
bool LoggerInstance::openLogFile(const char *inFileName)
{
    bool rc;

    std::string fileName = inFileName;

    rc = openLogFile(fileName);

    return rc;
}



std::string LoggerInstance::getCriticalityLabel(LoggerCriticality_t criticality)
{
    std::string label = LOGGER_UNKNOWN_LABEL;

    switch(criticality)
    {
        case LOGGER_EVENT_DEBUG:
        {
            label = LOGGER_DEBUG_LABEL;
            break;
        }

        case LOGGER_EVENT_INFO:
        {
            label = LOGGER_INFO_LABEL;
            break;
        }

        case LOGGER_EVENT_WARNING:
        {
            label = LOGGER_WARNING_LABEL;
            break;
        }

        case LOGGER_EVENT_ERROR:
        {
            label = LOGGER_ERROR_LABEL;
            break;
        }
    }

    return label;
}



/**
 * @brief writes message to log file if messageLevel is less than or equal to
 * logLevel. For example, if logLevel is set to LOGGER_VERBOSITY_DEBUG(highest
 * log level), then all messages are printed. Note that the file is truncated, so any old logs are gone.
 * However, if logLevel is set to LOGGER_VERBOSITY_SILENT, then no messages are
 * printed.
 * @param messageLevel the log level for this specific message.
 */
void LoggerInstance::LogEvent(LoggerCriticality_t inCriticality, const std::string &inMessage)
{
    if(isCriticalityEnabled(inCriticality))
    {
        /* Logging for this criticality is enabled. */
        std::string label = getCriticalityLabel(inCriticality);

        if(isLogFileOpen())
        {
            logFile << label << inMessage << std::endl;
            logFile.flush();
        }
        else
        {
            std::cout << label << inMessage << std::endl;
        }
    }
}



/**
 * @brief closes logFile.
 * @return returns true if the logFile was closed successfully. Otherwise it returns false.
 */
bool LoggerInstance::closeLogFile()
{
    logFile.close();

    if (!logFile.is_open())
    {
	    return true;
    }

    return false;
}



bool LoggerInstance::isCriticalityValid(LoggerCriticality_t criticality)
{
    bool rc;

    if((criticality < LOGGER_EVENT_ERROR) ||
       (criticality > LOGGER_EVENT_DEBUG))
    {
        /* This is not a valid value for criticality. */
        rc = false;
    }
    else
    {
        /* This is a valid value for criticality. */
        rc = true;
    }

    return rc;
}



bool LoggerInstance::isVerbosityValid(LoggerVerbosity_t verbosity)
{
    bool rc;

    if((verbosity < LOGGER_VERBOSITY_SILENT) ||
       (verbosity > LOGGER_VERBOSITY_DEBUG))
    {
        /* This is not a valid value for verbosity. */
        rc = false;
    }
    else
    {
        /* This is a valid value for verbosity. */
        rc = true;
    }

    return rc;
}



bool LoggerInstance::isCriticalityEnabled(LoggerCriticality_t criticality)
{
    bool rc = false;

    /* First check the input parameter. */
    if(isCriticalityValid(criticality))
    {
        /* This is a valid criticality.  Now check to see if the
         * verbosity level permits us to log this event. */
        switch(Verbosity)
        {
            case LOGGER_VERBOSITY_SILENT:
            {
                /* Verbosity is set to silent.  Do log anything. */
                rc = false;
                break;
            }

            case LOGGER_VERBOSITY_DEBUG:
            {
                if((criticality == LOGGER_EVENT_DEBUG) ||
                   (criticality == LOGGER_EVENT_INFO) ||
                   (criticality == LOGGER_EVENT_WARNING) ||
                   (criticality == LOGGER_EVENT_ERROR))

                {
                    /* Verbosity is set to a level that allows this event to
                     * be logged.
                     */
                    rc = true;
                }
                else
                {
                    /* Verbosity is not set to a level that allows this event
                     * to be logged.
                     */
                    rc = false;
                }

                break;
            }

            case LOGGER_VERBOSITY_INFO:
            {
                if((criticality == LOGGER_EVENT_INFO) ||
                   (criticality == LOGGER_EVENT_WARNING) ||
                   (criticality == LOGGER_EVENT_ERROR))
                {
                    /* Verbosity is set to a level that allows this event to
                     * be logged.
                     */
                    rc = true;
                }
                else
                {
                    /* Verbosity is not set to a level that allows this event
                     * to be logged.
                     */
                    rc = false;
                }

                break;
            }

            case LOGGER_VERBOSITY_WARNINGS:
            {
                if((criticality == LOGGER_EVENT_WARNING) ||
                   (criticality == LOGGER_EVENT_ERROR))
                {
                    /* Verbosity is set to a level that allows this event to
                     * be logged.
                     */
                    rc = true;
                }
                else
                {
                    /* Verbosity is not set to a level that allows this event
                     * to be logged.
                     */
                    rc = false;
                }

                break;
            }

            case LOGGER_VERBOSITY_ERRORS:
            {
                if(criticality == LOGGER_EVENT_ERROR)
                {
                    /* Verbosity is set to a level that allows this event to
                     * be logged.
                     */
                    rc = true;
                }
                else
                {
                    /* Verbosity is not set to a level that allows this event
                     * to be logged.
                     */
                    rc = false;
                }

                break;
            }
        }
    }

    return rc;
}



void LoggerInstance::setVerbosity(LoggerVerbosity_t inVerbosity)
{
    if(isVerbosityValid(inVerbosity))
    {
        Verbosity = inVerbosity;
    }
}



bool LoggerInstance::isLogFileOpen(void)
{
    bool rc;

    if (logFile.is_open())
    {
        rc = true;
    }
    else
    {
        rc = false;
    }

    return rc;
}
