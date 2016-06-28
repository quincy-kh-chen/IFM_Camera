#include "o3d3xx_tests.hpp"

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableFilterAmplitudeImage_testPositive)
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
    std::string command = "EnableFilterAmplitudeImage";
	{
		std::string value = "false";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}

	/** Getter Function Test */
	std::string command_get = "GetAmplitudeImageFilter";
	std::string reply;
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command_get, reply)) << cameraOperations.getLastError();
	EXPECT_EQ ("false", reply) << cameraOperations.getLastError();

	{
		std::string value = "true";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableFilterDistanceImage_testPositive)
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
    std::string command = "EnableFilterDistanceImage";
	std::string value = "false";
	std::string  response = "";

	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	
	/** Getter Function Test */
	command = "GetDistanceImageFilter";
	response = "";
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("false", response) << cameraOperations.getLastError();
	
	command = "EnableFilterDistanceImage";
	value = "true";
	response = "";
	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXSetSpatialFilterType_testPositive)
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
    std::string command = "SetSpatialFilterType";
	{ // Testing for Mean Filter
		std::string value = "1";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
	
	/** Getter Function Test */
	std::string command_get = "GetSpatialFilterType";
	std::string reply;
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command_get, reply)) << cameraOperations.getLastError();
	EXPECT_EQ ("1", reply) << cameraOperations.getLastError();
	
	{ // Testing for Median Filter
		std::string value = "2";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
	{ // Testing for Bilateral Filtering
		std::string value = "3";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
	{ // Testing for Filtering "OFF"
		std::string value = "0";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXSetSpatialFilterMaskSize_testPositive)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    /** Turn Spatial Filter ON. Required to be done to enable SetSpatialFilterMaskSize command */
	std::string command = "SetSpatialFilterType";
    std::string value = "1";
	std::string  response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

	/** Setting the command and value properties*/
    command = "SetSpatialFilterMaskSize";

	// Testing for 5X5 Mask 
    value = "1";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

	/** Getter Function Test */
	std::string command_get = "GetSpatialFilterMaskSize";
	std::string reply;
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command_get, reply)) << cameraOperations.getLastError();
	EXPECT_EQ ("1", reply) << cameraOperations.getLastError();
	
	// Testing for 3X3 Mask
	value = "0";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXTemporalFilterType_testPositive)
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
    std::string command = "SetTemporalFilterType";
	{ // Testing for Temporal Mean Filter
		// Put camera in process interface trigger mode. The Temporal mean filter is not available in free run mode.
		ASSERT_EQ (ERR_NONE, cameraOperations.setTriggerMode ("SetTriggerMode 2")) << cameraOperations.getLastError();

		std::string value = "1";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

		// Put camera in continuous (free run) trigger mode again.
		ASSERT_EQ (ERR_NONE, cameraOperations.setTriggerMode ("SetTriggerMode 1")) << cameraOperations.getLastError();
	}
    { // Testing for Temporal Adaptive Exponential
		std::string value = "2";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}

	/** Getter Function Test */
	std::string command_get = "GetTemporalFilterType";
	std::string reply;
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command_get, reply)) << cameraOperations.getLastError();
	EXPECT_EQ ("2", reply) << cameraOperations.getLastError();

	{ // Testing for Temporal Filtering off
		std::string value = "0";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableAmplitudeCorrection_testPositive)
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
    std::string command = "EnableAmplitudeCorrection";
	std::string value = "false";
	std::string  response = "";

	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	
	/** Getter Function Test */
	command = "GetAmplitudeCorrection";
	response = "";
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("false", response) << cameraOperations.getLastError();
	
	command = "EnableAmplitudeCorrection";
	value = "true";
	response = "";
	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableRectificationDistanceImage_testPositive)
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
    std::string command = "EnableRectificationDistanceImage";
	std::string value = "false";
	std::string  response = "";

	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	
	/** Getter Function Test */
	command = "GetRectificationDistanceImage";
	response = "";
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("false", response) << cameraOperations.getLastError();
	
	command = "EnableRectificationDistanceImage";
	value = "true";
	response = "";
	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableRectificationAmplitudeImage_testPositive)
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
    std::string command = "EnableRectificationAmplitudeImage";
	std::string value = "false";
	std::string  response = "";

	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	
	/** Getter Function Test */
	command = "GetRectificationAmplitudeImage";
	response = "";
	EXPECT_EQ (ERR_NONE, cameraOperations.getImagerSettings (command, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("false", response) << cameraOperations.getLastError();
	
	command = "EnableRectificationAmplitudeImage";
	value = "true";
	response = "";
	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}



/******** Negative Test Cases *******************/
TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableFilterAmplitudeImage_testNegative)
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
    std::string command = "EnableFilterAmplitudeImage";
    std::string value = "tr";
	std::string  response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    
	value = "1";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableFilterDistanceImage_testNegative)
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
    std::string command = "EnableFilterDistanceImage";
    std::string value = "tr";
	std::string  response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    
	value = "1";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXSetSpatialFilterType_testNegative)
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
    std::string command = "SetSpatialFilterType";
    std::string value = "4";
	std::string  response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    //EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

	value = "10";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    //EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXSetTemporalFilterType_testNegative)
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
    std::string command = "SetTemporalFilterType";
    std::string value = "5";
	std::string  response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    //EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

	value = "25";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    //EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

	// Testing for Temporal Mean Filter in Continuous mode
	value = "1";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_FUNCTION, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    //EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

	// Testing for Temporal adaptive exponential filter in Process interval trigger mode
	value = "2";
	response = "";

	// Put camera in process interface trigger mode. The Temporal adaptive exponential filter is not available in free run mode.
	ASSERT_EQ (ERR_NONE, cameraOperations.setTriggerMode ("SetTriggerMode 2")) << cameraOperations.getLastError();

	/** Set the filter parameter on camera */
	EXPECT_EQ (ERR_INVALID_FUNCTION, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

	// Put camera back in continuous trigger mode.
	ASSERT_EQ (ERR_NONE, cameraOperations.setTriggerMode ("SetTriggerMode 1")) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXSetSpatialFilterMaskSize_testNegative)
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
    std::string command = "SetSpatialFilterMaskSize";
    {
		std::string value = "5";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		//EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
	{
		std::string value = "1";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_INVALID_FUNCTION, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		//EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXSetTempMeanFilterNumberOfImages_testNegative)
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
    std::string command = "SetTempMeanFilterNumberOfImages";
	{
		std::string value = "50";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		//EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
	{
		std::string value = "3";
		std::string  response = "";

		/** Set the filter parameter on camera */
		EXPECT_EQ (ERR_INVALID_FUNCTION, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
		//EXPECT_EQ ("OK", response) << cameraOperations.getLastError();
	}
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();	
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableAmplitudeCorrection_testNegative)
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
    std::string command = "EnableAmplitudeCorrection";
    std::string value = "tr";
	std::string  response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    
	value = "1";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableRectificationDistanceImage_testNegative)
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
    std::string command = "EnableRectificationDistanceImage";
    std::string value = "tr";
	std::string  response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    
	value = "1";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX_FILTER_TESTS, O3D3XXEnableRectificationAmplitudeImage_testNegative)
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
    std::string command = "EnableRectificationAmplitudeImage";
    std::string value = "tr";
	std::string  response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
    
	value = "1";
	response = "";

    /** Set the filter parameter on camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

