/*=========================================================================
  *
  * Program:      Sen2agri-Processors
  * Language:     C++
  * Copyright:    2017, CS SI
  * See COPYRIGHT file for details.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.

 =========================================================================*/
 
#include "TheiaMetadataHelper.h"


TheiaMetadataHelper::TheiaMetadataHelper()
{
    m_AotQuantificationValue = 200.0;
    m_WatQuantificationValue = 20.0;
}

bool TheiaMetadataHelper::DoLoadMetadata()
{
    THEIAMetadataReaderType::Pointer theiaMetadataReader = THEIAMetadataReaderType::New();

    if (m_metadata = theiaMetadataReader->ReadMetadata(m_inputMetadataFileName)) {
        m_Mission = m_metadata->datasetIdentification.Project;
        m_ProducerName = m_metadata->datasetIdentification.Producer;
        std::string acquisitionDate = m_metadata->productCharacteristics.AcquisitionDate;
        acquisitionDate.erase(7,1);
        acquisitionDate.erase(4,1);
        m_AcquisitionDate = acquisitionDate.substr(0,acquisitionDate.find("T"));
        m_Instrument = m_metadata->productCharacteristics.Platform;
        m_CloudFileName = this->getCloudFileName();
        m_WaterFileName = this->getWaterFileName();
        m_ImageFileName = this->getImageFileName();
        m_AotFileName = this->getAotFileName();
        m_nAotBandIndex = 2; // start from 1 (1 = Water wapor)
        m_fAotQuantificationValue = 0.005; // 1/200
        m_fAotNoDataVal = 0;
        return true;
    }

    return false;
}

// TODO
std::string TheiaMetadataHelper::GetBandName(unsigned int nIdx, bool bRelativeIdx)
{
    std::string returnValue="";
    return returnValue;
}

// TODO
MetadataHelper::SingleBandShortImageType::Pointer 
TheiaMetadataHelper::GetMasksImage( MasksFlagType nMaskFlags, 
                                    bool binarizeResult) 
{
    std::vector< MetadataHelper::SingleBandShortImageType::Pointer> vecImgs;
    
    return vecImgs.front();
}

std::string TheiaMetadataHelper::getCloudFileName()
{
    return buildFullPath(m_metadata->productOrganisation.MASKS.find("Cloud")->second.find("R1")->second);
}

std::string TheiaMetadataHelper::getWaterFileName()
{
    return buildFullPath(m_metadata->productOrganisation.MASKS.find("Cloud_Shadow")->second.find("R1")->second);
}

std::string TheiaMetadataHelper::getImageFileName()
{
    THEIAImg theiaImg = m_metadata->productOrganisation.IMGS.find("Flat_Reflectance")->second;
    std::string theiaImgB2 = theiaImg.find("B2")->second;
    std::string theiaVRT = theiaImgB2;
    //SENTINEL2A_20170909-063507-461_L2A_T40KCB_D_V1-4_FRE_B2.tif
    //SENTINEL2A_20170909-063507-461_L2A_T40KCB_D_V1-4_FRE.vrt
    theiaVRT.replace(theiaImgB2.find("_B2.tif"),theiaImgB2.find("_B2.tif")+6,".vrt");
    return buildFullPath(theiaVRT);
}

std::string TheiaMetadataHelper::getAotFileName()
{
    return buildFullPath(m_metadata->productOrganisation.IMGS.find("Aerosol_Optical_Thickness")->second.find("R1")->second);
}
