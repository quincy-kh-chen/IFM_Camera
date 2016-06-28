/** @file
@brief This file represents the definition of class which contains functions to
operate the O3D3xx camera.

@author Winy Kakani\n\n
@Version: 1.0\n\n

Copyright (C) 2014 ifm electronic GmbH\n\n
See accompanied file LICENCE.TXT for license information.
*/

/*! \mainpage O3D3xx Camera Plugin

* \section introduction Introduction
* This source plugin is developed to communicate with O3D3xx camera.
* Commands are invoked via XMLRpc interface
* ONLY Windows 32 and 64 bit OS platforms are supported.
*
*\section prerequisites Prerequisites
* To connect to camera user needs following information.
*   - IP address of camera.
*   - Tcp port number of camera for xmlrpc communications.
*   - Tcp port of camera for pcic communication for getting
*    connected to camera.
*   .
*
* Also, user shall be aware of the valid range of integration time
* that can be set on the camera.
*
* \section operational_details Operational Details
* The camera is assumed to be running
* in free run mode, i.e. it is constantly capturing frames. There are two
* tcp ports on the camera. One tcp port is for
* xmlrpc communication. The other port is known as pcic tcp port, on which
* camera broadcasts the frame data to the connected client. At a time only
* one client should be connected to camera.\n\n
* This plugin provides functions to,
*   - Connect to camera.
*   - Get the resolution of camera.
*   - Get the integration time of camera.
*   - Set the integration time of camera.
*   - Get the size of frame in bytes.
*   - Get a frame from camera.
*   - Disconnect from camera.
*   .
*
*\section usage Usage
*
*\subsection sample_c_code Sample C code - shows usage of class functions via C code.
*

*Steps to use sample code:
*   -# If your camera ip is 192.168.0.69, xmlrpc port is 80 and pcic port is 50010, then directly go to step 4.
*   -# In solution window, open file "o3d3xx_camera_sample_usage_code.cpp" listed under "SampleO3D3xxCamera" project.
*   -# Steps to change default camera ip address, xmlrpc port number and pcic port number
*       -# If your camera ip address is not 192.168.0.69,
*       find following line of code,
*       #define IP_ADDRESS_OF_CAMERA "192.168.0.69"
*       Replace 192.168.0.69 with your camera's ip address.
*
*       -# If your xmlrpc port number is not 80, then
*       Find following line of code,
*       #define XMLRPC_PORT 80
*       Replace "80" with your xmlrpc port number.
*
*       -# If your pcic port number is not 50010, then
*       Find following line of code,
*       #define PCIC_PORT 50010
*       Replace "50010" with your pcic port number.
*       Save the solution.
*   -# Compile the solution.
*   -# After successful compilation of the project, the executable for the sample C code shall be generated as
*   "SampleO3D3xxCamera.exe" at "build/bin/<configuration_folder>".<configuration_folder> denotes the configuration
*   by which you build the solution.
*   Double click the executable. It shall demonstrate configuration of camera and grabbing of frames and image data.
*
*\subsection lightVis LightVis - shows usage of O3D3xx plugin via third party application.
*
* Steps to use LightVis to communicate with camera:
*   -# Compile the solution.
*   -# After, successful compilation of the project, open "build/bin/<configuration_folder>" folder.
*   <configuration_folder> denotes the configuration by which you built the solution.
*   -# If your camera ip is 192.168.0.69, xmlrpc port is 80 and pcic port is 50010, then directly go to step 5.
*   -# The "build/bin/<configuration_folder>" folder contains "lv.cfg" file.
*   Open "lv.cfg" file in editor. This is configuration file of LightVis. It gives the details of camera ip address,
*   xmlrpc port, pcic port, source plugin binary name and processing plugin binary name to LightVis.
*
*   -# Find following line in "lv.cfg" file,\n
*   "192.168.0.69:80:50010".
*       -# If your camera ip address is not 192.168.0.69,
*       Replace 192.168.0.69 with your camera's ip address.
*
*       -# If your xmlrpc port number is not 80, then
*       Replace "80" with your xmlrpc port number.
*
*       -# If your pcic port number is not 50010, then
*       Replace "50010" with your pcic port number.
*       Save the file.
*
*       Suppose your camera ip address is 192.168.0.1, xmlrpc port is "8080" and pcic port is "2000", then the "lv.cfg"
*       file should have\n
*       192.168.0.1:8080:2000
*   -# Double click on "LightVis.exe". This is the LightVis executable.
*   -# LightVis shall show images captured from camera.Also integration time can be configured via LightVis.
*/
#include <xmlrpc-c/client.h> // headers of our xmlrpc client library
#include <xmlrpc-c/base.h>
#include "config.h"
#include "o3d3xx_camera.hpp" // The xmlrpc wrapper class implementing basic camera operations

#include <iostream>
#include "clientsocket.hpp"
#include "err.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

/// @cond NOT_TO_BE_EXPOSED
/** Maximum length of session identifier*/
#define MAX_SESSION_ID_LENGTH 40
/** Url specifying root bject of O3D3xx camera interface */
#define ROOT_OBJECT_URL "/api/rpc/v1/com.ifm.efector/"

/* Macros for symbolizing edit and run mode of camera */
#define EDIT_MODE 1
#define RUN_MODE 0
/// @endcond

/** Default integration time in [us] */
#define DEFAULT_INTEGRATION_TIME 1000

/**Default frame rate of camera*/
#define DEFAULT_FRAME_RATE 25

#define DEFAULT_PROTOCOL_VERSION "3"

/*Maximum length of ip address */
#define MAX_IP_ADDRESS_LENGTH 15

/* Maximum number of Application supported by camera */
#define MAX_NUMBER_OF_APPLICATION_SUPPORTED 32

#define XMLRPCCHECK()\
{\
    int return_code = checkIfFaultOccurred(); \
    if(  return_code != 0 ) \
        { \
        return return_code;\
        } \
}

/// @cond NOT_TO_BE_EXPOSED

/** @struct
@brief Structure defining private data of this class
*/
struct O3d3xxCameraOperations::PrivateData
{
    PrivateData()
    {
        cameraXmlrpcPortNumber = HTTP_PORT_NUMBER;
        sessionUrl = "";
        applicationId = -1;
        isConnected = false;
        frameSizeInBytes = 0;
        isApplicationCreated = false;
        triggeredMode = 0;
        isConfigurationModeON = false;
        isEditModeON = false;
        protocolVesrion = "";
        previousActiveApplicationId = 0;
        imageHeight = 0;
        imageWidth = 0;
    }

    /**If a session is active, this url should be containing the
       session's url, else it should be empty, indicating that
       there is no active session.
       */
    string sessionUrl;

    /** This is id of application, which we create on camera.
        Parameters are configured at the application represented by this ID.
        Valid ID range: 0 to 32, -1 indicats an invalid ID.
        */
    int applicationId;

    /** This is id of previous active application, which was active on camera.
        Valid ID range: 1 to 32, 0 indicats no active application.
        */
    int previousActiveApplicationId;

    /** IP address of camera */
    string cameraIpAddress;

    /** Port number for XML-RPC communication */
    uint16_t cameraXmlrpcPortNumber;

    /** Port number of frame data (Protocol: PCIC) */
    uint16_t cameraImageOutputPortNumber;

    /** XML-RPC represents runtime errors as <fault> elements. These contain
    <faultCode> and <faultString> elements. During calling of xmlrpc functions,
    any error occurs, its details are logged in this variable. In other words,
    this variable keeps track of last occurred error */
	static xmlrpc_env environment;

    /** Data size of currentFrame > */
    uint32_t frameSizeInBytes;

    /** TCP Socket >*/
    ClientSocket *clientSocket;

    /** Flag indicating established connection. 0: not connected, 1: connection Ok */
    bool isConnected;

    /** Flag indicating, if a new application has been created on the camera by this plugin.
        0: new application created, 1: use active application on connect. */
    bool isApplicationCreated ;

    /** The trigger mode of the camera */
    int triggeredMode;

    /** to check configurationMode of Application */
    bool isConfigurationModeON;

    /** to check the Edit mode of application */
    bool isEditModeON;

    /** Count to store the number of connections */
    static int noOfConnections;

    /** Default protocol version */
    std::string protocolVesrion;

    /** Buffer holding current frame's data received from camera.*/
    std::vector<char> frameData;

    /** string for define PCIC communcation */
    std::string dataChannelConfigString;

    /** Description of error/fault occurred, if any, as reported by plugin instance*/
    std::string fault_string;

    /** Image Height*/
    uint32_t imageHeight;
    
    /** Image Width*/
    uint32_t imageWidth;
};

/* Definition for static member "noOfConnections".
   Not doing this, results in linking error "undefined reference"
*/
int O3d3xxCameraOperations::PrivateData::noOfConnections;
xmlrpc_env O3d3xxCameraOperations::PrivateData::environment;

/** @namespace
@brief provides some string operation for split and toString functionality
*/
namespace stringOperations
{
    /**
    @internal
    @details
    Compilers older than C++11, do not support to_string() function of
    string class. Therefor define it here.
    @param [in]
    Value to be converted to string
    @return
    Converted string value
    */
    template <typename T>
    std::string to_string (T value)
    {
        std::ostringstream os ;
        os << value ;
        return os.str() ;
    }
    /**
    @details
    @brief splits the string and stores in a vector of strings
    @param [in]
    string with separator as space(" ")
    @return
    vector with sitrings seprated by space
    */
    std::vector<std::string> splitCommand (const char *cmd)
    {
        std::istringstream ss (cmd);
        std::vector<std::string> el;
        do
        {
            std::string s;
            ss >> s;
            if (!s.length())
            {
                continue;
            }
            el.push_back (s);
        }
        while (ss);
        return el;
    }

};

