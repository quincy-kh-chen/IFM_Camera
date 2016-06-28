/** @file
@brief This file represents the class which contains functions to operate
O3D3xx camera

This file contains functions which allow to connect, disconnect, get/set
parameters to O3D3xx camera and capture frame from the O3D3xx camera.

@author Winy Kakani\n\n
Version: 1.0\n\n

Copyright (C) 2014 ifm electronic GmbH\n\n
See accompanied file LICENCE.TXT for license information.
*/
#ifndef O3D3XX_CAMERA_HPP_2014_06_19_04_44
#define O3D3XX_CAMERA_HPP_2014_06_19_04_44

#include <xmlrpc-c/base_int.h>

/* Standard include files */
#if (defined(_MSC_VER) && _MSC_VER < 1600)
//#include <pstdint.h>
#else
#include <stdint.h>
#endif

#include <string>
#include <vector>
#include "o3d3xx_defines.hpp"
#pragma warning( disable : 4290 )
#ifdef DLL_EXPORTS
#define DLL_INTERFACE_API __declspec(dllexport)/**< Macro specifying that the class definition be exported */
#else
#define DLL_INTERFACE_API __declspec(dllimport)/**< Macro specifying that the class definition be imported */
#endif
// Define the size of data frame header in bytes
#define O3D3xx_HEADER_BYTES 16

using namespace std;

/**< Forward declaration of frame data descriptor */
struct FrameDataDescriptor;

/** @class
@brief This class implements basic operations for accessing O3D3xx camera

This class contains functions which allow to connect, disconnect, capture frame,
and configure camera, from the O3D3xx camera.
*/
class DLL_INTERFACE_API O3d3xxCameraOperations
{
    public:
        /** Default constructor*/
        O3d3xxCameraOperations (void);

        /** Default destructor */
        ~O3d3xxCameraOperations (void);

        /**
        @brief Connected to the camera
        @param[in] ip_address
            ip address of camera to connect to
        @param[in] port_number
            port number of camera for xmlrpc commands communication
        @param[in] pcic_tcp_port
            the tcp ip port number of camera, at which image shall be
            outputted.
        @return ERR_NONE, if no error occurred, else relevant error code
        */
        int32_t connectToCamera (string ip_address, \
                                 uint16_t port_number = HTTP_PORT_NUMBER, uint16_t pcic_tcp_port = 0 , unsigned int applicationID = 0);

        /**
        @brief Disconnect from camera
        @param[in] none
        @return ERR_NONE, if no error occurred, else relevant error code
        */
        int32_t disconnectFromCamera (void);

        /**
        @brief Get the size of the last retrieved frame in bytes
        @param[in,out] dataSize
            Size of frame buffer in bytes
        @return ERR_NONE, if no error occurred, else relevant error code
        */
        int32_t getFrameDataSize (uint32_t *dataSize);

        /**
        @brief
            Set the integration time of the camera.
        @param[in] integrationTime
            Integration time to be configured in camera, in microseconds.
        @return ERR_NONE, if no error occurred, else error code
        @note This function supports first exposure time only (index=0).
        */
        int32_t setIntegrationTime (uint32_t integrationTime , unsigned int index);

		/**
        @brief
            Get the imager settings on the camera.
		@param[in] command
            Get the Filter parameters.
        @param[in,out] response
            Value of the parameter queried in string, on success.
        @return ERR_NONE, if no error occurred, else relevant error code
        @note This function supports first exposure time only (index=0).
        */
		int32_t getImagerSettings (std::string command, std::string &response);
		
		/**
        @brief
            Sets parameters for applying filtering on data.
        @param[in] command
            Command string to be implemented.
        @param[in] value
            Value to be set by the command. 
        @param[in,out] response
            Response String to indicate the nature of error, "OK" in case of success.
        @return ERR_NONE, if no error occurred, else error code
        */
        int32_t setImagerSettings (std::string command, std::string value, std::string &response);

		/**
        @brief
            Sets the imager setting type of the camera.
        @param[in] command
            Command string to be implemented.
        @param[in] value
            Value to be set by the command. 
        @param[in,out] response
            Response String to indicate the nature of error, "OK" in case of success.
        @return ERR_NONE, if no error occurred, else error code
        */
        int32_t setImagerType (std::string command, std::string value, std::string &response);

        /**
        @brief
            Sets the Modulation Frequency Channel.
        @param[in] value
            Value to be set by the command. 
        @return ERR_NONE, if no error occurred, else error code
        */
		int32_t setModulationFrequencyChannel(unsigned value);
		
		/**
        @brief
            Finds the optimum exposure time.
        @param[in] command
            Command string to be implemented.
        @param[in] value
            Value to be set by the command. 
        @param[in,out] response
            Response String to indicate the nature of error, "OK" in case of success.
        @return ERR_NONE, if no error occurred, else error code
        */
		int32_t optimize (std::string &response);

		/**
        @brief
            Get the integration time of the camera.
        @param[in,out] integrationTime
            Integration time read from camera, in microseconds.
        @return ERR_NONE, if no error occurred, else relevant error code
        @note 
        */
        int32_t  getIntegrationTime (uint32_t *integrationTime , unsigned int index);

        /**
        @brief
            Get the modlation frequency channel of the camera.
        @param[in,out] modulationFrequencyChannel
            Modulation Frequency Channel read from camera, in MHz.
        @return ERR_NONE, if no error occurred, else relevant error code
        @note 
        */
		int32_t getModulationFrequencyChannel (uint32_t *modulationFrequencyChannel);

