#include "o3d3xx_tests.hpp"

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetTriggerMode_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	{
		/** Setting the command and value properties*/
		char command[] = "SetTriggerMode 2"; // Trigger := Process Interface
		/** Setting trigger source of camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setTriggerMode (command)) << cameraOperations.getLastError();
	}
    
	{
		/** Setting the command and value properties*/
		char command[] = "SetTriggerMode 3"; // Trigger := Positive Edge
		/** Setting trigger source of camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setTriggerMode (command)) << cameraOperations.getLastError();
	}

	{
		/** Setting the command and value properties*/
		char command[] = "SetTriggerMode 4"; // Trigger := Negative Edge
		/** Setting trigger source of camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setTriggerMode (command)) << cameraOperations.getLastError();
	}

	{
		/** Setting the command and value properties*/
		char command[] = "SetTriggerMode 5"; // Trigger := Positive and Negative Edge
		/** Setting trigger source of camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setTriggerMode (command)) << cameraOperations.getLastError();
	}

	{
		/** Setting the command and value properties*/
		char command[] = "SetTriggerMode 1"; // Trigger := Continuous
		/** Setting trigger source of camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setTriggerMode (command)) << cameraOperations.getLastError();
	}

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetUnambiguousDistanceRange_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetUnambiguousDistanceRange"; 
	std::string value = "1"; // "upto30m"
	std::string response;
	/** Setting unambiguous range of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();
    
	/** Setting the command and value properties*/
    value = "2"; // "morethan30m"
	response = "";
	/** Setting unambiguous range of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();
	
	/** Getter Function Test */
	std::string command_get = "GetImagerSettingType";
	std::string reply;
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command_get, reply)) << cameraOperations.getLastError();
	std::string distance_range_str = reply.substr (0, reply.find ('_') );
	EXPECT_EQ ("morethan30m", distance_range_str) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    value = "0"; // "upto5m"
	response = "";
	/** Setting unambiguous range of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();

	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetExposureMode_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetExposureMode"; 
	std::string value = "1"; // "moderate"
	std::string response;
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();

	/** Getter Function Test */
	std::string command_get = "GetImagerSettingType";
	std::string reply;
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command_get, reply)) << cameraOperations.getLastError();
	std::string exposure_mode_str = reply.substr (reply.find ('_') + 1, reply.length());
	EXPECT_EQ ("moderate", exposure_mode_str) << cameraOperations.getLastError();
    
	/** Setting the command and value properties*/
    value = "2"; // "high"
	response = "";
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    value = "0"; // "low"
	response = "";
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();

	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}


TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetExposureTimeRatio_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetExposureMode"; 
	std::string value = "1"; // "moderate"
	std::string response;
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();

	command = "SetExposureTimeRatio";
    value = "20";
    response = "";
	/** Setting unambiguous range of camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    
    command = "SetExposureMode";
	value = "0"; // "low"
	response = "";
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();
	
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXOptimize_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	uint32_t integrationTime = 10000;
	/** Setting unambiguous range of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setIntegrationTime (integrationTime, 0)) << cameraOperations.getLastError();

	std::string response;
	/** Call Optimize function */
	EXPECT_EQ (ERR_NONE, cameraOperations.optimize (response)) << cameraOperations.getLastError();

	/** Getting integration time of camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.getIntegrationTime (&integrationTime , 0)) << cameraOperations.getLastError();
    EXPECT_GE (10000, integrationTime) << cameraOperations.getLastError();
    EXPECT_LE (0, integrationTime) << cameraOperations.getLastError();

	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetModulationFrequencyChannel_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	uint32_t modFreqChannel = 2;
	/** Setting modulation frequeny channel of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setModulationFrequencyChannel (modFreqChannel)) << cameraOperations.getLastError();

	/** Get modulation frequeny channel  */
	EXPECT_EQ (ERR_NONE, cameraOperations.getModulationFrequencyChannel (&modFreqChannel)) << cameraOperations.getLastError();
	EXPECT_EQ (2, modFreqChannel) << cameraOperations.getLastError();

	modFreqChannel = 3;
	/** Setting modulation frequeny channel of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setModulationFrequencyChannel (modFreqChannel)) << cameraOperations.getLastError();

	modFreqChannel = 1;
	/** Setting modulation frequeny channel of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setModulationFrequencyChannel (modFreqChannel)) << cameraOperations.getLastError();

	modFreqChannel = 0;
	/** Setting modulation frequeny channel of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setModulationFrequencyChannel (modFreqChannel)) << cameraOperations.getLastError();

	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetTargetFrameRate_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetTargetFrameRate"; 
	std::string value = "25.0"; // "moderate"
	std::string response;
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

	value = "5.0"; // "low"
	response = "";
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetImageResolution_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetImageResolution"; 
	std::string value = "1"; // "352 X 264"
	std::string response;
	/** Setting resolution of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    uint32_t frameWidth = 0;
    uint32_t frameHeight = 0;
    /** Getting resolution of camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.getResolution (&frameWidth, &frameHeight)) << cameraOperations.getLastError();
    EXPECT_EQ (FRAME_WIDTH_FULL, frameWidth) << cameraOperations.getLastError();
    EXPECT_EQ (FRAME_HEIGHT_FULL, frameHeight) << cameraOperations.getLastError();

	value = "0"; // "176 X 132"
	response = "";
	/** Setting resolution of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    EXPECT_EQ (ERR_NONE, cameraOperations.getResolution (&frameWidth, &frameHeight)) << cameraOperations.getLastError();
    EXPECT_EQ (FRAME_WIDTH, frameWidth) << cameraOperations.getLastError();
    EXPECT_EQ (FRAME_HEIGHT, frameHeight) << cameraOperations.getLastError();
	
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetClippingCuboid_testPositive)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetClippingCuboid"; 
	std::string value = "{\"XMin\": -10, \"XMax\": 10, \"YMin\": -20, \"YMax\": 20, \"ZMin\": 0, \"ZMax\": 30}";
	std::string response;
	/** Setting clipping cuboid of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    /** Getting clipping cuboid of camera */
    command = "GetClippingCuboid"; 
    response = "";
    EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command, response)) << cameraOperations.getLastError();
    EXPECT_STREQ(value.c_str(), response.c_str());
    
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

/***** Negative Cases **************/
TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetTriggerMode_testNegative)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	{
		/** Setting the command and value properties*/
		char command[] = "SetTriggerMode 10"; // Trigger := Process Interface
		/** Setting trigger source of camera */
		EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setTriggerMode (command)) << cameraOperations.getLastError();
	}
    
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetUnambiguousDistanceRange_testNegative)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetUnambiguousDistanceRange"; 
	std::string value = "7"; // "upto30m"
	std::string response;
	/** Setting unambiguous range of camera */
	EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();
    
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetExposureMode_testNegative)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetExposureMode"; 
	std::string value = "5"; // "moderate"
	std::string response;
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();
    
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}


TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetExposureTimeRatio_testNegative)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetExposureMode"; 
	std::string value = "1"; // "moderate"
	std::string response;
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();

	command = "SetExposureTimeRatio"; 
	value = "200"; // "moderate"
    response = "";
	/** Setting unambiguous range of camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    command = "SetExposureMode"; 
	value = "0"; // "low"
	response = "";
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();
	
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetModulationFrequencyChannel_testNegative)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	uint32_t modFreqChannel = 5;
	/** Setting unambiguous range of camera */
	EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setModulationFrequencyChannel (modFreqChannel)) << cameraOperations.getLastError();

	/** Change unambiguous distance range to "morethan30m" */
	/** Setting the command and value properties*/
    std::string command = "SetUnambiguousDistanceRange"; 
	std::string value = "2"; // "morethan30m"
	std::string response;
	/** Setting unambiguous range of camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerType (command, value, response)) << cameraOperations.getLastError();

	modFreqChannel = 2;
	/** Setting unambiguous range of camera */
	EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setModulationFrequencyChannel (modFreqChannel)) << cameraOperations.getLastError();

	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetTargetFrameRate_testNegative)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetTargetFrameRate"; 
	std::string value = "35.0"; // "moderate"
	std::string response;
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

	value = "0.010"; // "low"
	response = "";
	/** Setting exposure mode of camera */
	EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_IMAGER_SETTINGS, O3D3XXSetImageResolution_testNegative)
{
	/* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    std::string command = "SetImageResolution"; 
	std::string value = "2"; // "352 X 264"
	std::string response;
	/** Setting resolution of camera */
	EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    value = "12";
    /** Setting resolution of camera */
	EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	
	/*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}