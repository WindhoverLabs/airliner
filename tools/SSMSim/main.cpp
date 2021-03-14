#include <iostream>
#include "Logger.h"
#include <argp.h>
#include <yaml-cpp/yaml.h>
#include <stdint.h>
#include "OBC.hpp"
#include "Gnd.hpp"
#include "Sim.hpp"
#include <vector>
#include <string.h>



const char *argp_program_version = "SSMSim v1.0";
const char *argp_program_bug_address = "<mbenson@windhoverlabs.com>";

/* Program documentation. */
static char doc[] = "SSMSim simulates the Windhover Source Selection Module "
		            "for Software in the Loop (SITL) Airliner instances.";

/* A description of the arguments we accept. */
static char args_doc[] = "--config <FILE>";

/* The options we understand. */
static struct argp_option options[] =
{
    { "config", 'c', "FILE", 0, "Configuration file" },
    { 0 } };

/* Used by main to communicate with parse_opt. */
typedef struct
{
	char *config_file;
	bool config_file_set;
} arguments_t;

typedef enum
{
	PARSER_MAIN_STATE_OBC,
	PARSER_MAIN_STATE_SIM,
	PARSER_MAIN_STATE_GND,
} ParserMainState_t;

typedef enum
{
	PARSER_OBC_STATE_ROOT,
	PARSER_OBC_STATE_FSW,
	PARSER_OBC_STATE_GND,
	PARSER_OBC_STATE_SIM
} ParserOBCState_t;

typedef enum
{
	PARSER_GND_STATE_ROOT,
	PARSER_GND_STATE_TLM,
	PARSER_GND_STATE_CND
} ParserGndState_t;

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	 know is a pointer to our arguments structure. */
	arguments_t *arguments = (arguments_t *) state->input;

	switch (key)
	{
		case 'c':
		{
			arguments->config_file = arg;
			arguments->config_file_set = true;
			break;
		}

		case ARGP_KEY_ARG:
		{
			if (state->arg_num >= 2)
			{
			    /* Too many arguments. */
			    argp_usage (state);
			}

			break;
		}

		case ARGP_KEY_END:
		{
			/* Verify arguments, starting with the input file. */
			if (arguments->config_file_set == false)
			{
				printf("Error:  Configuration file not set.\n");
				argp_usage(state);
				return ARGP_KEY_ERROR;
			}

			break;
		}

		default:
		{
			return ARGP_ERR_UNKNOWN;
		}
	}

	return 0;
}

/* Our argp parser. */
static struct argp argp =
{ options, parse_opt, args_doc, doc };

int main(int argc, char *argv[])
{
	arguments_t        arguments;
	error_t            parse_error;
	std::vector<OBC*>  obcs;
	Sim               *sim;
	Gnd               *gnd;

	/* Set argument default values. */
	memset(&arguments, 0, sizeof(arguments));

	/* Parse our arguments; every option seen by parse_opt will
	 be reflected in arguments. */
	parse_error = argp_parse(&argp, argc, argv, 0, 0, &arguments);
	if (parse_error == 0)
	{
		int           rc;
        Logger        logger = Logger(LOGGER_VERBOSITY_INFO);
        YAML::Node    config = YAML::LoadFile(arguments.config_file);

        for (YAML::const_iterator it = config["obc"].begin(); it != config["obc"].end(); ++it)
        {
        	OBC *obc;
            YAML::Node sbc_config = *it;

            std::string name       = sbc_config["name"].as<std::string>();
            uint16_t    fswTlmPort = sbc_config["fsw"]["tlm"]["port"].as<uint16_t>();
            std::string fswCmdIP   = sbc_config["fsw"]["cmd"]["ip"].as<std::string>();
            uint16_t    fswCmdPort = sbc_config["fsw"]["cmd"]["port"].as<uint16_t>();
            std::string gndTlmIP   = sbc_config["gnd"]["tlm"]["ip"].as<std::string>();
            uint16_t    gndTlmPort = sbc_config["gnd"]["tlm"]["port"].as<uint16_t>();
            uint16_t    gndCmdPort = sbc_config["gnd"]["cmd"]["port"].as<uint16_t>();
            std::string simIP      = sbc_config["sim"]["ip"].as<std::string>();
            uint16_t    simPort    = sbc_config["sim"]["port"].as<uint16_t>();

            obc = new OBC(name, fswTlmPort, fswCmdIP, fswCmdPort, gndTlmIP, gndTlmPort, gndCmdPort, simIP, simPort);

            obcs.push_back(obc);

            if(obc->Start() < 0)
            {
            	return -1;
            }
        }

        std::string simIP      = config["sim"]["ip"].as<std::string>();
        uint16_t    simPort    = config["sim"]["port"].as<uint16_t>();
        sim = new Sim(simIP, simPort);
        if(sim->Start() < 0)
        {
        	return -1;
        }

        std::string gndTlmIP   = config["gnd"]["tlm"]["ip"].as<std::string>();
        uint16_t    gndTlmPort = config["gnd"]["tlm"]["port"].as<uint16_t>();
        uint16_t    gndCmdPort  = config["gnd"]["cmd"]["port"].as<uint16_t>();
        gnd = new Gnd(gndTlmIP, gndTlmPort, gndCmdPort);
        if(gnd->Start() < 0)
        {
        	return -1;
        }

		return 0;
	}
	else
	{
		return -1;
	}
}
