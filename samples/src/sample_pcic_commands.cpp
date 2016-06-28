#include "sample.hpp"

int o3d3xx_sample_pcic_commands(void)
{
	PMDHandle hnd = 0;
    std::string command = "";
	int res = 0;
    char err[256] = {0};
    char response[SOURCE_CMD_BUFFER_LENGTH] = {0};
	
    // connect to camera
    printf ("\n ====================O3D300 Sample Code for PCIC Commands====================");
    printf ("\n Connecting to camera: \n");
    res = pmdOpen (&hnd, SOURCE_PLUGIN, SOURCE_PARAM, PROC_PLUGIN, PROC_PARAM);

    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        getchar();
        return res;
    }

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n--------------------PCIC Commands Functionality-----------------------------");
    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Send PCIC Command to activate the application and save the setting\n");
	command = "SendPCICommands a01";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
	{
		printf(" Response:\n %s", response);
		printf ("\n DONE");
	}
	
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Send PCIC Command to request the occupancy of the application list\n");
	command = "SendPCICommands A?";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
	{
		printf(" Application List:\n %s", response);
		printf ("\n DONE");
	}

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Send PCIC Command to set the logic state of a specific io\n");
	command = "SendPCICommands o011";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
	{
		printf(" Response:\n %s", response);
		printf ("\n DONE");
	}
	
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Send PCIC Command to request the state of a specific io\n");
	command = "SendPCICommands O01?";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
	{
		printf(" IO State:\n %s", response);
		printf ("\n DONE");
	}

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Send PCIC Command to enable asynchronous result output\n");
	command = "SendPCICommands p1";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
	{
		printf(" Response:\n %s", response);
		printf ("\n DONE");
	}
    
    // Closing the connection.
    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Closing the camera connection\n");
    res = pmdClose (hnd);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not close the connection %s\n", err);

        return res;
    }
    printf ("\n Camera closed Successfully");
    printf ("\n ================================================================================");


    return PMD_OK;
}