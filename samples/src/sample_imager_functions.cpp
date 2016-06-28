#include "sample.hpp"

int o3d3xx_sample_imager_settings_function(void)
{
	PMDHandle hnd = 0;
    std::string command = "";
	int res = 0;
    char err[256] = {0};
    char response[SOURCE_CMD_BUFFER_LENGTH] = {0};
	unsigned int integrationTime = 0;

    // connect to camera
    printf ("\n ====================O3D300 Sample Code for Imager Settings====================");
    printf ("\n Connecting to camera: \n");
    res = pmdOpen (&hnd, SOURCE_PLUGIN, SOURCE_PARAM, PROC_PLUGIN, PROC_PARAM);

    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        getchar();
        return res;
    }

	// Change Exposure Mode to "moderate" 
    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n--------------------Imager Settings Functionality-----------------------------");
    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Get Imager Setting Type\n");
	command = "GetImagerSettingType";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not change exposure mode:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
	{
		printf("Type: %s", response);
		printf ("\n DONE");
	}
	
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Set Exposure Mode to 'moderate'\n");
	command = "SetExposureMode 1";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not change exposure mode:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Set unambiguous distance range to 'upto30m'\n");
	command = "SetUnambiguousDistanceRange 1";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not change unambiguous distance range:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Get Imager Setting Type\n");
	command = "GetImagerSettingType";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not change exposure mode:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
	{
		printf("Type: %s", response);
		printf ("\n DONE");
	}
	
	// Get the current Exposure Time and Exposure Time Ratio 
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Get Integration Time 1\n");
	// Call pmdGetIntegrationTime with index 0 to get exposure time
	res = pmdGetIntegrationTime (hnd, &integrationTime , 0);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not get integration time 1:\n%s\n", err);
		pmdClose (hnd);
		printf("Camera Connection Closed. \n");
		printf ("Exiting application after key press....");
        getchar();
		return res;
    }
	else
	{
		printf("Integration time 1 = %d usec", integrationTime);
		printf ("\n DONE");
	}

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Get Integration Time Ratio\n");
	command = "GetExposureTimeRatio";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not get integration time ratio:\n%s\n", err);
		getchar();
    }
	else
	{
        int integrationTimeRatio = atoi( response ); 
		printf("Integration time Ratio = %d", integrationTimeRatio);
		printf ("\n DONE");
	}

	// Change Exposure Time and Exposure Time Ratio 
	printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Setting exposureTime to 1000 usec \n");
	// Call pmdSetIntegrationTime with index 0 to set exposure time
    res = pmdSetIntegrationTime (hnd, 0 , 1000);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set the exposure time: \n%s\n", err);
		pmdClose (hnd);
		printf("Camera Connection Closed. \n");
		printf ("Exiting application after key press....");
        getchar();
		return res;
    }
	else
		printf("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Setting exposureTimeRatio to 50 \n");
	command = "SetExposureTimeRatio 50";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set the exposure time ratio: \n%s\n", err);
		getchar();
    }
	else
		printf("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Getting Exposure Time List \n");
	command = "GetExposureTimeList";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get the exposure time list: \n%s\n", err);
		getchar();
    }
	else
    {
        printf("Exposure Time List = %s", response);
		printf("\n DONE");
    }

	// Optimizing exposure time : Setting integration time 10,000 usec before optimizing 
	// exposure time to see the effect of optimizing functionality
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Set Exposure Mode to 'low'\n");
	command = "SetExposureMode 0";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not change exposure mode:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Setting integrationTime to 10,000 usec \n");

    res = pmdSetIntegrationTime (hnd, 0 , 10000);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set the integration time: \n%s\n", err);
		pmdClose (hnd);
		printf("Camera Connection Closed. \n");
		printf ("Exiting application after key press....");
        getchar();
		return res;
    }

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Call Optimize function\n");
	command = "OptimizeExposureTime";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Error in Optimization Procedure:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Get Integration Time 1\n");
	res = pmdGetIntegrationTime (hnd, &integrationTime , 0);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not get integration time 1:\n%s\n", err);
		pmdClose (hnd);
		printf("Camera Connection Closed. \n");
		printf ("Exiting application after key press....");
        getchar();
		return res;
    }
	else
	{
		printf("Integration time 1 = %d usec", integrationTime);
		printf ("\n DONE");
	}

	unsigned int modulationFrequencyChannel;
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Get Modulation Frequency Channel\n");
	res = pmdGetModulationFrequency (hnd, &modulationFrequencyChannel, 0);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not get modulation frequency:\n%s\n", err);
		pmdClose (hnd);
		printf("Camera Connection Closed. \n");
		printf ("Exiting application after key press....");
        getchar();
		return res;
    }
	else
	{
		printf("Modulation Frequency Channel = %d", modulationFrequencyChannel);
		printf ("\n DONE");
	}

	modulationFrequencyChannel = 2;
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Set Modulation Frequency Channel to 2\n");
	res = pmdSetModulationFrequency (hnd, 0, modulationFrequencyChannel);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set modulation frequency:\n%s\n", err);
		pmdClose (hnd);
		printf("Camera Connection Closed. \n");
		printf ("Exiting application after key press....");
        getchar();
		return res;
    }
	else
		printf ("\n DONE");


	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Get Modulation Frequency Channel\n");
	res = pmdGetModulationFrequency (hnd, &modulationFrequencyChannel, 0);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not get modulation frequency:\n%s\n", err);
		pmdClose (hnd);
		printf("Camera Connection Closed. \n");
		printf ("Exiting application after key press....");
        getchar();
		return res;
    }
	else
	{
		printf("Modulation Frequency Channel = %d", modulationFrequencyChannel);
		printf ("\n DONE");
	}

	modulationFrequencyChannel = 1;
	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Set Modulation Frequency Channel to 2\n");
	res = pmdSetModulationFrequency (hnd, 0, modulationFrequencyChannel);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set modulation frequency:\n%s\n", err);
		pmdClose (hnd);
		printf("Camera Connection Closed. \n");
		printf ("Exiting application after key press....");
        getchar();
        return res;
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n---------------Source Commands: Invalid Imager Settings Parameters------------");
    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Exposure Mode with invalid arguments: ""SetExposureMode 5""\n");
	command = "SetExposureMode 5";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        printf (" Result: \n");
		pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set exposure mode:\n%s\n", err);
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Unambiguous Range with invalid arguments: ""SetUnambiguousDistanceRange 5""\n");
	command = "SetUnambiguousDistanceRange 5";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        printf (" Result: \n");
		pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set unambiguous distance range:\n%s\n", err);
    }
	else
		printf ("\n DONE");


    // Closing the connection.
    res = pmdClose (hnd);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not close the connection %s\n", err);

        return res;
    }
    printf ("\n Camera closed Successfully");
    printf ("\n ================================================================================");


    return PMD_OK;
}