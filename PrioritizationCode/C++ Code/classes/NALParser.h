#ifndef NALPARSER_H
#define NALPARSER_H

#include <iostream>
#include <iomanip>
#include <cstdint>

using namespace std;

//Enumeration for NAL unit types
typedef enum {
	 NAL_TYPE_UNSPEC = 0
	,NAL_TYPE_PFRAME
	,NAL_TYPE_SLICE_PARTITION_A
	,NAL_TYPE_SLICE_PARTITION_B
	,NAL_TYPE_SLICE_PARTITION_C
	,NAL_TYPE_IDR
	,NAL_TYPE_ENHANCEMENT_INFO
	,NAL_TYPE_SPS
	,NAL_TYPE_PPS
	,NAL_TYPE_ACCESS_UNIT_DELIMITER
	,NAL_TYPE_END_OF_SEQUENCE
	,NAL_TYPE_END_OF_STREAM
	,NAL_TYPE_FILTER_DATA
	,NAL_TYPE_SPS_EXTENSION
	,NAL_TYPE_PREFIX_NAL_UNIT
	,NAL_TYPE_SUBSET_SPS
	,NAL_TYPE_SLICE_AUX_NO_PART = 19
	,NAL_TYPE_CODED_SLICE_EXT
	,NAL_TYPE_CODED_SLICE_EXT_FOR_DEPTH_VIEW
} NALUnitType;

//NAL parser class definiton
class NALParser 
{
	public:
	//Ctor
	NALParser();

	//Parse the data to find the NAL unit type
	NALUnitType parseNalType(uint8_t *buf, int length);
	private:

};

#endif
