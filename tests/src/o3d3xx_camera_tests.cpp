#include "o3d3xx_tests.hpp"

TEST (O3D3XX, O3D3XXConnectToCamera_testPositive)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;
    uint32_t status_code = 0;

    /*As this is positive test, we expect it to execute successfully with any errors */
    EXPECT_NO_THROW (status_code = cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();
    EXPECT_EQ (ERR_NONE, status_code);

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX, O3D3XXSetIntegrationTime_testPositive)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    uint32_t integrationTime = 2000;

    /** Setting integration time of camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.setIntegrationTime (integrationTime, 0)) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX, O3D3XXGetIntegrationTime_testPositive)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    uint32_t integrationTime = 0;

    /** Getting integration time of camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.getIntegrationTime (&integrationTime , 0)) << cameraOperations.getLastError();
    EXPECT_GE ((uint32_t)MAX_INTEGRATION_TIME, integrationTime) << cameraOperations.getLastError();
    EXPECT_LE ((uint32_t)MIN_INTEGRATION_TIME, integrationTime) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX, O3D3XXGetResolution_testPositive)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    /** Get resolution of camera*/
    uint32_t frameWidth = 0;
    uint32_t frameHeight = 0;

    /** Getting integration time of camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.getResolution (&frameWidth, &frameHeight)) << cameraOperations.getLastError();
    EXPECT_EQ (FRAME_WIDTH, frameWidth) << cameraOperations.getLastError();
    EXPECT_EQ (FRAME_HEIGHT, frameHeight) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX, O3D3XXDataConfiguration_testPositive)
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
    std::string command = "SetSourceData";
	std::string value = "6"; // Only Amplitude, Distance data enabled. 
	std::string  response = "";

    /** Set the filter parameter on camera */
	EXPECT_EQ (ERR_NONE, cameraOperations.setImagerSettings (command, value, response)) << cameraOperations.getLastError();
	EXPECT_EQ ("OK", response) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX, O3D3XXDisconnectPCIC_testPositive)
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
    std::string command = "DisconnectPCIC";
	EXPECT_EQ (ERR_NONE, cameraOperations.disconnectPCIC()) << cameraOperations.getLastError();
		
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX, O3D3XXConnectPCIC_testPositive)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

	/** Discconnect from PCIC before testing for "ConnectPCIC" command*/
    std::string command = "DisconnectPCIC";
	EXPECT_EQ (ERR_NONE, cameraOperations.disconnectPCIC()) << cameraOperations.getLastError();

    /** Setting the command and value properties*/
    command = "ConnectPCIC";
	EXPECT_EQ (ERR_NONE, cameraOperations.connecttoPCIC()) << cameraOperations.getLastError();
		
    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

TEST (O3D3XX, O3D3XXDisconnectCamera_testPositive)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}


/************* Negative Test Cases ******************/
TEST (O3D3XX, O3D3XXConnectToCamera_testNegative)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;
    string ip_address = "";
    uint16_t xmlrpc_port = 0;
    uint16_t pcic_port = 0;

    /****** Validating ip address *******/
    /*Validating length of ip address */
    ip_address = "192.168.0.643439";
    xmlrpc_port = 80;
    pcic_port = 50010;
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    /*Validating the octect of the ip address. It should be in range 0 to 255 */
    ip_address = "555.555.555.555";
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    /*Validating the ip address should not be a reserved one. "0.0.0.0" cannot be a real device ip address */
    ip_address = "0.0.0.0";
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    /*Validating that ip address should have exactly four dots/decimals */
    ip_address = "192.168.069";
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    /*Validating that a xmlrpc port number should not be 0 */
    ip_address = CAMERA_IP;
    xmlrpc_port = 0;
    pcic_port = 50010;
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    /*Validating that a pcic port number should not be 0 */
    ip_address = CAMERA_IP;
    xmlrpc_port = 80;
    pcic_port = 0;
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    ///*Check that when camera is not connected, error reflects it*/
    //ip_address = CAMERA_IP;
    //xmlrpc_port = 80;
    //pcic_port = 50010;
    //EXPECT_EQ (ERR_DEVICE_NOT_CONNECTED, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port));
}

TEST (O3D3XX, O3D3XXSetIntegrationTime_testNegative)
{
    /* Create an object of O3D3xx camera class, which allows to perform basic
    functions on camera*/
    O3d3xxCameraOperations cameraOperations;

    string ip_address = CAMERA_IP;
    uint16_t xmlrpc_port = 80;
    uint16_t pcic_port = 50010;

    /*As this is positive test, we expect it to execute successfully with any errors */
    ASSERT_EQ (ERR_NONE, cameraOperations.connectToCamera (ip_address, xmlrpc_port, pcic_port)) << cameraOperations.getLastError();

    uint32_t integrationTime = 2000;

    /** Setting integration time of camera */
    EXPECT_EQ (ERR_INVALID_ARG, cameraOperations.setIntegrationTime (integrationTime, 3)) << cameraOperations.getLastError();

    /*Disconnect from camera */
    EXPECT_EQ (ERR_NONE, cameraOperations.disconnectFromCamera()) << cameraOperations.getLastError();
}

