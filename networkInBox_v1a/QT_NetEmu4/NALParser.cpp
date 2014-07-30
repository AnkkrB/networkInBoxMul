#include "NALParser.h"

NALParser::NALParser() :
	m_current_nal(NAL_TYPE_IDR),
	m_current_ts(1.0),
	m_current_tg(1.0)
{}

//Prioritize the packet based on linear model
//This prioritization method is based on the model 
//developed by Perez and Garcia in their 2011 paper.
//The basic model is:
//P = alpha*P_s + beta*H + gamma*T_s + delta*T_G
//Coefficients are held consistent with the authors'
//recommendations
//All parameters are normalized between 0 and 1
//Parameters are:
//P_s: Type of frame, indicated by nal_unit_type
//H: Presence of NAL header
//T_s: Number of packets until next slice. Since we will not know this, we will adopt a decreasing exponential model. The first packet of the slice will have T_s = 1, and each subsequent packet will have T_s = T_s / 2
//T_G: Number of packets until next GOP. We will adapt this to number of frames until next GOP
PacketPriority NALParser::prioritizePacket(const char *buf, int length)
{
	PacketPriority priority = PRIORITY_HIGH;
	float H = 0.0;
	float P_s = 0.0;
	//Parse the NAL type	
	NALUnitType type = parseNalType(buf, length);
	//Check the NAL type
	switch(type)
	{
		case NAL_TYPE_IDR:
			m_current_nal = type;
			H = 1.0;
			m_current_ts = 1.0;
			m_current_tg = 1.0;
			P_s = 1.0;
			break;
		case NAL_TYPE_PFRAME:
			m_current_nal = type;
			H = 1.0;
			m_current_ts = 1.0;
			m_current_tg -= (float) 1 / GOP_LENGTH;
			P_s = 0.5;
			break;
		case NAL_TYPE_BFRAME:
			m_current_nal = type;
			H = 1.0;
			m_current_ts = 1.0;
			m_current_tg -= (float) 1 / GOP_LENGTH;
			P_s = 0.0;
			break;
		case NAL_TYPE_UNSPEC:
			H = 0.0;
			m_current_ts /= 2;
			P_s = (m_current_nal == NAL_TYPE_IDR ? 1.0 : (m_current_nal == NAL_TYPE_PFRAME ? 0.5 : 0.0));
			break;
	}

	//Perform the calculation
	float P = ALPHA * P_s + BETA * H + GAMMA * m_current_ts + DELTA * m_current_tg;

	cout << "Priority score: " << P << endl;
	//Heuristic placement of packets into priority bins
	if(P > 1002.0)
		priority = PRIORITY_HIGH;
	else if(P > 700.0)
		priority = PRIORITY_MED_HIGH;
	else if(P > 200.0)
		priority = PRIORITY_MED_LOW;
	else
		priority = PRIORITY_LOW;

    return priority;
}

//Go through the buffer to find the NAL unit type
NALUnitType NALParser::parseNalType(const char *buf, int length)
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
                char nal_header = (buf[i+2] == 0x01 ? buf[i+3] : buf[i+4]);
				cout << "NAL header: 0x" << hex << setw(2) << setfill('0') << (int) nal_header << "\tByte position: " << dec << (buf[i+2] == 0x01 ? i+3 : i+4) << endl;
				cout << hex << (int) buf[i+2] << "\t" << (int) buf[i+3] << "\t" << (int) buf[i+4] << endl;
				type = static_cast<NALUnitType>( nal_header & 0x1F );
				if(type == NAL_TYPE_PFRAME && ((nal_header & 0x60) == 0))
						type = NAL_TYPE_BFRAME;
			//	break;
				i += 4;
			}
		}
	}
	return type;
}
