
#include "LSuperIO.h"

#include "Windows/LAsusEC_SuperIO/OlsApi.h"
#include "Windows/LAsusEC_SuperIO/OlsDef.h"

LSuperIO::LSuperIO(WORD indexPort, WORD dataPort)
{
    INDEX_PORT = indexPort;
    DATA_PORT = dataPort;
}

LSuperIO::~LSuperIO()
{

}

void LSuperIO::WriteIndexPortByte(BYTE data)
{
    WriteIoPortByte(INDEX_PORT, data);
}

BYTE LSuperIO::ReadDataPortByte()
{
    return ReadIoPortByte(DATA_PORT);
}

void LSuperIO::WriteDataPortByte(BYTE data)
{
    WriteIoPortByte(DATA_PORT, data);
}

LWinbondIO::LWinbondIO()
    : LSuperIO(0x2e, 0x2f)
{

}

LWinbondIO::~LWinbondIO()
{

}