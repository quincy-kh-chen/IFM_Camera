#include "sample.hpp"

int main (int argc, char **argv)
{
    int res = 0;
    bool sampleBasicRun = true, sampleFilterRun = false, sampleImagerSettingRun = false; 
    bool sampleMultiCameraRun = false, sampleDataConfigRun = false, samplePCICRun = false; 
    // if(argc == 1)
    // {
    //     sampleBasicRun = true;
    //     sampleFilterRun = true;
    //     sampleImagerSettingRun = true;
    //     sampleMultiCameraRun = true;
    //     sampleDataConfigRun = true;
    //     samplePCICRun = true;
    // }
    // for (int i = 1; i < argc; i++) 
    // {
    //     std::string argument = argv[i];
    //     if(argument == "--BASIC")
    //         sampleBasicRun = true;
    //     else if(argument == "--FILTER")
    //         sampleFilterRun = true;
    //     else if(argument == "--IMAGER_SETTINGS")
    //         sampleImagerSettingRun = true;
    //     else if(argument == "--MULTI_CAMERA")
    //         sampleMultiCameraRun = true;
    //     else if(argument == "--DATA_CONFIG")
    //         sampleDataConfigRun = true;
    //     else if(argument == "--PCIC_COMMANDS")
    //         samplePCICRun = true;

    // }
    
    if(sampleBasicRun)
    {
        res = o3d3xx_sample_basic_functionalities();
        if(res != PMD_OK)
            return res;
    }
	/*
	  -----------------------Calling Image Filter Functionality----------------
	*/
    if(sampleFilterRun)
    {
	    res = o3d3xx_sample_filter_function();
        if(res != PMD_OK)
            return res;
    }
	/*
      -----------------------Calling Imager Settings Functionalities-----------
	*/
    if(sampleImagerSettingRun)
    {
	    res = o3d3xx_sample_imager_settings_function();
        if (res != PMD_OK)
            return res;
    }
    /*
	  -----------------------Multi-camera connection-----------
	*/
    if(sampleMultiCameraRun)
    {
	    res = o3d3xx_sample_multicamera_connect();
        if (res != PMD_OK)
            return res;
    }

    /*
	  -----------------------Data Stream Configuration-----------
	*/
    if(sampleDataConfigRun)
    {
	    res = o3d3xx_sample_data_configuration();
        if (res != PMD_OK)
            return res;
    }

    if(samplePCICRun)
    {
	    res = o3d3xx_sample_pcic_commands();
        if (res != PMD_OK)
            return res;
    }

    printf("\nSample Code Execution Done. Press any key to exit....\n");
    //getchar();
	return 0;
}