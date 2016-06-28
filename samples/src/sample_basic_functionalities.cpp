#include "sample.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

int o3d3xx_sample_basic_functionalities(void)
{
    PMDHandle hnd = 0; // connection handle
    int res = 0;
    PMDDataDescription dd;
    unsigned int integrationTime = 800;
    std::vector<float> amp;
    std::vector<float> dist;
	 std::vector<unsigned> flags;

	 std::vector<vector<float>> distances;
	 distances.resize(40);

    std::vector<vector<float>> intensity;
    intensity.resize(40);

    std::vector<vector<float>> xyz3Dcoordinate;
    xyz3Dcoordinate.resize(40);

	int imgHeight = 0;
   int imgWidth = 0;
   char err[256] = {0};
	std::string command = "";
   char response[SOURCE_CMD_BUFFER_LENGTH] = {0};
   bool filter = true;
   bool resolution_100k = false;
	bool savePointCloud = true;
	bool saveIntensity = true;
	bool saveDistances = true;

    printf ("\n =======================O3D300 Testing=========================");
    printf ("\n Connecting to camera: \n");
    // connect to camera
    res = pmdOpen (&hnd, SOURCE_PLUGIN, SOURCE_PARAM, PROC_PLUGIN, PROC_PARAM);

    if (res != PMD_OK)
    {
        fprintf (stderr, "Could not connect: \n");
        getchar();
        return res;
    }
    printf ("\n DONE");
     
    char result[128];
    /// @brief set resolution
    if(resolution_100k)
    {
        printf ("\n -----------------------------------------------------------------------------");
        printf ("\n Setting Resolution : 352 X 264\n");
        
        // Configure data for distance and normalized amplitude – 0 0000 0110. 
        res = pmdSourceCommand(hnd, result, 128, "SetImageResolution 1");
    }

    /// @brief set integration time
    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Setting and getting parameters viz : integrationTime \n");

    res = pmdSetIntegrationTime (hnd, 0 , integrationTime);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set the integration time: \n%s\n", err);
        pmdClose (hnd);
        printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }

    res = pmdGetIntegrationTime (hnd, &integrationTime , 0);
    if (res != PMD_OK)
    {
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, "Could not set the integration time:\n%s\n", err);
        pmdClose (hnd);
        printf("Camera Connection Closed. \n");
        getchar();
        return res;
    }
    printf(" Integration Time = %d ", integrationTime);
    cout << "ms" << endl;
    printf ("\n DONE");
    
    /// @brief set unambiguous distance
    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n Setting Unambiguous Range with invalid arguments: ""SetUnambiguousDistanceRange 5 m""\n");
    command = "SetUnambiguousDistanceRange 0";
    res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
    if (res != PMD_OK)
    {
        printf (" Result: \n");
        pmdGetLastError (hnd, err, 128);
        fprintf (stderr, " Could not set unambiguous distance range:\n%s\n", err);
    }
    else
    printf ("\n DONE");
    

    /// @brief set filter parameter
    if(filter)
    {
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

        // printf ("\n -----------------------------------------------------------------------------");
        // printf ("\n Enabling Amplitude Image Filter\n");
        // command = "EnableFilterAmplitudeImage true";
        // res = pmdSourceCommand (hnd, response, SOURCE_CMD_BUFFER_LENGTH, command.c_str());
        // if (res != PMD_OK)
        // {
        //     pmdGetLastError (hnd, err, 128);
        //     fprintf (stderr, "Could not enable amplitude image filtering:\n%s\n", err);
        //     printf ("Press any key to continue....");
        //     getchar();
        // }
        // else
        //     printf ("\n DONE");

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
    }

    printf ("\n");

    printf ("\n -----------------------------------------------------------------------------");
    printf ("\n--------------------  Source Commands: Capture Frames ------------------------");
    printf ("\n -----------------------------------------------------------------------------");    
    
    res = pmdSourceCommand(hnd, result, 128, "SetTargetFrameRate 15.0");

    /// do all resize
    if(1)
    {
        // printf ("\n -----------------------------------------------------------------------------");
        // printf ("\n Updating the framedata from camera ");
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
        // printf ("\n DONE");
        
        // printf ("\n -----------------------------------------------------------------------------");
        // printf ("\n Retrieving source data description\n");
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
        // printf ("\n DONE");
        
        imgWidth = dd.img.numColumns;
        imgHeight = dd.img.numRows;
        
        std::cout << std::endl;
        std::cout << "imgWidth  = " << dd.img.numColumns << std::endl;
        std::cout << "imgHeight = " << dd.img.numRows << std::endl;
        // printf ("\n -----------------------------------------------------------------------------");


        dist.resize (imgWidth * imgHeight);
        amp.resize (imgWidth * imgHeight);
        flags.resize (imgWidth * imgHeight);

		for (int ii = 0; ii < distances.size(); ++ii)
		{
			distances[ii].resize(imgWidth * imgHeight);
		}
        
        for(int ii = 0 ; ii < intensity.size(); ++ii)
        {
            intensity[ii].resize (imgWidth * imgHeight);
        }
        for(int ii = 0 ; ii < xyz3Dcoordinate.size(); ++ii)
        {
            xyz3Dcoordinate[ii].resize (imgHeight * imgWidth * 3); // value three is for 3 images
        }
    }

    /// @brief update frame
    clock_t begin_time = clock();
    for (int ii = 0; ii < 40; ++ii)
    {
        // printf ("\n -----------------------------------------------------------------------------");
        // printf ("\n Updating the framedata from camera ");
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
        // printf ("\n DONE");
        
        // printf ("\n -----------------------------------------------------------------------------");
        // printf ("\n Retrieving source data description\n");
     //    res = pmdGetSourceDataDescription (hnd, &dd);
     //    if (res != PMD_OK)
     //    {
     //        pmdGetLastError (hnd, err, 128);
     //        fprintf (stderr, "Could not get data description: \n%s\n", err);
     //        pmdClose (hnd);
    	// 	printf("Camera Connection Closed. \n");
     //        getchar();
     //        return res;
     //    }
     //    // printf ("\n DONE");
        
     //    imgWidth = dd.img.numColumns;
     //    imgHeight = dd.img.numRows;
        
    	// std::cout << std::endl;
     //    std::cout << "imgWidth  = " << dd.img.numColumns << std::endl;
     //    std::cout << "imgHeight = " << dd.img.numRows << std::endl;
     //    // printf ("\n -----------------------------------------------------------------------------");


     //    dist.resize (imgWidth * imgHeight);
     //    amp.resize (imgWidth * imgHeight);
     //    intensity.resize (imgWidth * imgHeight);

     //    flags.resize (imgWidth * imgHeight);
     //    xyz3Dcoordinate.resize (imgHeight * imgWidth * 3); // value three is for 3 images

      //   // printf ("\n Obtaining different image data from camera viz amplitude and Distance Image  \n");

    

      //   res = pmdGetAmplitudes (hnd, &amp[0], amp.size() * sizeof (float));
      //   if (res != PMD_OK)
      //   {
      //       pmdGetLastError (hnd, err, 128);
      //       fprintf (stderr, "Could not get amplitude data: \n%s\n", err);
      //       pmdClose (hnd);
    		// printf("Camera Connection Closed. \n");
      //       getchar();
      //       return res;
      //   }
      //   // printf ("\n Middle Amplitude: %f\n", amp[ (dd.img.numRows / 2) * dd.img.numColumns + dd.img.numColumns / 2]);

      //   res = pmdGetFlags (hnd, &flags[0], flags.size() * sizeof (float));
      //   if (res != PMD_OK)
      //   {
      //       pmdGetLastError (hnd, err, 128);
      //       fprintf (stderr, "Could not get flag data: \n%s\n", err);
      //       pmdClose (hnd);
    		// printf("Camera Connection Closed. \n");
      //       getchar();
      //       return res;
      //   }

        /* 3D coordinates are stored in interleved way .i.e.
        for every pixel i
        xcordinate value = xyz3Dcoordinate[i+0]
        ycordinate value = xyz3Dcoordinate[i+1]
        zcordinate value = xyz3Dcoordinate[i+2]*/
        
        // xyz3Dcoordinate.clear(); // value three is for 3 images
        // xyz3Dcoordinate.resize(imgHeight * imgWidth * 3); // value three is for 3 images
        
        // std::vector<float> tempPointCloud;
        // tempPointCloud.resize(imgHeight * imgWidth * 3); // value three is for 3 images


		// get distances
		if (saveDistances)
		{
			res = pmdGetDistances(hnd, &distances[ii][0], distances[ii].size() * sizeof(float));
			if (res != PMD_OK)
			{
				pmdGetLastError(hnd, err, 128);
				fprintf(stderr, "Could not get distance data: \n%s\n", err);
				pmdClose(hnd);
				printf("Camera Connection Closed. \n");
				getchar();
				return res;
			}
			// printf ("\n Middle pixel Distance value: %fm\n", dist[ (imgHeight / 2) * imgWidth + imgWidth / 2]);
		}

		// get pointcloud
		if (savePointCloud)
		{
			pmdGet3DCoordinates(hnd, &xyz3Dcoordinate[ii][0], xyz3Dcoordinate[ii].size() * sizeof(float));
			if (res != PMD_OK)
			{
				pmdGetLastError(hnd, err, 128);
				fprintf(stderr, "Could not get xyz data: \n%s\n", err);
				pmdClose(hnd);
				printf("Camera Connection Closed. \n");
				getchar();
				return res;
			}
			//cout << ii << " point cloud saved" << endl;
		}

        // get intensity 
		if (saveIntensity)
		{
			pmdGetIntensities(hnd, &intensity[ii][0], intensity[ii].size() * sizeof(float));
		}

		cout << " frame "  << ii << endl;
    }

    clock_t end_time = clock();
    cout << "time elapsed = " << float(end_time - begin_time)*1000/CLOCKS_PER_SEC << " ms" << endl;

    // //calculating the mean values of X/Y/Z/ Image
    // float XSum = 0 , YSum = 0 , ZSum = 0;
    // float ASum = 0, DSum = 0;
    // unsigned counter = 0;
    // for (int i = 0 ; i < imgHeight * imgWidth ; i++)
    // {
    //     if (! (flags[i] & 1)) // first bit is set to 1,if pixel is invalid
    //     {
    //         DSum += dist[i];
    //         ASum += amp[i];
    //         XSum += xyz3Dcoordinate[i * 3 + 0];
    //         YSum += xyz3Dcoordinate[i * 3 + 1];
    //         ZSum += xyz3Dcoordinate[i * 3 + 2];
    //         counter++;
    //     }
    // }

    // float DMean = counter ? (DSum / float (counter)) : 0.f;
    // float AMean = counter ? (ASum / float (counter)) : 0.f;
    // float XMean = counter ? (XSum / float (counter)) : 0.f;
    // float YMean = counter ? (YSum / float (counter)) : 0.f;
    // float ZMean = counter ? (ZSum / float (counter)) : 0.f;

    // printf (" DMean = %fm \n AMean = %f \n", DMean, AMean);
    // printf (" XMean = %fm \n YMean = %fm \n ZMean = %fm ", XMean, YMean, ZMean);

    // printf ("\n DONE");

  //   printf ("\n -----------------------------------------------------------------------------");
  //   printf ("\n Setting and getting parameters viz : integrationTime \n");

  //   res = pmdSetIntegrationTime (hnd, 0 , 1000);
  //   if (res != PMD_OK)
  //   {
  //       pmdGetLastError (hnd, err, 128);
  //       fprintf (stderr, "Could not set the integration time: \n%s\n", err);
  //       pmdClose (hnd);
		// printf("Camera Connection Closed. \n");
  //       getchar();
  //       return res;
  //   }

  //   res = pmdGetIntegrationTime (hnd, &integrationTime , 0);
  //   if (res != PMD_OK)
  //   {
  //       pmdGetLastError (hnd, err, 128);
  //       fprintf (stderr, "Could not set the integration time:\n%s\n", err);
  //       pmdClose (hnd);
		// printf("Camera Connection Closed. \n");
  //       getchar();
  //       return res;
  //   }
  //   printf("Integration Time = %d \n", integrationTime);
  //   printf ("\n DONE");

 //    printf ("\n -----------------------------------------------------------------------------");
	// printf ("\n Closing the connection \n");

 //    res = pmdClose (hnd);
 //    if (res != PMD_OK)
 //    {
 //        pmdGetLastError (hnd, err, 128);
 //        fprintf (stderr, "Could not close the connection %s\n", err);

 //        return res;
 //    }

 //    printf ("\n Camera closed Successfully");

 //    printf ("\n -----------------------------------------------------------------------------");
	// printf ("\n---------------Connecting to camera 1 at a specific application ID------------");
 //    printf ("\n -----------------------------------------------------------------------------");

 //    printf ("\n Connecting to camera 1 at application ID 1 : \n");
 //    // connect to camera at a specific application ID
 //    // If no application ID is provided in the source param, then, SDK creates a new
 //    // application. If application ID is provided, SDK will connect at the given ID.
 //    // If no application is found at given ID, the camera generates xmlrpc error 
 //    // resulting in pmdOpen function to fail.
 //    // For this sample code to work, camera should contain an application at index 1.
 //    res = pmdOpen (&hnd, SOURCE_PLUGIN, SOURCE_PARAM_APP_ID, PROC_PLUGIN, PROC_PARAM); 
 //    if (res != PMD_OK)
 //    {
 //        fprintf (stderr, "Could not connect: \n");
 //        getchar();
 //        return res;
 //    }
 //    printf ("\n DONE");

	// printf ("\n -----------------------------------------------------------------------------");
 //    printf ("\n -----------------------------------------------------------------------------");
 //    printf ("\n Updating the framedata from camera ");
 //    res = pmdUpdate (hnd); // to update the camera parameter and framedata
 //    if (res != PMD_OK)
 //    {
 //        pmdGetLastError (hnd, err, 256);
 //        fprintf (stderr, "Could not updateData: \n%s\n", err);
 //        pmdClose (hnd);
	// 	printf("Camera Connection Closed. \n");
 //        getchar();
 //        return res;
 //    }
 //    printf ("\n DONE");
    
 //    printf ("\n -----------------------------------------------------------------------------");
 //    printf ("\n Retrieving source data description\n");
 //    res = pmdGetSourceDataDescription (hnd, &dd);
 //    if (res != PMD_OK)
 //    {
 //        pmdGetLastError (hnd, err, 128);
 //        fprintf (stderr, "Could not get data description: \n%s\n", err);
 //        pmdClose (hnd);
	// 	printf("Camera Connection Closed. \n");
 //        getchar();
 //        return res;
 //    }
 //    printf ("\n DONE");

 //    printf ("\n -----------------------------------------------------------------------------");
 //    res = pmdGetIntegrationTime (hnd, &integrationTime , 0);
 //    if (res != PMD_OK)
 //    {
 //        pmdGetLastError (hnd, err, 128);
 //        fprintf (stderr, "Could not set the integration time:\n%s\n", err);
 //        pmdClose (hnd);
	// 	printf("Camera Connection Closed. \n");
 //        getchar();
 //        return res;
 //    }
 //    printf("Integration Time = %d \n", integrationTime);
 //    printf ("\n DONE");

 //    printf ("\n -----------------------------------------------------------------------------");
 //    printf ("\n Closing the connection \n");

 //    res = pmdClose (hnd);
 //    if (res != PMD_OK)
 //    {
 //        pmdGetLastError (hnd, err, 128);
 //        fprintf (stderr, "Could not close the connection %s\n", err);

 //        return res;
 //    }

 //    printf ("\n Camera closed Successfully");
 //    printf ("\n ================================================================================");

