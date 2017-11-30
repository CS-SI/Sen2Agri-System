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
 
#include "ComputeNDVI.h"
#include "MetadataHelperFactory.h"
#include "otbWrapperMacros.h"

ComputeNDVI::ComputeNDVI()
{
}

void ComputeNDVI::DoInit(std::string &xml, int nRes)
{
    m_inXml = xml;
    m_nResolution = nRes;
}

// The algorithm consists in a applying a formula for computing the NDVI for each pixel,
// using BandMathFilter
ComputeNDVI::OutputImageType::Pointer ComputeNDVI::DoExecute()
{
    auto factory = MetadataHelperFactory::New();
    auto pHelper = factory->GetMetadataHelper(m_inXml);
    // the bands are 1 based
    int nNirBandIdx = pHelper->GetRelNirBandIndex()-1;
    int nRedBandIdx = pHelper->GetRelRedBandIndex()-1;
    //Read all input parameters
    m_InputImageReader = ImageReaderType::New();
    std::string imgFileName = pHelper->GetImageFileName();

    std::cout << "ComputeNDVI -> Image File Name: " << imgFileName << std::endl;

    m_InputImageReader->SetFileName(imgFileName);
    m_InputImageReader->UpdateOutputInformation();
    ImageType::Pointer img = m_InputImageReader->GetOutput();
    int curRes = img->GetSpacing()[0];

    m_Functor = FilterType::New();
    m_Functor->GetFunctor().Initialize(nRedBandIdx, nNirBandIdx);
    m_Functor->SetInput(m_InputImageReader->GetOutput());
    m_Functor->UpdateOutputInformation();

    if(m_nResolution != curRes) {
        float fMultiplicationFactor = ((float)curRes)/m_nResolution;
        return m_ResampledBandsExtractor.getResampler(m_Functor->GetOutput(), fMultiplicationFactor)->GetOutput();
    } else {
        return m_Functor->GetOutput();
    }
}
