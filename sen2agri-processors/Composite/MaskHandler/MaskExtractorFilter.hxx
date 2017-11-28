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
 
#include "MaskExtractorFilter.h"

namespace otb
{

template <class TImageType, class TOutputImageType>
MaskExtractorFilter<TImageType, TOutputImageType>
::MaskExtractorFilter()
{

    m_MaskHandlerFunctor = FunctorFilterType::New();
    this->SetNumberOfRequiredInputs(2);
}

template <class TImageType, class TOutputImageType>
void
MaskExtractorFilter<TImageType, TOutputImageType>::
GenerateData()
{
  m_MaskHandlerFunctor->SetInput1(this->GetInput(0));
  m_MaskHandlerFunctor->SetInput2(this->GetInput(1));

  m_MaskHandlerFunctor->UpdateOutputInformation();
  m_MaskHandlerFunctor->GetOutput()->SetNumberOfComponentsPerPixel(3);
  m_MaskHandlerFunctor->GraftOutput(this->GetOutput());
  m_MaskHandlerFunctor->Update();

  this->GraftOutput(m_MaskHandlerFunctor->GetOutput());
}

template <class TImageType, class TOutputImageType>
void
MaskExtractorFilter<TImageType, TOutputImageType>::
SetBitsMask(int cloudMask, int waterMask, int snowMask)
{
    m_MaskHandlerFunctor->GetFunctor().SetBitsMask(cloudMask, waterMask, snowMask);
}

template <class TImageType, class TOutputImageType>
void
MaskExtractorFilter<TImageType, TOutputImageType>::
UpdateOutputInformation()
{
    Superclass::UpdateOutputInformation();

    this->GetOutput()->SetNumberOfComponentsPerPixel(3);
}

template <class TImageType, class TOutputImageType>
void
MaskExtractorFilter<TImageType, TOutputImageType>::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os
  << indent << "Threshold:" << this->m_Threshold
  << std::endl;
}

} /* end namespace otb */
