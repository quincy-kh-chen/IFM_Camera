/* Definitions of xmlrpc wrapper C++ class required
for doing basic camera operations */
#include "o3d3xx_camera.hpp"
#include "gtest/gtest.h"

/* Error code definitions */
#include "err.h"

/*Minimum and maximum value of valid integration time for O3D3XX camera */
#define MAX_INTEGRATION_TIME  17476
#define MIN_INTEGRATION_TIME 0

/*Resolution of camera*/
#define FRAME_HEIGHT 132
#define FRAME_WIDTH 176
#define FRAME_HEIGHT_FULL 264
#define FRAME_WIDTH_FULL 352


#define PIXEL_COUNT (FRAME_HEIGHT *  FRAME_WIDTH)

/** Defines the size of protocol
header at start of frame buffer. Current PCIC version is V3, which requires
"4 (Ticket) + 10 (<Länge>) + 2 (CR/LF) + 4 (Ticket) +  4(output description, i.e
"star/stop")= 24 Bytes"*/
#define PCIC_V3_PROTOCOL_HEADER_SIZE_START 24

/* Defines the size of protocol
header at the end of frame. It is 4(output description) + 2 (CR LF) = 6 bytes*/
#define PCIC_V3_PROTOCOL_HEADER_SIZE_STOP 6
/*Defines the size of image header present in
frame. It contains 9 fields each of 4 bytes*/
#define IMAGE_HEADER_SIZE (4*9)
/*Defines number of bytes in diagonistic data, in bytes*/
#define DIAGNOSTIC_DATA_SIZE 20
/*Defines number of phases used in camera, treated here as constant*/
#define NO_OF_PHASES 4
/*Defines number of images outputted by camera
in a frame */
#define NO_OF_IMAGES_IN_FRAME 6

/* Declaring the string variable to hold camera IP. The variable is defined in gtes_main.cc */ 
extern std::string CAMERA_IP;