/** @namespace
@brief provides functions which convert the different xmlrpc response to strings
*/
namespace xmlrpcConversions
{
    /**
     @brief function to write different xmlrpc data types to string.
     @param [in] xmlRPC variable
     @return result string
     */
    std::string extractFromXmlRpc (xmlrpc_value *val, xmlrpc_env &env, bool quotes) throw (std::string);

    /**
    * @brief Extracts the string  value from the xmlRpc response
    * @param [in]  reference to  xmlrpc_response value
    * @param [in]  xmlrpc environment variable
    * @param [in]  true to add quotes to the string else false
    * @return string of the xmlrpc response
    **/
    std::string extractStringFromXmlRpc (const xmlrpc_value *val, xmlrpc_env &env, bool quotes = false)
    {
        std::stringstream ss;
        const char *r;
        xmlrpc_read_string (&env, val, &r);
        if (quotes)
        {
            ss << "\"";
        }
        if (!env.fault_occurred)
        {
            ss << r;
        }
        if (quotes)
        {
            ss << "\"";
        }
        //  free ( (void *) r);
        return ss.str();
    }

    /**
    * @brief Extracts the int  value from the xmlRpc response
    * @param [in]  reference to  xmlrpc_response value
    * @param [in]  xmlrpc environment variable
    * @return  int value from xmlrpc_response
    **/
    int extractIntFromXmlRpc (const xmlrpc_value *val, xmlrpc_env &env)
    {
        int res;
        xmlrpc_read_int (&env, val, &res);
        return env.fault_occurred ? 0 : res;
    }
    /**
    * @brief Extracts the bool  value from the xmlRpc sesponse
    * @param [in]  reference to  xmlrpc_response value
    * @param [in]  xmlrpc environment variable
    * @return bool value from the xmlrpc response
    **/
    xmlrpc_bool extractBoolFromXmlRpc (const xmlrpc_value *val, xmlrpc_env &env)
    {
        xmlrpc_bool res;
        xmlrpc_read_bool (&env, val, &res);
        return env.fault_occurred ? 0 : res;
    }
    /**
    * @brief Extracts the double  value from the xmlRpc sesponse
    * @param [in]  reference to  xmlrpc_response value
    * @param [in]  xmlrpc environment variable
    * @return double value from the xmlrpc response
    **/
    double extractDoubleFromXmlRpc (const xmlrpc_value *val, xmlrpc_env &env)
    {
        double res;
        xmlrpc_read_double (&env, val, &res);
        return env.fault_occurred ? 0. : res;
    }

    /**
      *@brief function to write xmlrpc arrays to string.
      *@param [in] xmlRPC variable
      *@return result string
        */
    std::string extractArrayFromXmlRpc (xmlrpc_value *val, xmlrpc_env &env)
    {
        std::string s;
        int num = xmlrpc_array_size (&env, val);
        for (int i = 0; i < num; i++)
        {
            xmlrpc_value *value = 0;
            xmlrpc_array_read_item (&env, val, i, &value);
            if (i > 0)
            {
                s += ",";
            }
            s += "[" + extractFromXmlRpc (value, env, true) + "]"; // can have different primitive types
            //free (value);
        }
        return s;
    }

    /**
    @brief function to write xmlrpc structs to string.
    @param [in] xmlRPC variable
    @return result string
    */
    std::string extractStructFromXmlRpc (xmlrpc_value *val, xmlrpc_env &env)
    {
        std::string s;
        int num = xmlrpc_struct_size (&env, val);
        s += "{";
        for (int i = 0; i < num; i++)
        {
            xmlrpc_value *key = 0, *value = 0;
            xmlrpc_struct_read_member (&env, val, i, &key, &value);
            if (i > 0)
            {
                s += ",";
            }
            s += extractStringFromXmlRpc (key, env, true);
            s += ":";
            s += extractFromXmlRpc (value, env, true); // can have different primitive types
            //  free (key);
            //  free (value);  //donot need to free the memory as it is handeled by xmlrpc in xmlrpc_DECREF
        }
        s += "}";
        return s;
    }

    /**
     @brief function to write different xmlrpc data types to string.
     @param [in] xmlRPC variable
     @return result string
     */
    std::string extractFromXmlRpc (xmlrpc_value *val, xmlrpc_env &env, bool quotes) throw (std::string)
    {
        std::stringstream ss;
        switch (val->_type)
        {
            case XMLRPC_TYPE_INT:
                ss << extractIntFromXmlRpc (val, env);
                break;
            case XMLRPC_TYPE_BOOL:
                ss << extractBoolFromXmlRpc (val, env);
                break;
            case XMLRPC_TYPE_DOUBLE:
                ss << extractDoubleFromXmlRpc (val, env);
                break;
            case XMLRPC_TYPE_ARRAY:
                ss << extractArrayFromXmlRpc (val, env);
                break;
            case XMLRPC_TYPE_STRUCT:
                ss << extractStructFromXmlRpc (val, env);
                break;
            case XMLRPC_TYPE_STRING:
                ss << extractStringFromXmlRpc (val, env, quotes);
                break;
            default:
                throw std::string ("xmlrpc data type not supported");
                break;
        }
        return ss.str();
    }


};

///@endcond

using namespace stringOperations;
using namespace xmlrpcConversions;

O3d3xxCameraOperations::O3d3xxCameraOperations()
{
    mp_d = new PrivateData();
    mp_d->fault_string = "";
}

O3d3xxCameraOperations::~O3d3xxCameraOperations()
{
    if (mp_d->isConnected)
    {
        disconnectPCIC();
    }
    /* Free the allocated memory of private data */
    if (mp_d)
    {
        delete mp_d;
        mp_d = 0;
    }
}

int32_t O3d3xxCameraOperations::getFrameDataSize (uint32_t *dataSize)
{
    *dataSize = mp_d->frameSizeInBytes;
    RETURN_NOERROR;
}

void O3d3xxCameraOperations::setPCICDataConfigString (const std::string &s)
{
    mp_d->dataChannelConfigString = s;
}

std::string O3d3xxCameraOperations::getPCICDataConfigString()
{
    return mp_d->dataChannelConfigString;
}

int32_t O3d3xxCameraOperations::enterEditMode (void)
{
    std::string res;
    std::vector<std::string> param ;
    int ret = 0;
    if (!mp_d->isConfigurationModeON)
    {
        ret = switchCameraToConfigurationMode();

        if (ret != 0)
        {
            return ret;
        }
    }
    // setting it to edit mode ...
    if (mp_d->isConfigurationModeON)
    {
        param.clear();
        res = "";
        param.push_back (to_string(mp_d->applicationId));
        ret = xmlrpcCommandImplementation (mp_d->sessionUrl + "edit/", "editApplication", "(s)", param, res);
        if (ret != 0)
        {
            mp_d->isEditModeON = false;
            return ret;
        }

        mp_d->isEditModeON = true;
        return ret;
    }
    return ret;
}

int32_t O3d3xxCameraOperations::exitEditMode (void)
{
    std::string res;
    std::vector<std::string> param ;
    int ret = 0;

    if (mp_d->isEditModeON)
    {
        // Stop editing the application
        param.clear();
        res = "";
        ret = xmlrpcCommandImplementation (mp_d->sessionUrl + "edit/", "stopEditingApplication", "()", param, res);
        if (ret != 0)
        {
            return ret;
        }
        mp_d->isEditModeON = false;
        if (mp_d->isConfigurationModeON)
        {
            ret = exitFromCameraConfigurationMode();
            if (ret != 0)
            {
                return ret;
            }
        }

        return ret;

    }
    return 0;

}

int32_t O3d3xxCameraOperations::setProtocolVersion (std::string version)
{
    std::string res;
    std::vector<std::string> param ;
    int ret;

    int32_t status_code = switchCameraToConfigurationMode();
    if (status_code != 0)
    {
        // Clean up our error-handling environment.
        xmlrpc_env_clean (& (mp_d->environment));

        // Shutdown our XML-RPC client library
        xmlrpc_client_cleanup();

        mp_d->fault_string = "Camera device is not connected\n";

        //If camera not connected, return relevant error code
        RETURN_ERROR (status_code);
    }

    // Save and activate a default configuration on camera...

    std:: string edit_url = mp_d->sessionUrl + "edit/";

    std::string device_url = "";
    // Generating string representing device object's url
    device_url = edit_url + "device/";

    param.clear();
    param.push_back ("PcicProtocolVersion");
    param.push_back (version);

    ret = xmlrpcCommandImplementation (device_url, "setParameter", "(ss)", param, res);
    if (ret != 0)
    {
        return ret;
    }

    param.clear();
    res = "";
    ret = xmlrpcCommandImplementation (device_url, "save", "()", param, res);
    if (ret != 0)
    {
        return ret;
    }
    exitFromCameraConfigurationMode();
    return 0;
}

int32_t  O3d3xxCameraOperations::readResolution (uint32_t *frameWidth, uint32_t *frameHeight)
{
    *frameWidth = mp_d->imageWidth;
    *frameHeight = mp_d->imageHeight;
    RETURN_NOERROR;
}