		/**
        @brief
            Get the resolution of the camera by reading from stored data.
        @param[in,out] frameWidth
            Width of frame in pixels
        @param[in,out] frameHeight
            Height of frame in pixels
        @return ERR_NONE, if no error occurred, else relevant error code
        @note The get parameters shall be got from the current active application on
        camera.
        */
        int32_t  readResolution (uint32_t *frameWidth, uint32_t *frameHeight);

        /**
        @brief
            Get the resolution of the camera.
        @param[in,out] frameWidth
            Width of frame in pixels
        @param[in,out] frameHeight
            Height of frame in pixels
        @return ERR_NONE, if no error occurred, else relevant error code
        @note The get parameters shall be got from the current active application on
        camera.
        */
        int32_t  getResolution (uint32_t *frameWidth, uint32_t *frameHeight);

        /**
        @brief Passes the error string from the xmlRpc to the user
        @param[in] none
        @return error string
        @note:  provide functionality similar to pmdGetLastError of the PMDSDK
        */
        std::string getLastError (void);

        /**
        @brief Copies the last frame to the allocated memory buffer.
        @param[in] data the data buffer pointer
        @return error code
        @note: Use the getFrameDataSize function to get the buffer size.
        */
        uint32_t getPCICpacket (void *data);

        /**
        @brief Grab the next frame from camera
        @param[in] none
        @return error code
        @note: Send a software trigger before if in soft-trigger mode.
        */
        uint32_t updateCameraFrame();

        /**
        @brief Send PCIC command to camera
        @param[in] command
            PCIC sommand string
        @param[in,out] response
            Response from camera
        @return error code
        @note: Send a software trigger before if in soft-trigger mode.
        */
        int32_t sendPCICCommand(std::string command, std::string &response);

        /**
        @brief function checks the validity of the string for different xmlrpc Commands
        @param [in] xmlRPC command string
        @return  string  the response of the xmlrpcommand
        @note:
        */
        uint32_t xmlrpcCommand (const char *cmd, std::string &res);

        /**
        @brief function sets the trigger mode of the camera
        @param [in] String must in following format "SetTriggerMode mode "
                    mode can have following values
                    1  -free run mode
                    2  -process interface (software trigger)
        @return  string  the response of the xmlrpcommand
        @note:
        */
        uint32_t setTriggerMode (char *cmd);

		/**
        @brief function gets the trigger mode of the camera
        @param [in] String must in following format "GetTriggerMode"
        @return  string  Value containing the trigger mode
        @note:
        */
		int32_t getTriggerMode (char *cmd, std::string response);

        /**
        @brief used to connect to PCIC socket
        @param none
        @return 0 if successful and -1 otherwise
        @note:
        */
        int32_t connecttoPCIC();
        /**
        @brief used to disconnect PCIC socket
        @param none
        @return 0 if successful and -1 otherwise
        @note:
        */
        int32_t disconnectPCIC();

        /**
        @brief command for setting the PCIC command (c-command) string at PCIC data connection
        @param the command string
        */
        void setPCICDataConfigString (const std::string &s);

        /**
        @brief retrieving the PCIC configuration command as string
        @param none
        @return the c-command
        */
        std::string getPCICDataConfigString();

        /**
        @brief Configures the source data at output
        @param the command string
        @return 0 if successful
        */
        int32_t O3d3xxCameraOperations::generatePCICTcpResultSchema (const std::string &s);

    private:
        /// @cond NOT_TO_BE_EXPOSED
        /** Structure to hold private data of this class */
        struct PrivateData;
        struct PrivateData *mp_d;

        /**
        @brief This function changes mode of camera to edit/configuration.
        @return Error code
        */
        int32_t switchCameraToConfigurationMode (void);

        /**
        @brief This function exits the configuration mode of camera and changes
        camera mode to "run" mode.
        @param[in] none
        @return error code
        */
        int32_t exitFromCameraConfigurationMode (void);

        /**
        @brief This function creates a session and put application in edit mode .
        @return Error code
        */
        int32_t enterEditMode (void);

        /**
        @brief This function exit the edit mode
        @param[in] none
        @return error code
        */
        int32_t exitEditMode (void);
		
		/**
        @brief This function set the protocol version to V3
        @param[in] none
        @return error code
        */
        int32_t setProtocolVersion (std::string version);

        /**
        @brief This functions.checks the network parameters.
        @param[in] ip_address
        @param[in] point_number
        @param[in] pcic_tcp_port
        @param[in] applicationID
        @return error code
        */
        int checkParameterValidity (string ip_address, uint16_t port_number, uint16_t pcic_tcp_port , unsigned int applicatioID);

        /**
        @brief This function checks whether any error occurred during any of preceding xmlrpc calls.
            If error has occurred/logged, then an error code is returned.
        @param[in] none
        @return error code if fault occur
        */
        int32_t checkIfFaultOccurred ();

        /**
        @brief function to parse the string to xmlCommand and call appropriate command
        @param [in] xmlRPC command string
        @return string of the response of the xmlrpcommand
        @note:
        @ToDO inbangsa: need to add more data about function
        */
        uint32_t xmlrpcCommandImplementation (const std::string &url, const std::string &cmd, const std::string &format, const std::vector<std::string> &params, std::string &res);

        /**
        @brief Generates the PCIC TCp Reslt Schema String
        @param the command string
        @return 0 if successful
        */
        void generatePCICTcpResultSchema (std::string &s, uint32_t value);

        /**
        @brief Reads complete data from socket
        @param none
        @return 0 if successful
        */
        int32_t read_socket_data();

        ///@endcond
};

#endif //O3D3XX_CAMERA_HPP_2014_06_19_04_44