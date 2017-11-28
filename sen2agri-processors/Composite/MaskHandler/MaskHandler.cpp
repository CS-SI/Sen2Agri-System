/*=========================================================================
  *
  * Program:      Sen2agri-Processors
  * Language:     C++
  * Copyright:    2015-2016, CS Romania, office@c-s.ro
  * See COPYRIGHT file for details.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.

 =========================================================================*/
 
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbBandMathImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"

#include "itkBinaryFunctorImageFilter.h"

#include "MaskExtractorFilter.hxx"

#include <vector>
#include "libgen.h"
#include "MaskHandlerFunctor.h"
#include "MetadataHelperFactory.h"

namespace otb
{

namespace Wrapper
{

class MaskHandler : public Application
{
public:    

    typedef MaskHandler Self;
    typedef Application Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;
 
    itkNewMacro(Self)

    itkTypeMacro(MaskHandler, otb::Application)
 
    typedef otb::ImageFileReader<Int16VectorImageType>                          ReaderType;
    typedef otb::ImageList<Int16ImageType>                                      ImageListType;
    typedef ImageListToVectorImageFilter<ImageListType, Int16VectorImageType >  ListConcatenerFilterType;
    typedef MaskHandlerFunctor <Int16VectorImageType::PixelType,
                                    Int16VectorImageType::PixelType, Int16VectorImageType::PixelType> MaskHandlerFunctorType;

    typedef itk::BinaryFunctorImageFilter< Int16VectorImageType, Int16VectorImageType,
                                            Int16VectorImageType, MaskHandlerFunctorType > FunctorFilterType;

    typedef otb::MaskExtractorFilter<Int16VectorImageType, Int16VectorImageType> MaskExtractorFilterType;

private:

    void DoInit()
    {
        SetName("MaskHandler");
        SetDescription("Extracts Cloud, Water and Snow masks from _div.tif and _sat.tif SPOT files ");

        SetDocName("MaskHandler");
        SetDocLongDescription("long description");
        SetDocLimitations("None");
        SetDocAuthors("AG");
        SetDocSeeAlso(" ");        
        AddDocTag(Tags::Vector);        
        AddParameter(ParameterType_String, "xml", "General xml input file for L2A");
        AddParameter(ParameterType_Int, "sentinelres", "Resolution for which the masks sould be handled, SENTINEL-S2 only");
        MandatoryOff("sentinelres");

        AddParameter(ParameterType_OutputImage, "out", "Out file for cloud, water and snow mask");

    }

    void DoUpdateParameters()
    {
      // Nothing to do.
    }

    void DoExecute()
    {
        const std::string &inXml = GetParameterAsString("xml");
        auto factory = MetadataHelperFactory::New();

        int resolution = 10;
        if(HasValue("sentinelres")) {
            resolution = GetParameterInt("sentinelres");
        }
        auto pHelper = factory->GetMetadataHelper(inXml, resolution);
        std::string missionName = pHelper->GetMissionName();
        if((missionName.find(SENTINEL_MISSION_STR) != std::string::npos) &&
           !HasValue("sentinelres")) {
           itkExceptionMacro("In case of SENTINEL-S2, 'sentinelres' parameter with resolution as 10 or 20 meters should be provided");
        }

        m_MaskExtractor = MaskExtractorFilterType::New();
        m_ReaderCloud = ReaderType::New();
        m_ReaderWaterSnow = ReaderType::New();
        if(missionName.find(SPOT4_MISSION_STR) != std::string::npos ||
                (missionName.find(SPOT5_MISSION_STR) != std::string::npos)) {
            m_MaskExtractor->SetBitsMask(0, 1, 2);
        } else if ((missionName.find(LANDSAT_MISSION_STR) != std::string::npos) ||
                   (missionName.find(SENTINEL_MISSION_STR) != std::string::npos)) {
            m_MaskExtractor->SetBitsMask(0, 0, 5);
        } else {
            itkExceptionMacro("Unknown mission: " + missionName);
        }
        m_ReaderCloud->SetFileName(pHelper->GetCloudImageFileName());
        m_ReaderWaterSnow->SetFileName(pHelper->GetWaterImageFileName());

        m_MaskExtractor->SetInput(0, m_ReaderCloud->GetOutput());
        m_MaskExtractor->SetInput(1, m_ReaderWaterSnow->GetOutput());

        SetParameterOutputImagePixelType("out", ImagePixelType_int16);
        SetParameterOutputImage("out", m_MaskExtractor->GetOutput());

        return;
    }

    ReaderType::Pointer                 m_ReaderCloud;
    ReaderType::Pointer                 m_ReaderWaterSnow ;
    MaskHandlerFunctorType          m_Functor;
    MaskExtractorFilterType::Pointer             m_MaskExtractor;
};

}
}
OTB_APPLICATION_EXPORT(otb::Wrapper::MaskHandler)