int32_t O3d3xxCameraOperations::getResolution (uint32_t *frameWidth, uint32_t *frameHeight)
{
    std::string response;
    std::vector<std::string> param ;
    int ret;

    ret = getImagerSettings ("GetImageResolution", response);
    if(ret != ERR_NONE)
    {
        return ret;
    }

    if(atoi(response.c_str()) == 0)
    {
        *frameWidth = 176;
        *frameHeight = 132;
    }
    else if(atoi(response.c_str()) == 1)
    {
        *frameWidth = 352;
        *frameHeight = 264;
    }
    else
    {
        *frameWidth = 176;
        *frameHeight = 132;
    }
    /*
    // Variables to store first/last column/row number of a frame
    int indexofFirstRow = 0;
    int indexofLastRow = 0;
    int indexofFirstColumn = 0;
    int indexofLastColumn = 0;

    ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

    // Generating string representing application object's url
    string appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

    // Get index of first/last row and first/last column from camera.and calculate the matrix resolution...

    // Get index of first row of frame
    param.clear();
    res = "";
    param.push_back ("ClippingTop");
    ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }
    indexofFirstRow = atoi (res.c_str());

    // Get index of last row of frame
    param.clear();
    res = "";
    param.push_back ("ClippingBottom");
    ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }
    indexofLastRow = atoi (res.c_str());

    // Get index of first column of frame
    param.clear();
    res = "";
    param.push_back ("ClippingLeft");
    ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }
    indexofFirstColumn = atoi (res.c_str());

    // Get index of last column of frame
    param.clear();
    res = "";
    param.push_back ("ClippingRight");
    ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }
    indexofLastColumn = atoi (res.c_str());

    *frameWidth = indexofLastColumn - indexofFirstColumn + 1; // indices are zero based --> +1
    *frameHeight = indexofLastRow - indexofFirstRow + 1;

    ret = exitEditMode();
    if (ret != 0)
    {
        return ret;
    }
    */

    RETURN_NOERROR;
}

int32_t O3d3xxCameraOperations::getIntegrationTime (uint32_t *integrationTime , unsigned int index)
{
    std::string res;
    std::vector<std::string> param ;
	std::vector<uint32_t> integrationTimeList;
    int ret, ret_val ;
    
    if(index > EXP_TIME_MAX_INDEX)
    {
        mp_d->fault_string = "Index cannot be greater than " + to_string(EXP_TIME_MAX_INDEX) ;
        return ERR_INVALID_ARG;
    }

	ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

    string appimager_url = "";
    // Generating string representing application object's url
    appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

    // Get the "Type" of Imager.
    // Do not set integration time in "high" exposure mode.
    param.clear();
    res = "";
    param.push_back ("Type");
    ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }

    // Split "Type" string at underscore ('_')
	std::string exposure_mode_str = res.substr (res.find ('_') + 1, res.length());
    
    if(exposure_mode_str != "high")
    {
        ret_val = ERR_NONE;
        param.clear();
        res = "";
        param.push_back ("ExposureTime");
        ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
        *integrationTime = atoi(res.c_str());
    }
    else
    {
        ret_val = ERR_INVALID_FUNCTION;
        *integrationTime = 0;
    }
    

    ret = exitEditMode();
    if (ret != 0)
    {
        return ret;
    }

    return ret_val;
}

int32_t O3d3xxCameraOperations::setIntegrationTime (uint32_t integrationTime ,  unsigned int index)
{
    int ret_val;
    if (index > EXP_TIME_MAX_INDEX)
    {
        mp_d->fault_string = "Index cannot be greater than " + to_string(EXP_TIME_MAX_INDEX) ;
        return ERR_INVALID_ARG;
    }

	if(integrationTime < EXP_TIME_MIN || integrationTime > EXP_TIME_MAX)
	{
        mp_d->fault_string = "Valid values for integration time: [" + to_string(EXP_TIME_MIN) + "-"  + to_string(EXP_TIME_MAX) + "]" ;
		return ERR_INVALID_ARG;
	}
		
    std::string res;
    std::vector<std::string> param ;
    int ret ;

    ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

	string appimager_url = "";
    // string representing of application url
    appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

    // Get the "Type" of Imager.
    // Do not set integration time in "high" exposure mode.
    param.clear();
    res = "";
    param.push_back ("Type");
    ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }

    // Split "Type" string at underscore ('_')
	std::string exposure_mode_str = res.substr (res.find ('_') + 1, res.length());
    
    if(exposure_mode_str != "high")
    {
        ret_val = ERR_NONE;
        param.clear();
        res = "";
        param.push_back ("ExposureTime");
        param.push_back (to_string (integrationTime));
        ret = xmlrpcCommandImplementation (appimager_url, "setParameter", "(ss)", param, res);
        if (ret != 0)
        {
            return ret;
        }

        // do cleanup
	    if (mp_d->isEditModeON)
        {
            std:: string edit_url = mp_d->sessionUrl + "edit/";

            // Save the application...
            param.clear();
            res = "";
            std::string app_url = "";
            app_url = edit_url + "application/";
            ret = xmlrpcCommandImplementation (app_url, "save", "()", param, res);
            if (ret != 0)
            {
                return ret;
            }

            edit_url = "";
        }
    }
    else
        ret_val = ERR_INVALID_FUNCTION;

    //Generating string representing edit object's url
    ret = exitEditMode() ;
    if (ret != 0)
    {
        return ret;
    }

    return ret_val;
}

int32_t O3d3xxCameraOperations::getModulationFrequencyChannel (uint32_t *modulationFrequencyChannel)
{
    std::string res;
    std::vector<std::string> param ;
    int ret ;

	ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

    string appimager_url = "";
    // Generating string representing application object's url
    appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

    param.clear();
    res = "";
    param.push_back ("Channel");
    ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);

    *modulationFrequencyChannel = atoi (res.c_str());

    ret = exitEditMode();
    if (ret != 0)
    {
        return ret;
    }
    RETURN_NOERROR;
}

