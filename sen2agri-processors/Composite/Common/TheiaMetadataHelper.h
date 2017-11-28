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

#ifndef THEIAMETADATAHELPER_H
#define THEIAMETADATAHELPER_H

#include "MetadataHelper.h"

#include "../../MACCSMetadata/include/THEIAMetadataReader.hpp"

//TODO replace the namespace
typedef itk::THEIAMetadataReader    THEIAMetadataReaderType;

class TheiaMetadataHelper : public MetadataHelper
{

public:

    TheiaMetadataHelper();

    const char * GetNameOfClass() { return "TheiaMetadataHelper"; }
    
    // TODO
    virtual std::string GetBandName(unsigned int nIdx, bool bRelativeIdx=true);
    // TODO
    virtual int GetRelativeBandIndex(unsigned int nAbsBandIdx) { return nAbsBandIdx; }
    // TODO
    virtual MetadataHelper::SingleBandShortImageType::Pointer GetMasksImage(MasksFlagType nMaskFlags, bool binarizeResult);
    // TODO
    virtual int GetResolutionForAbsoluteBandIndex(int nAbsBandIdx) { return nAbsBandIdx; }
    

protected:
    virtual bool DoLoadMetadata();
    
    double m_AotQuantificationValue;
    double m_WatQuantificationValue;
    
    std::unique_ptr<THEIAMetadata> m_metadata;

    std::string getCloudFileName();
    std::string getWaterFileName();
};

#endif // THEIAMETADATAHELPER_H
