#ifndef SPOT4METADATAHELPER_H
#define SPOT4METADATAHELPER_H

#include "MetadataHelper.h"

#include "../../MACCSMetadata/include/SPOT4MetadataReader.hpp"
typedef itk::SPOT4MetadataReader                                   SPOT4MetadataReaderType;


class Spot4MetadataHelper : public MetadataHelper
{
public:
    Spot4MetadataHelper();

    const char * GetNameOfClass() { return "Spot4MetadataHelper"; }

protected:
    virtual bool DoLoadMetadata();

    std::string DeriveFileNameFromImageFileName(const std::string& replacement);
    std::string buildFullPath(const std::string& fileName);

    std::string getImageFileName();
    std::string getAotFileName();
    std::string getCloudFileName();
    std::string getWaterFileName();
    std::string getSnowFileName();

    std::unique_ptr<SPOT4Metadata> m_metadata;
};

#endif // SPOT4METADATAHELPER_H