int32_t O3d3xxCameraOperations::setModulationFrequencyChannel(unsigned value)
{
	int ret_val;
	if (value > MOD_FREQ_CHANNEL_MAX_VAL)
    {
        mp_d->fault_string = "Value Out of Range. Valid values are: [0-" + to_string(MOD_FREQ_CHANNEL_MAX_VAL) + "]" ;
		return ERR_INVALID_ARG;
    }

	std::string res;
    std::vector<std::string> param ;
    int ret;

    ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

    string appimager_url = "";
    // string representing of application url
    appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

	param.clear();
    res = "";
    param.push_back ("Type");
	ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }
	// Split "Type" string at underscore ('_')
	std::string distance_range_str = res.substr (0, res.find ('_') );

	if(distance_range_str != "morethan30m")
	{
		ret_val = ERR_NONE;
	}
	else
	{
		if(value > MOD_FREQ_CHANNEL_0)
			ret_val = ERR_INVALID_ARG;
		else
			ret_val = ERR_NONE;
	}

	if(ret_val == ERR_NONE)
	{
		param.clear();
		res = "";
		param.push_back ("Channel");
		param.push_back (to_string (value));
		ret = xmlrpcCommandImplementation (appimager_url, "setParameter", "(ss)", param, res);
		if (ret != 0)
		{
			return ret;
		}

		// do cleanup
		if (mp_d->isEditModeON)
		{
			// Save the application...
			param.clear();
			res = "";
			std::string app_url = "";
			app_url = mp_d->sessionUrl + "edit/application/";
			ret = xmlrpcCommandImplementation (app_url, "save", "()", param, res);
			if (ret != 0)
			{
				return ret;
			}
		}
	}
    //Generating string representing edit object's url
    ret = exitEditMode() ;
    if (ret != 0)
    {
        return ret;
    }

    return ret_val;

}

 int32_t O3d3xxCameraOperations::setImagerType (std::string command, std::string value, std::string &response)
 {
	 
	unsigned int val = atoi(value.c_str());
	
    unsigned int max_val;
    if(command == "SetUnambiguousDistanceRange")
        max_val = MAX_BGND_DIST_VAL;
    else if(command == "SetExposureMode")
        max_val = MAX_EXP_MODE_VAL;

    if (val > max_val)
    {
        mp_d->fault_string = "Valid values are: [0-" + to_string(max_val) + "] " ;
        return ERR_INVALID_ARG;
    }

	std::string res;
    std::vector<std::string> param ;
    int ret ;

	ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

	string appimager_url = "";
    // string representing of application url
    appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

	// Get Current Type on camera
	param.clear();
	param.push_back ("Type");
	ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
	if (ret != 0)
    {
        return ret;
    }

	// Split "Type" string at underscore ('_')
	std::string distance_range_str = res.substr (0, res.find ('_') );
    std::string exposure_mode_str = res.substr (res.find ('_') + 1, res.length());

	std::string new_type, imager_type_str;
	if(command == "SetUnambiguousDistanceRange")
	{
		switch(val)
		{
		case UNDER_5m:
			new_type = "under5m";
			break;

		case UPTO_30m:
			new_type = "upto30m";
			break;

		case MORE_THAN_30m:
			new_type = "morethan30m";
			break;

		default:
            new_type = "under5m";
			break;
		}
		imager_type_str = new_type + "_" + exposure_mode_str;
	}
	else if(command == "SetExposureMode")
	{
		switch(val)
		{
		case LOW:
			new_type = "low";
			break;

		case MODERATE:
			new_type = "moderate";
			break;

		case HIGH:
			new_type = "high";
			break;

		default:
            new_type = "low";
			break;
		}
		imager_type_str = distance_range_str + "_" + new_type;
	}
	
    param.clear();
    res = "";
    param.push_back (imager_type_str);
    ret = xmlrpcCommandImplementation (appimager_url, "changeType", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }

    // do cleanup
	if (mp_d->isEditModeON)
    {
        std:: string edit_url = mp_d->sessionUrl + "edit/";

        // Save the application...
        param.clear();
        res = "";
        std::string app_url = "";
        app_url = edit_url + "application/";
        ret = xmlrpcCommandImplementation (app_url, "save", "()", param, res);
        if (ret != 0)
        {
            return ret;
        }

        edit_url = "";
    }
    //Generating string representing edit object's url
    ret = exitEditMode() ;
    if (ret != 0)
    {
        return ret;
    }

	response = "OK";
    RETURN_NOERROR;
 }

 void O3d3xxCameraOperations::generatePCICTcpResultSchema (std::string &dataConfigString, uint32_t value)
{    
    if((value & ALL_IMAGES_BIT) )
    {
        dataConfigString = "{ \"layouter\": \"flexible\", \"format\": { \"dataencoding\": \"ascii\" }, \"elements\": [ { \"type\": \"string\", \"value\": \"star\", \"id\": \"start_string\" }, { \"type\": \"blob\", \"id\": \"normalized_amplitude_image\" }, { \"type\": \"blob\", \"id\": \"distance_image\" }, { \"type\": \"blob\", \"id\": \"x_image\" }, { \"type\": \"blob\", \"id\": \"y_image\" }, { \"type\": \"blob\", \"id\": \"z_image\" }, { \"type\": \"blob\", \"id\": \"confidence_image\" }, { \"type\": \"blob\", \"id\": \"diagnostic_data\" }, { \"type\": \"string\", \"value\": \"stop\", \"id\": \"end_string\" } ] }";
    }
    else
    {
        // Append start string.
        dataConfigString = "{ \"layouter\": \"flexible\", \"format\": { \"dataencoding\": \"ascii\" }, \"elements\": [ { \"type\": \"string\", \"value\": \"star\", \"id\": \"start_string\" }, ";
        if((value & DISTANCE_BIT))
        {// Append distance image in layouter
            dataConfigString = dataConfigString + "{ \"type\": \"blob\", \"id\": \"distance_image\" }, ";
        }

        if((value & NORM_AMPLITUDE_BIT))
        {// Append normalized amplitude image in layouter
            dataConfigString = dataConfigString + "{ \"type\": \"blob\", \"id\": \"normalized_amplitude_image\" }, ";
        }

        if((value & AMPLITUDE_BIT) )
        {// Append normalized amplitude image in layouter
            dataConfigString = dataConfigString + "{ \"type\": \"blob\", \"id\": \"amplitude_image\" }, ";
        }

        if((value & X_IMAGE_BIT))
        {// Append X image in layouter
            dataConfigString = dataConfigString + "{ \"type\": \"blob\", \"id\": \"x_image\" }, ";
        }

        if((value & Y_IMAGE_BIT))
        {// Append Y image in layouter
            dataConfigString = dataConfigString + "{ \"type\": \"blob\", \"id\": \"y_image\" }, ";
        }

        if((value & Z_IMAGE_BIT))
        {// Append Z image in layouter
            dataConfigString = dataConfigString + "{ \"type\": \"blob\", \"id\": \"z_image\" }, ";
        }

        if((value & CONFIDENCE_IMAGE_BIT))
        {// Append confidence image in layouter
            dataConfigString = dataConfigString + "{ \"type\": \"blob\", \"id\": \"confidence_image\" }, ";
        }

        if((value & DIAGNOSTIC_DATA_BIT))
        {// Append diagnostic data in layouter
            dataConfigString = dataConfigString + "{ \"type\": \"blob\", \"id\": \"diagnostic_data\" }, ";
        }
        // Append stop string
        dataConfigString = dataConfigString +  "{ \"type\": \"string\", \"value\": \"stop\", \"id\": \"end_string\" } ] }";
    }

}

