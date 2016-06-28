#include "sample.hpp"

int o3d3xx_sample_filter_function()
{
	PMDHandle hnd = 0;
    std::string command = "";
	int res = 0;
    char err[256] = {0};
    char response[SOURCE_CMD_BUFFER_LENGTH] = {0};

    // connect to camera
    printf ("\n ====================O3D300 Sample Code for Filter Functions====================");
    printf ("\n Connecting to camera: \n");
    res = pmdOpen (&hnd, SOURCE_PLUGIN, SOURCE_PARAM, PROC_PLUGIN, PROC_PARAM);

    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        getchar();
        return PMD_COULD_NOT_OPEN;
    }

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n--------------------Source Commands: Filter Parameters------------------------");
    printf ("\n -----------------------------------------------------------------------------");	
	printf ("\n Enabling Distance Image Filter\n");
	command = "EnableFilterDistanceImage true";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not enable distance image filtering:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Enabling Amplitude Image Filter\n");
	command = "EnableFilterAmplitudeImage true";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not enable amplitude image filtering:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Spatial Filter to Mean Filtering\n");
	command = "SetSpatialFilterType 1" ;
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set spatial filtering to Mean filter:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Spatial Filter to Median Filtering\n");
	command = "SetSpatialFilterType 2";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set spatial filtering to Median filter:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Spatial Filter Masksize to 5X5\n");
	command = "SetSpatialFilterMaskSize 1" ;
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set filter mask to 5X5:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Spatial Filter to OFF\n");
	command = "SetSpatialFilterType 0" ;
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set spatial filtering to OFF:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Tempoaral Filter to Adaptive Exponential Filtering\n");
	command = "SetTemporalFilterType 2" ; 
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set temporal filtering to adaptive exponential filter:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Trigger Mode to Process Interface\n");
	command = "SetTriggerMode 2" ; 
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set trigger mode to process interface:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Temporal filter to Mean Filtering\n");
	command = "SetTemporalFilterType 1" ; 
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set temporal filtering to mean filter:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Temporal filter to OFF\n");
	command = "SetTemporalFilterType 0" ; 
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set temporal filtering to OFF:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Switch Trigger Mode back to Continuous (Free-run)\n");
	command = "SetTriggerMode 1" ; 
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set trigger mode to continuous mode:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Disabling Distance Image Filter\n");
	command = "EnableFilterDistanceImage false";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not enable distance image filtering:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Disabling Amplitude Image Filter\n");
	command = "EnableFilterAmplitudeImage false";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not enable amplitude image filtering:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Diconnecting from PCIC\n");
	command = "DisconnectPCIC";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not disconnect from PCIC:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Connecting to PCIC\n");
	command = "ConnectPCIC";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not connect to PCIC:\n%s\n", err);
		printf (" Press any key to continue....");
        getchar();
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n---------------Source Commands: Invalid Filter Parameters---------------------");
    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Enabling Distance Image Filter with command: ""EnableFilterDistanceImage tr""\n");
	command = "EnableFilterDistanceImage tr";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        printf (" Result: \n");
		pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not enable distance image filtering:\n%s\n", err);
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Disabling Amplitude Image Filter with command: ""EnableFilterAmplitudeImage 0""\n");
	command = "EnableFilterAmplitudeImage 0";
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
		printf (" Result: \n");
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not enable amplitude image filtering:\n%s\n", err);
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Spatial Filter command: ""SetSpatialFilterType 5""\n");
	command = "SetSpatialFilterType 5" ;
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
		printf (" Result: \n");
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set spatial filter type:\n%s\n", err);
    }
	else
		printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Temporal filter command: ""SetTemporalFilterType 15"" \n");
	command = "SetTemporalFilterType 15" ; 
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
		printf (" Result: \n");
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set temporal filter type:\n%s\n", err);
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Number of Images for Temporal Mean Filter without setting temporal filtering to Mean Filtering: \n");
	command = "SetTempMeanFilterNumberOfImages 15" ; 
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
		printf (" Result: \n");
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set number of images for temporal mean filter:\n%s\n", err);
    }
	else
		printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
	printf ("\n Setting Mask size for Spatial Filter with spatial filtering set to OFF: \n");
	command = "SetSpatialFilterMaskSize 1" ; 
	res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
		printf (" Result: \n");
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set Spatial Filter Mask Size:\n%s\n", err);
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