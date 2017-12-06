/*=========================================================================
  *
  * Program:      Sen2agri-Processors
  * Language:     C++
  * Copyright:    2017, CS SI, mickael.savinaud@c-s.fr
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



#include "otbConcatenateVectorImageFilter.h"

#include "otbMultiChannelExtractROI.h"

#include "UpdateSynthesisS2VectorImageFilter.h"

namespace otb
{

namespace Wrapper
{
class UpdateSynthesisS2 : public Application
{
public:
    typedef UpdateSynthesisS2 Self;
    typedef Application Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    itkNewMacro(Self)

    itkTypeMacro(UpdateSynthesisS2, otb::Application)

    typedef otb::ConcatenateVectorImageFilter<	FloatVectorImageType,
    											FloatVectorImageType,
												FloatVectorImageType>
    												ConcatenateFilterType;

    typedef UpdateSynthesisS2VectorImageFilter<	FloatVectorImageType,
  												FloatVectorImageType>
    												UpdateSynthesisS2FilterType;

    typedef otb::MultiChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                        FloatVectorImageType::InternalPixelType>
    												ExtractMultiFilterType;

private:

    void DoInit()
    {
        SetName("UpdateSynthesisS2");
        SetDescription("Update synthesis using the recurrent expression of the weighted average (S2Only).");

        SetDocName("UpdateSynthesisS2");
        SetDocLongDescription("TODO long description");
        SetDocLimitations("None");
        SetDocAuthors("MSD");
        SetDocSeeAlso(" ");

        AddParameter(ParameterType_InputImage, "cl2arefl", "Current L2A input reflectance");
        AddParameter(ParameterType_InputImage, "cl2acsm", " Current L2A Cloud-Shadow Mask");
        AddParameter(ParameterType_InputImage, "cl2awm", "Current L2A Water mask");
        AddParameter(ParameterType_InputImage, "cl2asm", "Current L2A Snow mask");
        AddParameter(ParameterType_InputImage, "cl2aw", "Current L2A Weight value");
        AddParameter(ParameterType_InputImage, "cl2aedg", "Current L2A Edge mask");

        AddParameter(ParameterType_Int, "bluebandidx", "Index of the blue band in the L2A and L3A reflectance");
        AddParameter(ParameterType_Int, "duration", "Duration in days between current L2A date and the synthesis date");

        AddParameter(ParameterType_InputImage, "pl3aw", "Previous l3a weights");
        MandatoryOff("pl3aw");
        AddParameter(ParameterType_InputImage, "pl3ad", "Previous l3a dates");
        MandatoryOff("pl3ad");
        AddParameter(ParameterType_InputImage, "pl3arefl", "Previous l3a reflectances");
        MandatoryOff("pl3arefl");
        AddParameter(ParameterType_InputImage, "pl3aflag", "Previous l3a flags");
        MandatoryOff("pl3aflag");

        AddParameter(ParameterType_OutputImage, "cl3aw", "Current l3a weights");
        AddParameter(ParameterType_OutputImage, "cl3ad", "Current l3a dates");
        AddParameter(ParameterType_OutputImage, "cl3arefl", "Current l3a reflectances");
        AddParameter(ParameterType_OutputImage, "cl3aflag", "Current l3a flags");
    }

    void DoUpdateParameters()
    {
        m_concat1 = ConcatenateFilterType::New();
        m_concat2 = ConcatenateFilterType::New();
        m_concat3 = ConcatenateFilterType::New();
        m_concat4 = ConcatenateFilterType::New();
        m_concat5 = ConcatenateFilterType::New();
        m_concat6 = ConcatenateFilterType::New();

        m_UpdateSynthesisFilter = UpdateSynthesisS2FilterType::New();

        m_outL3AReflExtractor = ExtractMultiFilterType::New();
        m_outL3AWeightExtractor = ExtractMultiFilterType::New();
        m_outL3ADateExtractor = ExtractMultiFilterType::New();
        m_outL3AFlagExtractor = ExtractMultiFilterType::New();

    }

    // The algorithm consists in a applying a formula for computing the NDVI for each pixel,
    // using BandMathFilter
    void DoExecute()
    {
    	FloatVectorImageType::Pointer cL2ARefl = GetParameterFloatVectorImage("cl2arefl");
    	FloatVectorImageType::Pointer cL2ACSMask = GetParameterFloatVectorImage("cl2acsm");
    	FloatVectorImageType::Pointer cL2AWMask = GetParameterFloatVectorImage("cl2awm");
    	FloatVectorImageType::Pointer cL2ASMask = GetParameterFloatVectorImage("cl2asm");
    	FloatVectorImageType::Pointer cL2AWeight = GetParameterFloatVectorImage("cl2aw");
    	FloatVectorImageType::Pointer cL2AEDGMask = GetParameterFloatVectorImage("cl2aedg");

    	unsigned int nbBands = cL2ARefl->GetNumberOfComponentsPerPixel();

    	FloatVectorImageType::Pointer pL3AWeight;
    	FloatVectorImageType::Pointer pL3ADates;
    	FloatVectorImageType::Pointer pL3ARefl;
    	FloatVectorImageType::Pointer pL3AFlag;
        bool l3aprovided = false;

        if(HasValue("pl3aw") && HasValue("pl3ad") &&
           HasValue("pl3arefl") && HasValue("pl3aflag")) {
        	l3aprovided = true;
        	pL3AWeight = GetParameterFloatVectorImage("pl3aw");
        	pL3ADates = GetParameterFloatVectorImage("pl3ad");
        	pL3ARefl = GetParameterFloatVectorImage("pl3arefl");
        	pL3AFlag = GetParameterFloatVectorImage("pl3aflag");
        }

        m_concat1->SetInput1(cL2ARefl);
        m_concat1->SetInput2(cL2ACSMask);

        m_concat2->SetInput1(m_concat1->GetOutput());
        m_concat2->SetInput2(cL2AWMask);

        m_concat3->SetInput1(m_concat2->GetOutput());
        m_concat3->SetInput2(cL2ASMask);

        m_concat4->SetInput1(m_concat3->GetOutput());
        m_concat4->SetInput2(cL2ASMask);

        m_concat5->SetInput1(m_concat4->GetOutput());
        m_concat5->SetInput2(cL2AWeight);

        m_concat6->SetInput1(m_concat5->GetOutput());
        m_concat6->SetInput2(cL2AEDGMask);

    	// Add previous L3A informations if available
    	if ( l3aprovided ) {
    		// Add previous L3A reflectances
            m_concat7->SetInput1(m_concat6->GetOutput());
            m_concat7->SetInput2(pL3ARefl);

            m_concat8->SetInput1(m_concat7->GetOutput());
            m_concat8->SetInput2(pL3AWeight);

            m_concat9->SetInput1(m_concat8->GetOutput());
            m_concat9->SetInput2(pL3ADates);

            m_concat10->SetInput1(m_concat9->GetOutput());
            m_concat10->SetInput2(pL3AFlag);

    	}

        // Perform the update

        m_UpdateSynthesisFilter->SetDuration(GetParameterInt("duration"));
        m_UpdateSynthesisFilter->SetBlueBandIdx(GetParameterInt("bluebandidx"));
        m_UpdateSynthesisFilter->SetNbBandsRefl(nbBands);
        if ( l3aprovided ) {
        	m_UpdateSynthesisFilter->SetInput(m_concat10->GetOutput());
        }
        else {
        	m_UpdateSynthesisFilter->SetInput(m_concat6->GetOutput());
        }


        // Split the filter output into the different output (which contain 2*nbbands + 2)
        m_UpdateSynthesisFilter->UpdateOutputInformation();

        FloatVectorImageType::Pointer outFilterImg = m_UpdateSynthesisFilter->GetOutput();

        unsigned int nbOutFilterBands = outFilterImg->GetNumberOfComponentsPerPixel();

        std::cout << "Nb of output bands =" << nbOutFilterBands << " vs " << 2 * nbBands + 2 << std::endl;

        m_outL3AReflExtractor->SetInput( outFilterImg );
        m_outL3AReflExtractor->SetFirstChannel(1);
        m_outL3AReflExtractor->SetLastChannel(nbBands);
        m_outL3AReflExtractor->UpdateOutputInformation();

        m_outL3AWeightExtractor->SetInput( outFilterImg );
        m_outL3AWeightExtractor->SetFirstChannel(nbBands + 1);
        m_outL3AWeightExtractor->SetLastChannel(2*nbBands);
        m_outL3AWeightExtractor->UpdateOutputInformation();


        m_outL3ADateExtractor->SetInput( outFilterImg );
        m_outL3ADateExtractor->SetChannel( 2*nbBands + 1  );
        m_outL3ADateExtractor->UpdateOutputInformation();

        m_outL3AFlagExtractor->SetInput( outFilterImg );
        m_outL3AFlagExtractor->SetChannel( 2*nbBands + 2 );
        m_outL3AFlagExtractor->UpdateOutputInformation();

        // Set outputs
        SetParameterOutputImage("cl3arefl", m_outL3AReflExtractor->GetOutput());

        SetParameterOutputImage("cl3aw", m_outL3AWeightExtractor->GetOutput());

        SetParameterOutputImage("cl3ad", m_outL3ADateExtractor->GetOutput());

        SetParameterOutputImagePixelType("cl3aflag", ImagePixelType_uint8);
        SetParameterOutputImage("cl3aflag", m_outL3AFlagExtractor->GetOutput());

        return;
    }

    UpdateSynthesisS2FilterType::Pointer    m_UpdateSynthesisFilter;
    ConcatenateFilterType::Pointer  m_concat1;
	ConcatenateFilterType::Pointer  m_concat2;
	ConcatenateFilterType::Pointer  m_concat3;
	ConcatenateFilterType::Pointer  m_concat4;
	ConcatenateFilterType::Pointer  m_concat5;
	ConcatenateFilterType::Pointer  m_concat6;
	ConcatenateFilterType::Pointer  m_concat7;
	ConcatenateFilterType::Pointer  m_concat8;
	ConcatenateFilterType::Pointer  m_concat9;
	ConcatenateFilterType::Pointer  m_concat10;

    ExtractMultiFilterType::Pointer m_outL3AReflExtractor;
    ExtractMultiFilterType::Pointer m_outL3AWeightExtractor;
    ExtractMultiFilterType::Pointer m_outL3ADateExtractor;
    ExtractMultiFilterType::Pointer m_outL3AFlagExtractor;



};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::UpdateSynthesisS2)