int32_t O3d3xxCameraOperations::setImagerSettings (std::string command,  std::string value, std::string &response)
{
	std::string res, PCICDataConfig;
    std::vector<std::string> param ;
    int ret ;

	param.clear();
    res = "";

	/************************************************************************************* 
	 *									Parameter Value Validation
	 ************************************************************************************/
	if (command == "EnableFilterDistanceImage" || command == "EnableFilterAmplitudeImage" || \
		command == "EnableAmplitudeCorrection" || command == "EnableRectificationDistanceImage" || \
		command == "EnableRectificationAmplitudeImage" )
	{
		if(value.compare("true") != 0 && value.compare("false") != 0)
		{
			mp_d->fault_string = "Valid Values for the command is: true/false" ;
			response = "Valid Values for the command is: true/false" ;
			return ERR_INVALID_ARG;
		}
		else
		{
			param.push_back (command);
			param.push_back (value);
		}
	}
	else if(command == "EnableContinuousAutoExposure")
	{
		if(value.compare("true") != 0 && value.compare("false") != 0)
		{
			mp_d->fault_string = "Valid Values for EnableContinuousAutoExposure are: true/false" ;
			response = "Valid Values for EnableContinuousAutoExposure are: true/false" ;
			return ERR_INVALID_ARG;
		}
		else
		{
			param.push_back ("ContinuousAutoExposure");
			param.push_back (value);
		}
	}
	else if(command == "SetTargetFrameRate")
	{
		if (atof(value.c_str()) < TARGET_FRAME_RATE_MIN || atof(value.c_str()) > TARGET_FRAME_RATE_MAX)
		{
            mp_d->fault_string = "Value Out of Range. Valid values are: [" + to_string(TARGET_FRAME_RATE_MIN) + "to" + to_string(TARGET_FRAME_RATE_MAX) + "]" ;
			response = "Value Out of Range. Valid values are: [" + to_string(TARGET_FRAME_RATE_MIN) + "to" + to_string(TARGET_FRAME_RATE_MAX) + "]" ;
			return ERR_INVALID_ARG;
		}
		else
		{
			param.push_back ("FrameRate");
			param.push_back (value);
		}
	}
    else if(command == "SetExposureTimeRatio")
	{
		if (atof(value.c_str()) < EXP_TIME_RATIO_MIN || atof(value.c_str()) > EXP_TIME_RATIO_MAX)
		{
            mp_d->fault_string = "Value Out of Range. Valid values are: [" + to_string(EXP_TIME_RATIO_MIN) + "to" + to_string(EXP_TIME_RATIO_MAX) + "]" ;
			response = "Value Out of Range. Valid values are: [" + to_string(EXP_TIME_RATIO_MIN) + "to" + to_string(EXP_TIME_RATIO_MAX) + "]" ;
			return ERR_INVALID_ARG;
		}
		else
		{
			param.push_back ("ExposureTimeRatio");
			param.push_back (value);
		}
	}
	else if(command == "SetSpatialFilterType")
	{
		if (atoi(value.c_str()) > SPATIAL_FILT_MAX_VAL)
		{
			mp_d->fault_string = "Valid Values for SpatialFilterType are: [0-" + to_string(SPATIAL_FILT_MAX_VAL) + "]" ;
			response = "Valid Values for SpatialFilterType are: [0-" + to_string(SPATIAL_FILT_MAX_VAL) + "]" ;
			return ERR_INVALID_ARG;
		}
		else
		{
			param.push_back ("SpatialFilterType");
			param.push_back (value);
		}
	}
	else if(command == "SetTemporalFilterType")
	{
		if (atoi(value.c_str()) > TEMPORAL_FILT_MAX_VAL)
		{
			mp_d->fault_string = "Valid Values for TemporalFilterType are: [0-" + to_string(TEMPORAL_FILT_MAX_VAL) + "]" ;
			response = "Valid Values for TemporalFilterType are: [0-" + to_string(TEMPORAL_FILT_MAX_VAL) + "]" ;
			return ERR_INVALID_ARG;
		}
	}
	else if(command == "SetSpatialFilterMaskSize")
	{
		if (atoi(value.c_str()) > SPATIAL_FILT_MASKSIZE_MAX_VAL)
		{
			mp_d->fault_string = "Valid Values for SpatialFilter MaskSize are: [0-" + to_string(SPATIAL_FILT_MASKSIZE_MAX_VAL) + "]" ;
			response = "Valid Values for SpatialFilter MaskSize are: [0-" + to_string(SPATIAL_FILT_MASKSIZE_MAX_VAL) + "]" ;
			return ERR_INVALID_ARG;
		}
	}
	else if(command == "SetTempMeanFilterNumberOfImages")
	{
		if (atoi(value.c_str()) > TEMPORAL_MEAN_FILT_MAX_IMG || atoi(value.c_str()) < TEMPORAL_MEAN_FILT_MIN_IMG)
		{
			mp_d->fault_string = "Valid Values for Temporal Filter Number Of Images are: [" + to_string(TEMPORAL_MEAN_FILT_MIN_IMG) + "to" + to_string(TEMPORAL_MEAN_FILT_MAX_IMG) + "]" ;
			response = "Valid Values for Temporal Filter Number Of Images are: [" + to_string(TEMPORAL_MEAN_FILT_MIN_IMG) + "to" + to_string(TEMPORAL_MEAN_FILT_MAX_IMG) + "]" ;
			return ERR_INVALID_ARG;
		}
	}
    else if(command == "SetImageResolution")
	{
		if (atoi(value.c_str()) > RES_MAX)
		{
			mp_d->fault_string = "Valid Values for setting Image Resolution  are: [0-" + to_string(RES_MAX) + "]" ;
			response = "Valid Values for Temporal Filter Number Of Images are: [0-" + to_string(RES_MAX) + "]"  ;
			return ERR_INVALID_ARG;
		}
        else
		{
			param.push_back ("Resolution");
			param.push_back (value);
		}
	}
    else if(command == "SetClippingCuboid")
    {
        param.push_back ("ClippingCuboid");
        param.push_back (value);
    }


	// Setting Camera in Edit Mode.
    ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

	string appimager_url = "";
    // string representing of application url
    appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

	
	/********************************************************************************** 
	 *								Filter Function call validation 
	 *********************************************************************************/
	
	if(command == "SetTemporalFilterType")
	{
		if(value == to_string(TEMPORAL_FILT_OFF))
		{
			param.push_back ("TemporalFilterType");
			param.push_back (value);
		}
		else
		{
			std::string app_url= mp_d->sessionUrl + "edit/application/";
			param.push_back ("TriggerMode");
			ret = xmlrpcCommandImplementation (app_url, "getParameter", "(s)", param, res);
			if (ret != 0)
			{
				return ret;
			}
			unsigned int trigger_mode  = atoi (res.c_str());
			param.clear();
			res = "";
			if(value == to_string(TEMPORAL_FILT_MEAN) && trigger_mode == CONTINUOUS)
			{
				/*
				 * If Trigger Mode is "Continuous", Temporal Mean Filter will not work. 
				 * Observation: The Temporal Mean filter is not available only for continuous (free run) trigger mode.
				 * 
				 */
				mp_d->fault_string = "Trigger mode is set to Continuous (Free Run). Cannot apply Temporal Mean Filter." ;
				response = "Trigger mode is set to Continuous (Free Run). Cannot apply Temporal Mean Filter." ;
			}
			else if(value == to_string(TEMPORAL_FILT_ADP_EXP) && trigger_mode != CONTINUOUS)
			{
				/*
				 * If Trigger Mode is not "Continuous", Temporal Adaptive Exponential Filter will not work. 
				 * Observation: The Temporal Adaptive Exponential filter is available only for continuous (free run) trigger mode.
				 * In every other triggr mode, it is unavailable.
				 */
				mp_d->fault_string = "Trigger mode is set not Continuous (Free Run). Cannot apply Temporal Adaptive Expoonential Filter." ;
				response = "Trigger mode is set not Continuous (Free Run). Cannot apply Temporal Adaptive Expoonential Filter." ;	
			}
			else 
			{
				param.push_back ("TemporalFilterType");
				param.push_back (value);
			}
		}
	}
	else if(command == "SetSpatialFilterMaskSize")
	{
		/*
		* Checking if Spatial Filtering is ON. 
		* Observation: When spatial filtering is set to "off" and "SetSpatialFilterMaskSize 0/1" 
		* command is called, the xmlrpc call returns an error as Object url not present, since,
		* the url : 
		* "/api/rpc/v1/com.ifm.efector/session_d21c80db5bc1069932fbb9a3bd841d0b/edit/application/imager_001/spatialfilter"
		* will not be present.
		* Hence, checking if spatial filtering is ON before calling this command.
		*/
		param.push_back ("SpatialFilterType");
		ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
		if (ret != 0)
		{
			return ret;
		}
		unsigned int spatial_filter_type  = atoi (res.c_str());
		param.clear();
		res = "";
		if(spatial_filter_type == SPATIAL_FILT_OFF)
		{
			mp_d->fault_string = "Spatial Filter type is set to OFF. Command will not work." ;
			response = "Spatial Filter type is set to OFF. Command will not work." ;	
		}
		else
		{
			appimager_url = appimager_url + "/spatialfilter";
			param.push_back ("MaskSize");
			param.push_back (value);
		}
		
	}
	else if(command == "SetTempMeanFilterNumberOfImages")
	{
		/*
		* Checking if Temporal Mean Filtering is ON. 
		* Observation: The Number of Images is available only for Temporal Mean Filter
		* Hence, checking if temporal mean filtering is ON before calling this command.
		*/
		param.push_back ("TemporalFilterType");
		ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, res);
		if (ret != 0)
		{
			return ret;
		}
		unsigned int temporal_filter_type  = atoi (res.c_str());
		param.clear();
		res = "";
		if(temporal_filter_type != TEMPORAL_FILT_MEAN)
		{
			mp_d->fault_string = "Temporal Filter type is not set to Mean Filter. Command will not work." ;
			response = "Temporal Filter type is not set to Mean Filter. Command will not work." ;	
		}
		else
		{
			appimager_url = appimager_url + "/temporalfilter";
			param.push_back ("NumberOfImages");
			param.push_back (value);
		}
	}
    else if(command == "SetSourceDataBinSelect")
    {
        uint32_t data_value = atoi(value.c_str());
        generatePCICTcpResultSchema(PCICDataConfig, data_value);
        param.push_back ("PcicTcpResultSchema");
		param.push_back (PCICDataConfig);
        appimager_url = mp_d->sessionUrl + "edit/application/";
    }
    else if(command == "SetSourceDataJson")
    {
        param.push_back ("PcicTcpResultSchema");
        param.push_back (value);
        appimager_url = mp_d->sessionUrl + "edit/application/";
    }
	

	if(param.size() > 0)
	{
		ret = xmlrpcCommandImplementation (appimager_url, "setParameter", "(ss)", param, res);
		if (ret != 0)
		{
			return ret;
		}
	

		// do cleanup
		if (mp_d->isEditModeON)
		{
			// Save the application...
			param.clear();
			res = "";
			std::string app_url = "";
			app_url = mp_d->sessionUrl + "edit/application/";
			ret = xmlrpcCommandImplementation (app_url, "save", "()", param, res);
			if (ret != 0)
			{
				return ret;
			}
		}
	}
    //Generating string representing edit object's url
    ret = exitEditMode() ;
    if (ret != 0)
    {
        return ret;
    }

	// "response" string will be empty on successful run. In case of error, it will be
	// set to appropriate string message.
	if(response == "")
	{
		response = "OK";
		RETURN_NOERROR;
	}
	else
		return ERR_INVALID_FUNCTION;
}

int32_t O3d3xxCameraOperations::optimize (std::string &response)
{
	std::string res;
    std::vector<std::string> param ;
    int ret ;

    ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

	string appimager_url = "";
    // string representing of application url
    appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

    // Start Auto Exposure Time function
	param.clear();
    res = "";
    ret = xmlrpcCommandImplementation (appimager_url, "startCalculateExposureTime", "()", param, res);
    if (ret != 0)
    {
        return ret;
    }

	// Monitor progress of auto exposure time calculation function.
	double progress = 0.0;
	while (progress != 1.0)
	{
		res = "";
		ret = xmlrpcCommandImplementation (appimager_url, "getProgressCalculateExposureTime", "()", param, res);
		if (ret != 0)
		{
			return ret;
		} 
		progress = atof(res.c_str());
	}

    // do cleanup
	if (mp_d->isEditModeON)
    {
        std:: string edit_url = mp_d->sessionUrl + "edit/";

        // Save the application...
        param.clear();
        res = "";
        std::string app_url = "";
        app_url = edit_url + "application/";
        ret = xmlrpcCommandImplementation (app_url, "save", "()", param, res);
        if (ret != 0)
        {
            return ret;
        }

        edit_url = "";
    }
    //Generating string representing edit object's url
    ret = exitEditMode() ;
    if (ret != 0)
    {
        return ret;
    }

	response = "OK";
    RETURN_NOERROR;
}

int O3d3xxCameraOperations::checkParameterValidity (string ip_address, uint16_t port_number, uint16_t pcic_tcp_port , unsigned int applicatioID)
{

    // IP address validation
    if (ip_address.length() > MAX_IP_ADDRESS_LENGTH)
    {
        mp_d->fault_string = "Invalid ip address given as input.";
        RETURN_ERROR (ERR_INVALID_ARG);
    }

    if (ip_address == "0.0.0.0")
    {
        mp_d->fault_string = "Invalid ip address \"0.0.0.0\" given as input. Value of octet should be between 1 to 255";
        RETURN_ERROR (ERR_INVALID_ARG);
    }

    std::size_t found = ip_address.find_first_of (".");
    string octet = "";
    uint8_t octetStartPos = 0;
    uint8_t noofOctets = 0;
    while (octetStartPos < ip_address.length())
    {
        octet = ip_address.substr (octetStartPos, found - octetStartPos);
        if (atoi (octet.c_str()) > 255)
        {
            mp_d->fault_string = "Invalid ip address given as input. Value of octet should be between 1 to 255";
            RETURN_ERROR (ERR_INVALID_ARG);
        }

        noofOctets++;
        octetStartPos = found + 1;
        found = ip_address.find_first_of (".", found + 1);
        if (found == std::string::npos)
        {
            found = ip_address.length();
        }
    }


    if (noofOctets != 4)
    {
        mp_d->fault_string = "Invalid ip address given as input. 4 octets required in ip address";
        RETURN_ERROR (ERR_INVALID_ARG);
    }

    // Test that the port numbers provided as input are valid,i.e non zero
    if ( (port_number != 80) || (pcic_tcp_port != 50010))
    {
        mp_d->fault_string = "Invalid port number given as input. Valid value is from 1 to 65535";
        RETURN_ERROR (ERR_INVALID_ARG);
    }

    mp_d->cameraIpAddress = ip_address;
    mp_d->cameraXmlrpcPortNumber = port_number;
    mp_d->cameraImageOutputPortNumber = pcic_tcp_port;

    if (applicatioID <= MAX_NUMBER_OF_APPLICATION_SUPPORTED)  // max numberofapplication
    {
        mp_d->applicationId = applicatioID;
    }
    else
    {
        mp_d->applicationId = 0;
    }
    RETURN_NOERROR;
}

