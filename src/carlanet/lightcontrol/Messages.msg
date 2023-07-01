
import inet.common.INETDefs;
import inet.common.Units;
import inet.common.packet.chunk.Chunk;
import inet.networklayer.common.L3Address;

namespace inet;

class LightCommandMessage extends FieldsChunk{
	//simtime_t creationTime;
	string lightNextState;
}

class LightStatusMessage extends FieldsChunk{
	//simtime_t creationTime;
	string lightCurrState;
}