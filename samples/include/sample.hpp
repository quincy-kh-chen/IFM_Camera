#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <pmdsdk2.h>

// on error, prepend absolute path to files before plugin names
#define SOURCE_PLUGIN "O3D3xxCamera"

// Source Params string format:
// Ip Address : port Number : XmlRPC port : Application ID (Optional)
// If no application ID is provided in source params, a new application 
// is created on connect, and deleted at disconnect.
//#define CAMERA_IP_1 "192.168.0.69"
//#define CAMERA_IP_1 "169.254.48.238"
#define CAMERA_IP_1 "169.168.0.69"
#define CAMERA_IP_2 "192.168.0.71"
#define CAMERA_IP_3 "192.168.0.72"
#define PCIC_PORT_NUMBER "80"
#define XMLRPC_PORT_NUMBER "50010"
#define APPLICATION_ID "1"
#define SOURCE_PARAM CAMERA_IP_1 ":" PCIC_PORT_NUMBER ":" XMLRPC_PORT_NUMBER 
#define SOURCE_PARAM_2 CAMERA_IP_2 ":" PCIC_PORT_NUMBER ":" XMLRPC_PORT_NUMBER 
#define SOURCE_PARAM_3 CAMERA_IP_3 ":" PCIC_PORT_NUMBER ":" XMLRPC_PORT_NUMBER 
#define SOURCE_PARAM_APP_ID CAMERA_IP_1 ":" PCIC_PORT_NUMBER ":" XMLRPC_PORT_NUMBER ":" APPLICATION_ID 

#define PROC_PLUGIN "O3D3xxProc"
#define PROC_PARAM ""

// Define length of buffer for response from source command
#define SOURCE_CMD_BUFFER_LENGTH 256
// Define length for Clipping Cuboid String
#define CLIPPING_CUBOID_STRING_LENGTH 1024

// Sample code for basic functionalities like get distance, amplitude, XYZ, etc.
int o3d3xx_sample_basic_functionalities(void);
// Sample code containing code to set/get filter functionalitiees in camera.
int o3d3xx_sample_filter_function(void);
// Sample code containing code to set/get imager setting parameters.
int o3d3xx_sample_imager_settings_function(void);
// Sample code to connect to multiple cameras using SDK.
int o3d3xx_sample_multicamera_connect(void);
// Sample code to change data stream configuration from camera.
int o3d3xx_sample_data_configuration(void);
// Sample code to send pcic commands to camera and receive response.
int o3d3xx_sample_pcic_commands(void);