int32_t O3d3xxCameraOperations::connectToCamera (string ip_address, \
        uint16_t port_number, uint16_t pcic_tcp_port , unsigned int applicatioID)
{
    string edit_url, app_url, appimager_url, device_url;
    std::string res;
    std::vector<std::string> param ;
    int ret;

    if (checkParameterValidity (ip_address, port_number, pcic_tcp_port, applicatioID) != 0)
    {
        RETURN_ERROR (ERR_INVALID_ARG);
    }

    if (mp_d->noOfConnections == 0)
	{
        // Initialize our xmlrpc error-handling environment.
        xmlrpc_env_init (& (mp_d->environment));

        /*Check if any error occurred*/
        XMLRPCCHECK()

        /* Start up our XML-RPC client.*/
        xmlrpc_client_init2 (& (mp_d->environment), \
                             XMLRPC_CLIENT_NO_FLAGS, "Xmlrpc-c Test Client", "1.0", NULL, 0);
        XMLRPCCHECK()
	}

   /**Creating an application, configuring it with default values
    and activating it on camera*/

    int32_t status_code = switchCameraToConfigurationMode();
    if (status_code != 0)
    {
        // Clean up our error-handling environment.
        xmlrpc_env_clean (& (mp_d->environment));

        // Shutdown our XML-RPC client library
        xmlrpc_client_cleanup();

        mp_d->fault_string = "Camera device is not connected\n";

        //If camera not connected, return relevant error code
        RETURN_ERROR (status_code);
    }

    // Save and activate a default configuration on camera...

    edit_url = mp_d->sessionUrl + "edit/";

    device_url = "";
    // Generating string representing device object's url
    device_url = edit_url + "device/";

    param.clear();
    param.push_back ("PcicProtocolVersion");
    ret = xmlrpcCommandImplementation (device_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }
    mp_d->protocolVesrion = res;

     /************************************************************************
            Store previous active application on camera
      ************************************************************************/
    device_url = "";
    // Generating string representing device object's url
    device_url = edit_url + "device/";
    param.clear();
    param.push_back ("ActiveApplication");
    ret = xmlrpcCommandImplementation (device_url, "getParameter", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }
    mp_d->previousActiveApplicationId = atoi (res.c_str());

    if (mp_d->applicationId != 0) // user want to use specific application on camera
    {
        /************************************************************************
             Edit the application at given ID to read the trigger mode.
             Reqd for update_camera_frame to send software trigger in 
             "process inteface" trigger mode.
             If application at given ID is not present, then, connect fails 
             and generates xmlrpc error.
         ************************************************************************/ 

        // Setting application in edit mode
        param.clear();
        res = "";
        param.push_back (to_string<int> (mp_d->applicationId));

        ret = xmlrpcCommandImplementation (edit_url, "editApplication", "(s)", param, res);
        if (ret != 0)
        {
            return ret;
        }

        app_url = mp_d->sessionUrl + "edit/application/";

        param.clear();
        res = "";
        param.push_back ("TriggerMode");
        //getting the trigger mode
        ret = xmlrpcCommandImplementation (app_url, "getParameter", "(s)", param, res);
        if (ret != 0)
        {
            return ret;
        }

        mp_d->triggeredMode = atoi (res.c_str());

        param.clear();
        res = "";
        ret = xmlrpcCommandImplementation (edit_url, "stopEditingApplication", "()", param, res);
        if (ret != 0)
        {
            return ret;
        }

    }

    if (mp_d->applicationId == 0) // user has not specified application ID, so create a new application
    {
        param.clear();
        res = "";
        //creating the application
        ret = xmlrpcCommandImplementation (edit_url, "createApplication", "()", param, res);
        if (ret != 0)
        {
            return ret;
        }
        // storing the application ID
        mp_d->applicationId = atoi (res.c_str());

        mp_d->isApplicationCreated = true;
#ifdef DEBUG
        /* Printing the result value */
        printf ("Now, The application id  is %d\n", mp_d->applicationId);
#endif

        // Setting application in edit mode
        param.clear();
        res = "";
        param.push_back (to_string<int> (mp_d->applicationId));
        ret = xmlrpcCommandImplementation (edit_url, "editApplication", "(s)", param, res);
        if (ret != 0)
        {
            return ret;
        }

        app_url = mp_d->sessionUrl + "edit/application/";

        param.clear();
        res = "";
        param.push_back ("TriggerMode");
        //getting the trigger mode
        ret = xmlrpcCommandImplementation (app_url, "getParameter", "(s)", param, res);
        if (ret != 0)
        {
            return ret;
        }

        mp_d->triggeredMode = atoi (res.c_str());


        // Set integration time to camera as default
        appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

        param.clear();
        res = "";
        param.push_back ("ExposureTime");
        param.push_back (to_string ( (unsigned long long) DEFAULT_INTEGRATION_TIME));
        ret = xmlrpcCommandImplementation (appimager_url, "setParameter", "(ss)", param, res);
        if (ret != 0)
        {
            return ret;
        }

        /*Setting default frame rate */
        param.clear();
        res = "";
        param.push_back ("FrameRate");
        param.push_back (to_string ( (unsigned long long) DEFAULT_FRAME_RATE).c_str());

        appimager_url = "";
        // Generating string representing application imager object's url
        appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

        ret = xmlrpcCommandImplementation (appimager_url, "setParameter", "(ss)", param, res);
        if (ret != 0)
        {

            // Clean up our error-handling environment.
            std::string err = mp_d->environment.fault_string;
            xmlrpc_env_clean (& (mp_d->environment));

            // Shutdown our XML-RPC client library
            xmlrpc_client_cleanup();

            mp_d->fault_string = err;

            //If camera not connected, return relevant error code
            RETURN_ERROR (status_code);
            return ret;
        }

        // Save the application
        param.clear();
        res = "";
        app_url = "";
        // Generating string representing application object's url
        app_url = edit_url + "application/";
        ret = xmlrpcCommandImplementation (app_url, "save", "()", param, res);
        if (ret != 0)
        {
            // Clean up our error-handling environment.
            xmlrpc_env_clean (& (mp_d->environment));
            xmlrpc_client_cleanup();
            return ret;
        }
        param.clear();
        res = "";
        ret = xmlrpcCommandImplementation (edit_url, "stopEditingApplication", "()", param, res);
        if (ret != 0)
        {
            return ret;
        }

#ifdef DEBUG
        printf ("stopped application\n");
#endif
	}

    // Save device configuration
	param.clear();
	res = "";
	device_url = "";
	device_url = edit_url + "device/";
	param.clear();
	res = "";
	ret = xmlrpcCommandImplementation (device_url, "save", "()", param, res);
	if (ret != 0)
	{
		return ret;
	}

    ret = exitFromCameraConfigurationMode();
    if (ret != 0)
	{
		return ret;
	}

    /*Activate the application at given ID on camera */
    std::string serverUrl ("http://" + mp_d->cameraIpAddress + ":" + \
                      to_string ( (unsigned long long) (mp_d->cameraXmlrpcPortNumber)) + ROOT_OBJECT_URL);
    
    param.clear();
	res = "";
    param.push_back (to_string ( (unsigned long long) (mp_d->applicationId)));
	ret = xmlrpcCommandImplementation (serverUrl, "switchApplication", "(s)", param, res);
	if (ret != 0)
	{
		return ret;
	}

    // Increment the number of connections 
    mp_d->noOfConnections++;

    //setting the protocol version to V3
    setProtocolVersion (DEFAULT_PROTOCOL_VERSION);
    RETURN_NOERROR;
}


