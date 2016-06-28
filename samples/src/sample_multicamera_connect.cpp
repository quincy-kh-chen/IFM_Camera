#include "sample.hpp"

int o3d3xx_sample_multicamera_connect(void)
{
    PMDHandle hnd1 = 0, hnd2 = 0, hnd3 = 0; // connection handle
    int res = 0;
    PMDDataDescription dd1, dd2, dd3;
    char err[256] = {0};
	std::string command = "";
    
    printf ("\n =====================MultiCamera Connection===========================");
    printf ("\n Connecting to camera 1: \n");
    // connect to camera
    res = pmdOpen (&hnd1, SOURCE_PLUGIN, SOURCE_PARAM, PROC_PLUGIN, PROC_PARAM);

    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        getchar();
        return res;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Connecting to 2nd camera : \n");
    // connect to camera
    res = pmdOpen (&hnd2, SOURCE_PLUGIN, SOURCE_PARAM_2, PROC_PLUGIN, PROC_PARAM);

    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        pmdClose(hnd1);
        getchar();
        return res;
    }
    printf ("\n DONE");

	printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Connecting to 3rd camera : \n");
    // connect to camera
    res = pmdOpen (&hnd3, SOURCE_PLUGIN, SOURCE_PARAM_3, PROC_PLUGIN, PROC_PARAM);

    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        pmdClose(hnd1); 
        pmdClose(hnd2);
        getchar();
        return res;
    }
    printf ("\n DONE");


    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Updating the framedata from camera 1");
    res = pmdUpdate (hnd1); // to update the camera parameter and framedata
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd1, err, 256);
        fprintf (stderr, "Could not updateData: \n%s\n", err);
        pmdClose (hnd1);
        pmdClose (hnd2); 
        pmdClose (hnd3);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");
    
    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Retrieving source data description\n");
    res = pmdGetSourceDataDescription (hnd1, &dd1);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd1, err, 128);
        fprintf (stderr, "Could not get data description: \n%s\n", err);
        pmdClose (hnd1);
        pmdClose (hnd2); 
        pmdClose (hnd3);
		printf("Camera Connection Closed. \n");
        getchar();
        return res ;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");

	printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Updating the framedata from camera 2");
    res = pmdUpdate (hnd2); // to update the camera parameter and framedata
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd2, err, 256);
        fprintf (stderr, "Could not updateData: \n%s\n", err);
        pmdClose (hnd1);
        pmdClose (hnd2); 
        pmdClose (hnd3);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");


    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Retrieving source data description\n");
    res = pmdGetSourceDataDescription (hnd2, &dd2);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd2, err, 128);
        fprintf (stderr, "Could not get data description: \n%s\n", err);
        pmdClose (hnd1);
        pmdClose (hnd2); 
        pmdClose (hnd3);
		printf("Camera Connection Closed. \n");
        getchar();
        return res ;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");

    printf ("\n Closing connection 2 \n");

    res = pmdClose (hnd2);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd2, err, 128);
        fprintf (stderr, "Could not close the connection %s\n", err);
        pmdClose (hnd1);
        pmdClose (hnd3);
        return res;
    }

    printf ("\n Camera 2 closed Successfully");
    printf ("\n ================================================================================");

    printf ("\n Updating the framedata from camera 3");
    res = pmdUpdate (hnd3); // to update the camera parameter and framedata
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd3, err, 256);
        fprintf (stderr, "Could not updateData: \n%s\n", err);
        pmdClose (hnd1);
        pmdClose (hnd3);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");


    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Retrieving source data description\n");
    res = pmdGetSourceDataDescription (hnd3, &dd3);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd3, err, 128);
        fprintf (stderr, "Could not get data description: \n%s\n", err);
        pmdClose (hnd1);
        pmdClose (hnd3);
		printf("Camera Connection Closed. \n");
        getchar();
        return res ;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");

    printf ("\n ================================================================================");
    printf ("\n Closing connection 1 \n");
	res = pmdClose (hnd1);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd1, err, 128);
        fprintf (stderr, "Could not close the connection %s\n", err);
        pmdClose (hnd3);
        return res;
    }

    printf ("\n Camera 1 closed Successfully");
    printf ("\n ================================================================================");

    printf ("\n Closing connection 3 \n");
	res = pmdClose (hnd3);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd3, err, 128);
        fprintf (stderr, "Could not close the connection %s\n", err);
        return res;
    }

    printf ("\n Camera 3 closed Successfully");
    printf ("\n ================================================================================");

    return PMD_OK;

}

