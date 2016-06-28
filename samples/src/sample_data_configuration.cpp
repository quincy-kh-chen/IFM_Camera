#include "sample.hpp"

int o3d3xx_sample_data_configuration(void)
{

    PMDHandle hnd = 0;
    PMDDataDescription dd;
    int res = 0, diagnosticDataSize = 0;
    char response[256] = {0}, err[256] = {0};
    std::vector<float> ampl;
    std::vector<float> dist;
    std::vector<float> xyz3Dcoordinate;
    std::string diagnosticData = "";

    printf ("\n ====================O3D300 Sample Code for Data Configuration====================");
    printf ("\n Connecting to camera: \n");
    res = pmdOpen (&hnd, SOURCE_PLUGIN, SOURCE_PARAM , PROC_PLUGIN, PROC_PARAM);
    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        getchar();
        return PMD_COULD_NOT_OPEN;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");

    /*
       Data Configuration Command Value:
       Bit 31 | ..... |    Bit 8   |    Bit 7  | Bit 6 | Bit 5 | Bit 4 |  Bit 3   |   Bit 2    |  Bit 1  | Bit 0
         X              Diagnostic   Confidence    Z       Y       X     Amplitude  Normalized   Distance   All
                          Data          Image    Image   Image   Image     Data     Amplitude      Data    Images
     */
    printf ("\n Setting the configuration to read distance, norm. amp, and diagnostic data using Bit Mask Technique: \n");
    printf ("\n Bit Pattern to set: 1 0000 0110. Decimal Value = 262. \n");
	std::string command = "SetSourceDataBinSelect 262";
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
    printf ("\n Updating the framedata from camera ");
    res = pmdUpdate (hnd); // to update the camera parameter and framedata
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 256);
        fprintf (stderr, "Could not updateData: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Retrieving source data description\n");
    res = pmdGetSourceDataDescription (hnd, &dd);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get data description: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");
    
    int imgWidth = dd.img.numColumns;
    int imgHeight = dd.img.numRows;

    dist.resize (imgWidth * imgHeight);
    ampl.resize (imgWidth * imgHeight);
    xyz3Dcoordinate.resize (imgHeight * imgWidth * 3); // value three is for 3 images

    res = pmdGetDistances (hnd, &dist[0], dist.size() * sizeof (float));
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get distance data: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n Middle pixel Distance value: %fm\n", dist[ (imgHeight / 2) * imgWidth + imgWidth / 2]);

    res = pmdGetAmplitudes (hnd, &ampl[0], ampl.size() * sizeof (float));
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get amplitude data: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n Middle Amplitude: %f\n", ampl[ (dd.img.numRows / 2) * dd.img.numColumns + dd.img.numColumns / 2]);

    res = pmdGet3DCoordinates (hnd, &xyz3Dcoordinate[0], xyz3Dcoordinate.size() * sizeof (float));
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get xyz data: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }

    //calculating the mean values of X/Y/Z/ Image

    bool value_present_flag = false;
    for (int i = 0 ; i < imgHeight * imgWidth ; i++)
    {
        if(xyz3Dcoordinate[i * 3 + 0] != 0 ||  xyz3Dcoordinate[i * 3 + 1] != 0 || xyz3Dcoordinate[i * 3 + 2] != 0)
            value_present_flag = true;
    }

    if(!value_present_flag)
        printf("\nAll values for X,Y, and Z co-ordinates are zero, since, we did not request the XYZ in the data configuration.\n");
    else
        printf("Error!!!Data configuration failed.");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Retrieving diagnostic data \n");
    diagnosticData.resize(SOURCE_CMD_BUFFER_LENGTH);
    command = "GetDiagnosticData";
    res = pmdProcessingCommand (hnd, (char *)diagnosticData.c_str(), SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not enable distance image filtering:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
    {
        printf ("\n Diagnostic Data = %s", diagnosticData.c_str());
		printf("\n DONE. \n");
    }

    std::string PcicJsonString = "{ \"layouter\": \"flexible\", \"format\": { \"dataencoding\": \"ascii\" }, \"elements\": [ { \"type\": \"string\", \"value\": \"star\", \"id\": \"start_string\" }, { \"type\": \"blob\", \"id\": \"distance_image\" }, { \"type\": \"string\", \"value\": \"stop\", \"id\": \"end_string\" } ] }";
    printf ("\n Setting the configuration to read distance using JSON string: \n");
    command = "SetSourceDataJson " + PcicJsonString;
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
    printf ("\n Updating the framedata from camera ");
    res = pmdUpdate (hnd); // to update the camera parameter and framedata
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 256);
        fprintf (stderr, "Could not updateData: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Retrieving source data description\n");
    res = pmdGetSourceDataDescription (hnd, &dd);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get data description: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");
    
    imgWidth = dd.img.numColumns;
    imgHeight = dd.img.numRows;

    dist.clear();
    ampl.clear();

    dist.resize (imgWidth * imgHeight);
    ampl.resize (imgWidth * imgHeight);
    
    res = pmdGetDistances (hnd, &dist[0], dist.size() * sizeof (float));
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get distance data: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n Middle pixel Distance value: %fm\n", dist[ (imgHeight / 2) * imgWidth + imgWidth / 2]);

    res = pmdGetAmplitudes (hnd, &ampl[0], ampl.size() * sizeof (float));
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get amplitude data: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n Middle Amplitude: %f\n", ampl[ (dd.img.numRows / 2) * dd.img.numColumns + dd.img.numColumns / 2]);
    if(ampl[ (dd.img.numRows / 2) * dd.img.numColumns + dd.img.numColumns / 2] == 0)
        printf("\n Amplitude is zero since we did not request for amplitude data in data configuration\n");
    else
        printf("\n Error in data configuration\n");

    res = pmdClose(hnd);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not close the connection %s\n", err);

        return res;
    }

    printf ("\n Camera closed Successfully");
    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Connecting to camera: \n");
    res = pmdOpen (&hnd, SOURCE_PLUGIN, SOURCE_PARAM , PROC_PLUGIN, PROC_PARAM);
    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        getchar();
        return PMD_COULD_NOT_OPEN;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Updating the framedata from camera ");
    res = pmdUpdate (hnd); // to update the camera parameter and framedata
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 256);
        fprintf (stderr, "Could not updateData: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Retrieving source data description\n");
    res = pmdGetSourceDataDescription (hnd, &dd);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get data description: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");
    printf("\n Image Height = %d", dd.img.numRows);
    printf("\n Image Width = %d", dd.img.numColumns);

    printf ("\n -----------------------------------------------------------------------------");

    printf ("\n Changing Image Resolution to 352 X 264 \n");
    command = "SetImageResolution 1";
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
    printf ("\n Updating the framedata from camera ");
    res = pmdUpdate (hnd); // to update the camera parameter and framedata
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 256);
        fprintf (stderr, "Could not updateData: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Retrieving source data description\n");
    res = pmdGetSourceDataDescription (hnd, &dd);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get data description: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n DONE");
    printf("\n Image Height = %d", dd.img.numRows);
    printf("\n Image Width = %d", dd.img.numColumns);

    imgWidth = dd.img.numColumns;
    imgHeight = dd.img.numRows;

    dist.clear();
    ampl.clear();
    xyz3Dcoordinate.clear();

    dist.resize (imgWidth * imgHeight);
    ampl.resize (imgWidth * imgHeight);
    xyz3Dcoordinate.resize (imgHeight * imgWidth * 3); // value three is for 3 images

    res = pmdGetDistances (hnd, &dist[0], dist.size() * sizeof (float));
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get distance data: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n Middle pixel Distance value: %fm\n", dist[ (imgHeight / 2) * imgWidth + imgWidth / 2]);

    res = pmdGetAmplitudes (hnd, &ampl[0], ampl.size() * sizeof (float));
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get amplitude data: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf ("\n Middle Amplitude: %f\n", ampl[ (dd.img.numRows / 2) * dd.img.numColumns + dd.img.numColumns / 2]);

    res = pmdGet3DCoordinates (hnd, &xyz3Dcoordinate[0], xyz3Dcoordinate.size() * sizeof (float));
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not get xyz data: \n%s\n", err);
        pmdClose (hnd);
		printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }

    printf ("\n -----------------------------------------------------------------------------");
    char clippingCuboidStr[CLIPPING_CUBOID_STRING_LENGTH];
    command = "GetClippingCuboid";
    printf("\n Getting Clipping Cuboid JSON String\n");
    res = pmdSourceCommand (hnd, clippingCuboidStr, CLIPPING_CUBOID_STRING_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not enable distance image filtering:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
    {
		printf ("\n DONE");
        printf ("\n %s", clippingCuboidStr);
    }

    printf ("\n -----------------------------------------------------------------------------");
    std::string newClippingCuboidStr = "{\"XMin\": -10, \"XMax\": 10, \"YMin\": -20, \"YMax\": 20, \"ZMin\": 1, \"ZMax\": 5}";
    command = "SetClippingCuboid " + newClippingCuboidStr;
    printf("\n Setting new clipping cuboid \n");
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
    command = "GetClippingCuboid";
    printf("\n Getting new Clipping Cuboid JSON String\n");
    res = pmdSourceCommand (hnd, clippingCuboidStr, CLIPPING_CUBOID_STRING_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not enable distance image filtering:\n%s\n", err);
		printf ("Press any key to continue....");
        getchar();
    }
	else
    {
		printf ("\n DONE");
        printf ("\n %s", clippingCuboidStr);
    }


    printf ("\n -----------------------------------------------------------------------------");
    res = pmdClose(hnd);
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