int32_t O3d3xxCameraOperations::connecttoPCIC()
{
    try
    {
        if (mp_d->isConnected)
        {
            RETURN_NOERROR;
        }
        else
        {
            mp_d->clientSocket = new ClientSocket (mp_d->cameraIpAddress.c_str(), mp_d->cameraImageOutputPortNumber);
            if (!mp_d->dataChannelConfigString.empty())
            {

                std::string data ("1234");  // adding ticket
                data += mp_d->dataChannelConfigString + "\r\n"; // the command itself
                std::stringstream msg;
                msg << "1234L" << std::setfill ('0') << std::setw (9) << data.length() << "\r\n" << data; // adding envelope
                mp_d->clientSocket->sendFrame (msg.str().length(), msg.str().c_str());
                // check resonse
                char response[23];
                mp_d->clientSocket->receiveFrame (sizeof (response), &response);
                if (response[20] != '*')
                {
                    throw std::exception ("Sending PCIC config string not successfull");
                }
            }
            mp_d->isConnected = true;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        RETURN_ERROR (ERR_MEMORY);
    }
    RETURN_NOERROR;
}

int32_t O3d3xxCameraOperations::sendPCICCommand(std::string command, std::string &response)
{
    std::stringstream PCIC_packet, command_ticket;
    std::ostringstream command_ticket_length;
    std::vector<char> socketData;
    //Using "5555" as ticket number for sending PCIC commands
    command_ticket << "5555" << command << "\r\n";
    command_ticket_length << std::setw( 9 ) << std::setfill( '0' ) << command_ticket.str().length();
    PCIC_packet << "5555L" << command_ticket_length.str() << "\r\n" << command_ticket.str();

    mp_d->clientSocket->sendFrame (PCIC_packet.str().length(), PCIC_packet.str().c_str());

    bool response_received = false;
    while(!response_received)
    {
        // Read data on socket
        read_socket_data();

        // Check if response is for the command we have sent
        std::string ticket_number(mp_d->frameData.begin(), mp_d->frameData.begin() + 4);
        if(ticket_number == "5555")
        {
             response_received = true;
        }
        
    }
    /*------------------------------------------------------------------------------------------------------  
        Expected Response is [PCIC protocol Header + 4 digit ticket numbet + Command Response + /r/n]
        PCIC Protocol header at start of frame buffer. Current PCIC version is V3, which requires
        "4 (Ticket) + 10 (<Länge>) + 2 (CR/LF) + 4 (Ticket) = 20 Bytes". Skip over these header bytes.
        Exclude the /r/n at end. The remaining data is response.
     -------------------------------------------------------------------------------------------------------*/
    response = std::string(mp_d->frameData.begin() + 20 , mp_d->frameData.end() - 2);
    if(response == "!")
    {
        mp_d->fault_string = "PCIC command failed";
    }
    else if(response == "?")
    {
        mp_d->fault_string = "Invalid command length";
    }
    return ERR_NONE;
}

int32_t O3d3xxCameraOperations::disconnectPCIC()
{
    if (mp_d->clientSocket != NULL && mp_d->isConnected)
    {
        delete mp_d->clientSocket;
        this->mp_d->isConnected = false;
    }
    return 0;
}

int32_t O3d3xxCameraOperations::disconnectFromCamera()
{
    std::string res;
    std::vector<std::string> param ;
    int ret;

    switchCameraToConfigurationMode();

    string edit_url = "";
    edit_url = mp_d->sessionUrl + "edit/";
    // Deleting the application, that we created
    if (mp_d->isApplicationCreated)
    {
        param.clear();
        res = "";
        param.push_back (to_string ( (unsigned long long) (mp_d->applicationId)));
        ret = xmlrpcCommandImplementation (edit_url, "deleteApplication", "(s)", param, res);
        if (ret != 0)
        {
            return ret;
        }
    }
    
    string device_url = "";
    device_url = edit_url + "device/";
    /*Activate the previously active application on camera */
    if (mp_d->previousActiveApplicationId != 0)
    {
	    param.clear();
	    res = "";
	    param.push_back ("ActiveApplication");
        param.push_back (to_string ( (unsigned long long) (mp_d->previousActiveApplicationId)));
	    ret = xmlrpcCommandImplementation (device_url, "setParameter", "(ss)", param, res);
	    if (ret != 0)
	    {
		    return ret;
	    }
    }
    param.clear();
    res = "";
    // Save device configuration
    ret = xmlrpcCommandImplementation (device_url, "save", "()", param, res);
    if (ret != 0)
    {
        return ret;
    }

    exitFromCameraConfigurationMode();
    
    mp_d->frameData.clear();
    setProtocolVersion (mp_d->protocolVesrion);
    mp_d->noOfConnections--;
    // Shutdown our XML-RPC client library.
    if(mp_d->noOfConnections == 0)
    {
        xmlrpc_env_clean (&mp_d->environment);
        xmlrpc_client_cleanup();
    }

    disconnectPCIC();

    RETURN_NOERROR;
}

/// @cond NOT_TO_BE_EXPOSED


uint32_t O3d3xxCameraOperations::updateCameraFrame()
{
    int res = read_socket_data();

    /*  
        PCIC Protocol header at start of frame buffer. Current PCIC version is V3, which requires
        "4 (Ticket) + 10 (<Länge>) + 2 (CR/LF) + 4 (Ticket) +  4(output description, i.e
        "star/stop")= 24 Bytes". Skip over these header bytes. After this chunk header starts.

        Chunk (image) header contains 9 fields each of 4 bytes. Total = (9*4) = 36 bytes
          
        Read image width at offset of 0x0010 (16) from start of chunk header and image height at offset of 0x0014 (20)
        from start of chunk header 
        
     */

    //memcpy(&mp_d->imageWidth, &(mp_d->frameData[24+16]), 4);
    //memcpy(&mp_d->imageHeight, &(mp_d->frameData[24+20]), 4);

    mp_d->imageHeight = *((uint32_t *)(&(mp_d->frameData[24+20])));
    mp_d->imageWidth = *((uint32_t *)(&(mp_d->frameData[24+16])));

    return res;
}

// This function is used to read image data for getting new frames and also to get response for PCIC commands.
// Hence, made it into a new private function.
int32_t O3d3xxCameraOperations::read_socket_data()
{
    char frameLengthfrmHeader[10];
    char frameheader[O3D3xx_HEADER_BYTES];

    if (mp_d->isConnected)
    {
        memset (frameheader, 0, O3D3xx_HEADER_BYTES);
        // by Default camera is put in V3 and reset to origninal version
        std::string  trigger = "1111L000000008\r\n1111T?\r\n";
        if (mp_d->triggeredMode == 2)
        {
            mp_d->clientSocket->sendFrame (trigger.length(), trigger.c_str());
        }

        int index = mp_d->applicationId;
        mp_d->clientSocket->receiveFrame (O3D3xx_HEADER_BYTES, frameheader); // reading 1st 16byte of header

        //protocol tck 4byte 'L0000000000' 9 byte length and one byte for L and /r/n
        memcpy (frameLengthfrmHeader , frameheader + 5 , 9); // +5 is to get to the start of the length,
        // next 9 byte will content length value

        uint32_t frameSize = atoi (frameLengthfrmHeader) + O3D3xx_HEADER_BYTES; // making an integer value

        // handling memory for different mode
        if (frameSize != mp_d->frameSizeInBytes)
        {
            mp_d->frameData.clear();
            mp_d->frameData.resize (frameSize);
        }
        mp_d->frameSizeInBytes  = frameSize; // copying new data size value
        memcpy (& (mp_d->frameData[0]), frameheader, O3D3xx_HEADER_BYTES); // adding the intial data

        // receiveFrame
        mp_d->clientSocket->receiveFrame (mp_d->frameSizeInBytes - O3D3xx_HEADER_BYTES  , & (mp_d->frameData[O3D3xx_HEADER_BYTES]));

        RETURN_NOERROR;
    }
    RETURN_ERROR (-1);
}


int32_t O3d3xxCameraOperations::switchCameraToConfigurationMode()
{
    std::string res;
    std::vector<std::string> param ;
    int ret ;
    if (mp_d->isConnected)
    {
        disconnectPCIC();
    }

    string serverUrl ("http://" + mp_d->cameraIpAddress + ":" + \
                      to_string ( (unsigned long long) (mp_d->cameraXmlrpcPortNumber)) + ROOT_OBJECT_URL);

    param.clear();
    res = "";
    param.push_back ("");
    param.push_back ("");
    //creating session
    ret = xmlrpcCommandImplementation (serverUrl, "requestSession", "(ss)", param, res);
    if (ret != 0)
    {
        return ret;
    }

    //Generating string representing session object's url
    mp_d->sessionUrl =  serverUrl + "session_" + res + "/";

    /* Setting operating mode to "edit" mode */
    param.clear();
    res = "";
    param.push_back (to_string<int> (EDIT_MODE));

    ret = xmlrpcCommandImplementation (mp_d->sessionUrl, "setOperatingMode", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }
    mp_d->isConfigurationModeON = true;
    RETURN_NOERROR;
}

int32_t O3d3xxCameraOperations::exitFromCameraConfigurationMode()
{
    std::string res;
    std::vector<std::string> param ;
    int  ret;

    // Setting operating mode to "run" mode
    param.clear();
    res = "";
    param.push_back (to_string<int> (RUN_MODE));
    ret = xmlrpcCommandImplementation (mp_d->sessionUrl, "setOperatingMode", "(s)", param, res);
    if (ret != 0)
    {
        return ret;
    }

    param.clear();
    res = "";

    ret = xmlrpcCommandImplementation (mp_d->sessionUrl, "cancelSession", "()", param, res);
    if (ret != 0)
    {
        return ret;
    }

    mp_d->sessionUrl = "";
    mp_d->isConfigurationModeON = false;

    if (!mp_d->isConnected)
    {
        connecttoPCIC();
    }
    RETURN_NOERROR;
}

int32_t O3d3xxCameraOperations::checkIfFaultOccurred()
{
    if (mp_d->environment.fault_occurred)
    {
        fprintf (stdout, "ERROR: %s (%d)\n",
                 mp_d->environment.fault_string, \
                 mp_d->environment.fault_code);
        return mp_d->environment.fault_code;
    }

    RETURN_NOERROR;
}

std::string O3d3xxCameraOperations:: getLastError (void)
{
    return mp_d->fault_string;
}

uint32_t  O3d3xxCameraOperations::getPCICpacket (void *data)
{
    memcpy (data, & (mp_d->frameData[0]), mp_d->frameSizeInBytes);
    return 0;
}

uint32_t O3d3xxCameraOperations::xmlrpcCommand (const char *cmd, std::string &res)
{

    std::vector<std::string> el = splitCommand (cmd);
    switch (el.size())
    {
        case 0:
            res = "empty xml url";
            RETURN_ERROR (1);
            break;
        case 1:
            res = "missing command";
            RETURN_ERROR (1);
            break;
        case 2:
            res = "missing format and parameters";
            RETURN_ERROR (1);
            break;
        default:
            break;
    }
    std::string url ("http://" + mp_d->cameraIpAddress + ":" + \
                     to_string ( (unsigned long long) (mp_d->cameraXmlrpcPortNumber)) + el[0]);
    el.erase (el.begin());
    std::string command = el[0];
    el.erase (el.begin());
    std::string format;
    if (el.size() > 0)
    {
        format = el[0];
        el.erase (el.begin());
    }

    return xmlrpcCommandImplementation (url, command, format, el, res);
}

uint32_t O3d3xxCameraOperations::xmlrpcCommandImplementation (const std::string &url, const std::string &cmd, const std::string &format,
        const std::vector<std::string> &params, std::string &res)
{
    if (url.empty())
    {
        res = "empty url";
        RETURN_ERROR (1);
    }

    xmlrpc_value *response = 0;
    xmlrpc_env env;
    memset (&env, 0, sizeof (env));
    //SY-km Please check if there is a easier way to do that.



    switch (params.size())
    {
        case 0:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str());
            break;
        case 1:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str());
            break;
        case 2:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str());
            break;
        case 3:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str());
            break;
        case 4:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str());
            break;
        case 5:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str());
            break;
        case 6:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str());
            break;
        case 7:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str());
            break;
        case 8:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str());
            break;
        case 9:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str(),
                                           params[8].c_str());
            break;
        case 10:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str(),
                                           params[8].c_str(), params[9].c_str());
            break;
        case 11:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str(),
                                           params[8].c_str(), params[9].c_str(), params[10].c_str());
            break;
        case 12:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str(),
                                           params[8].c_str(), params[9].c_str(), params[10].c_str(), params[11].c_str());
            break;
        case 13:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str(),
                                           params[8].c_str(), params[9].c_str(), params[10].c_str(), params[11].c_str(), params[12].c_str());
            break;
        case 14:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str(),
                                           params[8].c_str(), params[9].c_str(), params[10].c_str(), params[11].c_str(), params[12].c_str(),
                                           params[13].c_str());
            break;
        case 15:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str(),
                                           params[8].c_str(), params[9].c_str(), params[10].c_str(), params[11].c_str(), params[12].c_str(),
                                           params[13].c_str(), params[14].c_str());
            break;
        case 16:
            response = xmlrpc_client_call (& (env),
                                           url.c_str(), cmd.c_str(), format.c_str(), params[0].c_str(), params[1].c_str(), params[2].c_str(),
                                           params[3].c_str(), params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str(),
                                           params[8].c_str(), params[9].c_str(), params[10].c_str(), params[11].c_str(), params[12].c_str(),
                                           params[13].c_str(), params[14].c_str(), params[15].c_str());
            break;
        default:
            res = "maximum number of parameters exceeded";
            RETURN_ERROR (1);
            break;
    }

    if (env.fault_occurred)
    {
        mp_d->fault_string = env.fault_string;

        int tempfaultCode = env.fault_code;
        xmlrpc_env_clean (&env);
        return env.fault_code;
    }

    try
    {
        res = extractFromXmlRpc (response, env, false);

    }
    catch (const std::string &exc)
    {
        mp_d->fault_string = exc;

        int tempfaultCode = env.fault_code;
        xmlrpc_env_clean (&env);
        return ERR_GENERIC;
    }

    if (env.fault_occurred)
    {
        mp_d->fault_string = env.fault_string;

        int tempfaultCode = env.fault_code;
        xmlrpc_env_clean (&env);
        return env.fault_code;
    }
    if (response != NULL)
    {
        xmlrpc_DECREF (response);
        int tempfaultCode = env.fault_code;
        xmlrpc_env_clean (&env);
    }

    RETURN_NOERROR;
}

