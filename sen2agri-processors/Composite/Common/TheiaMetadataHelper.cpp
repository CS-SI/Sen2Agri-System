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
        m_CloudFileName = this->getCloudFileName();
        m_WaterFileName = this->getWaterFileName();
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
