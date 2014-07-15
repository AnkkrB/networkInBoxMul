#include "NALParser.h"

NALParser::NALParser()
{}

//Go through the buffer to find the NAL unit type
NALUnitType NALParser::parseNalType(uint8_t *buf, int length)
{
	NALUnitType type = NAL_TYPE_UNSPEC;
	//Iterate through buffer until NAL type is found
	for(int i = 0; i < length; i++)
	{
		//If this byte is a zero
		if(buf[i] == 0x00 && i < length - 4)
		{
			if(buf[i+1] == 0x00 &&
					(buf[i+2] == 0x01 || 
					 (buf[i+2] == 0x00 && buf[i+3] == 0x01)))
			{
				//Inspect the next byte for the type
				uint8_t nal_header = (buf[i+2] == 0x01 ? buf[i+3] : buf[i+4]);
				cout << "NAL header: 0x" << hex << setw(2) << setfill('0') << (int) nal_header << "\tByte position: " << dec << (buf[i+2] == 0x01 ? i+3 : i+4) << endl;
				cout << hex << (int) buf[i+2] << "\t" << (int) buf[i+3] << "\t" << (int) buf[i+4] << endl;
				type = static_cast<NALUnitType>( nal_header & 0x1F );
			//	break;
				i += 4;
			}
		}
	}
	return type;
}