uint32_t  O3d3xxCameraOperations::setTriggerMode (char *cmd)
{
    std::string res;
    std::vector<std::string> param ;
    std::vector<std::string> el = splitCommand (cmd);
    int ret;


    if (atoi (el.at (1).c_str()) < TRIGGER_MIN_VAL || atoi (el.at (1).c_str()) > TRIGGER_MAX_VAL)
    {
		return ERR_INVALID_ARG ;
    }

    ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }
    string app_url = "";

    /* Generating string representing application object's url */
    app_url = mp_d->sessionUrl + "edit/application/";
    mp_d->triggeredMode = atoi (el.at (1).c_str());
	
	param.clear();
    res = "";
    param.push_back ("TriggerMode");
    param.push_back (to_string ( (unsigned long long) (mp_d->triggeredMode)));
    /*setting the trigger mode */
    ret = xmlrpcCommandImplementation (app_url, "setParameter", "(ss)", param, res);
    if (ret != 0)
    {
        return ret;
    }

    param.clear();
    res = "";
    // saving the application
    ret = xmlrpcCommandImplementation (app_url, "save", "()", param, res);
    if (ret != 0)
    {
        return ret;
    }

    //SY-km Make a function
    /*if (mp_d->isEditModeON)
    {
        string edit_url = "";
        edit_url = mp_d->sessionUrl + "edit/";

        param.clear();
        res = "";
        ret = xmlrpcCommandImplementation (edit_url, "stopEditingApplication", "()", param, res);
        if (ret != 0)
        {
            return ret;
        }
        mp_d->isEditModeON = false;

        string edit_device = "";
        edit_device = mp_d->sessionUrl + "edit/device/";

        param.clear();
        res = "";
        param.push_back ("ActiveApplication");
        param.push_back (to_string ( (unsigned long long) (mp_d->applicationId)));
        ret = xmlrpcCommandImplementation (edit_device, "setParameter", "(ss)", param, res);
        if (ret != 0)
        {
            return ret;
        }

        exitFromCameraConfigurationMode();

        /* As we have ended session, set the session url variable to an empty string *//*
        mp_d->sessionUrl = "";
        if (!mp_d->isConnected)
        {
            connecttoPCIC();
        }

    }*/
    exitFromCameraConfigurationMode();
    RETURN_NOERROR;
}

int32_t O3d3xxCameraOperations::getImagerSettings (std::string command, std::string &response)
{
	std::vector<std::string> param ;
    int ret ;
	bool xml_call_valid = true;

	param.clear();
    if(command == "GetDistanceImageFilter")
	{
		param.push_back ("EnableFilterDistanceImage");
	}
	else if(command == "GetAmplitudeImageFilter")
	{
		param.push_back ("EnableFilterAmplitudeImage");		
	}
	else if(command == "GetSpatialFilterType")
	{
		param.push_back ("SpatialFilterType");
	}
	else if(command == "GetTemporalFilterType")
	{
		param.push_back ("TemporalFilterType");		
	}
	else if(command == "GetSpatialFilterMaskSize")
	{
		param.push_back ("MaskSize");		
	}
	else if(command == "GetTempMeanFilterNumberOfImages")
	{
		param.push_back ("NumberOfImages");
	}
	else if(command == "GetTriggerMode")
	{
		param.push_back ("TriggerMode");
	}
	else if(command == "GetImagerSettingType")
	{
		param.push_back ("Type");
	}
	else if(command == "GetAmplitudeCorrection")
	{
		param.push_back ("EnableAmplitudeCorrection");
	}
	else if(command == "GetRectificationDistanceImage")
	{
		param.push_back ("EnableRectificationDistanceImage");
	}
	else if(command == "GetRectificationAmplitudeImage")
	{
		param.push_back ("EnableRectificationAmplitudeImage");
	}
    else if(command == "GetTargetFrameRate")
	{
		param.push_back ("FrameRate");
	}
    else if(command == "GetExposureTimeList")
    {
        param.push_back ("ExposureTimeList");
    }
    else if(command == "GetExposureTimeRatio")
    {
        param.push_back ("ExposureTimeRatio");
    }
    else if(command == "GetImageResolution")
    {
        param.push_back ("Resolution");
    }
    else if(command == "GetClippingCuboid")
    {
        param.push_back ("ClippingCuboid");
    }
    else
    {
        return ERR_NOT_IMPL;
    }

	// Setting Camera in Edit Mode.
    ret = enterEditMode();
    if (ret != 0)
    {
        return ret;
    }

	string appimager_url = "";
    // string representing of application url
    appimager_url = mp_d->sessionUrl + "edit/application/imager_001";

	
	/********************************************************************************** 
	 *								Filter Function call validation 
	 *********************************************************************************/
	if(command == "GetSpatialFilterMaskSize")
	{
		appimager_url = appimager_url + "/spatialfilter";
	}
	else if(command == "GetTempMeanFilterNumberOfImages")
	{
		appimager_url = appimager_url + "/temporalfilter";
	}
	else if(command == "GetTriggerMode")
	{
		appimager_url = mp_d->sessionUrl + "edit/application/";
	}
		
	ret = xmlrpcCommandImplementation (appimager_url, "getParameter", "(s)", param, response);
	if (ret != 0)
	{
		return ret;
	}
		
    //Generating string representing edit object's url
    ret = exitEditMode() ;
    if (ret != 0)
    {
        return ret;
    }

	RETURN_NOERROR;
}

///@endcond
/* @} */
/* @} */