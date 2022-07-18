# YAMCS Configuration
This current configuration is not done yet. We still need to update the config files and our code base to use the [Plugin](https://yamcs.org/docs/yamcs-server-manual/yamcs-plugin-format/) Plugin interface in order for us to hook into the YAMCS workflow.

# param_to_csv.py Manual
This section of the document is dedicated to the python script to download csv data from yamcs server.  The file name is `param_to_csv.py`.

## Purpose
The purpose of the script is to download and format a csv file from yamcs that contains timed data.

## Usage
### Environment Generation
Before using the code, the python environment is to be set for the script to run corrently.
We do this by setting up a virtual environment for the script.
This is done by invoking the following set of commands in this folder.

    make clean
    make python-env
    source venv/bin/activate

The last command activates the virtual environment.  To deactivate the environment, use the following command.

    deactivate

Leave the virutal environment acitavated to run the script.
### Running the script
The following command runs the script.

    param_to_csv.py --file <path to yaml file>

## YAML file
The Yaml file has the following key entries
### uri
The uri points to the yamcs server REST Api.  On local host it is the following:

    http://localhost:8090/api/archive/

### instance
The instance key points to the target from which data is being collected.  In our example it is being collected from:

    fsw

### method
The method key is the REST Api method that needs to be called to receive the csv data.  It is always going to be the 
following:

    exportParameterValues

### param_array
This key holds an array of yamcs parameters that need to be stored in csv as a column.

### start
This is the timestamp from when to start data collection.  This timestamp is the RFC 3339 format of Packet time in yamcs.

### stop
This is the timestamp from when to stop data collection.  This timestamp is the RFC 3339 format of Packet time in yamcs.

### yamcs_csv_file
This is the path to the csv file that is dumped by yamcs when the REST Api method is called.

### output_csv_file
This is the path of the csv output file, which has formatted data of the fields that are requested.