/// save intensity
	if (saveIntensity)
	{
		for (int ii = 0; ii < intensity.size(); ++ii)
		{
			string result;          // string which will contain the result
			ostringstream convert;   // stream used for the conversion
			convert << ii;      // insert the textual representation of 'Number' in the characters in the stream
			result = convert.str(); // set 'Result' to the contents of the stream

			string filename = "c:\\Quincy\\IFM\\IFM_Camera\\Data\\intensity_wall_";
			filename = filename + result + ".txt";

			// ofstream myfile("c:\\temp\\point_cloud.txt");
			ofstream myfile(filename);
			if (myfile.is_open())
			{
				// for (int ii = 0; ii < intensity.size(); ++ii)
				// {
				//     myfile << intensity[ii];
				//     myfile << "\n";
				// }
				for (int hh = 0; hh < imgHeight; ++hh)
				{
					for (int ww = 0; ww < imgWidth; ++ww)
					{
						myfile << intensity[ii][ww + hh*imgWidth];
						myfile << " ";
					}
					myfile << "\n";
				}
			}

		}
	}

// save point clouds
	if (savePointCloud)
	{
		for (int ff = 0; ff < xyz3Dcoordinate.size(); ++ff)
		{
			// int numPoints = xyz3Dcoordinate[ff].size()/3;
			// cout << "************** number of points = " << numPoints << " **************" << endl;

			string result;          // string which will contain the result
			ostringstream convert;   // stream used for the conversion
			convert << ff;      // insert the textual representation of 'Number' in the characters in the stream
			result = convert.str(); // set 'Result' to the contents of the stream

			string filename = "c:\\Quincy\\IFM\\IFM_Camera\\Data\\point_cloud_wall_";
			filename = filename + result + ".txt";

			// ofstream myfile("c:\\temp\\point_cloud.txt");
			ofstream myfile(filename);
			if (myfile.is_open())
			{
				for (int ii = 0; ii < xyz3Dcoordinate[ff].size(); ii += 3)
				{
					myfile << xyz3Dcoordinate[ff][ii];
					myfile << " ";
					myfile << xyz3Dcoordinate[ff][ii + 1];
					myfile << " ";
					myfile << xyz3Dcoordinate[ff][ii + 2];
					myfile << "\n";
				}
			}
		}
	}

	// save distance
	if (saveDistances)
	{
		for (int ii = 0; ii < distances.size(); ++ii)
		{
			string result;          // string which will contain the result
			ostringstream convert;   // stream used for the conversion
			convert << ii;      // insert the textual representation of 'Number' in the characters in the stream
			result = convert.str(); // set 'Result' to the contents of the stream

			string filename = "c:\\Quincy\\IFM\\IFM_Camera\\Data\\distances_wall_";
			filename = filename + result + ".txt";

			// ofstream myfile("c:\\temp\\point_cloud.txt");
			ofstream myfile(filename);
			if (myfile.is_open())
			{
				// for (int ii = 0; ii < intensity.size(); ++ii)
				// {
				//     myfile << intensity[ii];
				//     myfile << "\n";
				// }
				for (int hh = 0; hh < imgHeight; ++hh)
				{
					for (int ww = 0; ww < imgWidth; ++ww)
					{
						myfile << distances[ii][ww + hh*imgWidth];
						myfile << " ";
					}
					myfile << "\n";
				}
			}

		}
	}
    return PMD_OK;
}