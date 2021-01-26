/****************************************************************************
 *
 *   Copyright (c) 2019 Windhover Labs, L.L.C. All rights reserved.
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

#ifndef CPP_LIB_H
#define CPP_LIB_H

#include "cfe.h"
#include "prm_lib.h"


#define PRMLIB_MAX_REGISTRATIONS     (50)

typedef struct
{
	char         	   Name[PRMLIB_MSG_PARAM_NAME_LEN];
    void*              Value;
    PRMLIB_ParamType_t Type;
} PRMLIB_ParamRegistration_t;

typedef struct
{
	const char                 PipeName[OS_MAX_API_NAME];
	uint32                     PipeDepth;
	PRMLIB_ParamRegistration_t Registrations[PRMLIB_MAX_REGISTRATIONS];
} PRMLIB_ParamRegistrations_t;



class ParamsConsumer
{
    public:
	    ParamsConsumer();
	    ~ParamsConsumer();

        PRMLIB_ParamRegistration_t* GetParamRegistration(const char *Name);
	    int32 InitParams(PRMLIB_ParamRegistrations_t &Params);
	    int32 CheckParams(void);
        virtual void onParamsChange(PRMLIB_ParamRegistration_t *ParamsData, uint32 ParamsCount);
        virtual osalbool onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], uint8 Value);
        virtual osalbool onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], int8 Value);
        virtual osalbool onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], uint16 Value);
        virtual osalbool onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], int16 Value);
        virtual osalbool onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], uint32 Value);
        virtual osalbool onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], int32 Value);
        virtual osalbool onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], float Value);
        virtual osalbool onParamValidate(void* Address, uint8 Value);
        virtual osalbool onParamValidate(void* Address, int8 Value);
        virtual osalbool onParamValidate(void* Address, uint16 Value);
        virtual osalbool onParamValidate(void* Address, int16 Value);
        virtual osalbool onParamValidate(void* Address, uint32 Value);
        virtual osalbool onParamValidate(void* Address, int32 Value);
        virtual osalbool onParamValidate(void* Address, float Value);

    private:
        /** \brief Param Pipe ID */
        CFE_SB_PipeId_t            ParamPipeId;
        PRMLIB_ParamRegistration_t ParamRegistration[PRMLIB_MAX_REGISTRATIONS];
};



#endif /* CPP_LIB_H */

/************************/
/*  End of File Comment */
/************************/
