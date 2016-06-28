/**< Defines default port for HTTP protocol */
#define HTTP_PORT_NUMBER 80

/** Define Filter types available on O3D3xx */

// Spatial Filter Types
#define SPATIAL_FILT_OFF 0				// Filtering OFF
#define SPATIAL_FILT_MEAN 1				// Mean Filtering
#define SPATIAL_FILT_MEDIAN 2			// Median Filtering
#define SPATIAL_FILT_BILATERAL 3		// Bilateral Filtering 
#define SPATIAL_FILT_MAX_VAL 3			// Maximum Valid value for SpatialFilterType

// Spatial Filter Mask Size
#define SPATIAL_FILT_MASKSIZE_3X3 0		// Use 3X3 Mask
#define SPATIAL_FILT_MASKSIZE_5X5 1		// Use 5X5 Mask 
#define SPATIAL_FILT_MASKSIZE_MAX_VAL 1 // Maximum Valid value for SpatialFilter MaskSize

// Temporal Filter Types
#define TEMPORAL_FILT_OFF 0				// Filtering OFF
#define TEMPORAL_FILT_MEAN 1			// Mean Filter
#define TEMPORAL_FILT_ADP_EXP 2			// Adaptive Exponential Filter
#define TEMPORAL_FILT_MAX_VAL 2			// Maximum Valid value for TemporalFilterType	

// Temporal Mean Filter Number of Images
// Valid values range is [2-25]. Currently, using 2 images for temporal mean filter.
#define TEMPORAL_MEAN_FILT_MIN_IMG 2	// Minimum number of images to be used for temporal mean filtering
#define TEMPORAL_MEAN_FILT_MAX_IMG 25	// Maximum number of images to be used for temporal mean filtering

/** Trigger Mode for O3d3xx */
#define CONTINUOUS 1					// Free Run Mode
#define PROCESS_INTERFACE 2				// Process Interface Trigger
#define POSITIVE_EDGE 3					// Positive Edge Trigger - From Pin in power cable
#define NEGATIVE_EDGE 4					// Negative Edge Trigger - From Pin in power cable
#define POSITIVE_AND_NEGATIVE_EDGE 5	// Positive and negative Edge Trigger - From Pin in power cable
#define TRIGGER_MIN_VAL 1				// Minimum Valid value for Trigger Mode
#define TRIGGER_MAX_VAL 5				// Maximum Valid value for Trigger Mode

/** Maximum Background Distance - Single/Double/Triple Frequency based distance calculation */
#define UNDER_5m 0						// Less than 5 m [1 frequency]
#define UPTO_30m 1						// Upto 30 m [2 frequency]
#define MORE_THAN_30m 2					// More than 30 m [3 frequency]
#define MAX_BGND_DIST_VAL 2				// Maximum Valid value	

/** Exposure Mode - Low/Moderate/High Exposure modes */
#define LOW 0							// Low [1 integration time]
#define MODERATE 1						// Moderate [2 integration times]
#define HIGH 2							// High [3 integration times]
#define MAX_EXP_MODE_VAL 2			    // Maximum Valid value	

/** Exposure Time - Min and Max Values in usec */
#define EXP_TIME_MIN 0 
#define EXP_TIME_MAX 10000 

/** Exposure Time Ratio - Min and Max Values */
#define EXP_TIME_RATIO_MIN 2 
#define EXP_TIME_RATIO_MAX 50

/** Index for Setting Exposure Time */
#define EXP_TIME_INDEX 0                // Index to set exposure time
//#define EXP_TIME_RATIO_INDEX 1          // Index to set exposure time ratio
#define EXP_TIME_MAX_INDEX 0            // Maximum value of index used to set exposure time

/** Modulation Frequency Channel */
#define MOD_FREQ_CHANNEL_0 0
#define MOD_FREQ_CHANNEL_1 1
#define MOD_FREQ_CHANNEL_2 2
#define MOD_FREQ_CHANNEL_3 3
#define MOD_FREQ_CHANNEL_MAX_VAL 3

/** TARGET FRAME RATE in fps */
#define TARGET_FRAME_RATE_MIN 0.0167
#define TARGET_FRAME_RATE_MAX 30.0

/** DATA CONFIG BIT DEFINITION */
#define ALL_IMAGES_BIT          0x01
#define DISTANCE_BIT            0x02
#define NORM_AMPLITUDE_BIT      0x04
#define AMPLITUDE_BIT           0x08
#define X_IMAGE_BIT             0x10
#define Y_IMAGE_BIT             0x20
#define Z_IMAGE_BIT             0x40
#define CONFIDENCE_IMAGE_BIT    0x80
#define DIAGNOSTIC_DATA_BIT     0x100

/** IMAGE RESOLUTION */
#define RES_176X132 0
#define RES_352X264 1
#define RES_MAX 1

